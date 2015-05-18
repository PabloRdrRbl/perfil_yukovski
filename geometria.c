#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

/* Ecuaciones paramétricas. 
La función calcula el punto de la circunferencia en función del ángulo */
float * punto_circunferencia(float centro[], float R, float t)
{
	// Punto a devolver
	float * punto;
	punto = (float *) malloc(2 * sizeof(float));


	// Ecuaciones paramétricas de la circunferencia
	punto[0] = centro[0] + R * cos(t);
	punto[1] = centro[1] + R * sin(t);


	return(punto);
}


/* Devuelve matriz con todos los puntos de la circunferencia */
float ** matriz_circunferencia(float centro[], float R, int n)
{
	int i;
	float ** puntos_circunferencia;
	puntos_circunferencia = (float **) malloc(n * sizeof(float *)); // Reserva de memoria
																	// Para cada vector

	for (i=0; i < n; i++)
	{
		puntos_circunferencia[i] = (float *) malloc(2 * sizeof(float)); // Reserva de memoria
	}																   // Para las dos coordenadas de cada vector


	// Valores de ángulo t para las ecuaciones paramétricas
	float * valores_t;
	valores_t = (float *) malloc(n * sizeof(float));

	valores_t = linspace(0, 2*M_PI, n);


	// Cálculo de cada punto para cada valor de t
	// Almacenamiento en lista

	for (int j = 0; j < n; ++j)
	{
		puntos_circunferencia[j] = punto_circunferencia(centro, R, valores_t[j]);
	}
	// TODO_p: terminar con el primer punto
	return(puntos_circunferencia);
}


/* Imprime una lista de puntos (matriz nx2) */
int imprimir_matriz(float ** circunferencia, int n)
{
	int i;

	for (i=0; i < n; i++)
	{
		printf("%f %f\n", circunferencia[i][0], circunferencia[i][1]);
	}

	return(0);
}


float transformacion_yukovski_X(float x, float y, float b)
{
	return(x*(1+((pow(b, 2))/(pow(x, 2)*pow(y, 2))))); // Expresión para la transformación
													   // de las coordenadas X
}


float transformacion_yukovski_Y(float x, float y, float b)
{
	return(y*(1-((pow(b, 2))/(pow(x, 2)*pow(y, 2))))); // Expresión para la transformación
													   // de las coordenadas Y
}


float ** transformacion_yukovski(float ** circunferencia, int n, float b)
{
	int i; // Control para bucle for

	float ** perfil; // Matriz nx2 que almacenará los puntos transformados
	perfil = (float **) malloc(n * sizeof(float *));


	for (i = 0; i < n; i++)
	{
		perfil[i] = (float *) malloc(2 * sizeof(float)); // Reserva de memoria
		perfil[i][0] = transformacion_yukovski_X(circunferencia[i][0], circunferencia[i][1], b);
		perfil[i][1] = transformacion_yukovski_Y(circunferencia[i][0], circunferencia[i][1], b);
	}

	return(perfil);
}





int main()
{
    FILE *gp;
    gp = popen("gnuplot -persist","w"); // gp es el numbre de la tubería
    if (gp==NULL)
        {
        	printf("Error abriendo la tubería hacia GNU Plot.\n");
            return(0);
        }


	// Coordenadas del centro
	float * centro;
	centro = (float *) malloc(2 * sizeof(float));

	centro[0] = -0.3; // Xc
	centro[1] = 0.2; // Yc


	// Radio
	float R = 1;


	// Puntos de la circunferencia
	int n = 100; // Número de puntos para la circunferencia


	// b donde se corta la circunferencia con el eje X
	float beta; // Ángulo donde y es igual a 0
	float b; 

	beta = asin(centro[1]/R);
	b = centro[0] + R * cos(beta);

	float ** circunferencia;
	circunferencia = (float **) malloc(n * sizeof(float *));

	circunferencia = matriz_circunferencia(centro, R, n);

	// imprimir_matriz(circunferencia, n);


	float ** perfil; // Matriz nx2 que almacenará los puntos transformados
	perfil = (float **) malloc(n * sizeof(float *));

	perfil = transformacion_yukovski(circunferencia, n, b);

	imprimir_matriz(perfil, n);

	return(0);
}