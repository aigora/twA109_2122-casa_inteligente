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
int mostrar_tiempo(TIEMPO* u);
void listado_tiempo(datos* lista);
void borrar_lista(datos* lista);
int escribir_fichero(datos* registro);


typedef struct
{
	int hora;
	int minutos;
	int segundos;
}TIEMPO;
struct DATOS
{
	TIEMPO* tiempo;
	struct DATOS* siguiente;
};
typedef struct DATOS datos;
TIEMPO* leer_tiempo(void)
{
	TIEMPO* tiempo;
	char intro;
	scanf_s("%d:%d:%d", tiempo->hora, tiempo->minutos,tiempo->segundos);
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
int mostrar_tiempo(TIEMPO* u)
{
	printf("%d:\t%d:\t%d\n", u->hora, u->minutos, u->segundos);

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

int escribir_fichero(datos* registro)
{
	FILE* fichero;
	errno_t e;
	e = fopen_s(&fichero, "Registros.txt", "wt");
	if (e != 0)
	{
		printf("No se ha podido abrir el fichero\n");
	}
	else
	{
		while (lista != NULL) {
			fprintf(fichero, "% d:\t % d:\t % d\n", mov_t->hora, mov_t->minutos, mov_t->segundos);
			registro = registro->siguiente;
		}
		fclose(fichero);
	}
	return e;
}

int main(void)
{
	datos* plista = NULL;
	TIEMPO* mov_t;
	
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
			break;
		case 2:
			borrar_lista(&plista);
			break;
		case 3:
			break;
		}
	} while (opcion != 3);

	return 0;
}