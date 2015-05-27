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
int linspace(float * vector, float x0, float x, int n)
{
	int i;
	float parte;

	vector[0] = x0; // Valor incial
	vector[n] = x; // Valor final

	parte = (x - x0) / (float) (n-1); // Intervalo

	for (i=1; i < n; i++)
	{
		vector[i] = vector[i-1] + parte;
	}

	return 0;
}


int meshgrid(float ** xx, float ** yy, complex double ** tt)
{
	int i, j;

	int xmax = 8;
	int ymax = 3;

	float * rangox;
	rangox = (float *) malloc(M * sizeof(float));

	float * rangoy;
	rangoy = (float *) malloc(M * sizeof(float));

	linspace(rangox, -xmax, xmax, M);
	linspace(rangoy, -ymax, ymax, M);

	for (i = 0; i < M; i++)
	{
		xx[i] = rangox;
	}

	for (i = 0; i < M; i++)
		for (j = 0; j < M; j++)
		{
			yy[i][j] = rangoy[i];
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
			if (cabs(tt[i][j]-t0) < R)
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

int imprimir_xxtau(float ** matriz)
{
	FILE * matriz_archivo;
	matriz_archivo = fopen("xxtau.dat", "w+");

	int i, j;

	for (i = 0; i < M; i++)
	{
		for (j = 0; j < M; j++)
		{
			fprintf(matriz_archivo, "%f ", matriz[i][j]);
		}

		fprintf(matriz_archivo, "\n");
	}
	
	return 0;
}

int imprimir_yytau(float ** matriz)
{
	FILE * matriz_archivo;
	matriz_archivo = fopen("yytau.dat", "w+");

	int i, j;

	for (i = 0; i < M; i++)
	{
		for (j = 0; j < M; j++)
		{
			fprintf(matriz_archivo, "%f ", matriz[i][j]);
		}

		fprintf(matriz_archivo, "\n");
	}
	
	return 0;
}



int imprimir_psitau(double ** matriz)
{
	FILE * matriz_archivo;
	matriz_archivo = fopen("psitau.dat", "w+");

	int i, j;

	for (i = 0; i < M; i++)
	{
		for (j = 0; j < M; j++)
		{
			fprintf(matriz_archivo, "%f ", matriz[i][j]);
		}

		fprintf(matriz_archivo, "\n");
	}
	
	return 0;
}

int imprimir_todo(float **xxtau, float **yytau, double **psi)
{
	FILE * matriz_archivo;
	matriz_archivo = fopen("putos.dat", "w+");

	int i, j;

	for (j = 0; j < M; j++)
	{
		for (i = 0; i < M; i++)
		{
			fprintf(matriz_archivo, "%f ", xxtau[i][j]);
			fprintf(matriz_archivo, "%f ", yytau[i][j]);
			fprintf(matriz_archivo, "%lf \n", psi[i][j]);
		}

		fprintf(matriz_archivo, "\n");
	}
	
	return 0;
}

int imprimir_todo_izq(float **xxtau, float **yytau, double **psi)
{
	FILE * matriz_archivo;
	matriz_archivo = fopen("putos1.dat", "w+");

	int i, j;

	for (i = 0; i < M; i++)
	{
		for (j = 0; j < M; j++)
		{
			if (xxtau[i][j] < -2.54)
			{
				fprintf(matriz_archivo, "%f ", xxtau[i][j]);
				fprintf(matriz_archivo, "%f ", yytau[i][j]);
				fprintf(matriz_archivo, "%lf \n", psi[i][j]);
			}
		}
	}
	
	return 0;
}

int imprimir_todo_der(float **xxtau, float **yytau, double **psi)
{
	FILE * matriz_archivo;
	matriz_archivo = fopen("putos2.dat", "w+");

	int i, j;

	for (i = 0; i < M; i++)
	{
		for (j = 0; j < M; j++)
		{
			if (xxtau[i][j] > 2.31)
			{
				fprintf(matriz_archivo, "%f ", xxtau[i][j]);
				fprintf(matriz_archivo, "%f ", yytau[i][j]);
				fprintf(matriz_archivo, "%lf \n", psi[i][j]);
			}
		}
	}
	
	return 0;
}

int imprimir_todo_deb(float **xxtau, float **yytau, double **psi)
{
	FILE * matriz_archivo;
	matriz_archivo = fopen("putos3.dat", "w+");

	int i, j;

	for (i = 0; i < M; i++)
	{
		for (j = 0; j < M; j++)
		{
			if (xxtau[i][j] < 2.31 && xxtau[i][j] > -2.54)
			{
				if (yytau[i][j] < -0.28)
				{
					fprintf(matriz_archivo, "%f ", xxtau[i][j]);
					fprintf(matriz_archivo, "%f ", yytau[i][j]);
					fprintf(matriz_archivo, "%lf \n", psi[i][j]);
				}
			}
		}
	}
	
	return 0;
}

int imprimir_todo_arr(float **xxtau, float **yytau, double **psi)
{
	FILE * matriz_archivo;
	matriz_archivo = fopen("putos4.dat", "w+");

	int i, j;

	for (i = 0; i < M; i++)
	{
		for (j = 0; j < M; j++)
		{
			if (xxtau[i][j] < 2.31 && xxtau[i][j] > -2.54)
			{
				if (yytau[i][j] > 1.35)
				{
					fprintf(matriz_archivo, "%f ", xxtau[i][j]);
					fprintf(matriz_archivo, "%f ", yytau[i][j]);
					fprintf(matriz_archivo, "%lf \n", psi[i][j]);
				}
			}
		}
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

	imprimir_xxtau(xxtau);
	imprimir_yytau(yytau);
	imprimir_psitau(psi);

	imprimir_todo(xxtau, yytau, psi);
	imprimir_todo_izq(xxtau, yytau, psi);
	imprimir_todo_der(xxtau, yytau, psi);
	imprimir_todo_deb(xxtau, yytau, psi);
	imprimir_todo_arr(xxtau, yytau, psi);

	return 0;
}


int main(int argc, char const *argv[])
{
	flujo();

	return 0;
}