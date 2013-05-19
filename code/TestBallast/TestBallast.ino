/* --- Header --- */
#include "DualVNH5019MotorShield.h"
#define BALLAST_SPEED_PIN 2
#define LIMIT_NEAR_PIN 32
#define LIMIT_FAR_PIN 34

/* --- Declarations --- */ 
DualVNH5019MotorShield motors; // two motors, M1 and M2
int LIMIT_FAR = LOW;
int LIMIT_NEAR = LOW;
int MOTOR_SPEED = 0;
int BALLAST_SPEED = 0;
int BAUD = 9600;

/* --- Setup --- */
void setup() {
  Serial.begin(BAUD);
  Serial.println("STARTING TEST");
  pinMode(BALLAST_SPEED_PIN, INPUT);
  pinMode(LIMIT_NEAR_PIN, INPUT);
  pinMode(LIMIT_FAR_PIN, INPUT);
}

/* --- Loop --- */
void loop() {
  Serial.println("---------");
  delay(1000);
  ballast();
}

/* --- Ballast --- */
void ballast() {
  
  // Read Ballast control input
  BALLAST_SPEED = analogRead(BALLAST_SPEED_PIN);
  MOTOR_SPEED = 100;
  
  // Read limit states
  LIMIT_NEAR = digitalRead(LIMIT_NEAR_PIN);
  LIMIT_FAR = digitalRead(LIMIT_FAR_PIN);
  
  // If limit switches not activated
  if (!LIMIT_NEAR || !LIMIT_FAR) {
    
    // Display potentiometer positions
    Serial.println(BALLAST_SPEED);
    
    // Set speed
    if (BALLAST_SPEED > ) {
      motors.setM2Speed(MOTOR_SPEED); // no ballast if
    } 
    else if (BALLAST_SPEED ) {
      motors.setM2Speed(-MOTOR_SPEED); // no ballast if ;
    }
    else {
      motors.setM2Speed(0); // no ballast if ;
    }
  }
  
  // If limit switches triggered
  else {
    Serial.println("AT LIMIT");
    motors.setM2Speed(0); // no ballast if at limit
  }
}
