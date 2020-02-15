#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random>
#include <deque>
#include <atomic>
#include "Semaphore.h"

using namespace std ;
using namespace SEM ;

//**********************************************************************
// variables compartidas

const int num_items = 40 ,   // número de items
	       tam_vec   = 10 ;   // tamaño del buffer

//deque<int> buffer;
int buffer[tam_vec];
atomic<int> i_leer, i_write;

mutex mtx;

int nprod=2, ncons=4;

unsigned  cont_prod[num_items] = {0}, // contadores de verificación: producidos
          cont_cons[num_items] = {0}; // contadores de verificación: consumidos
Semaphore puede_leer=0,
					puede_escribir=tam_vec;


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

//**********************************************************************
// funciones comunes a las dos soluciones (fifo y lifo)
//----------------------------------------------------------------------

int producir_dato()
{
   static int contador = 0 ;
   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));

   cout << "producido: " << contador << endl << flush ;

   cont_prod[contador] ++ ;
   return contador++ ;
}
//----------------------------------------------------------------------

void consumir_dato( unsigned dato )
{
   assert( dato < num_items );
   cont_cons[dato] ++ ;
   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));

   cout << "                  consumido: " << dato << endl ;

}


//----------------------------------------------------------------------

void test_contadores()
{
   bool ok = true ;
   cout << "comprobando contadores ...." ;
   for( unsigned i = 0 ; i < num_items ; i++ )
   {  if ( cont_prod[i] != 1 )
      {  cout << "error: valor " << i << " producido " << cont_prod[i] << " veces." << endl ;
         ok = false ;
      }
      if ( cont_cons[i] != 1 )
      {  cout << "error: valor " << i << " consumido " << cont_cons[i] << " veces" << endl ;
         ok = false ;
      }
   }
   if (ok)
      cout << endl << flush << "solución (aparentemente) correcta." << endl << flush ;
}

//----------------------------------------------------------------------

void  funcion_hebra_productora( int ind )
{
   for( unsigned i = ind ; i < num_items ; i+=nprod )
   {
		 sem_wait(puede_escribir);
      int dato = producir_dato() ;
      //buffer.push_back(dato);
			buffer[i_write]=dato;
			i_write=(i_write+1)%tam_vec;
			//index++;
			sem_signal(puede_leer);
   }
}

//----------------------------------------------------------------------

void funcion_hebra_consumidora( int ind )
{
   for( unsigned i = ind ; i < num_items ; i+=ncons )
   {
		  sem_wait(puede_leer);
      int dato ;
    //  dato=buffer.front();
		//	buffer.pop_front();
		  mtx.lock();
		  dato=buffer[i_leer];
			i_leer=(i_leer+1)%tam_vec;
			consumir_dato( dato ) ;
			mtx.unlock();

			sem_signal(puede_escribir);
    }
}
//----------------------------------------------------------------------

int main()
{
   cout << "--------------------------------------------------------" << endl
        << "Problema de los productores-consumidores (solución FIFO)." << endl
        << "--------------------------------------------------------" << endl
        << flush ;

	 thread productores[nprod], consumidores[ncons];

	 for (int i=0; i<nprod; i++)
	 				productores[i]=thread(funcion_hebra_productora, i);

	 for (int i=0; i<ncons; i++)
	 				consumidores[i]=thread(funcion_hebra_consumidora, i);

		for (int i=0; i<nprod; i++)
		 				productores[i].join();

 	 for (int i=0; i<ncons; i++)
 	 				consumidores[i].join();
   test_contadores();
}
