#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N = 100

float *linspace(float x0, float x, int n)
{
	int i=1;
	float parte;

	float *vector; 
	vector = (float *) malloc(n * sizeof(float)); 

	vector[0] = x0; 
	vector[n] = x; 

	parte = (x - x0) / (float) n; 
	//	printf("%f\n", vector[0]);

	for (i=1; i < n; i++)
	{
		vector[i] = vector[i-1] + parte;
	//		printf("%f\n", vector[i]);
	}

	//	printf("%f\n", vector[n]);

	return(vector);
}

float *circunferencia(float centro[], float R, float t)
{
	float *Xco;
	Xco = (float *) malloc(1 * sizeof(float));

	float *Yco;
	Yco = (float *) malloc(1 * sizeof(float));

	*Xco = centro[0] + R * cos(t);
	*Yco = centro[1] + R * sin(t);

	float circunferencia [1][2] = {*Xco, *Yco};

	return(*punto);
}

int main()
{
	float * centro;
	centro = (float *) malloc(2 * sizeof(float));
	float a;
	printf("Xc: ");
	scanf ("%f", &centro[0]);
	printf("Yc: ");
	scanf ("%f", &centro[1]);
	printf("R: ");
	scanf ("%f", &a);

	int n = 200; 

	float * valores_t;
	valores_t = (float *) malloc(n * sizeof(float));
	valores_t = linspace(0, 2*M_PI, N);

/*	int c = 0;
	for (c = 0; c < n; ++c)
	{
		printf("%f\n", valores_t[c]);
	}
*/

	float ptos [n][2];



/*	int c = 0;
	for (c = 0; c < n; ++c)
	{
		printf("%f %f\n", ptos[c][1], ptos[c][2]);
	}
*/

	FILE *ptos_circ;
	ptos_circ = fopen ("a.dat", "w+");
	for (i = 0; i < n; ++i)
	{
		fprintf(ptos_circ, "%f %f\n", ptos[i][1], ptos[i][2]);
	}
	fprintf(ptos_circ, "%f %f\n", ptos[0][1], ptos[0][2]);
	fclose (ptos_circ);
 
/*	FILE *cent;
	cent = fopen ("c.dat", "w+");
	c = 0;
	for (c = 0; c < n; ++c)
	{
		fprintf(cent, "%f %f\n", centro[0], centro[1]);
	}
	fclose (cent);
*/

	FILE *pipe = popen ("gnuplot -pesist","w"); 
	fprintf(pipe, "set nokey \n set size square \n set arrow from %f,%f to %f,%f nohead lt 0 lw 1 \n plot [%f:%f] [%f:%f] \"a.dat\" pt 7 ps 0.25 lt 7, %f lt 0 lw 1\n", centro[0], centro[1]-a-1, centro[0], centro[1]+a+1, centro[0]-a-1, centro[0]+a+1, centro[1]-a-1, centro[1]+a+1, centro [1]);
	pclose (pipe);

	FILE *pipe1 = popen ("gnuplot -pesist","w"); 
	fprintf(pipe1, "set nokey \n set size square \n set arrow from %f,%f to %f,%f nohead lt 0 lw 1 \n plot [%f:%f] [%f:%f] \"a.dat\" using 1:2 with lines lt 1, %f lt 0 lw 2\n", centro[0], centro[1]-a-1, centro[0], centro[1]+a+1, centro[0]-a-1, centro[0]+a+1, centro[1]-a-1, centro[1]+a+1, centro [1]);
	pclose (pipe1);

	return 0;
}