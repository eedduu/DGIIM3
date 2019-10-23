#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <atomic>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "Semaphore.h"

using namespace std ;
using namespace SEM ;

mutex mtx;

const int tam_prov=2;
Semaphore estanquero=0; //Semáforo que depende de si hay algo en el mostrador
Semaphore rfumadores[3]={0,0,0};
Semaphore proveedor=tam_prov;

int pale[tam_prov];
atomic<int> lote;

void pedir(int i){
  rfumadores[i].sem_wait(); //Bloquea al fumador que espera el recurso i (cada
                            //uno se bloquea a si mismo) hasta que el estanquero le ofrezca el recurso
  mtx.lock();
  cout << "El fumador " << i << " obtiene suministros" << endl;
  mtx.unlock();
  estanquero.sem_signal();  // El fumador coge el recurso y deja el mostrador libre, desbloqueando al estanquero
}

void proveer(int i){
  estanquero.sem_wait(); //Bloquea al estanquero para que no pueda poner nada más en el mostrador hasta que alguien lo recoja
  mtx.lock();
  cout << "Estanquero repone" << endl;
  mtx.unlock();
  rfumadores[i].sem_signal(); //Desbloquea al fumador que estaba esperando el recurso i.
}






//**********************************************************************
// plantilla de función para generar un entero aleatorio uniformemente
// distribuido entre dos valores enteros, ambos incluidos
// (ambos tienen que ser dos constantes, conocidas en tiempo de compilación)
//----------------------------------------------------------------------

template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}

//----------------------------------------------------------------------
// función que ejecuta la hebra del estanquero

void funcion_hebra_estanquero(  )
{
  while(true){
    estanquero.sem_wait();
    int comodin=pale[lote];
    lote--;
    int recurso= aleatorio<0,2>(); //Genera recurso aleatorio

    proveer(recurso); // Lo pone en el mostrador
    proveedor.sem_signal();
  }
}

//-------------------------------------------------------------------------
// Función que simula la acción de fumar, como un retardo aleatoria de la hebra

void fumar( int num_fumador )
{

   // calcular milisegundos aleatorios de duración de la acción de fumar)
   chrono::milliseconds duracion_fumar( aleatorio<20,200>() );

   // informa de que comienza a fumar
   mtx.lock();
    cout << "Fumador " << num_fumador << "  :"
          << " empieza a fumar (" << duracion_fumar.count() << " milisegundos)" << endl;
   mtx.unlock();
   // espera bloqueada un tiempo igual a ''duracion_fumar' milisegundos
   this_thread::sleep_for( duracion_fumar );

   // informa de que ha terminado de fumar
    mtx.lock();
    cout << "Fumador " << num_fumador << "  : termina de fumar, comienza espera de ingrediente." << endl;
    mtx.unlock();
}

//----------------------------------------------------------------------
// función que ejecuta la hebra del fumador
void  funcion_hebra_fumador( int num_fumador )
{
  /*
      Fumador 0: necesita cerillas
      Fumador 1: necesita tabaco
      Fumador 2: necesita papel
  */

   while( true )
   {
    pedir(num_fumador); //El fumador siempre coge 1 unidad del recurso y se va a fumar, por lo que necesita recargar
    fumar(num_fumador);
   }
}


void funcion_hebra_suministradora(){
    while(true){
      proveedor.sem_wait();
      int producto=3; // El 3 representa el ingrediente comodin
      pale[lote]=producto;
      lote++;
      mtx.lock();
      cout << "Se ha suministrando un lote" << endl;
      mtx.unlock();
      estanquero.sem_signal();



    }
}

//----------------------------------------------------------------------

int main()
{
   thread fumadores[3], estanquero;
   thread proveedor(funcion_hebra_suministradora);
     estanquero=thread(funcion_hebra_estanquero);

   for (int i=0; i<3; i++)
    fumadores[i]=thread(funcion_hebra_fumador, i);



  estanquero.join();

  for (int i=0; i<3; i++)
   fumadores[i].join();

  return 0;
}
