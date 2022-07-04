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
const int Dist1m = 75;
const int Dist1b = 40;
const int Dist1a = 125;
const int Dist2m = 400;
const int Dist2b = 200;
const int Dist2a = 600;
const int luzm = 35;
const int luzb = 20;
const int luza = 70;
int Dist1;
int Dist2;
int luz;

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

   int Dist1 = Dist1m;
   int Dist2 = Dist2m;
   int luz = luzm;

   if(distancia<Dist1){
    digitalWrite(ALARMA, HIGH);
   }
   if(distancia>Dist1){
    digitalWrite(ALARMA, LOW);
   }

   if(distancia2<Dist2){
    digitalWrite(ALARMA2, HIGH);
   }
   if(distancia2>Dist2){
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

   
   if(valor>luz){
      if(estado_per==1){

        digitalWrite(3,LOW);
        digitalWrite(2,HIGH);
        delay(2000);
        digitalWrite(3,LOW);
        digitalWrite(2,LOW);  
        estado_per=0;      
      }
   }
   
   if(valor<luz){
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
        if(mensaje_entrada.compareTo("Dist1a")==0){
          Dist1=Dist1a;
        }
        else
        if(mensaje_entrada.compareTo("Dist1m")==0){
          Dist1=Dist1m;
        }
        else
        if(mensaje_entrada.compareTo("Dist1b")==0){
          Dist1=Dist1b;
        }
        else
        if(mensaje_entrada.compareTo("Dist2a")==0){
          Dist2=Dist2a;
        }
        else
        if(mensaje_entrada.compareTo("Dist2m")==0){
          Dist2=Dist2m;
        }
        else
        if(mensaje_entrada.compareTo("Dist2b")==0){
          Dist2=Dist2b;
        }else
        if(mensaje_entrada.compareTo("luza")==0){
          luz=luza;
        }
        else
        if(mensaje_entrada.compareTo("luzm")==0){
          luz=luzm;
        }
        else
        if(mensaje_entrada.compareTo("luzb")==0){
          luz=luzb;
        }
        
      
      else
        mensaje_salida="COMANDO DESCONOCIDO";

    Serial.println(mensaje_salida);
  }
}
