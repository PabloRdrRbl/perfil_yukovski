#include <stdio.h> //COMENTARIOS: XX.flaten no las usas? es necesario imprimir la matriz psi?
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#define M 50 


// Imprime matriz MxM float
int imprimir_matriz_float(float ** matriz)
{
	int i, j;

	for (i = 0; i < M; i++)
	{
		for (j = 0; j < M; j++)
		{
			printf("%f", matriz[i][j]);
		}

		printf("\n");
	}
	
	return(0);
}

// Imprime matriz MxM double
int imprimir_matriz_double(double ** matriz)
{
	int i, j;

	for (i = 0; i < M; i++)
	{
		for (j = 0; j < M; j++)
		{
			printf("%lf", matriz[i][j]);
		}

		printf("\n");
	}
	
	return(0);
}

// Imprime matriz MxM complex
int imprimir_matriz_compleja(complex double ** matriz)
{
	int i, j;

	for (i = 0; i < M; i++)
	{
		for (j = 0; j < M; j++)
		{
			printf("%f %f", creal(matriz[i][j]), cimag(matriz[i][j]));
		}

		printf("\n");
	}
	
	return(0);
}


/* La función devuelve las n divisiones equiespaciadas del intervalo dado */ 
float * linspace(float x0, float x, int n)
{
	int i;
	float parte;

	float *vector; // Vector de salida del espacio
	vector = (float *) malloc(n * sizeof(float)); // Memoria reservada para el vector

	vector[0] = x0; // Valor incial
	vector[n] = x; // Valor final

	parte = (x - x0) / (float) n; // Intervalo

	for (i=1; i < n; i++)
	{
		vector[i] = vector[i-1] + parte;
	}

	return(vector);
}

int meshgrid(float ** xx, float ** yy, complex double ** tt)
{
	int i, j;

	int xmax = 5;

	float * rango;
	rango = (float *) malloc(M * sizeof(float));

	rango = linspace(-xmax, xmax, M);

	for (i = 0; i < M; i++)
	{
		xx[i] = rango;
	}

	for (i = 0; i < M; i++)
		for (j = 0; j < M; j++)
		{
			yy[i][j] = rango[i];
		}

	for (i = 0; i < M; i++)
		for (j = 0; j < M; j++)
		{
			tt[i][j] = xx[i][j]+yy[i][j]*I;
		}

	return(0);
}	

int calculo_flujo(complex double ** tt, double ** psi)
{
	int i,j;

	complex double t0 = 0.2 + 0.3 * I;

	double T = 3.76991118431; //Valor del flujo
	double U = 1;
	double alpha = 0;
	double R = 1;

	for (i = 0; i < M; ++i)
		for (j = 0; j < M; ++j)
		{
			psi[i][j] = cimag(U * ((tt[i][j]-t0)*cexp(-alpha*I)+pow(R,2)/(tt[i][j]-t0)*cexp(-alpha*I)) + I * T/(2*M_PI)*clog(tt[i][j]-t0));
		}

	return 0;
}


int imprimir_flujo(float ** xx, float ** yy, double ** psi)
{
	FILE * file_flujo_cilindro;
	file_flujo_cilindro = fopen("pts_flujo_cilindro.dat", "w+");

	int i, j;

	for (i = 0; i < M; ++i)
		for (j = 0; j < M; ++j)
		{
			fprintf(file_flujo_cilindro, "%f %f %f\n", xx[i][j], yy[i][j], psi[i][j]); // Escribe cada punto (fila de la matriz) en el archivo
		}

	fclose(file_flujo_cilindro); // Cierre del archivo

	return(0);
}

int arregla_malla(complex double **tt)
{
	complex double t0 = 0.2 + 0.3 * I;
	double R = 1;

	int i, j;
	for (i = 0; i < M; ++i)
		for (j = 0; j < M; ++j)
		{
			if (cabs(tt[i][j]-t0) < 0.95 * R)
				tt[i][j]=0;
		}

	return 0;
}

int nueva_malla (float **xxtau, float **yytau, complex double ** tt)
{
	complex double tau;
	double a = 1;

	int i, j;

	for (i = 0; i < M; ++i)
		for (j = 0; j < M; ++j)
		{
			tau = tt[i][j] + pow(a,2)/tt[i][j];
			xxtau[i][j] = creal(tau);
			yytau[i][j] = cimag(tau);
		}

	return 0;
}

int flujo(/*float * dperfil ,float * opf*/) // TODO_p: al juntar unir las opciones
{
	int i;

	float ** xx;
	xx = (float **) malloc(M * sizeof(float *));
	for (i=0; i < M; i++)
		xx[i] = (float *) malloc(M * sizeof(float));

	float ** yy;
	yy = (float **) malloc(M * sizeof(float *));
	for (i=0; i < M; i++)
		yy[i] = (float *) malloc(M * sizeof(float));

	complex double ** tt;
	tt = (complex double **) malloc(M * sizeof(complex double *));
	for (i=0; i < M; i++)
		tt[i] = (complex double *) malloc(M * sizeof(complex double));

	double ** psi;
	psi = (double **) malloc(M * sizeof(double *));
	for (i=0; i < M; i++)
		psi[i] = (double *) malloc(M * sizeof(double));

	float ** xxtau;
	xxtau = (float **) malloc(M * sizeof(float *));
	for (i=0; i < M; i++)
		xxtau[i] = (float *) malloc(M * sizeof(float));

	float ** yytau;
	yytau = (float **) malloc(M * sizeof(float *));
	for (i=0; i < M; i++)
		yytau[i] = (float *) malloc(M * sizeof(float));

	meshgrid(xx, yy, tt);

	//imprimir_matriz_compleja(tt);

	calculo_flujo(tt, psi);

	//imprimir_matriz_double(psi);

	//TODO_p npi de porque no me va
	//imprimir_flujo(xx,yy,psi); 

	arregla_malla(tt);

	nueva_malla(xxtau, yytau, tt);

	return 0;
}


int main(int argc, char const *argv[])
{
	flujo();

	return 0;
}