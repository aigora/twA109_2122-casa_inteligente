#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>
#include <time.h>

#include "SerialClass/SerialClass.h"
#define MAX_BUFFER 200
#define PAUSA_MS 200 

int menu_alarma(void);
void verifica_sensores(Serial*, char*);
void activar_alarma_distancia(Serial* Arduino);
void comprobar_mensajes(Serial*);
float leer_sensor_distancia(Serial*);
int Enviar_y_Recibir(Serial*, const char*, char*);
float float_from_cadena(char* cadena);


int main(void)
{
	Serial* Arduino;
	char puerto[] = "COM3"; // Puerto serie al que está conectado Arduino
	int opcion_menu;

	Arduino = new Serial((char*)puerto);
	do {
		opcion_menu = menu_alarma();
		switch (opcion_menu) {

		case 1: verifica_sensores(Arduino, puerto);
			break;

		case 2: activar_alarma_distancia(Arduino);
			break;

		case 3:
			break;

		default:printf("\nOpción incorrecta\n");
		}
	} while (opcion_menu != 3);

	return 0;
}

int menu_alarma(void) {
	int opcion_alarma;

	printf("\n");
	printf("Menu Principal\n");
	printf("==============\n");
	printf("1 - Verificar sensores.\n");
	printf("2 - Activar/Desactivar alarma\n");
	printf("3 - Salir de la aplicación\n");
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

				fprintf(registros, "Se ha detectado intruso a las %.19s\n", timebuf);
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