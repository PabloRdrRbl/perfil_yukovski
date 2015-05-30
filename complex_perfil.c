#include <stdio.h> //COMENTARIOS: XX.flaten no las usas? es necesario imprimir la matriz psi?
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#define N 200 // Número de puntos de la circunferencia
#define M 50 

/* Introducción de los valores para calcular perfil */
int datos_perfil(float * dperfil) 
{
	printf("\nIntroduzca los valores:\n");

	printf("Xc: "); 
	scanf ("%f", &dperfil[0]);
	printf("Yc: ");
	scanf ("%f", &dperfil[1]);
	printf("R: ");
	scanf ("%f", &dperfil[4]);

	// beta
	dperfil[3] = asin(dperfil[1]/dperfil[2]);

	// b
	dperfil[2] = dperfil[4] * sqrt((pow(1 + dperfil[0],2) + pow(dperfil[1],2)));
															 
	// El caso queda definido al pasar el vector por limites
	dperfil[5] = 0; // Por ahora

	//Futuros limites dinamicos del ploteo
	dperfil[6] = dperfil[0] - dperfil[2] - 1; //-X ploteo circunferencia
	dperfil[7] = dperfil[0] + dperfil[2] + 1; //+X ploteo circunferencia
	dperfil[8] = dperfil[1] - dperfil[2] - 1; //-Y ploteo circunferencia
	dperfil[9] = dperfil[1] + dperfil[2] + 1; //+Y ploteo circunferencia

	//Datos dinamicos del ploteo del perfil inicicalizados a cero
	dperfil[10] = 0; //-X ploteo perfil
	dperfil[11] = 0; //+X ploteo perfil
	dperfil[12] = 0; //-Y ploteo perfil
	dperfil[13] = 0; //+Y ploteo perfil
	printf("%f\n", dperfil[2]);

	return 0;
}

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

