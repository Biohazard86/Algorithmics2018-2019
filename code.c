// CODIGO EN C 

// David davidbarrios@usal.es



/*
Se ha diseñado un juego que contiene varios niveles, en cada uno de ellos, el personaje del mismo deberá viajar por un mapa cuadrado, que consta de NxN habitaciones, por las cuales puede viajar. Cada habitación puede tener hasta 4 puertas, para poder ir a las 4 habitaciones contiguas. Para saber las puertas disponibles se dispone de un array de NxN enteros llamado puertas, de modo que puertas(X,Y) es una tupla de 4 booleanos indicando las puertas existentes estando en la habitación (X,Y), en orden (Norte, Sur, Este, Oeste), por ejemplo: puertas(3,4)=(Verdad,Falso,Verdad,Falso), significa que desde habitación situada en (3,4) sólo podemos ir en dirección Norte, es decir a (3,3) y Este, es decir a (4,4). Las puertas pueden ser unidireccionales, es decir podría haber una puerta para ir de (3,4) a (4,4), pero haberla para ir de (4,4) a (3,4).

Además en el mapa hay K<=32 tesoros que almacenaremos en un array llamado tesoro y que contendrá ternas de enteros, en el que los dos primeros valores serán las coordenadas, y el último el valor, es decir tesoro(I)=(X,Y,V) quiere decir que el I-ésimo tesoro esta en la habitación (X,Y) y tiene valor V. Obviamente, sólo se obtiene el tesoro la primera vez que se pasa por la habitación que lo contiene.

Para pasar al siguiente nivel hay que viajar de la habitación (1,1) a la (N,N), recolectando como mínimo tesoros por valor de M, y pasando como máximo por S habitaciones. Y deseamos realizar un programa que nos ayude a diseñar los niveles, calculándonos el número mínimo de habitaciones a visitar para resolver cada nivel (es decir, para conseguir tesoros cuya suma de valores sea mayor o igual que M), para poner un valor de S que permita resolverlo. 

1. Desarrollar un algoritmo empleando programación dinámica para resolver el problema, es decir, dado un mapa, con sus puertas y tesoros, y un valor de M, nos debe calcular el valor de S mínimo que debemos poner para resolverlo.
2. Calcular el tiempo que precisa para ejecutarse en función de N y K.
3. Traducir el algoritmo anterior a C, incluyendo una función para leer un fichero de disco con el mapa y los tesoros, que será de la forma siguiente:
- la primera línea contendrá los valores de N, K y M. Recuérdese que K<=32.
- las NxN líneas siguientes contendrán los valores de puertas(X,Y) por filas, es decir, primero el de (1,1), (1,2), ..., (1,N), (2,1), ..., (2,N), ... ,(N,1), ..., (N,N).
- las K líneas siguientes contendrán los tesoros en la forma: X, Y, V, siendo (X,Y) la habitación que lo contiene y V su valor.

Observación 1: La tabla a construir podría ser MINS(X,Y,B) que contendría el mínimo valor de S para que podamos viajar de (1,1) a (X,Y) obteniendo como mínimo los tesoros indicados en B, que es un valor binario, compuesto de K bits, cada uno de los cuales, si es 1, nos dice que en esa solución hemos usado el tesoro correspondiente. La razón de incluir B es que para resolver el problema final, no sabemos cuales son los tesoros que minimizan S, por que hay que hacer todos los casos posibles. Por ejemplo, supongamos que hay 4 tesoros, de valores (5,2,4,6), MINS(7,3,1100)=15 significa que el número mínimo de habitaciones a visitar para ir de (1,1) a (7,3), obteniendo sólo los 2 primeros tesoros (es decir, el valor de M obtenido sería 7) es de 15.


Observación 2: Dado que B tiene que tomar todos los valores posibles de un número de K bits, que son 2^K, la tabla se puede hacer gigantesca, y puede que no tengamos memoria para contenerla, de modo que en la práctica habría que tomar valores de K, no muy grandes, por ejemplo hasta 10. Obviamente cuanto mayor sea K, mayor será el tiempo para rellenar la tabla, lo cual es una razón más para no poner valores grandes para K.

Observación 3: Una vez que tengamos la tabla, el valor mínimo de S se obtendría considerando todas las colecciones de tesoros posibles cuyos valores sumen al menos M, y hacer el mínimo de los correspondientes valores de la tabla, eso es lo que debe devolver el programa como resultado, no hace falta que nos diga cuales son los tesoros correspondientes, o la ruta para obtener ese valor, sólo debe calcular el valor de S.
*/

