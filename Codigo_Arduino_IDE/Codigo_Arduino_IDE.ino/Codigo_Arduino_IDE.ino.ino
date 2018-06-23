/*
 * ******************************* ANALIZADOR DE ESPECTRO DE BAJO COSTO *****************************************************
 * DESARROLLADORES: MIGUEL ANGEL CALIFA URQUIZA - CARLOS MAURICIO PALLARES - MIGUEL GALEZO - DUVAN VARELA - MARIA INES
 * MATERIA: MEDIOS DE TRANSMISION - CARRERA: INGENIERIA ELECTRONICA - UNIVERSIDAD: FRANCISCO DE PAULA SANTANDER
 * AÑO DE DESARROLLO: 2018 - I. VERSION: 1.00 PRODUKTIV.
 * INTERFAZ DE GRAFICAS: MATLAB - GUIDE.
 * VERSION DE ARDUINO: 1.8.5
 * VERSION DE MATLAB: r2015a
 * 
 * 
 * HARDWARE:
 * Arduino nano ATMEGA 328P
 * Nivelador de voltaje  TXB-0108B
 * Pantalla Nokia-5110 PCD - Tarjeta Roja.
 * Leds RGB x 4
 * Regulador 3V3 ASM1117.
 * Convertidor FDTI - USB To RS232.
 * Resistencia de 10k (1)
 * Resistencias de 330 Ohms (3).
 * 
 * Software:
 * Arduino 1.8.5
 * Matlab r2015a
 * 
 */

//Librerias para la lectura del modulo Si-4432 y la LCD NOKIA 5110
#include <SPI.h>
#include <RF22.h>
#include "U8glib.h"

//Configuraciones generales 
#define BaudRate 115200 // Default: 115200 - Si cambias aqui debes cambiar en el guide tambien.
// Las conexiones acontinuacion estan igual en el hardware(PCB). Cambiar tambien en la pcb si desean actualizarlas.
#define ledBlue 4 
#define ledRed 3
#define fUp_btnUp A0
#define fUp_btnDown A1
#define fDown_btnUp A2
#define fDown_btnDown A3

//Declaracion de los modulos perifericos
U8GLIB_PCD8544 u8g(5, 6, 8, 7, 9); 
RF22 rf22;

//Variables empleadas en el programa
int FSuperior = 460; //Frecuencia mas alta del modulo Si-4432
int FInferior = 420; //Frecuencia mas baja del modulo Si-4432
double Paso = 0.1;//Minimo cambio (Delta de frecuencia).

double f = 0; //Frecuencia inicial empleada en el for.
  

void setup() 
{
  //Inicializacion del puerto serie.
  Serial.begin(BaudRate);
  //Declaracion de entradas y salidas.
  pinMode(ledBlue,OUTPUT);
  pinMode(ledRed,OUTPUT);
  pinMode(fUp_btnUp,INPUT_PULLUP);
  pinMode(fUp_btnDown,INPUT_PULLUP);
  pinMode(fDown_btnUp,INPUT_PULLUP);
  pinMode(fDown_btnDown,INPUT_PULLUP);
  //inicializacion de los modulos
  while (!rf22.init())
  {
    //En caso que no conecte la primera vez, parpadea (Rojo - Azul - Rojo ....)
    setLed(true,false);
    delay(200);
    setLed(false,true);
    delay(200);
  }
  //Seleccion del modo de operacion (TX --> Transmision | RX --> Recepcion)
  rf22.setModeRx();  
  //Si se ha inicializado correctamente los led encienden en azul y en la LCD se grafica el mensaje de bienvenida
  setLed(false,true);   
  mensajeLCD("Analizador de","Espectro","240~930 MHz","Medios Tx","2018-UFPS");
}
void loop()
{
  //Primero se grafica en la LCD las frecuencias en que se va a muestrear
  mensajeLCD("Rango_Medicion","F_Superior:","   "+String(FSuperior)+ " Hz","F_Inferior:","   "+String(FInferior)+ " Hz");
  //Se realiza lectura de los botones
  readBtn();
  //Se realizan las mediciones y se publica atravez del puerto serie.
  publishSerial();
}
void publishSerial()
{
  //En esta funcion se calcula los RSSi y se envian por el puerto serie en el formato (000, Fmin, Fmax, Step, Rssi(1), Rssi(2), Rssi(n))
  f = 0;
  Serial.print("000,");
  Serial.print(FInferior);
  Serial.print(",");
  Serial.print(FSuperior);
  Serial.print(",");
  Serial.print(Paso);
  f = FInferior;
  while( f < FSuperior + 1) 
  {
     rf22.setFrequency(f);
     delayMicroseconds(1125);//825~1125
     Serial.print(",");
     Serial.print(rf22.rssiRead()); 
     f = f + Paso;
  }
  //Con la palabra END se finaliza la transmision
  Serial.println(",END");
}
void readBtn(){
  //En caso de presionar un boton se realiza la accion (Subir Frecuencia o Bajar Frecuencia)
  if(!digitalRead(fUp_btnUp) && FSuperior<460)
  {
    FSuperior+=1; 
  }else if(!digitalRead(fUp_btnDown) && FSuperior>420)
  {
    FSuperior-=1; 
  }else if(!digitalRead(fDown_btnUp) && FInferior<460)
  {
    FInferior+=1;
  }else if(!digitalRead(fDown_btnDown) && FInferior>420)
  {
    FInferior-=1;
  }
}

void mensajeLCD(String linea1, String linea2, String linea3,String linea4, String linea5)
{
  //Con este metodo se publican los mensajes en el puerto serie
  u8g.firstPage();  
  do {
    u8g.setFont(u8g_font_unifont);
    //u8g.setFont(u8g_font_osb21);
    u8g.setFont(u8g_font_6x10);
       u8g.setPrintPos( 0, 7); 
       u8g.print(linea1);
       u8g.setPrintPos( 0, 17); 
       u8g.print(linea2);
       u8g.setPrintPos( 0, 25); 
       u8g.print(linea3);
       u8g.setPrintPos( 0, 35); 
       u8g.print(linea4);
       u8g.setPrintPos( 0, 45); 
       u8g.print(linea5);
   }while( u8g.nextPage() ); 
}
void setLed(bool red, bool blue)
{
  //Con esta funcion se fija el color de los leds.ç
    digitalWrite(ledBlue,!blue);
    digitalWrite(ledRed,!red);
}

