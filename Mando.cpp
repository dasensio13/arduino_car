#include "Mando.h"
#include "IRremote.h"
#include "Arduino.h"

//CODIGOS HEXADECIMALES MANDO A DISTANCIA
//Botón           Dirección (HEX) comando (HEX) Dato 32 bits (HEX)
//OK              0x00  0x02  0x00FF02FD
//Arriba (↑)      0x00  0x62  0x00FF629D
//Abajo (↓)       0x00  0XA8  0x00FFA857
//Izquierda (←)   0x00  0x22  0x00FF22DD
//Derecha (→)     0x00  0xC2  0x00FFC23D
//1               0x00  0x68  0x00FF6897
//2               0x00  0x98  0x00FF9867
//3               0x00  0xB0  0x00FFB04F
//4               0x00  0x30  0x00FF30CF
//5               0x00  0x18  0x00FF18E7
//6               0x00  0x7A  0x00FF7A85
//7               0x00  0x10  0x00FF10EF
//8               0x00  0x38  0x00FF38C7
//9               0x00  0x5A  0x00FF5AA5
//0               0x00  0x4A  0x00FF4AB5
//*               0x00  0x42  0x00FF42BD
//#               0x00  0x52  0x00FF52AD

const int IR_PIN = 6;
IRrecv irrecv(IR_PIN);

void MandoInit() {
  //Serial.begin(9600);
  irrecv.enableIRIn();
}

char dump(decode_results *results) {
  //Serial.print("value: ");
  //Serial.println(results->value, HEX);
  char tecla_mando = ' ';
  if (results->value == 0x00FF6897)
    tecla_mando = '1';
  else if (results->value == 0x00FF9867)
    tecla_mando = '2';
  else if (results->value == 0x00FFB04F)
    tecla_mando = '3';
  else if (results->value == 0x00FF30CF)
    tecla_mando = '4';
  else if (results->value == 0x00FF18E7)
    tecla_mando = '5';
  else if (results->value == 0x00FF7A85)
    tecla_mando = '6';
  else if (results->value == 0x00FF10EF)
    tecla_mando = '7';
  else if (results->value == 0x00FF38C7)
    tecla_mando = '8';
  else if (results->value == 0x00FF5AA5)
    tecla_mando = '9';
  else if (results->value == 0x00FF4AB5)
    tecla_mando = '0';
  else if (results->value == 0x00FF42BD)
    tecla_mando = '*';
  else if (results->value == 0x00FF52AD)
    tecla_mando = '#';
  else if (results->value == 0x00FF02FD)
    tecla_mando = 'k';
  else if (results->value == 0x00FF629D)
    tecla_mando = 'u';
  else if (results->value == 0x00FFA857)
    tecla_mando = 'd';
  else if (results->value == 0x00FF22DD)
    tecla_mando = 'l';
  else if (results->value == 0x00FFC23D)
    tecla_mando = 'r';
  return tecla_mando;
}

char TeclaPulsada() {
  decode_results results;
  char tecla = ' ';
  if (irrecv.decode(&results)) {
    tecla = dump(&results);
    //Serial.print("t: ");
    //Serial.println(tecla);
    irrecv.resume();
  }
  return tecla;
}