// Librerias incluidas en el codigo:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define ColorRojo "\x1b[31m"	// Definimos un color rojo (Que usaremos para el tablero y mostrar datos por pantalla)
#define ColorVerde  "\x1b[32m"	// Definimos un color verde (Que usaremos para el tablero y mostrar datos por pantalla)
#define ColorAzul   "\x1b[34m"	// Definimos un color azul (Que usaremos para el tablero y mostrar datos por pantalla)
#define ColorMagenta "\x1b[35m"	// Definimos un color Magenta (Que usaremos para el tablero y mostrar datos por pantalla)
#define ColorCyan   "\x1b[36m"	// Definimos un color cyan (Que usaremos para el tablero y mostrar datos por pantalla)

#define salto printf("\n")	// Definimos un salto de carro para asi ser mas facil una correcta presentacion por pantalla

#define Error printf("\n####################\n")	// Definimos un printf con un codigo de error

typedef struct Tesoro	// Definimos una estructura TESORO con los valores enteros "valor, ID, trofeo"
{
	int valor, ID, trofeo;
}TESORO; 

typedef struct Puertas	// Definimos una estructura PUERTAS (Como se requiere en el enunciado)
{
	int **aux;
	int binario;
}PUERTA;


// ####################  PROTOTIPOS DE LAS FUNCIONES:
void Puertas( int *a); // Se le pasa un puntero y como resultado imprime por pantalla las puertas disponibles.

void Moverse(int *i, int *j,char *pos, int *aux1);	// Recibe 4 punteros a enteros

int* Binario(int n);	 

int* Reserva1D(int N);	// Reserva n enteros en memoria

int** Reserva2D(int N);	// Esta funcion esta comentada.

int* EvaluarContiguas( int **a, int i, int j, int n);	// Evalua las habitaciones contiguas a la actual.

void Jugada( int **tablero, int N, TESORO **tesoro);	// 

TESORO** Premios(TESORO **premio, int N);	// Se usa para llevar la cuenta de los premios que llevamos 

void ImprimirPremios(TESORO **premio, int **tabla, int N);	// Para imprimir los premios

void Calificacion(TESORO **tesoro ,int N);	// La calificacion que llevamos 

int int_pow(int base, int exp); // Transforma la funcion POW (en math.h) a una funcion que trabaja con enteros. La usamos en la funcion de abajo, BinarioDecimal

int BinarioDecimal( int *codigo);

int* Abierta(int a);

int** GenerarTabla(int n);	// Genera la tabla de la partida.

int** PilotoAutomatico(int **a, int i, int j, int I, int J, int N);


int** Recolector(int **auxTablero, TESORO **tesoro, int N, int contador); 

// ################## FIN DE LOS PROTOTIPOS.

int main(int argc, char const *argv[])
{
	double tiempo = 0; 

	clock_t begin = clock(); 
	TESORO **tesoro = NULL;
	srand( (unsigned) time(NULL));
	int **tablero = NULL;
	int N;


		printf("Introduzca el valor del tablero:\n");
		scanf("%d", &N);

		 tablero = Reserva2D(N+2);

		 tesoro = (TESORO**)malloc((N+2)*sizeof(TESORO*));
		 	for( int i = 0; i<N+2; i++)	tesoro[i] = (TESORO*)malloc((N+2)*sizeof(TESORO));

		tesoro = Premios(tesoro, N+2);

		//tablero = Recolector(tablero, tesoro, N+2,1);
		//PilotoAutomatico(tablero, I,J, N+2);
		Jugada( tablero, N+2, tesoro);

		

			free(tesoro);
			free(tablero);

			clock_t end = clock();

			tiempo += (double) (end - begin)/ CLOCKS_PER_SEC; 

			printf("\n-------------------------\n");
			printf("Tiempo de ejecucion = %f segundos \n", tiempo);
printf("\n");
return 0;
}


int** Reserva2D(int N)
{
	int **aux = (int**)calloc(N, sizeof(int*));
		for( int i = 0; i<N; i++)	aux[i] = (int*)calloc(N, sizeof(int));
return aux;			
}


