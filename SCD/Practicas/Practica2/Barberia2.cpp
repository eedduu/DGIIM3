
#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include "HoareMonitor.h"

using namespace std ;
using namespace HM;

mutex mtx;

static const int NC=5;


//Generador de números aleatorios
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
}

//El cliente que ha entrado en la barberia comprueba si tiene que despertar al barbero, se va a la sala
// de espera y espera a que el barbero le llame.
void Barberia::cortarPelo(int i){
  //Si el barbero está durmiendo y hay alguien en la sala de espera, despierta al barbero
  if(!barbero.empty() && (!espera.empty())){
    mtx.lock();
    cout << "Cliente " << i << " despierta al barbero" << endl;
    mtx.unlock();

    barbero.signal();
  }
  //Espera a que lo llamen
  mtx.lock();
  cout << "El cliente " << i << " se pone a esperar." << endl;
  mtx.unlock();
  espera.wait();

  //Una vez le han llamado, se sienta en la silla.
  mtx.lock();
  cout << "Cliente " << i << " se sienta en la silla" << endl;
  mtx.unlock();

  silla.wait();
}

//El barbero comprueba si hay 2 personas (por lo menos) en sala de espera.
// Si las hay, las llama a pelarse, en caso contrario, se duerme.
void Barberia::siguienteCliente(){
  if(espera.get_nwt()<2){
    mtx.lock();
    cout << "El barbero se pone a dormir" << endl;
    mtx.unlock();

    barbero.wait();
  }

  if(silla.empty()){
    mtx.lock();
    cout << "El barbero llama a dos clientes" << endl;
    mtx.unlock();

    espera.signal();
    espera.signal();
  }
}

//El barbero avisa a los dos clientes que estaba pelando de que han terminado.
void Barberia::finCliente(){
  silla.signal();
  silla.signal();
}

void EsperarFueraBarberia(int i){
  chrono::milliseconds duracion_espera( aleatorio<100,200>() );

  mtx.lock();
  cout << "Cliente " << i << " espera fuera de la barbería (" << duracion_espera.count() << " milisegundos)..." << endl;
  mtx.unlock();

  this_thread::sleep_for(duracion_espera);

  mtx.lock();
  cout << "Cliente " << i<< " ha entrado en la barberia.";
  mtx.unlock();
}

void CortarPeloACliente(){
  mtx.lock();
  cout << "El barbero empieza a pelar a los dos clientes." << endl;
  mtx.unlock();

  chrono::milliseconds duracion_pelar( aleatorio<20,200>() );

   this_thread::sleep_for( duracion_pelar );

   mtx.lock();
   cout << "El barbero termina y tarda: (" << duracion_pelar.count() << " milisegundos)." << endl;
   mtx.unlock();
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
