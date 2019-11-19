
#include <iostream>
#include <iomanip>
#include <cassert>
#include <thread>
#include <mutex>
#include <atomic>
#include <random>
#include <condition_variable>

using namespace std ;



const     int num_items        = 40;
mutex mtx; //Para hacer cout's
unsigned
   cont_prod[num_items], // contadores de verificación: producidos
   cont_cons[num_items];

int num_productores=2, num_consumidores=3;

class ProdConsLIFO
{
private:
  static const int num_celdas_total=10;
  int buffer[num_celdas_total],
  primera_libre;

  mutex cerrojo_monitor;
  condition_variable ocupadas, libres;

public:
  ProdConsLIFO();
  int leer();
  void escribir(int valor);
};

ProdConsLIFO::ProdConsLIFO(){
  primera_libre=0;
}

int ProdConsLIFO::leer(){
  unique_lock<mutex> guarda(cerrojo_monitor);

  if(primera_libre==0) //Si no hay huecos, se espera
    ocupadas.wait(guarda);

  assert(0<primera_libre);
  primera_libre--;
  const int valor = buffer[primera_libre];

  libres.notify_one(); //Avisa de que se ha quedado un hueco libre para escribir

  return valor;
}

void ProdConsLIFO::escribir(int dato){
  unique_lock<mutex> guarda(cerrojo_monitor);

  if(primera_libre==num_celdas_total)
    libres.wait(guarda);

  assert(primera_libre<num_celdas_total);
  buffer[primera_libre]=dato;
  primera_libre++;

  ocupadas.notify_one();
}

// ────────────────────────────────────────────────────────────────────────────────

    /*
        Plantilla de función para generar un entero aleatorio uniformemente
        distribuido entre dos valores enteros, ambos incluidos
        (ambos tienen que ser dos constantes, conocidas en tiempo de compilación)
    */

    template< int min, int max > int aleatorio() {
        static default_random_engine generador( (random_device())() );
        static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
        return distribucion_uniforme( generador );
    }

//
// ──────────────────────────────────────────────────────── FUNCIONES COMUNES ─────
//

int producir_dato()
{
   static int contador = 0 ;
   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));
   mtx.lock();
   cout << "producido: " << contador << endl << flush ;
   mtx.unlock();
   cont_prod[contador] ++ ;
   return contador++ ;
}


    void consumir_dato( unsigned dato )
    {
       if ( num_items <= dato )
       {
          cout << " dato === " << dato << ", num_items == " << num_items << endl ;
          assert( dato < num_items );
       }
       cont_cons[dato] ++ ;
       this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));
       mtx.lock();
       cout << "                  consumido: " << dato << endl ;
       mtx.unlock();
    }


// ────────────────────────────────────────────────────────────────────────────────


    void test_contadores() {
        bool ok = true ;
        cout << "comprobando contadores ...." ;

        for( unsigned i = 0 ; i < num_items ; i++ ) {
            if ( cont_prod[i] != 1 ) {
                cout << "error: valor " << i << " producido " << cont_prod[i] << " veces." << endl;
                ok = false ;
            }
            if ( cont_cons[i] != 1 ) {
                cout << "error: valor " << i << " consumido " << cont_cons[i] << " veces" << endl ;
                ok = false ;
            }
        }

        if (ok)
            cout << endl << flush << "solución (aparentemente) correcta." << endl << flush ;
    }

//
// ──────────────────────────────────────────────────────────── SOLUCION LIFO ─────
//



    void  funcion_hebra_productora_LIFO (int index, ProdConsLIFO *monitor) {
        for( unsigned i = index ; i < num_items; i += num_productores ) {
            int dato = producir_dato();

            monitor->escribir(dato);
        }
    }


    void funcion_hebra_consumidora_LIFO (int index, ProdConsLIFO *monitor) {
        for( unsigned i = index ; i < num_items ; i+=num_consumidores ) {
            int dato= monitor->leer();
            consumir_dato(dato);


        }
    }

//
// ──────────────────────────────────────────────────────────── SOLUCION FIFO ─────
//

class ProdConsFIFO
{
private:
  static const int num_celdas_total=10;
  int buffer[num_celdas_total],
  primera_libre,
  primera_ocupada,
  n;

  mutex cerrojo_monitor;
  condition_variable ocupadas, libres;

public:
  ProdConsFIFO();
  int leer();
  void escribir(int valor);
};

ProdConsFIFO::ProdConsFIFO(){
  primera_libre=0;
  primera_ocupada=0;
  n=0;
}

int ProdConsFIFO::leer(){
  unique_lock<mutex> guarda(cerrojo_monitor);

  if(n==0)
    ocupadas.wait(guarda);

  assert(n>0);
  const int dato=buffer[primera_ocupada];
  primera_ocupada=(primera_ocupada+1)%num_celdas_total;

  n--;
  libres.notify_one();

  return dato;
}

void ProdConsFIFO::escribir(int dato){
  unique_lock<mutex> guarda(cerrojo_monitor);

  if(n==num_celdas_total)
    libres.wait(guarda);


  assert(n<num_celdas_total);
  n++;
  buffer[primera_libre]=dato;
  primera_libre=(primera_libre+1)%num_celdas_total;




  ocupadas.notify_one();


}

    void funcion_hebra_productora_FIFO (int index, ProdConsFIFO *monitor) {
        for( unsigned i = index ; i < num_items ; i+=num_productores ) {
            int dato = producir_dato();
            monitor->escribir(dato);
        }
    }


    void funcion_hebra_consumidora_FIFO (int index, ProdConsFIFO *monitor) {
      for( unsigned i = index ; i < num_items ; i+=num_consumidores ) {
            int dato;
            dato=monitor->leer();
            consumir_dato( dato) ;
        }
    }


// ────────────────────────────────────────────────────────────────────────────────


    int main(int argc, char const *argv[]) {
        int choice = 0;

        while (choice != 1 && choice != 2) {
            cout << "¿LIFO (1) o FIFO? (2):\t";
            cin >> choice;
        }

        thread productores[num_productores];
        thread consumidores[num_consumidores];




        if (choice == 1)
        {
          ProdConsLIFO monitor;

            cout << "--------------------------------------------------------"  << endl
                 << "Problema de los productores-consumidores (solución LIFO)." << endl
                 << "--------------------------------------------------------"  << endl
                 << flush ;

            for (int i = 0; i < num_consumidores; i++)
                consumidores[i] = thread(funcion_hebra_consumidora_LIFO, i, &monitor);

            for (int i = 0; i < num_productores; i++)
                productores[i] = thread(funcion_hebra_productora_LIFO, i, &monitor);

        }
        else if (choice == 2)
        {
          ProdConsFIFO monitor;
            cout << "--------------------------------------------------------"  << endl
                 << "Problema de los productores-consumidores (solución FIFO)." << endl
                 << "--------------------------------------------------------"  << endl
                 << flush ;


            for (int i = 0; i < num_consumidores; i++)
                consumidores[i] = thread(funcion_hebra_consumidora_FIFO, i, &monitor);

            for (int i = 0; i < num_productores; i++)
                productores[i] = thread(funcion_hebra_productora_FIFO, i, &monitor);

        }

        for (int i = 0; i < num_consumidores; i++)
            consumidores[i].join();

        for (int i = 0; i < num_productores; i++)
            productores[i].join();

        test_contadores();
    }