int** GenerarTabla(int N)
{
	int **aux = Reserva2D(N);

	int Norte[] = {1,2,3,4,5,6,7};
	int	Sur[] = {1,2,3,8,9,10,11};
	int Este[] = {1,4,5,8,9,12,13};
	int Oeste[] = {2,4,6,8,10,12,14};

	for( int i = 0; i<N; i++)
	{
		for( int j = 0; j<N; j++)
		{	
			if( i != 0 && i != N-1 && j != 0 && j != N-1)
			{
				aux[i][j] = rand()%16;
				if( i ) aux[1][j] = Norte[ rand()%7 ];
				
				if( i == N-2 ) aux[N-2][j] = Sur[ rand()%7 ];

				if( j == N-2) aux[i][N-2] = Este[ rand()%7 ];

				if( j ) aux[i][1] = Oeste[ rand()%7 ];

				aux[1][1] = 6;

				aux[1][N-2] = 5;
				aux[N-2][1] = 10;
				aux[N-2][N-2] = 9;

			}
		}		
	}
return aux;
}

void Mapa( int N, int **aux)
{
	for( int i = 0; i<N; i++)
	{
		for( int j = 0; j<N; j++)
		{	

			if ( !aux[i][j] )
			{
				if( !j ) printf( ColorVerde " | " ColorVerde );
					else if( !i ) printf(ColorVerde" - "ColorVerde);
						else if( j == N-1 )	printf(ColorVerde" | "ColorVerde);
							else if( i == N-1 ) printf(ColorVerde" - "ColorVerde);
								else printf(ColorAzul" X "ColorAzul);


			}else if( aux[i][j] == 2  ) printf(ColorRojo " X " ColorRojo);
				else if ( aux[i][j] == 3  ) printf(ColorMagenta " $ " ColorMagenta);
					else if( aux[i][j] == 4 ) printf(ColorCyan " X " ColorCyan);
		}		
		printf("\n");
	}
}




void Puertas( int *a)
{
	int i1;
	char direccion[][10] = { "Norte","Sur", "Este", "Oeste"}, *auxc;

	for( int i = 0; i<4; i++)
	{
		 i1 = strlen(direccion[i]);
		 auxc = malloc((i1++)*sizeof(char));

		 strcpy( auxc, direccion[i]);
	}

		printf("\nLas puertas disponibles son : \n\n");
			for( int i = 0; i<4 ; i++ )
			{
				if( a[i] ) printf("%s\n", direccion[i]);
			}
}

void Moverse(int *i, int *j,char *pos, int *aux1)
{
		if( *pos == 'W' || *pos == 'w' ) 
			{
				*i = *i - 1;
					*aux1 = 0;

			}else if ( *pos == 'S' || *pos == 's' )
					{
						*i = *i + 1;
						*aux1 = 1;

					}else if( *pos == 'D' || *pos == 'd' )
							{
								*j = *j + 1; 
								*aux1 = 2;

							}else if( *pos == 'A' || *pos == 'a' ) 
									{
										*j = *j - 1; 
										*aux1 = 3;
									}
system("clear");
}


int* Binario(int n)
{
	int i = 0, *a = calloc(5,sizeof(int));
	do{
		a[3 - i] = n % 2;
		n = n/2;
		++i;
	}while(  n > 0 );
return a;
}

int* Reserva1D(int N)
{
	int *aux = calloc(N,sizeof(int));
return aux;
}


int* EvaluarContiguas( int **a, int i, int j, int n)
{
	int *aux0 = Reserva1D(n);
	int *c = Reserva1D(n), *a0 = Reserva1D(n), *a1 = Reserva1D(n), *a2 = Reserva1D(n), *a3 = Reserva1D(n);

		c = Binario( a[i][j] );

 		a0 = Binario( a[i - 1][j] );// Norte
 		a1 = Binario( a[i + 1][j] );// Sur

 		a2 = Binario( a[i][j + 1] );// Este
 		a3 = Binario( a[i][j - 1] );// Oeste 


				 if( c[0] && a0[1] )	aux0[0] = 1; // norte
				 	else aux0[0] = 0;

				 if( c[1] && a1[0] )	aux0[1] = 1; // sur
				 	else aux0[1] = 0;

				 if( c[2] && a2[3] )	aux0[2] = 1; // este
				 	else aux0[2] = 0;

			 	 if( c[3] && a3[2] )	aux0[3] = 1; // oeste
			 	 	else aux0[3] = 0;
return aux0;
}

