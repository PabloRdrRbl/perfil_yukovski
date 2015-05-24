#include <stdio.h>
#include <string.h>
int menu (int control)
{
	int numero;
	if (control==0)
	{
		printf("\033[33m************************************************************************\n");
		printf("Bienvenido al programa. Elija la opcion que desea ejecutar:\n1:Construccion del perfil del ala\n2:Flujo en el perfil del ala\n3:Opciones\n4:Salir\n");
		printf("\033[33m************************************************************************");
		printf("\033[0m\n");	
	}
	scanf("%d",&numero);
	switch(numero)
	{
		case 1:
		//Perfiles del ala(construccion)
		    printf("Perfil\n");    
		    menu(0);
		    break;
		            
		case 2:
		//Flujo en los distintos tipos de perfil
		    printf("Flujo\n");
		    menu(0);
		    break;

		case 3:
		//Opciones
		    printf("Opciones\n");
		    menu(0);
		    break;

        case 4:
		//Salir del programa//
	        printf("\033[34mProyecto final en C de la asignatura de Informatica\nGrado en Ingenieria Aeroespacial\n\n              Adrian Delgado Marcos\n              Javier Lobato Perez\n              Pablo Rodriguez Robles");
	        printf("\033[0m\n\n");
	        return(0);
     	    break; 
     	
     	default:
            menu(1); 
            break;        
	}	
  }  		
int main()
{
	menu(0);
	return(0);
}