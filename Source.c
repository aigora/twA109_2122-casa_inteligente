#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define longitud 50

/*int main()
{
	if (mensaje == se enciende led){
	 ampliar lista con la funcion
	 }  //cuando arduino envia mensaje se enciende led, usa la funcion registrar tiempo para guardar el tiempo.

	char tmpbuf[128];
	_strtime_s(tmpbuf, 128);
	printf("OS time:\t\t\t\t%s\n", tmpbuf);
}
*/

int registro_tiempo(datos** plista);
int mostrar_tiempo(TIEMPO u);
void listado_tiempo(datos* lista);
void borrar_lista(datos* lista);

typedef struct
{
	int hora;
	int minutos;
	int segundos;
}TIEMPO;
struct DATOS
{
	TIEMPO tiempo;
	struct DATOS* siguiente;
};
typedef struct DATOS datos;
TIEMPO leer_tiempo(void)
{
	TIEMPO tiempo;
	char intro;
	scanf_s("%d:%d:%d", &tiempo.hora, &tiempo.minutos, &tiempo.segundos);
	scanf_s("%c", &intro);
	return tiempo;
}

int registro_tiempo(datos** plista)
{
	datos* cab = *plista;
	datos* p;
	int cod_error = 0;
	p = (datos*)malloc(sizeof(datos));
	if (p == NULL)
	{
		printf("No se ha podido reservar memoria para el tiempo nuevo\n");
		cod_error = -1;
	}
	else
	{
		p->tiempo = leer_tiempo();
		p->siguiente = cab;
		cab = p;
	}
	*plista = cab;
	return cod_error;
}
int mostrar_tiempo(TIEMPO u)
{
	printf("%d:\t%d:\t%d\n", u.hora, u.minutos, u.segundos);

}
void registrar_tiempo(TIEMPO u)
{

}

void listado_tiempo(datos* lista)
{
	datos* p;
	if (lista == NULL)
		printf("Ahora no hay tiempos guardados\n");
	else
	{
		printf("Horas:Minutos:Segundos\n");

		for (p = lista; p != NULL; p = (datos*)p->siguiente)
			mostrar_tiempo(p->tiempo);
	}
}
void borrar_lista(datos* lista)
{
	int n, dato;
	datos* aux = lista;
	while (aux != NULL)
	{
		lista = lista->siguiente;
		free(aux);
		aux = lista;
	}
}



int main(void)
{
	datos* plista = NULL;
	FILE* registros;
	errno_t e;
	e = fopen(&registros, "Registros.txt", "a+t");
	TIEMPO mov_t;
	if (registros == NULL)
		printf("No se podrán registrar los datos\n");

	else
	{
		printf("Puede consultar los registros de movimiento en el fichero  de texto registros\n");
		
		while (!feof(registros))
		{
			printf("Se registrara cualquier actividad detectada");			
			fscanf_s(registros, "% d:\t % d:\t % d\n", mov_t.hora, mov_t.minutos, mov_t.segundos);
		
			fprintf(registros, "Se ha activado la alarma a las %d:%d:%d\n", mov_t.hora, mov_t.minutos, mov_t.segundos); // Graba en el fichero las horas de activacion
		}
		fclose(registros);
	}
	return 0;
	int opcion;

	printf("1-Lista de tiempo registrado");
	printf("2-Borrar lista completa");
	printf("3-Salir");
	do
	{
		switch (opcion)
		{
		case 1:
			listado_tiempo(&plista);
			
		}


			break;
		case 2:
			borrar_lista(&plista);
			break;
		case 3:
			break;
		}
	} while (opcion != 3);

}