int* Abierta(int a)
{
	int *aux = Reserva1D(4);
	aux = Binario(a);
return aux;
}

TESORO** Premios(TESORO **premio, int N)
{
	int K = 0,i,j, contador = 1, aleatorio, aux = 1;

	do
	{
		 i = rand()%N;
		 j = rand()%N;
			if( i != 0 && i != N-1 && j != 0 && j != N-1)
	 		{
				premio[i][j].valor = 1;

				aleatorio = rand()%(5 - 1 + 1) + 1;
				
				if( aleatorio != 0)	premio[i][j].trofeo = aleatorio;			
			}
		K++;
	}while( K <= 32);
				
	for(int i = 0; i< N; i++)
	{
		for( int j = 0; j < N; j++)
		{
			if( i != 0 && i != N-1 && j != 0 && j != N-1)
			{
							aux = contador;
 				if( premio[i][j].trofeo != 0)
 				{
 						premio[i][j].ID = contador;
 						contador++;
 				}else{
 						premio[i][j].ID = aux;
 						contador = aux;
 				}
 			} 
		}
	}
return premio;
}


void Calificacion(TESORO **tesoro ,int N)
{
	int **aux = (int**)calloc(N+2,sizeof(int*));
			for( int i = 0; i<N+2; i++)	aux[i] = (int*)calloc(N+2, sizeof(int));
 
 int **aux1 = (int**)calloc(N+2,sizeof(int*));
			for( int i = 0; i<N+2; i++)	aux1[i] = (int*)calloc(N+2, sizeof(int));
	int contador = 0, a = 0, b = 0, T = 0;

		for( int i = 0; i< N; i++)
		{
			for(int j = 0; j < N; j++)
			{
				if( tesoro[i][j].valor > 1)
				{
					aux[a++][b++] = contador++;
					T   = T + tesoro[i][j].trofeo;
					aux1[i][j] = tesoro[i][j].ID; 
				}	
			}
		}	
			printf("\nPremios encontrados = %d ---- trofeos = %d \n Ha encontrado los tesoros : ", contador ,T);

	for(int i = 0; i<N; i++)
		for( int j = 0; j < N; j++)	if( aux1[i][j] ) printf("%d ",aux1[i][j] );
printf("\n");
}

int int_pow(int base, int exp)
{
    int result = 1;
    while (exp)
    {
        if (exp & 1)
           result *= base;
        exp /= 2;
        base *= base;
    }
    return result;
}


int BinarioDecimal( int *codigo)
{
	int suma = 0, i = 3, temporal;
	
	while( i >  -1)
	{
		
		suma  = suma + codigo[i] * (temporal=int_pow(2,3-i));
		i--;
	}
return suma;
}


void ImprimirPremios(TESORO **premio, int **tabla, int N)
{
	printf("\nLa tabla de premios es : \n");
	for(int i = 0; i<N; i++)
	{
		for( int j = 0; j < N; j++)
		{
			printf("%d, ", premio[i][j].valor);
		}
	printf("\n");
	}

salto;salto;
	printf("La tabla es : \n");

	for(int i = 0; i<N; i++)
	{
		for( int j = 0; j < N; j++)
		{
			printf("%d, ", tabla[i][j]);
		}
	printf("\n");
	}
salto;salto;
printf("Los Trofeos son: \n");

	for(int i = 0; i<N; i++)
	{
		for( int j = 0; j < N; j++)
		{
			printf("%d, ", premio[i][j].trofeo);
		}
	printf("\n");
	}
	salto;salto;

printf("El array trofeo es: : \n");

	for(int i = 0; i<N; i++)
	{
		for( int j = 0; j < N; j++)
		{
			if( premio[i][j].trofeo != 0)	printf(" Premio[%d] = {%d,%d,%d}\n",premio[i][j].ID, i,j,premio[i][j].trofeo);
		}
	}
}

