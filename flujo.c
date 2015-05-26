#include <stdio.h> //COMENTARIOS: XX.flaten no las usas? es necesario imprimir la matriz psi?
#include <stdlib.h>
#include <math.h>

#define N 200 // Número de puntos de la circunferencia
#define M 31 // Número de puntos para la malla

int menu_flujo_cil (float * opc, float * opp, float * opfc);

/* Funcion interpolada para  limites cuando --> x<1 && y<1 */
float f1(float x, float y) 
{
	float alim;
	alim = 1.0002 * (1.43 * (x - 0.0001) * (y - 0.0001) + (1 - x) * (y - 0.0001) + 1.1 * (x - 0.0001) * (1 - y) + 0.0002 * (1 - x) * (1 - y));
	return alim;
}


/* Funcion interpolada para  limites cuando --> x>1 && y<1 */
float f2(float x, float y) 
{
	float alim;
	alim = 0.25 * (5.2 * (x - 1) * (y - 0.0001) + 1.43 * (5 - x) * (y - 0.0001) + 5.1 * (x - 1) * (1 - y) + 1.1 * (5 - x) * (1 - y));
	return alim;
}


/* Funcion interpolada para  limites cuando --> x<1 && y>1 */
float f3 (float x, float y) 
{
	float alim;
	alim = 0.25 * (7 * (x - 0.0001) * (y - 1) + 5 * (1 - x) * (y - 1) + 1.43 * (x - 0.0001) * (5 - y) + (1 - x) * (5 - y));
	return alim;
}


/* Funcion interpolada para  limites cuando --> x>1 && y>1 */
float f4(float x, float y)
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

	//Futuros limites dinamicos del ploteo incializados a cero
	dperfil[6] = dperfil[0] - dperfil[2] - 1; 
	dperfil[7] = dperfil[0] + dperfil[2] + 1;
	dperfil[8] = dperfil[1] - dperfil[2] - 1;
	dperfil[9] = dperfil[1] + dperfil[2] + 1;
	dperfil[10] = 0; //-X ploteo perfil
	dperfil[11] = 0; //+X ploteo perfil
	dperfil[12] = 0; //-Y ploteo perfil
	dperfil[13] = 0; //+Y ploteo perfil
	return 0;
}


/* Limita el valor de a (radio) para que la transformación sea eficaz */
int limites(float * dperfil)
{
	if (dperfil[0]>50 || dperfil[1]>50 || dperfil[2]>50)
	{
		printf("\033[31mValores muy grandes \n"); 
		printf("\033[0m\n");	
		return 0;
	}

	if (dperfil[2] <= 0) // El valor del radio siempre tiene que ser mayor que cero
	{
		printf("\033[31mValores no válidos (R<=0)\n"); 
		printf("\033[0m\n");	
		return 0; // 
	}

	if (dperfil[2] < dperfil[1]) // El valor del radio siempre tiene que ser mayor que el alejamiento del eje y
	{
		printf("\033[31mValores no válidos (R<Yc)\n"); 
		printf("\033[0m\n");	
		return 0;
	}

	if (dperfil[0]==0 && dperfil[1]==0) // CASO 1: no planteado 
	{
		printf("\033[31mValores no válidos (Xc==0 && Yc==0)\n");
		printf("\033[0m\n");	
		return 0;
	}	

	if (dperfil[0]==0) //CASO 2
	{
		if (dperfil[2]==dperfil[1])
		{
			//printf("Valores válidos (a=yc)\n"); //TODO_p: como dijo Panizo, si funciona bien no hace falta saberlo
			dperfil[5]=2;
			return 1;			
		}
		else //Si a<xc o a>xc
		{
			printf("\033[31mValores no válidos (R!=Yc)\n"); 
			printf("\033[0m\n");	
			return 0;
		}	
	}
 
	if (dperfil[1]==0) //CASO 3
	{
		if (dperfil[2]>dperfil[0])
		{
			if (dperfil[2]>dperfil[0]+1)
			{
				dperfil[5]=3;
				return 1;
			}
			else
			{
				printf("\033[31mValores no válidos (Xc muy proximo a R)\n"); 
				printf("\033[0m\n");	
				return 0;	
			}
		}
		else //Si a==xc o a<xc
		{
			printf("\033[31mValores no válidos (R<=Xc)\n"); 
			printf("\033[0m\n");	
			return 0;
		}
			
	}

	else //CASO 4
	{
		if (dperfil[0]>6 || dperfil[1]>6) //Limitacion dperfil menos que 6
		{
			printf("\033[31mValores no válidos (Xc>6 || Yc>6)\n");
			printf("\033[0m\n");	
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
						printf("\033[31mValores no válidos (f1(Xc,Yc)>R)\n"); 
						printf("\033[0m\n");	
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
						printf("\033[31mValores no válidos (f3(Xc,Yc)>R)\n"); 
						printf("\033[0m\n");	
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
						printf("\033[31mValores no válidos (f2(Xc,Yc)>R)\n"); 
						printf("\033[0m\n");	
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
						printf("\033[31mValores no válidos (f4(Xc,Yc)>R)\n"); 
						printf("\033[0m\n");	
						return 0;
					}
				}
			}
		}
	}
}


