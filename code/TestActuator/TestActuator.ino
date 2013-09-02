/*
  Test Steering
  Author: Trevor Stanhope
  Date: 2013-05-05
  Summary: Control linear actuator with encoder and limit switches
  Schematic:
*/ 

/* --- Libraries --- */
#include "DualVNH5019MotorShield.h"

/* --- Declarations --- */
#define ACTUATOR_MEDIUM 50
#define BAUD 9600
DualVNH5019MotorShield motors; // M1 is steering actuator, M2 is ballast motor
int ACTUATOR_FAULT = 0;

/* --- Setup --- */
void setup() { 
  // Initialize serials
  Serial.begin(BAUD);
  Serial.println("START"); // a personal quirk
}

/* --- Loop --- */
void loop(){
  motors.setM1Speed(-400);
  Serial.println(motors.getM1CurrentMilliamps());
  (1000);
  motors.setM1Speed(0);
  Serial.println(motors.getM1CurrentMilliamps());
  delay(1000);
  motors.setM1Speed(400);
  Serial.println(motors.getM1CurrentMilliamps());
  delay(1000);
  motors.setM1iSpeed(0);
  delay(1000);
  Serial.println(motors.getM1CurrentMilliamps());
}