void Jugada( int **tablero, int N, TESORO **tesoro)
{
	int i , j , aux = 0;
	i = j = 1;

	//int *aux1 = Reserva1D(4);
	int auxi = 0, auxj = 0;

	int *llave = calloc(4, sizeof(int));

	int **accesibles =  (int**)calloc(N,sizeof(int*));
			for( int i = 0; i<N; i++)	accesibles[i] = (int*)calloc(N, sizeof(int));

				int **auxTablero =  (int**)calloc(N,sizeof(int*));
			for( int i = 0; i<N; i++)	auxTablero[i] = (int*)calloc(N, sizeof(int));
	char letra;

		
	do{
			accesibles = GenerarTabla(N);

			tablero[1][1] = auxTablero[1][1]=4;

				if( tesoro[1][1].valor )
					{ 
						auxTablero[1][1] = 3;
						tesoro[1][1].valor += 1;
						Mapa(N,auxTablero);
						Calificacion(tesoro,N);
					}	else Mapa(N,auxTablero);	
				
				llave = Abierta( accesibles[1][1] );
			do{
			auxj = auxi = 0;	
					if( i != 0 && i != N-1 && j != 0 && j != N-1)
					{

						printf("\n---------------------------------\n");
						Puertas(llave);
						printf("\n---------------------------------\n");
	
						scanf("%c", &letra);
						printf("Posicion siguiente(W, A, S, D): \n");
						scanf("%c", &letra);

						auxj = j, auxi = i;

			 			Moverse(&i, &j, &letra, &aux);
			 			
			 			tablero = auxTablero;
			 			if( llave[aux]  )
			 			{
			 				llave = Abierta( accesibles[i][j] );
	
			 	 			tablero[ i ][ j ] = 2;// rojo
			 	 			auxTablero[ i ][ j ] = 2;// rojo

			 	 			if( tesoro[i][j].valor )
			 	 			{
			 	 				auxTablero[i][j] = 3; // lila
			 	 				tesoro[i][j].valor += 1;
			
			 	 			Mapa(N, auxTablero);
							//	salto;salto;
			 	 			}else{
			 	 			 tablero[i][j] = 2;// rojo

			 	 			Mapa(N, tablero);	

							tablero[i][j] = 4;
			 	 			}				 	 	
			 	 			Calificacion(tesoro,N);
			 			}else{
							llave = Abierta(accesibles[auxi][auxj]);

			 				Mapa(N,tablero);
			 				Error;
			 				printf("La puerta no esta disponible\n\a\a");
			 				Error;
			 				i = auxi; j = auxj;
			 			}	
					}else{
						printf("Fuera del tablero\n");
						break;
					} 	
			 	}while( tablero != NULL );
		}while( tesoro != NULL);
	}
	
// ############################ PILOTO AUTOMATICO Y RECOLECTOR ESTAN IMPLEMENTADAS MAS ABAJO.	


// int** PilotoAutomatico(int **a, int *i, int *j, int I, int J, int N)
// {
// 	//int i = 1, j = 1;

// // 	do
// // 	{
// // 		if( I <= J)
// // 		{

// // 			do{
// // 					a[i][j++] = 3;
// // 					printf("%d",j );
// // 			}while( j < J);
// // 				salto;

// // 			do{
// // 				a[i++][j] = 3;
// // 				printf("%d",i );

// // 			}while( i < I);
// // salto;
// // 		}else if( I > J)
// // 			{

// // 				do{
// // 					a[i++][j] = 3;
// // 					printf("%d",i );
// // 				}while( i < I);
// // 				salto;
// // 				do{
// // 						a[i][j++] = 3;
// // 					printf("%d",j );
// // 				}while( j < J);
// // 				salto;
// // 			}
// // 	}while( i != I && j != J );

// 	do
// 	{
// 		if( I <= J)
// 		{

// 			do{
// 					a[ *i ][(*j)++] = 3;
// 					printf("%d",*j );
// 			}while( *j < J);
// 				salto;

// 			do{
// 				a[ (*i)++][*j] = 3;
// 				printf("%d",*i );

// 			}while( *i < I);
// salto;
// 		}else if( I > J)
// 			{

// 			do{
// 				a[ (*i)++][*j] = 3;
// 				printf("%d",*i );

