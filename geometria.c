#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 100 // Número de puntos de la circunferencia

// PI queda definido como M_PI al importar math.h
// #define M_PI 3.14159265358979323846

/*Funcion interpolada para  limites cuando --> x<1 && y<1 */
float f1 (float x, float y) 
{
	float alim;
	alim = 1.0002 * (1.43 * (x - 0.0001) * (y - 0.0001) + (1 - x) * (y - 0.0001) + 1.1 * (x - 0.0001) * (1 - y) + 0.0002 * (1 - x) * (1 - y));
	return alim;
}

/*Funcion interpolada para  limites cuando --> x>1 && y<1 */
float f2 (float x, float y) 
{
	float alim;
	alim = 0.25 * (5.2 * (x - 1) * (y - 0.0001) + 1.43 * (5 - x) * (y - 0.0001) + 5.1 * (x - 1) * (1 - y) + 1.1 * (5 - x) * (1 - y));
	return alim;
}

/*Funcion interpolada para  limites cuando --> x<1 && y>1 */
float f3 (float x, float y) 
{
	float alim;
	alim = 0.25 * (7 * (x - 0.0001) * (y - 1) + 5 * (1 - x) * (y - 1) + 1.43 * (x - 0.0001) * (5 - y) + (1 - x) * (5 - y));
	return alim;
}

/*Funcion interpolada para  limites cuando --> x>1 && y>1 */
float f4 (float x, float y)
{
	float alim;
	alim = (9 * (x - 1) * (y - 1) + 7 * (5 - x) * (y - 1) + 5.2 * (x - 1) * (5 - y) + 1.43 * (5 - x) * (5 - y))/16;
	return alim;
}

/* La función devuelve las n divisiones equiespaciadas del intervalo dado */ 
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

/*Introducción valores para calcular perfil*/
int datos_perfil (float *dperfil) 
{
	printf("\nIntroduzca los valores:\n");

	printf("Xc: "); 
	scanf ("%f", &dperfil[0]);
	printf("Yc: ");
	scanf ("%f", &dperfil[1]);
	printf("R: ");
	scanf ("%f", &dperfil[2]);

	dperfil[3] = asin (dperfil[1]/dperfil[2]); //Beta
	dperfil[4] = dperfil[0] + dperfil[2] * cos (dperfil[3]); //b
															//El caso queda definido al pasar el vector por limites

	return 0;
}

/*Limita el valor de a para que la transformacion sea eficaz*/
int limites (float *dperfil)
{
	if (dperfil[2] <= 0) //El valor del radio siempre tiene que ser mayor que cero
	{
		printf("Valores no válidos (a<=0)\n"); 
		return 0;
	}

	if (dperfil[2] < dperfil[1]) //El valor del radio siempre tiene que ser mayor que el alejamiento del eje y
	{
		printf("Valores no válidos (a<yc)\n"); 
		return 0;
	}

	if (dperfil[0]==0 && dperfil[1]==0) //CASO 1: funcion no implementada - puede tener limites TODO_j
	{
		printf("Not implemented yet!\n");
		dperfil[5]=1;
		return 0;
	}	

	if (dperfil[0]==0) //CASO 2: considerar formula especial - y sus posibles limites TODO_j
	{
		if (dperfil[2]==dperfil[1])
		{
			//printf("Valores válidos (a=yc)\n");
			dperfil[5]=2;
			return 1;			
		}
		else //Si a<xc o a>xc
		{
			printf("Valores no válidos (a!=yc)\n"); 
			return 0;
		}	
	}
 
	if (dperfil[1]==0) //CASO 3
	{
		if (dperfil[2]>dperfil[0])
		{
			//printf("Valores válidos (a>xc)\n");
			dperfil[5]=3;
			return 1;
		}
		else //Si a==xc o a<xc
		{
			printf("Valores no válidos (a=xc || a<xc)\n"); 
			return 0;
		}
			
	}

	else //CASO 4
	{
		if (dperfil[0]>6 || dperfil[1]>6) //Limitacion dperfil menos que 6
		{
			printf("Valores no válidos (xc>6 || yc>6)\n");
			return 0;
		}
		else
		{
			if (dperfil[0]<=1)
			{
				if (dperfil[1]<=1) //xc<1 yc<1 ----> f1
				{
					if (f1(dperfil[0],dperfil[1])<=dperfil[2])
					{
						//printf("Valores válidos (f1(xc,yc)<=a)\n"); 
						dperfil[5]=4;
						return 1;
					}
					else
					{
						printf("Valores no válidos (f1(xc,yc)>a)\n"); 
						return 0;
					}
				}
				else //xc<1 yc>1 ----> f3
				{
					if (f3(dperfil[0],dperfil[1])<=dperfil[2])
					{
						//printf("Valores válidos (f3(xc,yc)<=a)\n"); 
						dperfil[5]=4;
						return 1;
					}
					else
					{
						printf("Valores no válidos (f3(xc,yc)>a)\n"); 
						return 0;
					}
				}
			}
			else
			{
				if (dperfil[1]<=1) //xc>1 yc<1 ----> f2
				{
					if (f2(dperfil[0],dperfil[1])<=dperfil[2])
					{
						//printf("Valores válidos (f2(xc,yc)<=a)\n"); 
						dperfil[5]=4;
						return 1;
					}
					else
					{
						printf("Valores no válidos (f2(xc,yc)>a)\n"); 
						return 0;
					}
				}
				else //xc>1 yc>1 ----> f4
 				{
					if (f4(dperfil[0],dperfil[1])<=dperfil[2] )
					{
						//printf("Valores válidos (f4(xc,yc)<=a)\n"); 
						dperfil[5]=4;
						return 1;
					}
					else
					{
						printf("Valores no válidos (f4(xc,yc)>a)\n"); 
						return 0;
					}
				}
			}
		}
	}
}

