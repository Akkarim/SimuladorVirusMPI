/* Archivo:      mpi_plantilla.cpp
* Prop�sito:   ....
*
* Compilaci�n:   mpicxx -g -Wall -o mpi_plantilla mpi_plantilla.cpp
* Ejecuci�n:     mpiexec -n <num_proc> ./mpi_plantilla <secuencia de valores de par�metros>
*
* Entradas:     ...
* Salidas:    ...
*
* Notas:
* 1.  bandera DEBUG produce salida detallada para depuraci�n.
*
*/

#include <mpi.h> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;

//#define DEBUG

void uso(string nombre_prog);

void obt_args(
	char*    argv[]        /* in  */,
	int&     dato_salida  /* out */);

/*---------------------------------------------M�todos------------------------------------------------*/
//void persona() {}

/*---------------------------------------------M�todos------------------------------------------------*/


int main(int argc, char* argv[]) {
	int mid; // id de cada proceso
	int cnt_proc; // cantidad de procesos
	int n,np,local_n;
	int* poblacion;

	MPI_Status mpi_status; // para capturar estado al finalizar invocaci�n de funciones MPI
						   /* Arrancar ambiente MPI */
	MPI_Init(&argc, &argv);             		/* Arranca ambiente MPI */
	MPI_Comm_rank(MPI_COMM_WORLD, &mid); 		/* El comunicador le da valor a id (rank del proceso) */
	MPI_Comm_size(MPI_COMM_WORLD, &cnt_proc);  /* El comunicador le da valor a p (n�mero de procesos) */

	n = strtol(argv[1], NULL, 10);
	np = n*4;
	local_n = np / mid;//
	poblacion = (int*)malloc(np * sizeof(int));
	/*****************************************Inicio Inicializar*****************************************************/
	if (mid == 0) {
		for (int i = 0; i<np ; i += 4){
			/*
			pair<x,y>=generarPos();
			 
			poblacion[i] = x;
			poblacion[i + 1] = y;
			poblacion[i + 2] = semana;
			poblacion[i + 3] = estado;
			*/
		}
	}
	/******************************************Fin Inicializar*******************************************************/
#  ifdef DEBUG 
	if (mid == 0)
		cin.ignore();
	MPI_Barrier(MPI_COMM_WORLD);
#  endif

/*-------------------------------------ejecuci�n del proceso principal--------------------------------*/

	if (mid == 0) {

	  //std::string orbits ("365.24 29.53");
	  //std::string::size_type sz;     // alias of size_t
	  //double earth = std::stod (orbits,&sz);
	  //double moon = std::stod (orbits.substr(sz));
	  //std::cout << "The moon completes " << (earth/moon) << " orbits per Earth year.\n";

		ifstream lectura;
		ifstream archivo;
		string::size_type sz; // algo para la stiring

		string nombre;
		double prInfeccion;
		double temp;
		int personas;
		string dato;

		int cont = 0;
		archivo.open("DATOS.txt", ios::in);
		char c = archivo.get();
		while (!archivo.eof()) {
			if (c != ';') {
				dato += c;
			}
			cout << dato << endl;//*-*-*-*-*-*-*-*-*-*-*-*
			if (c == ';' && cont == 0) {
				nombre = dato;
				dato.clear();
				cont++;
			}
			else if (c == ';' && cont == 1) {
				prInfeccion = stod(dato, &sz);
				dato.clear();
				cont++;
			}
			else if (c == ';' && cont == 2) {
				personas = stoi(dato, &sz);
				dato.clear();
				cont++;
			}
			c = archivo.get();
		}
		cout << nombre << "Proba de infeccion: " << prInfeccion << "Semanas: " << personas << endl;

		archivo.close();
	}


		// Matriz de enfermos sexuales

	


/*------------------------------------finalizaci�n de la ejecuci�n paralela---------------------------*/
	if (mid == 0)
		cin.ignore();
	MPI_Barrier(MPI_COMM_WORLD); // para sincronizar la finalizaci�n de los procesos

	MPI_Finalize();
	return 0;
}/*-------------------------------------------------main final---------------------------------------*/


 /*---------------------------------------------M�todos------------------------------------------------*/


 /*---------------------------------------------M�todos------------------------------------------------*/

   /*---------------------------------------------------------------------
   * REQ: N/A
   * MOD: N/A
   * EFE: despliega mensaje indicando c�mo ejecutar el programa y pasarle par�metros de entrada.
   * ENTRAN:
   *		nombre_prog:  nombre del programa
   * SALEN: N/A
   */
void uso(string nombre_prog /* in */) {
	cerr << nombre_prog.c_str() << " secuencia de par�metros de entrada" << endl;
	exit(0);
}  /* uso */

   /*---------------------------------------------------------------------
   * REQ: N/A
   * MOD: dato_salida
   * EFE: obtiene los valores de los argumentos pasados por "l�nea de comandos".
   * ENTRAN:
   *		nombre_prog:  nombre del programa
   * SALEN:
   *		dato_salida: un dato de salida con un valor de argumento pasado por "l�nea de comandos".
   */
void obt_args(
	char*    argv[]        /* in  */,
	int&     dato_salida  /* out */) {

	dato_salida = strtol(argv[1], NULL, 10); // se obtiene valor del argumento 1 pasado por "l�nea de comandos".

#  ifdef DEBUG
	cout << "dato_salida = " << dato_salida << endl;
#  endif
}  /* obt_args */



//End of file with a Cow (Bettsy)
//                               __.----.___
//   ||            ||  (\(__)/)-'||      ;--` ||
//  _||____________||___`(QQ)'___||______;____||_
//  -||------------||----)  (----||-----------||-
//  _||____________||___(o  o)___||______;____||_
//  -||------------||----`--'----||-----------||-
//   ||            ||       `|| ||| || ||     ||
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^