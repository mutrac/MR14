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
#define SHORTER 250
#define STEERING_A_PIN  19
#define STEERING_B_PIN  20
#define BAUD 9600
#define OFF 0
#define SHORTEST 1
#define ACTUATOR_SPEED 300
#define THRESHOLD 27000
#define SENSITIVITY 15
DualVNH5019MotorShield motors; // M1 is steering actuator, M2 is ballast motor
volatile int STEERING_POSITION = 0;
volatile int ACTUATOR_POSITION = 0;

/* --- Setup --- */
void setup() { 
  // Steering
  pinMode(STEERING_A_PIN, INPUT); 
  pinMode(STEERING_B_PIN, INPUT);
  digitalWrite(STEERING_A_PIN, HIGH); // turn on pullup resistor
  digitalWrite(STEERING_B_PIN, HIGH); // turn on pullup resistor

  // Initialize serials
  Serial.begin(BAUD);
  Serial.println("START"); // a personal quirk
  
  // Attach interrupt.
  attachInterrupt(4, encoder, CHANGE); // Pin 18
}

/* --- Loop --- */
void loop(){
  while (1) {
    Serial.println("----------");
    steering();
    delay(SHORTER);
    if (!nokill()) {
      break;
    }
    // Interval 2
    steering();
    delay(SHORTER);
    if (!nokill()) {
      break;
    }
    // Interval 3
    steering();
    delay(SHORTER);
    if (!nokill()) {
      break;
    }
    // Interval 4
    steering();
    delay(SHORTER);
    if (!nokill()) {
      break;
    }
    Serial.println(ACTUATOR_POSITION);
    Serial.println(STEERING_POSITION);
  }
}

/* --- Steering --- */
void steering() {
  // Until actuator is left of steering wheel, adjust right.
  if (ACTUATOR_POSITION > STEERING_POSITION/SENSITIVITY) {
    while (ACTUATOR_POSITION > STEERING_POSITION/SENSITIVITY) {
      motors.setM1Speed(ACTUATOR_SPEED);
      delay(SHORTEST);
      Serial.println("LEFT");
      Serial.print("mA: ");
      Serial.println(motors.getM1CurrentMilliamps());
      if (motors.getM1CurrentMilliamps() > THRESHOLD) {
        Serial.println("OFF");
        motors.setM1Speed(OFF);
        ACTUATOR_POSITION = STEERING_POSITION/SENSITIVITY;  
      }
      else {
        ACTUATOR_POSITION--;
      }
    }
  }
  
  // Until actuator at position of steering wheel, adjust left.
  else if (ACTUATOR_POSITION < STEERING_POSITION/SENSITIVITY) {
    while (ACTUATOR_POSITION < STEERING_POSITION/SENSITIVITY) {
      motors.setM1Speed(-ACTUATOR_SPEED);
      delay(SHORTEST);
      Serial.println("RIGHT");
      Serial.println("mA: ");
      Serial.println(motors.getM1CurrentMilliamps());
      if (motors.getM1CurrentMilliamps() > THRESHOLD) {
        Serial.println("OFF");
        motors.setM1Speed(OFF);
        ACTUATOR_POSITION = STEERING_POSITION/SENSITIVITY;
      }
      else {
        ACTUATOR_POSITION++;
      }
    }
  }
    
  // After actuator is adjusted, disable it.
  else {
      motors.setM1Speed(OFF);
      Serial.println("AT POSITION");
      Serial.print("mA: ");
      Serial.println(motors.getM1CurrentMilliamps());
  }
}

/* --- Encoder --- */
void encoder() {
  
  // Get the current encoder position and limit if oversteered.
  if (digitalRead(STEERING_A_PIN) == digitalRead(STEERING_B_PIN)) {
    STEERING_POSITION++;
  }
  else {
    STEERING_POSITION--;
  }
}

boolean nokill() {
  return true;
}
