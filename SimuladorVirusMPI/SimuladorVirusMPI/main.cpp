/* Archivo:      mpi_plantilla.cpp
* Propósito:   ....
*
* Compilación:   mpicxx -g -Wall -o mpi_plantilla mpi_plantilla.cpp
* Ejecución:     mpiexec -n <num_proc> ./mpi_plantilla <secuencia de valores de parámetros>
*
* Entradas:     ...
* Salidas:    ...
*
* Notas:
* 1.  bandera DEBUG produce salida detallada para depuración.
*
*/

#include <mpi.h> 
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;

//#define DEBUG

void uso(string nombre_prog);

void obt_args(
	char*    argv[]        /* in  */,
	int&     dato_salida  /* out */);

/*---------------------------------------------Métodos------------------------------------------------*/
//void persona() {}

/*---------------------------------------------Métodos------------------------------------------------*/


int main(int argc, char* argv[]) {
	int mid; // id de cada proceso
	int cnt_proc; // cantidad de procesos
	MPI_Status mpi_status; // para capturar estado al finalizar invocación de funciones MPI
						   /* Arrancar ambiente MPI */
	MPI_Init(&argc, &argv);             		/* Arranca ambiente MPI */
	MPI_Comm_rank(MPI_COMM_WORLD, &mid); 		/* El comunicador le da valor a id (rank del proceso) */
	MPI_Comm_size(MPI_COMM_WORLD, &cnt_proc);  /* El comunicador le da valor a p (número de procesos) */

#  ifdef DEBUG 
	if (mid == 0)
		cin.ignore();
	MPI_Barrier(MPI_COMM_WORLD);
#  endif

/*-------------------------------------ejecución del proceso principal--------------------------------*/
	if (mid == 0) {
		int i, n;
		cout << "Ingrese la cantidad de personas: " << endl;
		cin >> n;
		int* persona = (int*)malloc((n*4 + 1) * sizeof(int)); // talvez ocupe paréntesis
		int fila = n, col = n;
		int *M = (int*)malloc(fila * col * sizeof(int)); // Matriz de enfermos sexuales
		for (i = 0; i < n; i++) {
			M[i] = rand()% 100;
		}
		cout << M[i] << endl;
	}
	


/*------------------------------------finalización de la ejecución paralela---------------------------*/
	if (mid == 0)
		cin.ignore();
	MPI_Barrier(MPI_COMM_WORLD); // para sincronizar la finalización de los procesos

	MPI_Finalize();
	return 0;
}/*-------------------------------------------------main final---------------------------------------*/


 /*---------------------------------------------Métodos------------------------------------------------*/


 /*---------------------------------------------Métodos------------------------------------------------*/

   /*---------------------------------------------------------------------
   * REQ: N/A
   * MOD: N/A
   * EFE: despliega mensaje indicando cómo ejecutar el programa y pasarle parámetros de entrada.
   * ENTRAN:
   *		nombre_prog:  nombre del programa
   * SALEN: N/A
   */
void uso(string nombre_prog /* in */) {
	cerr << nombre_prog.c_str() << " secuencia de parámetros de entrada" << endl;
	exit(0);
}  /* uso */

   /*---------------------------------------------------------------------
   * REQ: N/A
   * MOD: dato_salida
   * EFE: obtiene los valores de los argumentos pasados por "línea de comandos".
   * ENTRAN:
   *		nombre_prog:  nombre del programa
   * SALEN:
   *		dato_salida: un dato de salida con un valor de argumento pasado por "línea de comandos".
   */
void obt_args(
	char*    argv[]        /* in  */,
	int&     dato_salida  /* out */) {

	dato_salida = strtol(argv[1], NULL, 10); // se obtiene valor del argumento 1 pasado por "línea de comandos".

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