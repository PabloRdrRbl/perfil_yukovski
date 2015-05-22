#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 100 // Número de puntos de la circunferencia
#define M 5 // Número de puntos para la malla


float * linspace(float x0, float x, int n)
{
	int i;
	float parte;

	float *vector; // Vector de salida del espacio
	vector = (float *) malloc(n * sizeof(float)); // Memomoria reservada para el vector

	vector[0] = x0; // Valor incial
	vector[n] = x; // Valor final

	parte = (x - x0) / (float) n; // Intervalo

	for (i=1; i < n; i++)
	{
		vector[i] = vector[i-1] + parte;
	}

	return(vector);
}


int meshgrid(float ** xx, float ** yy)
{
	int i, j;

	float * rango;
	rango = (float *) malloc(M * sizeof(float));

	rango = linspace(0, 5, M);

	for (i = 0; i < M; i++)
	{
		xx[i] = rango;
	}

	for (i = 0; i < M; i++)
		for (j = 0; j < M; j++)
		{
			yy[i][j] = rango[i];
		}

	return(0);
}	

int flatten(float ** matriz, float * vector)
{
	int i, j;

	for (i = 0; i < M; i++)
		for (j = 0; j < M; j++)
		{
			vector[i*M + j] = matriz[i][j];
		}

	return(0);
}

// nxn
int imprimir_matriz(float ** matriz)
{
	int i, j;

	for (i = 0; i < M; i++)
	{
		for (j = 0; j < M; j++)
		{
			printf("%f ", matriz[i][j]);
		}

		printf("\n");
	}
	
	return(0);
}

int imprimir_vector(float * vector)
{
	int i;

	for (i = 0; i < M*M; ++i)
	{
		printf("%f ", vector[i]);
	}

	return(0);
}


int main(int argc, char const *argv[])
{
	int i;

	float ** xx;
	xx = (float **) malloc(M * sizeof(float *));

	for (i=0; i < N; i++)
		xx[i] = (float *) malloc(M * sizeof(float));


	float ** yy;
	yy = (float **) malloc(M * sizeof(float *));

	for (i=0; i < N; i++)
		yy[i] = (float *) malloc(M * sizeof(float));

	float * xxflat;
	xxflat = (float *) malloc(M * M * sizeof(float));

	//float * yyflat;
	//yyflat = (float *) malloc(M * M * sizeof(float));

	meshgrid(xx, yy);

	imprimir_matriz(xx);
	printf("\n");
	imprimir_matriz(yy);

	flatten(xx, xxflat);

	imprimir_vector(xxflat);


	return 0;
}