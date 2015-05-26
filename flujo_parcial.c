#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 200 // Número de puntos de la circunferencia
#define M 50 // Número de puntos para la malla


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


int datos_flujo (float * dperfil, float * dflujo)
{
	printf("\nIntroduzca los valores:\n");

	printf("U: "); 
	scanf ("%f", &dflujo[0]);
	printf("Alpha: ");
	scanf ("%f", &dflujo[1]);

	return 0;
}


int meshgrid(float ** xx, float ** yy)
{
	int i, j;

	float * rango;
	rango = (float *) malloc(M * sizeof(float));
	// TODO_p: hay que calcular límites para esto
	rango = linspace(-3, 3, M);

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


// Imprime matriz MxM
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

int calculo_flujo(float * dperfil, float * dflujo, float ** xx, float ** yy, float ** psi)
{
	int i,j;
	float rho, theta, T;

	// TODO_p: quitar centro y eje x


	for (i = 0; i < M; ++i)
		for (j = 0; j < M; ++j)
		{
			rho = sqrt(pow(xx[i][j] - dperfil[0],2) + pow(yy[i][j] - dperfil[1],2));
			theta = atan((yy[i][j] - dperfil[1])/(xx[i][j] - dperfil[0]));


			T = 4 * M_PI * dperfil[4] * dflujo[0] * sin(dflujo[1]);


			if (xx[i][j] > dperfil[0])
				psi[i][j] = dflujo[0] * (rho * sin(theta - dflujo[1]) - (1/rho) * sin(theta - dflujo[1])) + (T/(2*M_PI)) * log(fabs(rho));
			else
				psi[i][j] = -dflujo[0] * (rho * sin(theta - dflujo[1]) - (1/rho) * sin(theta - dflujo[1])) + (T/(2*M_PI)) * log(fabs(rho));
		}

	return 0;
}


int imprimir_flujo(float ** xx, float ** yy, float ** psi)
{
	FILE * file_flujo_cilindro;
	file_flujo_cilindro = fopen("pts_flujo_cilindro.dat", "w+");

	if (file_flujo_cilindro == NULL)
	{
		printf("Error al abrir el archivo\n");
		return(0);
		//TODO_p: a dónde vamos, al menú
	}

	int i, j;

	for (i = 0; i < M; ++i)
		for (j = 0; j < M; ++j)
		{
			fprintf(file_flujo_cilindro, "%f %f %f\n", xx[i][j], yy[i][j], psi[i][j]); // Escribe cada punto (fila de la matriz) en el archivo
		}

	fclose(file_flujo_cilindro); // Cierre del archivo

	return(0);
}

int imprimir_psi(float ** matriz)
{
	FILE * matriz_archivo;
	matriz_archivo = fopen("psi.dat", "w+");

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


/* Aplica la transformación de Yukovski a cada punto de la circunferencia */
int transformacion_yukovski_perfil(float * dperfil, float ** xx, float ** yy)
{
	int i, j; 
	float x, y;

	for (i = 0; i < M; ++i) //Para el resto de casos funcionan estas ecuaciones
		for (j = 0; i < M; ++j)
		{
			x = xx[i][j];
			y = yy[i][j];

			xx[i][j] = x * (1+(pow(dperfil[4],2)/(pow(x,2)+pow(y,2))));
			yy[i][j] = y * (1-(pow(dperfil[4],2)/(pow(x,2)+pow(y,2))));
			printf("%f %f\n", xx[i][j], yy[i][j]);
		}


	return(0);
}




int flujo(float * dperfil/* ,float * opf*/) // TODO_p: al juntar unir las opciones
{
	int i;

	// Vector con los datos del flujo
	float * dflujo;
	dflujo = (float *) malloc(7 * sizeof(float)); // Reserva de memoria para el vector

	datos_flujo(dperfil, dflujo);

	float ** xx;
	xx = (float **) malloc(M * sizeof(float *));

	for (i=0; i < M; i++)
		xx[i] = (float *) malloc(M * sizeof(float));

	float ** yy;
	yy = (float **) malloc(M * sizeof(float *));

	for (i=0; i < M; i++)
		yy[i] = (float *) malloc(M * sizeof(float));

	float ** psi;
	psi = (float **) malloc(M * sizeof(float *));

	for (i=0; i < M; i++)
		psi[i] = (float *) malloc(M * sizeof(float));

	float * xxflat;
	xxflat = (float *) malloc(M * M * sizeof(float));

	float * yyflat;
	yyflat = (float *) malloc(M * M * sizeof(float));

	meshgrid(xx, yy);
	
	calculo_flujo(dperfil, dflujo, xx, yy, psi);

	imprimir_flujo(xx, yy, psi);

	imprimir_psi(psi);

	imprimir_matriz(xx);

	printf("\n");

	imprimir_matriz(yy);

	//transformacion_yukovski_perfil(dperfil, xx, yy);

	imprimir_matriz(xx);

	printf("\n");

	imprimir_matriz(yy);

	return 0;
}

int main(int argc, char const *argv[])
{
	float dperfil[6] = {0.3, 0.2, 2, 5.033, 0.615, 4};

	flujo(dperfil);


	return 0;
}