#include <stdio.h>  //TODO_j funcion centro []  centro []

float f1 (float x, float y) //Funcion interpolada --> x<1 && y<1
{
	float alim;
	alim = 1.0002 * (1.43 * (x - 0.0001) * (y - 0.0001) + (1 - x) * (y - 0.0001) + 1.1 * (x - 0.0001) * (1 - y) + 0.0002 * (1 - x) * (1 - y));
	return alim;
}

float f2 (float x, float y) //Funcion interpolada --> x>1 && y<1
{
	float alim;
	alim = 0.25 * (5.2 * (x - 1) * (y - 0.0001) + 1.43 * (5 - x) * (y - 0.0001) + 5.1 * (x - 1) * (1 - y) + 1.1 * (5 - x) * (1 - y));
	return alim;
}

float f3 (float x, float y) //Funcion interpolada --> x<1 && y>1
{
	float alim;
	alim = 0.25 * (7 * (x - 0.0001) * (y - 1) + 5 * (1 - x) * (y - 1) + 1.43 * (x - 0.0001) * (5 - y) + (1 - x) * (5 - y));
	return alim;
}

float f4 (float x, float y) //Funcion interpolada --> x>1 && y>1
{
	float alim;
	alim = (9 * (x - 1) * (y - 1) + 7 * (5 - x) * (y - 1) + 5.2 * (x - 1) * (5 - y) + 1.43 * (5 - x) * (5 - y))/16;
	return alim;
}

int main ()
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

	if (a <= 0)
	{
		printf("Valor no valido (a <= cero)\n"); 
		return 0;
	}

	if (a < yc)
	{
		printf("Valor no valido (a < yc)\n"); 
		return 0;
	}

	if (xc==0 && yc==0) //CASO 1: funcion no implementada - puede tener limites TODO_j
	{
		printf("Not implemented yet!\n");
		return 0;
	}	

	if (xc==0) //CASO 2: considerar formula especial - y sus posibles limites TODO_j
	{
		if (a==yc)
		{
			printf("Valor valido (a = yc)\n");
			return 0;			
		}
		else //Si a<xc o a>xc
		{
			printf("Valor no valido (a != yc)\n"); 
			return 0;
		}	
	}
 
	if (yc==0) //CASO 3
	{
		if (a>xc)
		{
			printf("Valor valido (a > xc)\n");
			return 0;
		}
		else //Si a==xc o a<xc
		{
			printf("Valor no valido (a = xc || a < xc)\n"); 
			return 0;
		}
			
	}

	else //CASO 4
	{
		if (xc>6 || yc>6) //Limitacion centro menos que 6
		{
			printf("Valor no valido (xc>6 || yc>6)\n");
			return 0;
		}
		else
		{
			if (xc<1)
			{
				if (yc<1) //xc<1 yc<1 ----> f1
				{
					if (f1(xc,yc)<=a)
					{
						printf("Valor valido (f1(xc,yc)<=a)\n"); 
						return 0;
					}
					else
					{
						printf("Valor no valido (f1(xc,yc)>a)\n"); 
						return 0;
					}
				}
				else //xc<1 yc>1 ----> f3
				{
					if (f3(xc,yc)<=a)
					{
						printf("Valor valido (f3(xc,yc)<=a)\n"); 
						return 0;
					}
					else
					{
						printf("Valor no valido (f3(xc,yc)>a)\n"); 
						return 0;
					}
				}
			}
			else
			{
				if (yc<1) //xc>1 yc<1 ----> f2
				{
					if (f2(xc,yc)<=a)
					{
						printf("Valor valido (f2(xc,yc)<=a)\n"); 
						return 0;
					}
					else
					{
						printf("Valor no valido (f2(xc,yc)>a)\n"); 
						return 0;
					}
				}
				else //xc>1 yc>1 ----> f4
 				{
					if (f4(xc,yc)<=a)
					{
						printf("Valor valido (f4(xc,yc)<=a)\n"); 
						return 0;
					}
					else
					{
						printf("Valor no valido (f4(xc,yc)>a)\n"); 
						return 0;
					}
				}
			}
		}
	}
	return 0;
}