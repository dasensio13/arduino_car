#include "SR04.h"
#include "Mando.h"
#include "RobotCarLib.h"

#define TRIG_PIN A0
#define ECHO_PIN A1

SR04 ds = SR04(ECHO_PIN, TRIG_PIN);
long distancia;

RobotCarLib motor;
bool adelante;

const int SERVO_PIN = 1;


char tecla;

void setup() {
  //Serial.begin(9600); //Si descomentamos, motor izdo atras no funciona.

  //Posicionar servo al medio
  pinMode(SERVO_PIN, OUTPUT);
  Set_servopulse(90);

  MandoInit();

  motor.Init();
  //testMotor();
}

void loop() {
  tecla = TeclaPulsada();
  medirDistancia();
  if (distancia<30 and adelante) {
    adelante = false;
    motor.Stop();
  }
  IR_Control();
  delay(10);
}

// Control
void IR_Control() {
  switch (tecla) {
    case 'u': motor.MoveForward();  //Up
      adelante = true;
      break;
    case 'd': motor.MoveBackward();   //Down
      adelante = false;
      break;
    case 'l': motor.SlowTurnLeft();  //Left
      delay(500);
      if (adelante)
        motor.MoveForward();
      else 
        motor.MoveBackward();
      break;
    case 'r': motor.SlowTurnRight(); //Right
      delay(500);
      if (adelante)
        motor.MoveForward();
      else
        motor.MoveBackward();
      break;
    case 'k': motor.Stop();  //Stop
      adelante = false;
      break;
    default:
      break;
  }
}

// Sensor ultrasonidos
long distance() {
  long d = ds.Distance();
  //Serial.print(d);
  //Serial.println("cm");
  return d;
}

void medirDistancia() {
  distancia = distance();
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

// Servo
void servopulse(int servopin, int myangle) { //defining a function of pulse
  int pulsewidth = (myangle * 11) + 500; //converting angle into pulse width value at 500-2480
  digitalWrite(SERVO_PIN, HIGH); //increasing the level of motor interface to upmost
  delayMicroseconds(pulsewidth); //delaying microsecond of pulse width value
  digitalWrite(SERVO_PIN, LOW); //decreasing the level of motor interface to the least
  delay(20 - pulsewidth / 1000);
}

void Set_servopulse(int set_val) {
  for (int i = 0; i <= 10; i++) //giving motor enough time to turn to assigning point
    servopulse(SERVO_PIN, set_val); //invokimg pulse function
}
