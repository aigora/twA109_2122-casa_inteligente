#include<stdio.h>
#include<stdlib.h>

int menu_ppal(int); 





int menu_ppal(int opcion) {

	do {
		printf("CASA INTELIGENTE");
		printf("-------------------");
		printf("1-Alarma");
		printf("2-Persianas");
		printf("3-Luces automáticas");
		printf("4-Ver registros");
		printf("5-Salir");
		scanf_s("%d", &opcion);

	} while ((opcion != 1) && (opcion != 2) && (opcion != 3) && (opcion != 4) && (opcion != 5));
}