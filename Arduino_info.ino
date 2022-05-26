enum Estados {MODO_MANUAL, MODO_ALARMA_ON,MODO_ALARMA_OFF};
Estados estado=MODO_MANUAL;
const int ALARMA2 = 4;
const int ECHO2 = 5;
const int TRIG2 = 6;
const int ALARMA = 7;
const int ECHO = 8;
const int TRIG = 9;
String mensaje_entrada;
String mensaje_salida;
int estado_per;

void setup() {

  Serial.begin(9600);

  pinMode(ALARMA, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ALARMA2, OUTPUT);
  pinMode(ECHO2, INPUT);
  pinMode(TRIG2, OUTPUT);

}

void loop() {

   long distancia = medir(TRIG,ECHO);
   long distancia2 = medir(TRIG2,ECHO2);

   int valor = analogRead(A2);
   

   if(distancia<75){
    digitalWrite(ALARMA, HIGH);
   }
   if(distancia>75){
    digitalWrite(ALARMA, LOW);
   }

   if(distancia2<400){
    digitalWrite(ALARMA2, HIGH);
   }
   if(distancia2>400){
    digitalWrite(ALARMA2, LOW);
   }

    
  procesar_mensajes();
  switch (estado)
  {
    case MODO_MANUAL:
        break;

    case MODO_ALARMA_OFF:
        
        estado=MODO_MANUAL;
        break;
  }

   
   if(valor>35){
      if(estado_per==1){

        digitalWrite(3,LOW);
        digitalWrite(2,HIGH);
        delay(2000);
        digitalWrite(3,LOW);
        digitalWrite(2,LOW);  
        estado_per=0;      
      }
   }
   
   if(valor<35){
      if(estado_per==0){

        digitalWrite(3,HIGH);
        digitalWrite(2,LOW);
        delay(2000);
        digitalWrite(3,LOW);
        digitalWrite(2,LOW);   
        estado_per=1;          
      }
   }

  
  
}
int medir(int TRIG, int ECHO){

  long distancia;
  
   digitalWrite(TRIG,HIGH);
   delay(1000);
   digitalWrite(TRIG,LOW);

   distancia = pulseIn(ECHO,HIGH)*343/(2*1000);

   return distancia;
}


void procesar_mensajes(void)
{
  if( Serial.available()> 0) 
  {
    mensaje_entrada = Serial.readStringUntil('\n');
    if (mensaje_entrada.compareTo("GET_DISTANCIA")==0)
    {
      mensaje_salida= medir(TRIG, ECHO);
    }
    else
      if (mensaje_entrada.compareTo("SET_MODO_ALARMA")==0)
      {
        if (estado==MODO_ALARMA_OFF || estado==MODO_MANUAL)
        {
            estado=MODO_ALARMA_ON;
            mensaje_salida=String("ALARMA_OPERATIVA");
        }
        else
        {
          estado=MODO_ALARMA_OFF;
          mensaje_salida=String("ALARMA_INACTIVA");
         }
      }
      else
        mensaje_salida="COMANDO DESCONOCIDO";

    Serial.println(mensaje_salida);
  }
}
