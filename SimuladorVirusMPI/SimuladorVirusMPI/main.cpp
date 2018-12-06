#include <mpi.h> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <random>
#include <time.h>

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
	/*-------------------------------------Recuperación de Datos--------------------------------------*/
	MPI_Bcast(&poblacion, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&dimension, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&infInicial, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&duracion, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&probInf, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&probRec, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	/*-------------------------------------Recuperación de Datos--------------------------------------*/


	/*-------------------------------------Inicializar------------------------------------------------*/
	poblacion *= 4; //La poblacion debe ser en cuestiones de tamanno, cuatro veces mas grande, pues una persona son 4 espacios del arreglo.
	int local_n = poblacion / cnt_proc;
	int *personasLocal;
	personasLocal = (int*)malloc(poblacion * sizeof(int)); // Para un array de tamaño población
	int *personas;
	personas = (int*)malloc(poblacion * sizeof(int)); // Para un array de tamaño población
	/*Matriz*/
	int **cantInfc = (int **)malloc(poblacion/4 * sizeof(int*));
	for (int i = 0; i < poblacion / 4; i++) cantInfc[i] = (int *)malloc(poblacion / 4 * sizeof(int));

	if (mid == 0) {
		int random;
		pair <int, int> pos;
		/*
		-X
		-Y
		-Estado
		-Tics enfermo
		*/
		///HACER LAS VARAS DEL MAE PARALELIZADO
		int infectados = (poblacion*(infInicial))/4; //PRUEBA 
		cout << "Infectados:  " << infectados << endl;
		default_random_engine gen;
		uniform_int_distribution<int> distribution(0, dimension - 1);
		for (int i = 0; i < poblacion; i+=4) {
			do {
				pos.first = distribution(gen);
				pos.second = distribution(gen);
				personas[i] = pos.first; 
				personas[i + 1] = pos.second;
			} while (cantInfc[pos.first][pos.second] == 1);//Hace que las psiciones no sean iguales al inicio
			cantInfc[pos.first][pos.second] = 1;

			/*Enfermar al 10%*/
			if (infectados > 0) {
				personas[i + 2] = 1; // Estado
				infectados--;
			}
			else {
				personas[i + 2] = 0; // No reuerdo los estados
			}
			/*
			Estados:
			0-Sano
			1-Enfermo
			2-Inmune
			3-Paul Walker
			*/
			personas[i + 3] = 0; //Tics [Debe arrancar en 0]
		}

		//MPI_Allgather(personas, poblacion, MPI_INT, personasLocal, poblacion, MPI_INT, MPI_COMM_WORLD);http://mpitutorial.com/tutorials/mpi-scatter-gather-and-allgather/
		
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Scatter(personas, local_n, MPI_INT, personasLocal, local_n, MPI_INT, 0, MPI_COMM_WORLD);
	//cout << "Soy el proceso: " << mid << endl;

#  ifdef DEBUG 
	for (int i = 0; i < 100; i += 4) { // imprime el vector de cada hilo
		cout << " " << i << " " << "x: " << personasLocal[i] << " y: " << personasLocal[i + 1] << " Estado: " << personasLocal[i + 2] << " Semanas: " << personasLocal[i + 3] << " Proceso: " << mid << endl;
	}
# endif

	//----------------------Inicialización de la MAtriz---------------------------------------------------------
	int *enfermos;
	enfermos = (int*)malloc(dimension * dimension *  sizeof(int *)); //matriz de enfermos sexuales
	
	int x, y, i = 0; //posición de las personas
	
	while(i < dimension){
		x = personasLocal[i];
		y = personasLocal[i + 1];
		//cout <<"Impreso por  " << mid <<" Mi x es " << x << " Mi y es " << y << endl;
		i += 3;
	}

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
pair<int, int> generarPosRandom(int tam/*, int[tam][tam] enfermos*/) {//Pasar la maatriz por parámetro

	pair<int, int> pos;
	//default_random_engine gen;
	//uniform_int_distribution<int> distribution(0, tam - 1);
	//pos.first = distribution(gen);
	//pos.second= distribution(gen);
	/*srand(time(NULL));
	pos.first = rand() % tam;
	pos.second = rand() % tam;*/

	/*do {
		pos.first = rand() % tam;
		pos.second = rand() % tam;
	} while (enfermos[pos.first][pos.second] == 0 || enfermos[pos.first][pos.second] == 1);*/

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