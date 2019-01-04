#include "Servo.h"
#include "SR04.h"
#include "IRremote.h"

#define TRIG_PIN A0
#define ECHO_PIN A1

#define SERVO_PIN 5

#define IR_PIN 12
//CODIGOS HEXADECIMALES DEL MANDO A DISTANCIA
//Botón           HEX
//OK              FF02FD
//Arriba (↑)      FF629D
//Abajo (↓)       FFA857
//Izquierda (←)  FF22DD
//Derecha (→)    FFC23D
//1               FF6897
//2               FF9867
//3               FFB04F
//4               FF30CF
//5               FF18E7
//6               FF7A85
//7               FF10EF
//8               FF38C7
//9               FF5AA5
//0               FF4AB5
//*               FF42BD
//#               FF52AD

#define ENA_PIN 11
#define IN1_PIN 8
#define IN2_PIN 7
#define ENB_PIN 3
#define IN3_PIN 1
#define IN4_PIN 2

SR04 ds = SR04(ECHO_PIN, TRIG_PIN);
long distanciaCentro;
long distanciaDerecha;
long distanciaIzquierda;

int pos = 90;
Servo servo;

decode_results results;
IRrecv irrecv(IR_PIN);

int velocidad = 100;

void setup() {
//  Serial.begin(9600); //Si descomentamos, motor izdo atras no funciona.
  servo.attach(SERVO_PIN);
  servo.write(pos);

//  irrecv.enableIRIn(); //Si descomentamos no funcionan los motores.

  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(ENB_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);

//  testMotorDerecho();
//  testMotorIzquierdo();

}

void loop() {
  medirDistanciaCentro();
  esquivarObstaculos();
  delay(10);
}

void esquivarObstaculos() {
  if (distanciaCentro > 50) {
    adelante();
  } else if (distanciaCentro < 10) {
    atras();
    delay(100);
  } else {
    parar();
    medirDistancias();
    if (distanciaDerecha > distanciaIzquierda) {
      rotarDerecha();
    } else {
      rotarIzquierda();
    }
    delay(200);
  }
}

// Sensor ultrasonidos y servo
long distancia() {
  long distancia = ds.Distance();
  Serial.print(distancia);
  Serial.println("cm");
  return distancia;
}

void medirDistanciaCentro() {
  servoAlCentro();
  distanciaCentro = distancia();
}

void medirDistancias() {
  posicionServo(0);
  for (pos = 0; pos <= 180; pos += 1) {
    servo.write(pos);
    delay(15);
    if (pos == 0) {
      distanciaDerecha = distancia();
    } else if (pos == 90) {
      distanciaCentro = distancia();
    } else if (pos == 180) {
      distanciaIzquierda = distancia();
    }
  }
  servoAlCentro();
}

void servoAlCentro() {
  posicionServo(90);
}

void posicionServo(int posicion) {
  while (pos != posicion) {
    pos = pos > posicion ? pos - 1 : pos + 1;
    servo.write(pos);
    delay(5);
  }
}

// Sensor infrarrojos
void tecla() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume();
  }
}

// Motores
void parar() {
  motorDerechoParar();
  motorIzquierdoParar();
}

void adelante() {
  motorDerechoAdelante(velocidad);
  motorIzquierdoAdelante(velocidad);
}

void atras() {
  motorDerechoAtras(velocidad);
  motorIzquierdoAtras(velocidad);
}

void girarDerecha() {
  motorDerechoAdelante(velocidad);
  motorIzquierdoParar();
}

void girarIzquierda() {
  motorIzquierdoAdelante(velocidad);
  motorDerechoParar();
}

void rotarDerecha() {
  motorDerechoAdelante(velocidad);
  motorIzquierdoAtras(velocidad);
}

void rotarIzquierda() {
  motorIzquierdoAdelante(velocidad);
  motorDerechoAtras(velocidad);
}

void testMotorDerecho() {
  motorDerechoAdelante(velocidad);
  delay(1000);
  motorDerechoAtras(velocidad);
  delay(1000);
  motorDerechoParar();
}

void testMotorIzquierdo() {
  motorIzquierdoAdelante(velocidad);
  delay(1000);
  motorIzquierdoAtras(velocidad);
  delay(1000);
  motorIzquierdoParar();
}

void motorDerechoAdelante(int s) {
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
  analogWrite(ENB_PIN, s);
}

void motorDerechoAtras(int s) {
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);
  analogWrite(ENB_PIN, s);
}

void motorDerechoParar() {
  analogWrite(ENB_PIN, 0);
}

void motorIzquierdoAdelante(int s) {
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  analogWrite(ENA_PIN, s);
}

void motorIzquierdoAtras(int s) {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);
  analogWrite(ENA_PIN, s);
}

void motorIzquierdoParar() {
  analogWrite(ENA_PIN, 0);
}
