#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>
#include <time.h>

#include "SerialClass/SerialClass.h"
#define MAX_BUFFER 200
#define PAUSA_MS 200 
#define TAM 1000

int menu_alarma(void);
void verifica_sensores(Serial*, char*);
void activar_alarma_distancia(Serial* Arduino);
void Dist1a(Serial* Arduino);
void Dist1m(Serial* Arduino);
void Dist1b(Serial* Arduino);
void Dist2a(Serial* Arduino);
void Dist2m(Serial* Arduino);
void Dist2b(Serial* Arduino);
void luza(Serial* Arduino);
void luzm(Serial* Arduino);
void luzb(Serial* Arduino);
void comprobar_mensajes(Serial*);
float leer_sensor_distancia(Serial*);
int Enviar_y_Recibir(Serial*, const char*, char*);
float float_from_cadena(char* cadena);



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

int main(void)
{
	Serial* Arduino;
	char puerto[] = "COM3"; // Puerto serie al que está conectado Arduino
	int opcion_menu, opcion_parametro, opcion_lista, contador=1, i=0;
	FILE *registros;
	char letra;
	char a[TAM];


	Arduino = new Serial((char*)puerto);
	do {
		opcion_menu = menu_alarma();
		switch (opcion_menu) {

		case 1: verifica_sensores(Arduino, puerto);
			break;

		case 2: activar_alarma_distancia(Arduino);
			break;

		case 3: registros=fopen_s("Registros.txt","r");
				if (registros == NULL){
					printf("Error al abrir el fichero");
				} 
				else
				{
					while(feof(registros)==0){
						letra=fgetc(registros);
						printf("%c", letra);
						
					}
				}
				fclose(registros);
			break;
		
		case 4: printf("Linea a eliminar:");
				scanf_s("%d", &opcion_lista);
		
				registros=fopen_s("Registros.txt","r");
				if (registros == NULL){
					printf("Error al abrir el fichero");
				} 
				else
				{
					while(feof(registros)==0){
						letra=fgetc(registros);
						printf("%c", letra);
						if (letra=='\0'){
							contador++;
						}
						do{
							A[i]=letra;//Guarda lo que queramos
							i++;
						}while(contador!=opcion_lista);
					}
				}
				fclose(registros);

		
				registros=fopen_s(&registros, "Registros.txt", "w");
				if (registros == NULL)
					printf("No se podran registrar los datos\n");
	
				else
					for (i = 0;i <= 1000;i++){
					fprintf(registros, "%c", A[i]);//Sobrescribe lo guardado
					}
		
				fclose(registros);
			break;
		
		case 5: printf("\n");
			printf("Menu Principal\n");
			printf("==============\n");
			printf("1 - Distancia 1 alta\n");
			printf("2 - Distancia 1 media\n");
			printf("3 - Distancia 1 baja");
			printf("4 - Distancia 2 alta\n");
			printf("5 - Distancia 2 media\n");
			printf("6 - Distancia 2 baja");
			printf("7 - Luz alta");
			printf("8 - Luz media");
			printf("9 - Luz baja\n");
			printf("10 - Salir de parametros\n");
			do{
				case 1: Dist1a(Arduino);
					break;

				case 2: Dist1m(Arduino);
					break;

				case 3: Dist1b(Arduino);
					break;
		
				case 4: Dist2a(Arduino);
					break;

				case 5: Dist2m(Arduino);
					break;

				case 6: Dist2b(Arduino);
					break;
					
				case 7: luza(Arduino);
					break;

				case 8: luzm(Arduino);
					break;

				case 9: luzb(Arduino);
					break;
					
				case 10:
					break;
					
				default:printf("\nOpción incorrecta\n");
					
			}while(opcion_parametro != 10);
			
			break;
			
		case 6:
			break;

		default:printf("\nOpción incorrecta\n");
		}
	} while (opcion_menu != 6);

	return 0;
}

int menu_alarma(void) {
	int opcion_alarma;

	printf("\n");
	printf("Menu Principal\n");
	printf("==============\n");
	printf("1 - Verificar sensores.\n");
	printf("2 - Activar/Desactivar alarma\n");
	printf("3 - Lista de tiempo registrado");
	printf("4 - Borrar lista completa");
	printf("5 - Cambiar parámetros");
	printf("6 - Salir de la aplicación\n");
	printf("Opcion:");
	scanf_s("%d", &opcion_alarma);

	return opcion_alarma;
}

void verifica_sensores(Serial* Arduino, char* port)
{
	float distancia;
	struct tm newtime;
	char am_pm[] = "AM";
	__time64_t long_time;
	char timebuf[26];
	errno_t err;
	FILE* registros;
	errno_t e;

	_time64(&long_time);

	err = _localtime64_s(&newtime, &long_time);


	err = asctime_s(timebuf, 26, &newtime);

	if (Arduino->IsConnected())
	{
		distancia = leer_sensor_distancia(Arduino);
		if ((distancia != -1) && (distancia > 75))
			printf("\nNo se detecta nada\n");
		if ((distancia != -1) && (distancia < 75)) {
			printf("\nSe detecta intruso\n");
			e = fopen_s(&registros, "Registros.txt", "a+t");
			if (registros == NULL)
				printf("No se podran registrar los datos\n");

			else

				fprintf(registros, "Se ha detectado intruso el %.19s\n", timebuf);
		}
	}
	else
	{
		printf("\nNo se ha podido conectar con Arduino.\n");
		printf("Revise la conexión, el puerto %s y desactive el monitor serie del IDE de Arduino.\n", port);
	}
}