int calculo_flujo(complex double ** tt, double ** psi, float * dperfil)
{
	int i,j;

	complex double t0; // Centro de la circunferencia
	t0 =  dperfil[4] * (-dperfil[0] + dperfil[1] * I);

	double U = 1;
	double alpha = 0;
	double T = 4 * M_PI * dperfil[4] * U * (dperfil[1] + (1+dperfil[0]) * alpha);
	printf("%lf\n", T);
	for (i = 0; i < M; ++i)
		for (j = 0; j < M; ++j)
		{
			psi[i][j] = cimag(U * ((tt[i][j]-t0)*cexp(-alpha*I)+pow(dperfil[2],2)/(tt[i][j]-t0)*cexp(alpha*I)) + I * T/(2*M_PI)*clog(tt[i][j]-t0));
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
			if (cabs(tt[i][j]-t0) < 1.05 * R)
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

int simetrica_matriz(float ** matriz)
{
	int i, j;

	float ** copia;
	copia = (float **) malloc(M * sizeof(float *));
	for (i=0; i < M; i++)
		copia[i] = (float *) malloc(M * sizeof(float));

	for (i = 0; i < M; ++i)
		for (j = 0; j < M; ++j)
		{
			copia[i][j] = matriz[i][j];
		}

	for (i = 0; i < M; ++i)
		for (j = 0; j < M; ++j)
		{
			matriz[i][M-1-j] = copia[i][j];
		}


	return 0;
}


int simetrica_matriz_double(double ** matriz)
{
	int i, j;

	float ** copia;
	copia = (float **) malloc(M * sizeof(float *));
	for (i=0; i < M; i++)
		copia[i] = (float *) malloc(M * sizeof(float));


	for (i = 0; i < M; ++i)
		for (j = 0; j < M; ++j)
		{
			copia[i][j] = (float) matriz[i][j];
		}

	for (i = 0; i < M; ++i)
		for (j = 0; j < M; ++j)
		{
			matriz[i][M-1-j] = (double) copia[i][j];
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


/* Copian en un archivo .dat una lista de puntos (matriz nx2) para imprimir en GNU Plot */
int imprimir_perfil_imaginario(double ** circunferencia)
{
	// Apertura del archivo donde se almacenan los puntos de la circunferencia para ser impresos con GNU Plot
	FILE * file_perfil_imaginario; 
	file_perfil_imaginario = fopen("pts_perfil_imaginario.dat", "w+");

	if (file_perfil_imaginario == NULL)
	{
		printf("\033[31mError al abrir el archivo\n");
		printf("\033[0m\n");	
		return 1;
	}

	int i;

	for (i=0; i < (N); i++) // Escribe cada punto (fila de la matriz) en el archivo
	{
		fprintf(file_perfil_imaginario, "%f %f\n", circunferencia[i][0], circunferencia[i][1]);
	}

	fprintf(file_perfil_imaginario, "%f %f\n", circunferencia[0][0], circunferencia[0][1]); // Termina con el primer punto (para cerrar el polígono)

	fclose(file_perfil_imaginario); // Cierre del archivo


	return 0;
}


int transformacion_yukovski(float * dperfil, complex double * circunferencia_compleja)
{
	int i;

	for (i = 0; i < N; ++i)
	{
		circunferencia_compleja[i] = circunferencia_compleja[i] + pow(dperfil[4],2)/circunferencia_compleja[i];
	}

	
	return 0;
}


int perfil_imaginario(float * dperfil)
{
	int i;

	double ** circunferencia;
	circunferencia = (double **) malloc(N * sizeof(double *));
	for (i=0; i < N; i++)
		circunferencia[i] = (double *) malloc(N * sizeof(double));

	complex double t0; // Centro de la circunferencia
	t0 =  dperfil[4] * (-dperfil[0] + dperfil[1] * I);

	// Valores de ángulo t para las ecuaciones paramétricas
	float * valores_t;
	valores_t = (float *) malloc(N * sizeof(float));

	linspace(valores_t, 0, 2*M_PI, N); // linspace divide uniformemente el intervalo 2*pi en N partes

	// Puntos de la circunferencia
	for (i = 0; i < N; ++i)
	{
		circunferencia[i][0] = creal(t0) + (double) dperfil[2] * cos(valores_t[i]);
		circunferencia[i][1] = cimag(t0) + (double) dperfil[2] * sin(valores_t[i]);
	}


	complex double * circunferencia_compleja;
	circunferencia_compleja = (complex double *) malloc(N * sizeof(complex double));

	// Circunferencia en números complejos
	for (i = 0; i < N; ++i)
		circunferencia_compleja[i] = circunferencia[i][0] + circunferencia[i][1] * I;

	// Transformación
	transformacion_yukovski(dperfil, circunferencia_compleja);

	// Puntos con el perfil
	for (i = 0; i < N; ++i)
	{
		circunferencia[i][0] = creal(circunferencia_compleja[i]);
		circunferencia[i][1] = cimag(circunferencia_compleja[i]);
	}


	imprimir_perfil_imaginario(circunferencia);


	return 0;
}



int flujo(float * dperfil /*float * opf*/) // TODO_p: al juntar unir las opciones
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


	perfil_imaginario(dperfil);

	meshgrid(xx, yy, tt);

	calculo_flujo(tt, psi, dperfil);

	arregla_malla(tt);

	nueva_malla(xxtau, yytau, tt);

//	simetrica_matriz(xxtau);
//	simetrica_matriz(yytau);
//	simetrica_matriz_double(psi);

	imprimir_xxtau(xxtau);
	imprimir_yytau(yytau);
	imprimir_psitau(psi);

	imprimir_todo(xxtau, yytau, psi);

	return 0;
}


int main(int argc, char const *argv[])
{
	// Vector con los datos del flujo
	float * dperfil;
	dperfil = (float *) malloc(14 * sizeof(float)); // Reserva de memoria para el vector

	datos_perfil(dperfil);

	flujo(dperfil);

	return 0;
}