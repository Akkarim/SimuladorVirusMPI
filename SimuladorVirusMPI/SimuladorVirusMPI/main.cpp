#include <mpi.h> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <random>

using namespace std;

//#define DEBUG

void uso(string nombre_prog);

void obt_args(
	char*    argv[]        /* in  */,
	int&     dato_salida  /* out */);

pair<int, int> generarPosRandom(int tam);


int main(int argc, char* argv[]) {
	int mid; // id de cada proceso
	int cnt_proc; // cantidad de procesos

	MPI_Status mpi_status; // para capturar estado al finalizar invocaci�n de funciones MPI
						   /* Arrancar ambiente MPI */
	MPI_Init(&argc, &argv);             		/* Arranca ambiente MPI */
	MPI_Comm_rank(MPI_COMM_WORLD, &mid); 		/* El comunicador le da valor a id (rank del proceso) */
	MPI_Comm_size(MPI_COMM_WORLD, &cnt_proc);  /* El comunicador le da valor a p (n�mero de procesos) */


#  ifdef DEBUG 
	if (mid == 0)
		cin.ignore();
	MPI_Barrier(MPI_COMM_WORLD);
#  endif

	/*-------------------------------------PROGRAMA PRINCIPAL----------------------------*/
	double prInfeccion;
	int poblacion;
	int dimension;
	double infInicial; //Infección inicial
	int duracion;
	double probInf; //Probabilidad de infección
	double probRec; //Probabilidad de Recupereci+on

	/*-------------------------------------Recuperación de Datos--------------------------------------*/
	if (mid == 0) {

		ifstream lectura;
		ifstream archivo;
		string::size_type sz; // algo para la stiring
		string dato;

		int cont = 0;
		archivo.open("DATOS.txt", ios::in);
		char c = archivo.get();
		while (!archivo.eof()) {
			if (c != ';') {
				dato += c;
			}
			//cout << dato << endl;///*-*-*-*-*-*-*-*-*-*-*-*
			if (c == ';' && cont == 0) { // Poblacion
				poblacion = stoi(dato, &sz);
				dato.clear();
				cont++;
			}
			else if (c == ';' && cont == 1) { //Dimensión
				dimension = stoi(dato, &sz);
				dato.clear();
				cont++;
			}
			else if (c == ';' && cont == 2) { // infección inicial
				infInicial = stod(dato, &sz);
				dato.clear();
				cont++;
			}
			else if (c == ';' && cont == 3) { // Duración
				duracion = stoi(dato, &sz);
				dato.clear();
				cont++;
			}
			else if (c == ';' && cont == 4) { // Proba de infección
				probInf = stod(dato, &sz);
				dato.clear();
				cont++;
			}
			else if (c == ';' && cont == 5) { // Proba de Recuperación
				probRec = stod(dato, &sz);
				dato.clear();
				cont++;
			}
			c = archivo.get();
		}
		cout << " -Poblacion: " << poblacion <<"\n" <<  " -Dimension: " << dimension << "\n" 
			<< " -Infeccion Inicial: " << infInicial << "\n" << " -Duracion: " << duracion << "\n" 
			<< " -Prob Infeccion: " << probInf << "\n" 
			<< " -Proba de Recuperacion: " << probRec << endl;
		archivo.close();
	}

	MPI_Bcast(&poblacion, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&dimension, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&infInicial, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&duracion, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&probInf, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&probRec, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	/*-------------------------------------Recuperación de Datos--------------------------------------*/


	/*-------------------------------------Inicializar------------------------------------------------*/

	if (mid == 0) {

		int *personas;
		pair <int, int> pos;
		/*
		-X
		-Y
		-Estado
		-Tics enfermo
		*/
		personas = (int*)malloc(poblacion * sizeof(int)); // Para un array de tamaño población
		int persona = 4; //para las casillas
		///HACER LAS VARAS DEL MAE PARALELIZADO
		for (int i = 0; i < poblacion; i++) {
			pos = generarPosRandom(dimension);
			personas[i] = pos.first; // Hacer un método de genera una pos random que compruebe si ya esta usada
			personas[i + 1] = pos.second; //Same
			personas[i + 2] = 2; // Estado
			personas[i + 3] = 4; //Tics
			personas[i + 4] = -1; ///Separación de cada persona
			i += 4;
		}

		for (int i = 0; i < 100; i++) {
			cout << personas[i]  << endl;
		}


	}

	// Matriz de enfermos sexuales

	/*-------------------------------------Inicializar------------------------------------------------*/



	/*--------------------------------finalizaci�n de la ejecuci�n paralela---------------------------*/
	if (mid == 0)
		cin.ignore();
	MPI_Barrier(MPI_COMM_WORLD); // para sincronizar la finalizaci�n de los procesos

	MPI_Finalize();
	return 0;
}/*-------------------------------------------------main final---------------------------------------*/


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


/*
*REQ: Dimensión
*EFE: Genera una posicón Random, una X y Y y busca si está en la matrix
*MOD: Nothing
*/
pair<int, int> generarPosRandom(int tam) {
	pair<int, int> pos;
	default_random_engine gen;
	uniform_int_distribution<int> distribution(0, tam - 1);
	pos.first = distribution(gen);
	pos.second= distribution(gen);
	
	/*
	do {
		pos.first = distribution(gen);
		pos.second = distribution(gen);
	} while (poblacionInfectada[pos.first][pos.second] == 0 || poblacionInfectada[pos.first][pos.second] == 1);*/

	return pos;

}


   //End of file with a Cow (Bettsy)
   //                               __.----.___
   //   ||            ||  (\(__)/)-'||      ;--` ||
   //  _||____________||___`(QQ)'___||______;____||_
   //  -||------------||----)  (----||-----------||-
   //  _||____________||___(o  o)___||______;____||_
   //  -||------------||----`--'----||-----------||-
   //   ||            ||       `|| ||| || ||     ||
   //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^