#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 200 // Número de puntos de la circunferencia
#define M 50 // Número de puntos de la malla


/* Introducción de los valores para calcular perfil */
int datos_perfil(float * dperfil) 
{
	printf("\nIntroduzca los valores:\n");

	printf("Xc: "); 
	scanf ("%f", &dperfil[0]);
	printf("Yc: ");
	scanf ("%f", &dperfil[1]);
	printf("R: ");
	scanf ("%f", &dperfil[2]);

	// beta
	dperfil[3] = asin (dperfil[1]/dperfil[2]);

	// b
	dperfil[4] = dperfil[0] + dperfil[2] * cos (dperfil[3]);
															 
	// El caso queda definido al pasar el vector por limites
	dperfil[5] = 0; // Por ahora

	return 0;
}

int datos_flujo (float *dperfil, float *dflujo)
{
	printf("\nIntroduzca los valores:\n");

	printf("Alpha: "); 
	scanf ("%f", &dflujo[0]);
	printf("U: ");
	scanf ("%f", &dflujo[1]);
	printf("Densidad del aire: ");
	scanf ("%f", &dflujo[2]);

	// Valor 'a' que no es radio :$
	dflujo[3] = dperfil[2]/(sqrt(pow(1+dperfil[0],2)+pow(dperfil[1],2)));

	// Corriente (T)
	dflujo[4] = 4*M_PI*dflujo[1]*dflujo[3]*sin(dflujo[0]+dperfil[3]);

	//Sustentación por unidad de envergadura (Y)
	dflujo[5] = dflujo[2]*dflujo[1]*dflujo[4];

	//Coeficiente de sustentación (CL) 
	dflujo[6] = 2*M_PI*sin(dflujo[0]+dperfil[3]);

	return 0;
}

int main ()
{
	// Vector con los datos del perfil {Xc, Yc, a, beta, b, caso}
	float * dperfil;
	dperfil = (float *) malloc(6 * sizeof(float)); // Reserva de memoria para el vector

	// Vector con los datos del perfil {Xc, Yc, a, beta, b, caso}
	float * dflujo;
	dflujo = (float *) malloc(7 * sizeof(float)); // Reserva de memoria para el vector

	datos_perfil (dperfil);

	datos_flujo (dperfil, dflujo);

	printf("\nXc= %f\n", dperfil[0]);
	printf("Yc= %f\n", dperfil[1]);
	printf("R= %f\n", dperfil[2]);
	printf("beta= %f\n", dperfil[3]);
	printf("b= %f\n", dperfil[4]);
	printf("Caso= %f\n \n", dperfil[5]);

	printf("Alpha= %f\n", dflujo[0]);
	printf("U= %f\n", dflujo[1]);
	printf("Densidad del aire= %f\n", dflujo[2]);
	printf("a= %f\n", dflujo[3]);
	printf("T= %f\n", dflujo[4]);	
	printf("Y= %f\n", dflujo[5]);
	printf("CL= %f\n", dflujo[6]);	

	return 0;
}