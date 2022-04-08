//Dentro de la opción alarma(distancia todavía sin definir)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>

#include "SerialClass/SerialClass.h"
#define MAX_BUFFER 200
#define PAUSA_MS 200 


void verifica_sensores(Serial* Arduino,char* port)
{
 float distancia;
 if (Arduino->IsConnected())
 { distancia = leer_sensor_distancia(Arduino);
if (distancia!=-1)
 printf("\nDistancia: %f\n", distancia);
 }
 else
 { printf("\nNo se ha podido conectar con Arduino.\n");
 printf("Revise la conexión, el puerto %s y desactive el monitor serie del
 IDE de Arduino.\n",port);
 }
} 

void monitorizar_sensor_distancia(Serial* Arduino)
{
float frecuencia, distancia;
char tecla;
do
 {
 printf("Establezca frecuencia de muestreo (0,5 Hz - 2,0 Hz):");
 scanf_s("%f", &frecuencia);
 } while (frecuencia < 0.5 || frecuencia>2.0);
 printf("Pulse una tecla para finalizar la monitorización\n");
do
 {
 if (Arduino->IsConnected())
 {
 distancia = leer_sensor_distancia(Arduino);
 if (distancia != -1)
 printf("%.2f ", distancia);
 else
 printf("XXX ");
 }
 else
 printf("\nNo se ha podido conectar con Arduino.\n");
 if ((1 / frecuencia) * 1000 > PAUSA_MS)
 Sleep((1 / frecuencia) * 1000 - PAUSA_MS);
 } while (_kbhit() == 0);
 tecla = _getch();
return;
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

int main(void)
{
Serial* Arduino;
char puerto[] = "COM6"; // Puerto serie al que está conectado Arduino
int opcion_menu;
 setlocale(LC_ALL, "es-ES");
 Arduino = new Serial((char*)puerto);
do
 { opcion_menu = menu();
 switch (opcion_menu)
 {
 case 1: verifica_sensores(Arduino,puerto);
 break;
 case 2: monitorizar_sensor_distancia(Arduino);
 break;
 case 3: activar_alarma_distancia(Arduino,&estado_alarma);
 break;
 case 4: break;
 default:printf("\nOpción incorrecta\n");
 }
 } while (opcion_menu != 4);
return 0;
}

int menu_alarma(void)
{
int opcion_alarma;
 printf("\n");
 printf("Menú Principal\n");
 printf("==============\n");
 printf("1 - Verificar sensores.\n");
 printf("2 - Monitorizar sensores.\n");
 printf("3 - Activar/Desactivar alarma por distancia\n");
 printf("4 - Salir de la aplicación\n");
 printf("Opción:");
 scanf_s("%d", &opcion_alarma);
return opcion_alarma;
} 