// 			}while( *i < I);
// salto;
// 				salto;
// 			do{
// 					a[ *i ][(*j)++] = 3;
// 					printf("%d",*j );
// 			}while( *j < J);
// 				salto;
// 			}
// 	}while( *i != I && *j != J );
// 	printf("La tb es \n");
// 	for(int i = 0; i<N; i++)
// 	{
// 		for( int j = 0; j < N; j++)
// 		{
// 			printf("%d ",a[i][j]);
// 		}
// 		salto;
// 	}
// salto;salto;
// 	// 	for(int i = 0; i<N; i++)
// 	// {
// 	// 	for( int j = 0; j < N; j++)
// 	// 	{
// 	// 		printf("%d,%d ",i,j);
// 	// 	}
// 	// 	salto;
// 	// }
// return a;
// }



// int** Recolector( TESORO **tesoro, int I, int J, int N, int inicial, int jinicial, int i1)
// {
// 	int **aux = Reserva2D(N);
// 	int **auxTablero = Reserva2D(N);

// 	printf("La tabla de premios es \n");
// 	int i , j;
// 	for(int i = 0; i<N; i++)
// 	{
// 		for( int j = 0; j < N; j++)
// 		{
// 			printf("%d ",tesoro[i][j].valor);
// 		}
// 		salto;
// 	}salto;salto;


// 	if( I == N && J == N)
// 	{
// 		return auxTablero;
			
// 	}else{
// 		printf(" inicial %d\n ", inicial);
// 		 // i = j = 1;

// 			printf("La nueva i = %d\n", i);

// 			//aux = auxTablero;

// 			// for(int a = 0; a<N; a++)
// 			// {
// 			// 	for( int b = 0; b < N; b++)
// 			// 	{
// 			// 			if( tesoro[ a ][ b ].valor )
// 			// 			{
// 			// 				aux[a][b] = ++i1; 		
// 			// 			}
// 			// 	}salto;salto;
// 			// }


// 			for(int a = 0; a<N; a++)
// 			{
// 				for( int b = 0; b < N; b++)
// 				{
// 					if( tesoro[a][b].ID )	
// 					{
// 						I = a; J = b;
// 					}
// 				}
// 			}

// 			printf(" I = %d J = %d\n",I,J );
// 			i = 1; j = 1;
// 			auxTablero =  PilotoAutomatico(aux, &i, &j, I, J, N);
// 			return auxTablero;


	

// 				i = I; j = J;
// 			}//else	Recolector( tesoro, I+1, J+1, N, inicial+ 1, jinicial+1);
		
// 		//Recolector( tesoro, i+1, j+1, I,J);
// 		//aux = Recolector( tesoro, I+1, J+1, N, inicial+ 1, jinicial+1, 1);

	
// }

int** PilotoAutomatico(int **a, int i, int j, int I, int J, int N)
{
	if( i == I && j == J ) return NULL;
	else do
	{
		if( I <= J)
		{

			do{
					a[i][j++] = 3;
					printf("%d",j );
			}while( j < J);
				salto;

			do{
				a[i++][j] = 3;
				printf("%d",i );

			}while( i < I);
salto;
		}else if( I > J)
			{

				do{
					a[i++][j] = 3;
					printf("%d",i );
				}while( i < I);
				salto;
				do{
						a[i][j++] = 3;
					printf("%d",j );
				}while( j < J);
				salto;
			}
	}while( i != I && j != J );

return a;
}
int** Recolector(int **auxTablero, TESORO **tesoro, int N, int contador)
{
	int **aux = Reserva2D(N);
//	int **auxTablero = Reserva2D(N);

	int a,b,i1,j1;
	// for(int i = 0; i<N; i++)
	// 	for( int j = 0; j < N; j++)
	// 		if( tesoro[i][j].ID == 5 )
	// 		{
	// 			a = i; b = j;
	// 		}
	// auxTablero 

			printf("contador %d\n", contador);
	if( auxTablero == NULL) printf("tonto\n");
		else return auxTablero;	

	if( contador == N ) return auxTablero;
		else{

			i1 = a; j1 = b;

			for(int i = 0; i<N; i++)
				for( int j = 0; j < N; j++)
			if( tesoro[i][j].ID ==  contador)
			{
				a = i; b = j;
			}

			if( tesoro[a][b].ID != 0 )
			auxTablero = PilotoAutomatico( auxTablero, i1,j1, a ,b, N);


			for(int i = 0; i<N; i++)//tesoro[i][j].valor
			{
				for( int j = 0; j < N; j++)
				{
					printf("%d ",auxTablero[i][j]);
				}
				salto;
			}salto;salto;
		return Recolector(auxTablero, tesoro,N, contador+1);
		}
}
