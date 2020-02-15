#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "HoareMonitor.h"

using namespace std ;
using namespace HM;

mutex mtx;

static const int NF=3;
static const int NE=2;



class Estanco : public HoareMonitor {
private:
  CondVar estanquero;
  CondVar fumadores[NF];
  int recurso;
public:
  Estanco();
  void Proveer(int i, int ne);
  void pedir(int i);
  void esperarRecogida();
};

Estanco::Estanco(){
  recurso=-1;
  estanquero=newCondVar();
  for(int i=0;i<NF;i++)
    fumadores[i]=newCondVar();
}

void Estanco::Proveer(int i, int ne){
  recurso=i;

  mtx.lock();
  cout << "Estanquero " << ne << " repone recurso " << i << endl;
  mtx.unlock();

  fumadores[i].signal();
}

void Estanco::pedir(int i){
  if(i!=recurso )
    fumadores[i].wait();

  recurso=-1;
  estanquero.signal();
}

void Estanco::esperarRecogida(){
  if(recurso!=-1)
    estanquero.wait();
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

int ProducirIngrediente(){
  int r= aleatorio<0,2>();
  chrono::milliseconds duracion_producir( aleatorio<20,200>() );
  this_thread::sleep_for( duracion_producir );
   return r;
}
//----------------------------------------------------------------------
// función que ejecuta la hebra del estanquero

void funcion_hebra_estanquero( int i, MRef<Estanco> monitor )
{
  while(true){
    int elem=ProducirIngrediente();
    monitor->Proveer(elem, i); // Lo pone en el mostrador y espera
    monitor->esperarRecogida();
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
void  funcion_hebra_fumador( int num_fumador, MRef<Estanco> monitor )
{
   while( true )
   {
    monitor->pedir(num_fumador); //El fumador siempre coge 1 unidad del recurso y se va a fumar, por lo que necesita recargar
    mtx.lock();
    cout << "Fumador " << num_fumador << "  : recoge recurso " << endl;
    mtx.unlock();
    fumar(num_fumador);
   }
}

//----------------------------------------------------------------------

int main()
{
  MRef<Estanco> monitor= Create<Estanco>();
   thread fumadores[NF], estanquero[NE];

   for (int i=0; i<NE; i++)
     estanquero[i]=thread(funcion_hebra_estanquero, i, monitor);

   for (int i=0; i<NF; i++)
    fumadores[i]=thread(funcion_hebra_fumador, i, monitor);


  for (int i=0;i<NE;i++)
  estanquero[i].join();

  for (int i=0; i<NF; i++)
   fumadores[i].join();

  return 0;
}
