
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

static const int NC=3;
static const int NB=1;

template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}


class Barberia : public HoareMonitor {
private:
  CondVar barbero;
  CondVar espera;
  CondVar silla;
//  int num_clientes;

public:
  Barberia();
  void cortarPelo(int i);
  void siguienteCliente();
  void finCliente();
};

Barberia::Barberia(){
  barbero=newCondVar();
  espera=newCondVar();
  silla=newCondVar();
  //num_clientes=0;
}

void Barberia::cortarPelo(int i){
//  num_clientes++;
  if(!barbero.empty()){
    mtx.lock();
    cout << "Cliente " << i << " despierta al barbero" << endl;
    mtx.unlock();

    barbero.signal();
  }

  if((!silla.empty()) || (!espera.empty())){
    mtx.lock();
    cout << "El cliente " << i << " se pone a esperar." << endl;
    mtx.unlock();
    
    espera.wait();
  }

  mtx.lock();
  cout << "Cliente " << i << " empieza a pelarse" << endl;
  mtx.unlock();

  silla.wait();
  //num_clientes--;
}
void Barberia::siguienteCliente(){
  if(espera.empty()){
    mtx.lock();
    cout << "El barbero se pone a dormir" << endl;
    mtx.unlock();
    barbero.wait();
  }

  if(silla.empty()){
    mtx.lock();
    cout << "El barbero llama al cliente" << endl;
    mtx.unlock();
    espera.signal();
  }
}

void Barberia::finCliente(){
  silla.signal();
}

void EsperarFueraBarberia(int i){
  chrono::milliseconds duracion_espera( aleatorio<20,200>() );

  mtx.lock();
  cout << "Cliente " << i << " espera fuera de la barbería (" << duracion_espera.count() << " milisegundos)..." << endl;
  mtx.unlock();

  this_thread::sleep_for(duracion_espera);

  mtx.lock();
  cout << "Cliente " << i<< " ha entrado en la barberia.";
  mtx.unlock();
}

void CortarPeloACliente(){
  chrono::milliseconds duracion_pelar( aleatorio<20,200>() );

   this_thread::sleep_for( duracion_pelar );

   cout << "El barbero termina y tarda: (" << duracion_pelar.count() << " milisegundos)." << endl;
}

//----------------------------------------------------------------------
// función que ejecuta la hebra del barbero

void funcion_hebra_barbero(MRef<Barberia> monitor )
{
  while(true){
    monitor->siguienteCliente();
    CortarPeloACliente();
    monitor->finCliente();
  }
}

//----------------------------------------------------------------------
// función que ejecuta la hebra del cliente
void  funcion_hebra_cliente( int i, MRef<Barberia> monitor )
{
   while( true )
   {
    monitor->cortarPelo(i);
    EsperarFueraBarberia(i);
   }
}

//----------------------------------------------------------------------

int main()
{
  MRef<Barberia> monitor= Create<Barberia>();
   thread barbero, clientes[NC];

   barbero=thread(funcion_hebra_barbero, monitor);

   for (int i=0; i<NC; i++)
    clientes[i]=thread(funcion_hebra_cliente, i, monitor);

  for (int i=0; i<NC; i++)
    clientes[i].join();

  return 0;
}
