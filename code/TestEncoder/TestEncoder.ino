/*
  Test Quadrature Encoder
  Author: Trevor Stanhope
  Reference: Max Wolf
  Date: 2013-05-05
  Schematic:
    A --> ENCODER_A_PIN
    B --> ENCODER_B_PIN
    COMMON --> GROUND
*/  

/* --- Libraries --- */

/* --- Declarations --- */
#define ENCODER_A_PIN 2
#define ENCODER_B_PIN 4
#define BAUD 9600
int POSITION = 0;
int A = LOW;
int A_LAST = LOW;
int B = LOW;

/* --- Setup --- */
void setup() { 
  pinMode(ENCODER_A_PIN, INPUT);
  pinMode(ENCODER_B_PIN, INPUT);
  digitalWrite(ENCODER_A_PIN, HIGH); // turn on pullup resistor
  digitalWrite(ENCODER_B_PIN, HIGH); // turn on pullup resistor
  Serial.begin(BAUD);
} 

/* --- Loop --- */
void loop() { 
  
  // Read A
  A = digitalRead(ENCODER_A_PIN);
  
  // Compare Current state to last state
  if ((A_LAST == LOW) && (A == HIGH)) {
    if (digitalRead(ENCODER_B_PIN) == LOW) {
      POSITION--;
    }
    else {
       POSITION++;
    }
    Serial.println(POSITION);
    Serial.println("-----------");
  } 
  
  // Set A to A-Last
  A_LAST = A;
} 