int datos_flujo (float *dperfil, float *dflujo)
{
	printf("\nIntroduzca los valores:\n");

	float U, alpha;

	printf("U: ");
	scanf ("%f", &U);
	while (U>=3 || U<=0.9)
	{
		printf("Valor no valido:");
		scanf ("%f", &U);
		printf("\n");
	}
	dflujo[0]= U;

	printf("Alpha: ");
	scanf ("%f", &alpha);
	if (alpha>360)
	{
		while (alpha>360)
		{
			alpha = alpha - 360;
		}
	}
	if (alpha<-360)
	{
		while (alpha<-360)
		{
			alpha = alpha + 360;
		}
	}
	dflujo[1]= alpha;

	printf("Densidad del aire: ");
	scanf ("%f", &dflujo[2]);

	// Corriente (T)
	dperfil[3] = 4 * M_PI * dperfil[4] * dflujo[0] * sin(dflujo[1]);

	//Coeficiente de sustentación (CL) 
	dflujo[4] = 2*M_PI*sin(dflujo[1]+dperfil[3]);

	return 0;
}

/* Devuelve matriz nx2 con todos los puntos de la circunferencia */
int matriz_circunferencia(float * dperfil, float ** circunferencia)
{
	int i;

	// Valores de ángulo t para las ecuaciones paramétricas
	float * valores_t;
	valores_t = (float *) malloc(N * sizeof(float));

	valores_t = linspace(0, 2*M_PI, N); // linspace divide uniformemente el intervalo 2*pi en N partes


	// Cálculo de cada punto para cada valor de t
	// Almacenamiento en matriz nx2
	for (i = 0; i < N; ++i)
	{
		circunferencia[i][0] = dperfil[0] + dperfil[2] * cos(valores_t[i]);
		circunferencia[i][1] = dperfil[1] + dperfil[2] * sin(valores_t[i]);
	}

	return(0);
}


/* Copian en un archivo .dat una lista de puntos (matriz nx2) para imprimir en GNU Plot */
int imprimir_circunferencia(float ** circunferencia)
{
	// Apertura del archivo donde se almacenan los puntos de la circunferencia para ser impresos con GNU Plot
	FILE * file_circunferencia; 
	file_circunferencia = fopen("pts_circun.dat", "w+");

	if (file_circunferencia == NULL)
	{
		printf("\033[31mError al abrir el archivo\n");
		printf("\033[0m\n");	
		return(0);
		//TODO_p: ¿a dónde vamos?, al menú
	}

	int i;

	for (i=0; i < (N); i++) // Escribe cada punto (fila de la matriz) en el archivo
	{
		fprintf(file_circunferencia, "%f %f\n", circunferencia[i][0], circunferencia[i][1]);
	}

	fprintf(file_circunferencia, "%f %f\n", circunferencia[0][0], circunferencia[0][1]); // Termina con el primer punto (para cerrar el polígono)

	fclose(file_circunferencia); // Cierre del archivo

	return(0);
}

