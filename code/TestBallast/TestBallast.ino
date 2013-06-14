/* --- Header --- */
#include "DualVNH5019MotorShield.h"
#include "config.h"

/* --- Declarations --- */ 
DualVNH5019MotorShield motors; // two motors, M1 and M2
int LIMIT_FAR = LOW;
int LIMIT_NEAR = LOW;
int MOTOR_SPEED = 0;
int BALLAST_SPEED = 0;

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
int ballast() {
  
  // Get Ballast control input and well as far/near limit switches.
  BALLAST_SPEED = analogRead(BALLAST_SPEED_PIN);
  LIMIT_NEAR = digitalRead(LIMIT_NEAR_PIN);
  LIMIT_FAR = digitalRead(LIMIT_FAR_PIN);
  
  Serial.print("BALLAST: "); Serial.println(BALLAST_SPEED);
  Serial.print("NEAR: "); Serial.println(LIMIT_NEAR);
  Serial.print("FAR: "); Serial.println(LIMIT_FAR);
  if (BALLAST_SPEED > 0) {
    if (LIMIT_FAR || (motors.getM2CurrentMilliamps() > 25000)) {
      motors.setM2Speed(OFF);
      Serial.println("OFF");
      Serial.println(motors.getM2CurrentMilliamps());
    }
    else {
      motors.setM2Speed(-BALLAST_SPEED);
      Serial.println("ON");
      Serial.println(motors.getM2CurrentMilliamps());
    }
  }
  else {
    if (LIMIT_NEAR || (motors.getM2CurrentMilliamps() > 20000)) {
      motors.setM2Speed(OFF);
      Serial.println("OFF");
      Serial.println(motors.getM2CurrentMilliamps());
    }
    else {
      motors.setM2Speed(REVERSE);
      Serial.println("REVERSE");
      Serial.println(motors.getM2CurrentMilliamps());
    }
  }
  
  // Set motor and return values.
  return BALLAST_SPEED;
}
