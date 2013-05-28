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
#define STEERING_A_PIN  18
#define STEERING_B_PIN  19
#define ACTUATOR_FAULT_PIN 6
#define ACTUATOR_MEDIUM 400
#define BAUD 9600
DualVNH5019MotorShield motors; // M1 is steering actuator, M2 is ballast motor
volatile int STEERING_POSITION = 0;
volatile int ACTUATOR_POSITION = 0;
int ACTUATOR_FAULT = 0;

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
  }
}

/* --- Steering --- */
void steering() {
  
  // Get actuator fault reading.
  //ACTUATOR_FAULT = digitalRead(ACTUATOR_FAULT_PIN);
  ACTUATOR_FAULT = 0;
  Serial.println(STEERING_POSITION);
  Serial.println(ACTUATOR_POSITION);
  
  // If actuator is stable, enable actuator.
  if (!ACTUATOR_FAULT) {
    
    // Until actuator is left of steering wheel, adjust right.
    while (ACTUATOR_POSITION < STEERING_POSITION) {
      motors.setM2Speed(400);
      ACTUATOR_POSITION++;
      delay(1);
    }
    
    // Until actuator is right of steering wheel, adjust left.
    while (ACTUATOR_POSITION > STEERING_POSITION) {
      motors.setM2Speed(-400);
      ACTUATOR_POSITION--;
      delay(1);
    }
    
    motors.setM2Speed(0);
  }
  
  // Otherwise, disable motor;
  else {
    motors.setM2Speed(0);
    Serial.println("AT LIMIT");
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
