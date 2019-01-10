#include "Servo.h"
#include "SR04.h"
#include "IRremote.h"
#include "RobotCarLib.h"

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

SR04 ds = SR04(ECHO_PIN, TRIG_PIN);
long distanciaCentro;
long distanciaDerecha;
long distanciaIzquierda;

int pos = 90;
Servo servo;

decode_results results;
IRrecv irrecv(IR_PIN);

int velocidad = 100;

RobotCarLib motor;

void setup() {
//  Serial.begin(9600); //Si descomentamos, motor izdo atras no funciona.
  servo.attach(SERVO_PIN); //Deshabilita PWM 9 y 10.
  servo.write(pos);

//  irrecv.enableIRIn(); //Si descomentamos no funcionan los motores. PWM 3 y 11 no funcionan.

  motor.Init();
//  testMotor();
}

void loop() {
  medirDistanciaCentro();
  esquivarObstaculos();
  delay(10);
}

void esquivarObstaculos() {
  if (distanciaCentro > 50) {
    motor.MoveForward();
  } else if (distanciaCentro < 10) {
    motor.MoveBackward();
    delay(100);
  } else {
    motor.Stop();
    medirDistancias();
    if (distanciaDerecha > distanciaIzquierda) {
      motor.TurnRight();
    } else {
      motor.TurnLeft();
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

// Motor
void testMotor() {
  motor.MoveForward();
  delay(1000);
  motor.MoveBackward();
  delay(1000);
  motor.TurnLeft();
  delay(1000);
  motor.TurnRight();
  delay(1000);
  motor.Stop();
  delay(1000);
}