int meshgrid(float ** xx, float ** yy, float *dperfil)
{
	int i, j;

	float * rango;
	rango = (float *) malloc(M * sizeof(float));

	rango = linspace(dperfil[6]-2, dperfil[7]+2, M);

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
				psi[i][j] = dflujo[0] * (rho * sin(theta - dflujo[1]) - (pow(dperfil[2],2)/rho) * sin(theta - dflujo[1])) + (T/(2*M_PI)) * log(fabs(rho));
			else
				psi[i][j] = -dflujo[0] * (rho * sin(theta - dflujo[1]) - (pow(dperfil[2],2)/rho) * sin(theta - dflujo[1])) + (T/(2*M_PI)) * log(fabs(rho));
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
	
	return(0);
}


int flujo(float * dperfil/* ,float * opf*/) // TODO_p: al juntar unir las opciones
{
	int i;

	// Vector con los datos del flujo
	float * dflujo;
	dflujo = (float *) malloc(5 * sizeof(float)); // Reserva de memoria para el vector

	datos_flujo(dperfil, dflujo);

	float ** xx;
	xx = (float **) malloc(M * sizeof(float *));

	for (i=0; i < N; i++)
		xx[i] = (float *) malloc(M * sizeof(float));

	float ** yy;
	yy = (float **) malloc(M * sizeof(float *));

	for (i=0; i < N; i++)
		yy[i] = (float *) malloc(M * sizeof(float));

	float ** psi;
	psi = (float **) malloc(M * sizeof(float *));

	for (i=0; i < N; i++)
		psi[i] = (float *) malloc(M * sizeof(float));

	float * xxflat;
	xxflat = (float *) malloc(M * M * sizeof(float));

	float * yyflat;
	yyflat = (float *) malloc(M * M * sizeof(float));

	meshgrid(xx, yy, dperfil);

	calculo_flujo(dperfil, dflujo, xx, yy, psi);

	imprimir_flujo(xx, yy, psi);

	imprimir_psi(psi);

	printf("\n");
	printf("\nFlujo: %f\n", dflujo[3]);
	printf("Coeficiente de sustentación: %f\n", dflujo[4]);

	return 0;
}

/* Menú para modificar opciones de plot de circunferencia */
int menu_flujo_cil (float * opc, float * opp, float * opfc)
{
	printf("\033[33m   1. Color de flujo \n   2. Tamaño de la linea de flujo\n   3. Color del cilindro \n   4. Salir\n"); // 1: (l)ine(c)olor flujo   2: (l)ine(w)idth   3:(l)ine(c)olor cilindro  
	printf("\033[0m\n");
	int opcion, lcf, lcc;
	float lw;

	scanf ("%d", &opcion);
	while (opcion!=1 && opcion!=2 && opcion!=3 && opcion!=4) //En caso de que el valor introducido sea diferente del esperado, espera otra introduccion
	{
		printf("\033[31m         Valor no valido\n"); // TODO_j si no quereis que haga nada - color
		printf("\033[0m\n");
		scanf("%d", &opcion);
	}

	switch(opcion)
	{
		case 1:
			printf("\033[33m  (1) Rojo \n  (2) Verde \n  (3) Azul \n  (7) Negro \n  (9) Gris\n");
			printf("\033[0m\n");
			scanf ("%d", &lcc);

			while (lcc!=1 && lcc!=2 && lcc!=3 && lcc!=7 && lcc!=9)  // En caso de que el valor introducido sea diferente del esperado, espera otra introduccion
			{	
				printf("\033[31mValor no valido\n"); // TODO_j si no quereis que haga nada - color
				printf("\033[0m\n");
				scanf("%d", &lcc);
			}

			opfc[0]= (float) lcc;  // Introducimos el valor obtenido (con su correspondiente casting) en el vector

			menu_flujo_cil (opc, opp, opfc); // Se vuelve al menu del circulo
			break;

		case 2:
			printf("\033[33m  Tamaño de la linea de flujo: ");
			printf("\033[0m");
			scanf ("%f", &lw);

			opfc[2]=lw;  // Introducimos el valor obtenido en el vector

			menu_flujo_cil (opc, opp, opfc); // Se vuelve al menu del circulo
			break;

		case 3:
			printf("\033[33m  (1) Rojo \n  (2) Verde \n  (3) Azul \n  (7) Negro \n  (9) Gris\n");
			printf("\033[0m\n");
			scanf ("%d", &lcf);

			while (lcf!=1 && lcf!=2 && lcf!=3 && lcf!=7 && lcf!=9)  // En caso de que el valor introducido sea diferente del esperado, espera otra introduccion
			{	
				printf("\033[31mValor no valido\n"); // TODO_j si no quereis que haga nada - color
				printf("\033[0m\n");
				scanf("%d", &lcf);
			}

			opfc[2]= (float) lcf;  // Introducimos el valor obtenido (con su correspondiente casting) en el vector

			menu_flujo_cil (opc, opp, opfc); // Se vuelve al menu del circulo
			break;

		/*case 4: TODO_j
			menu_opciones(opc, opp, opfc); // Se vuelve al menu de opciones
			break;*/
	}

	return 0;
}


