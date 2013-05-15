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
#define STEERING_A_PIN  2
#define STEERING_B_PIN  4
#define LIMIT_LEFT_PIN 32
#define LIMIT_RIGHT_PIN 34
#define ACTUATOR_FAULT_PIN 6
#define ACTUATOR_MEDIUM 50
#define BAUD 9600
DualVNH5019MotorShield motors; // M1 is steering actuator, M2 is ballast motor
volatile int STEERING_POSITION = 0;
volatile int ACTUATOR_POSITION = 0;
int LIMIT_LEFT = LOW;
int LIMIT_RIGHT = LOW;
int ACTUATOR_FAULT = 0;

/* --- Setup --- */
void setup() { 
  
  // Initialize I/O Pins
  pinMode(STEERING_A_PIN, INPUT); 
  pinMode(STEERING_B_PIN, INPUT);
  pinMode(LIMIT_LEFT_PIN, INPUT);
  pinMode(LIMIT_RIGHT_PIN, INPUT);
  digitalWrite(STEERING_A_PIN, HIGH); // turn on pullup resistor
  digitalWrite(STEERING_B_PIN, HIGH); // turn on pullup resistor

  // Initialize serials
  attachInterrupt(0, encoder, CHANGE); // encoder pin on interrupt 0 - pin 2
  Serial.begin (BAUD);
  Serial.println("start"); // a personal quirk
  
  // Attach interrupt.
  attachInterrupt(4, encoder, CHANGE); // Pin 18
}

/* --- Loop --- */
void loop(){
  steering();
}

/* --- Steering --- */
void steering() {
  
  // Get actuator fault reading.
  ACTUATOR_FAULT = digitalRead(ACTUATOR_FAULT_PIN);
  
  // If actuator is stable, enable actuator.
  if (!ACTUATOR_FAULT) {
    
    // Until actuator is left of steering wheel, adjust right.
    if (ACTUATOR_POSITION < STEERING_POSITION) {
      motors.setM1Speed(ACTUATOR_MEDIUM);
      ACTUATOR_POSITION++;
    }
    
    // Until actuator is right of steering wheel, adjust left.
    else if (ACTUATOR_POSITION > STEERING_POSITION) {
      motors.setM1Speed(-ACTUATOR_MEDIUM);
      ACTUATOR_POSITION--;
    }
    
    // Otherwise, disable actuator.
    else {
      motors.setM1Speed(0);
    }
  }
  
  // Otherwise, disable motor;
  else {
    motors.setM1Speed(0);
  }
}

/* --- Encoder --- */
void encoder() {
  
  // Get the current encoder position and limit if oversteered.
  if (digitalRead(STEERING_A_PIN) == digitalRead(STEERING_B_PIN)) {
    STEERING_POSITION++;
    if (STEERING_POSITION > 440) {
      STEERING_POSITION = 440; // limit right steer to 440
    }
  }
  else {
    STEERING_POSITION--;
    if (STEERING_POSITION < -440) {
    STEERING_POSITION = -440; // limit left steer to -440
    }
  }
  
}