float leer_sensor_distancia(Serial* Arduino)
{
	float distancia;
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Enviar_y_Recibir(Arduino, "GET_DISTANCIA\n", mensaje_recibido);

	if (bytesRecibidos <= 0)
		distancia = -1;
	else
		distancia = float_from_cadena(mensaje_recibido);
	return distancia;
}

int Enviar_y_Recibir(Serial* Arduino, const char* mensaje_enviar, char* mensaje_recibir)
{
	int bytes_recibidos = 0, total = 0;
	int intentos = 0, fin_linea = 0;


	Arduino->WriteData((char*)mensaje_enviar, strlen(mensaje_enviar));
	Sleep(PAUSA_MS);

	bytes_recibidos = Arduino->ReadData(mensaje_recibir, sizeof(char) * MAX_BUFFER - 1);

	while ((bytes_recibidos > 0 || intentos < 5) && fin_linea == 0)
	{
		if (bytes_recibidos > 0)
		{
			total += bytes_recibidos;
			if (mensaje_recibir[total - 1] == 13 || mensaje_recibir[total - 1] == 10)
				fin_linea = 1;
		}
		else
			intentos++;
		Sleep(PAUSA_MS);
		bytes_recibidos = Arduino->ReadData(mensaje_recibir + total, sizeof(char) * MAX_BUFFER - 1);
	}
	if (total > 0)
		mensaje_recibir[total - 1] = '\0';

	//printf("LOG: %d bytes -> %s\nIntentos=%d - EOLN=%d\n", total, mensaje_recibir,intentos,fin_linea);
	return total;
}

float float_from_cadena(char* cadena)
{
	float numero = 0;
	int i, divisor = 10, estado = 0;


	for (i = 0; cadena[i] != '\0' && estado != 3 && i < MAX_BUFFER; i++)
		switch (estado)
		{
		case 0:// Antes del número
			if (cadena[i] >= '0' && cadena[i] <= '9')
			{
				numero = cadena[i] - '0';
				estado = 1;
			}
			break;
		case 1:// Durante el número
			if (cadena[i] >= '0' && cadena[i] <= '9')
				numero = numero * 10 + cadena[i] - '0';
			else
				if (cadena[i] == '.' || cadena[i] == ',')
					estado = 2;
				else
					estado = 3;
			break;
		case 2: // Parte decimal
			if (cadena[i] >= '0' && cadena[i] <= '9')
			{
				numero = numero + (float)(cadena[i] - '0') / divisor;
				divisor *= 10;
			}
			else
				estado = 3;
			break;
		}
	return numero;
}

void activar_alarma_distancia(Serial* Arduino)
{
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Enviar_y_Recibir(Arduino, "SET_MODO_ALARMA\n", mensaje_recibido);
	if (bytesRecibidos <= 0)
		printf("\nNo se ha recibido confirmación\n");
	else
		printf("\n%s\n", mensaje_recibido);
}

void Dist1a(Serial* Arduino)
{
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Enviar_y_Recibir(Arduino, "Dist1a\n", mensaje_recibido);
	if (bytesRecibidos <= 0)
		printf("\nNo se ha recibido confirmación\n");
	else
		printf("\n%s\n", mensaje_recibido);
}
void Dist1m(Serial* Arduino)
{
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Enviar_y_Recibir(Arduino, "Dist1m\n", mensaje_recibido);
	if (bytesRecibidos <= 0)
		printf("\nNo se ha recibido confirmación\n");
	else
		printf("\n%s\n", mensaje_recibido);
}
void Dist1b(Serial* Arduino)
{
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Enviar_y_Recibir(Arduino, "Dist1b\n", mensaje_recibido);
	if (bytesRecibidos <= 0)
		printf("\nNo se ha recibido confirmación\n");
	else
		printf("\n%s\n", mensaje_recibido);
}
void Dist2a(Serial* Arduino)
{
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Enviar_y_Recibir(Arduino, "Dist2a\n", mensaje_recibido);
	if (bytesRecibidos <= 0)
		printf("\nNo se ha recibido confirmación\n");
	else
		printf("\n%s\n", mensaje_recibido);
}
void Dist2m(Serial* Arduino)
{
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Enviar_y_Recibir(Arduino, "Dist2m\n", mensaje_recibido);
	if (bytesRecibidos <= 0)
		printf("\nNo se ha recibido confirmación\n");
	else
		printf("\n%s\n", mensaje_recibido);
}
void Dist2b(Serial* Arduino)
{
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Enviar_y_Recibir(Arduino, "Dist2b\n", mensaje_recibido);
	if (bytesRecibidos <= 0)
		printf("\nNo se ha recibido confirmación\n");
	else
		printf("\n%s\n", mensaje_recibido);
}
void luza(Serial* Arduino)
{
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Enviar_y_Recibir(Arduino, "luza\n", mensaje_recibido);
	if (bytesRecibidos <= 0)
		printf("\nNo se ha recibido confirmación\n");
	else
		printf("\n%s\n", mensaje_recibido);
}
void luzm(Serial* Arduino)
{
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Enviar_y_Recibir(Arduino, "luzm\n", mensaje_recibido);
	if (bytesRecibidos <= 0)
		printf("\nNo se ha recibido confirmación\n");
	else
		printf("\n%s\n", mensaje_recibido);
}
void luzb(Serial* Arduino)
{
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Enviar_y_Recibir(Arduino, "luzb\n", mensaje_recibido);
	if (bytesRecibidos <= 0)
		printf("\nNo se ha recibido confirmación\n");
	else
		printf("\n%s\n", mensaje_recibido);
}




FILE *pf;

