#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 100 // Número de puntos de la circunferencia

/*
RECIBO: 
float * dperfil
{centro[0], centro[1], a, beta, b, caso}


*/


// PI queda definido como M_PI al importar math.h
// #define M_PI 3.14159265358979323846

/* La función devuelve las n divisiones equiespaciadas del intervalo dado */ 
float * linspace(float x0, float x, int n)
{
	int i;
	float parte;

	float *vector; // Vector de salida del espacio
	vector = (float *) malloc(n * sizeof(float)); // Memomoria reservada para el vector
												  // TODO_p: free(vector)

	vector[0] = x0; // Valor incial
	vector[n] = x; // Valor final

	parte = (x - x0) / (float) n; // Intervalo

	for (i=1; i < n; i++)
	{
		vector[i] = vector[i-1] + parte;
	}

	return(vector);
}

// TODO_p = meter todo con el vector dperfil

/* Devuelve matriz con todos los puntos de la circunferencia */
float ** matriz_circunferencia(float * centro, float a, float ** circunferencia)
{
	int i;

	// Valores de ángulo t para las ecuaciones paramétricas
	float * valores_t;
	float t;
	valores_t = (float *) malloc(N * sizeof(float));

	valores_t = linspace(0, 2*M_PI, N);


	// Cálculo de cada punto para cada valor de t
	// Almacenamiento en lista

	for (i = 0; i < N; ++i)
	{
		t = valores_t[i];
		circunferencia[i][0] = centro[0] + a * cos(t);
		circunferencia[i][1] = centro[1] + a * sin(t);
	}

	return(0);
}


/* Imprime una lista de puntos (matriz nx2) */
int imprimir_circunferencia(float ** circunferencia)
{
	FILE * file_circunferencia;
	file_circunferencia = fopen("pts_circun.dat", "w+");

	if (file_circunferencia == NULL)
	{
		printf("Error al abrir el archivo\n");
		return(0);
		//TODO_p: a dónde vamos, al menú
	}

	int i;

	for (i=0; i < (N); i++)
	{
		fprintf(file_circunferencia, "%f %f\n", circunferencia[i][0], circunferencia[i][1]);
	}

	fprintf(file_circunferencia, "%f %f\n", circunferencia[0][0], circunferencia[0][1]); // Termina con el primer punto

	fclose(file_circunferencia);

	return(0);
}


int main()
{
	int i;

	/**************************/
	/* ESTOS DATOS LOS RECIBO */
	/**************************/

	// Coordenadas del centro
	float * centro;
	centro = (float *) malloc(2 * sizeof(float));

	centro[0] = -0.3; // Xc
	centro[1] = 0.2; // Yc

	// Radio
	float a = 1;

	// b donde se corta la circunferencia con el eje X
	float beta; // Ángulo donde y es igual a 0
	float b; 

	beta = asin(centro[1]/a);
	b = centro[0] + a * cos(beta);


	float ** circunferencia;
	circunferencia = (float **) malloc(N * sizeof(float *)); // Reserva de memoria
																	    // Para cada vector

	for (i=0; i < N; i++)
	{
		circunferencia[i] = (float *) malloc(2 * sizeof(float)); // Reserva de memoria
	}																   // Para las dos coordenadas de cada vector

	matriz_circunferencia(centro, a, circunferencia);

	imprimir_circunferencia(circunferencia);


	return(0);
}