/* Plotea los puntos del flujo del cilindro*/
int plotfc (float *opfc)
{
	// Tubería UNIX para usar GNU Plot desde el programa
	FILE *pipefc = popen ("gnuplot -pesist","w"); 

	fprintf(pipefc, "set terminal push\n set terminal unknown\n set table 'temp.dat'\n set dgrid3d 31,31\n set view map\n unset surface\n set contour\n set cntrparam bspline\n set cntrparam levels incr -10,0.3,10\n splot 'pts_flujo_cilindro.dat' using 1:2:3 with lines\n unset table\n unset dgrid3d\n unset key\n set terminal pop\n");
	fprintf(pipefc, "set size ratio 1\n plot 'temp.dat' with lines lc %.0f lw %f, 'pts_circun.dat' with filledcurves x1 fs pattern 3 lc %.0f\n !rm temp.dat\n", opfc[0], opfc[1], opfc[2]);

	pclose (pipefc);

	return 0;
}

int main(int argc, char const *argv[])
{
	// Vector con los datos del flujo
	float * dperfil;
	dperfil = (float *) malloc(14 * sizeof(float)); // Reserva de memoria para el vector

	float *opc; // Vector de salida del espacio
	opc = (float *) malloc(3 * sizeof(float)); // Memoria reservada para el vector

	float *opp; // Vector de salida del espacio
	opp = (float *) malloc(3 * sizeof(float)); // Memoria reservada para el vector

	float *opfc; // Vector de salida del espacio
	opfc = (float *) malloc(3 * sizeof(float)); // Memoria reservada para el vector
	opfc[0] = 3;
	opfc[1] = 1.5;
	opfc[2] = 9;

	int i;

	// Matriz Nx2 que almacenará los puntos de la circunferencia
	float ** circunferencia;
	circunferencia = (float **) malloc(N * sizeof(float *)); // Reserva de memoria para cada vector

	for (i=0; i < N; i++)
	{
		circunferencia[i] = (float *) malloc(2 * sizeof(float)); // Reserva de memoria para las dos coordenadas del vector
	}

	menu_flujo_cil(opc, opp, opfc);

	do
	{
		datos_perfil (dperfil);
	} while (limites(dperfil) == 0); // TODO_p: ¿cómo añade el caso al vector dperfil? - A: lo añade cuando comprueba que caso es a la ultima coordenada del vector en cada caso (lineas 126, 141, ...)

	// Calcula los puntos de la circunferencia
	matriz_circunferencia(dperfil, circunferencia);

	// Guarda los puntos de la circunferencia en el archivo pts_circunferencia.dat
	imprimir_circunferencia(circunferencia);

	flujo(dperfil);

	plotfc(opfc);

	return 0;
}