/* Devuelve matriz con todos los puntos de la circunferencia */
int matriz_circunferencia(float * dperfil, float ** circunferencia)
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
		circunferencia[i][0] = dperfil[0] + dperfil[2] * cos(t);
		circunferencia[i][1] = dperfil[1] + dperfil[2] * sin(t);
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

/*Plotea la grafica de la circunferencia*/
int plotc (float *dperfil, float *opc)
{
	float *rangoc; //Crea rango dinamico
	rangoc = (float *) malloc(4 * sizeof(float));

	rangoc [0] = dperfil[0]-dperfil[2]-1; 
	rangoc [1] = dperfil[0]+dperfil[2]+1;
	rangoc [2] = dperfil[1]-dperfil[2]-1;
	rangoc [3] = dperfil[1]+dperfil[2]+1;

	if (rangoc[0]<0 && rangoc[1]<0)
		rangoc[1]=5;
	else if (rangoc[0]>0 && rangoc[1]>0)
		rangoc[0]=-5;

	if (rangoc[2]<0 && rangoc[3]<0)
		rangoc[3]=5;
	else if (rangoc[2]>0 && rangoc[3]>0)
		rangoc[2]=-5;

	FILE *pipec = popen ("gnuplot -pesist","w"); 
	fprintf(pipec, "set size square \n set nokey \n set xzeroaxis \n set yzeroaxis \n plot [%f:%f] [%f:%f] \"pts_circun.dat\" pt %.0f ps %.0f lt %.0f, \"pts_circun.dat\" w filledcurves x1 fs  pattern %.0f lc %.0f\n", rangoc[0], rangoc[1], rangoc[2], rangoc[3], opc[0], opc[1], opc[2], opc[3], opc[4]); //TODO_j nombre archiv
	pclose (pipec);
	return 0;
}

/*TODO_p comentar funcion*/
int tranformacion_yukovski(float * dperfil, float ** circunferencia)
{
	int i; // Control para los bucles for
	float x, y;

	switch((int) dperfil[5]) // Elección de las ecuaciones de la transformación según el caso particular
	{
		case 1:
		case 2:
		case 3: //El caso 3 funciona tambien con las ecuaciones del caso 4
			for (i = 0; i < N ; i++)
			{
				x = circunferencia[i][0];
				y = circunferencia[i][1];
				circunferencia[i][0] = x * (1+(pow(dperfil[4],2)/(pow(x,2)+pow(y,2))));
				circunferencia[i][1] = y * (1-(pow(dperfil[4],2)/(pow(x,2)+pow(y,2))));
			}

		case 4:
			for (i = 0; i < N ; i++)
			{
				x = circunferencia[i][0];
				y = circunferencia[i][1];
				circunferencia[i][0] = x * (1+(pow(dperfil[4],2)/(pow(x,2)+pow(y,2))));
				circunferencia[i][1] = y * (1-(pow(dperfil[4],2)/(pow(x,2)+pow(y,2))));
			}
			return(0);

		default:
			printf("Caso inválido. Introduzca nuevos datos.\n");
			return(0);
	}
}

/*TODO_p comentar funcion*/
int imprimir_perfil(float ** circunferencia)
{
	FILE * file_perfil;
	file_perfil = fopen("pts_perfil.dat", "w+");

	if (file_perfil == NULL)
	{
		printf("Error al abrir el archivo\n");
		return(0);
		//TODO_p: a dónde vamos, al menú
	}

	int i;

	for (i=0; i < (N); i++)
	{
		fprintf(file_perfil, "%f %f\n", circunferencia[i][0], circunferencia[i][1]);
	}

	fprintf(file_perfil, "%f %f\n", circunferencia[0][0], circunferencia[0][1]); // Termina con el primer punto

	fclose(file_perfil);

	return(0);
}


int main()
{
	int i;

	float *dperfil; //1:xc 2:yc 3:a 4:beta 5:b 6:caso
	dperfil = (float *) malloc(6 * sizeof(float)); // Reserva de memoria
													      // Para cada vector

	float ** circunferencia;
	circunferencia = (float **) malloc(N * sizeof(float *)); // Reserva de memoria
																	    // Para cada vector
	for (i=0; i < N; i++)
	{
		circunferencia[i] = (float *) malloc(2 * sizeof(float)); // Reserva de memoria
	}																   // Para las dos coordenadas de cada vector

	float *opc; //Opciones por defecto ploteo circunferencia
	opc = (float *) malloc(5 * sizeof(float)); //Reserva de memoria para cada vector
	opc[0] = 7;  // 1: (p)oint(t)ype   2: (p)oint(s)ize   3: (l)ine(t)ype    4:(pattern) malla     5:(l)ine(c)olor malla
	opc[1] = 0.75;
	opc[2] = 1;
	opc[3] = 2;
	opc[4] = 7;
 
	float *opp;	 //TODO_j modificar valores iniciales
	opp = (float *) malloc(5 * sizeof(float)); 

	float *opf; ///TODO_j modificar valores iniciales
	opf = (float *) malloc(5 * sizeof(float));


	datos_perfil (dperfil);

	while (limites(dperfil)==0)
	{
		datos_perfil (dperfil);
	}

	matriz_circunferencia(dperfil, circunferencia);

	imprimir_circunferencia(circunferencia);

	plotc (dperfil, opc);

	tranformacion_yukovski(dperfil, circunferencia);

	imprimir_perfil(circunferencia);


	return(0);
}