/*
  Test Steering
  Author: Trevor Stanhope
  Date: 2013-05-05
  Summary: Control linear actuator with encoder and limit switches
  Schematic:
*/ 

/* --- Libraries --- */
/* --- Declarations --- */
#define ENCODER_A_PIN  2
#define ENCODER_B_PIN  4
#define LIMIT_LEFT_PIN 52
#define LIMIT_LEFT_PIN 53
#define BAUD 9600
volatile int POSITION = 0;
int LEFT = LOW;
int RIGHT = LOW;

/* --- Setup --- */
void setup() { 
  
  // Initialize I/O Pins
  pinMode(ENCODER_A_PIN, INPUT); 
  pinMode(ENCODER_B_PIN, INPUT);
  pinMode(LIMIT_LEFT_PIN, INPUT);
  pinMode(LIMIT_RIGHT_PIN, INPUT);
  digitalWrite(ENCODER_A_PIN, HIGH); // turn on pullup resistor
  digitalWrite(ENCODER_B_PIN, HIGH); // turn on pullup resistor

  // Initialize serials
  attachInterrupt(0, doEncoder, CHANGE); // encoder pin on interrupt 0 - pin 2
  Serial.begin (BAUD);
  Serial.println("start"); // a personal quirk
} 

/* --- Loop --- */
void loop(){
  // do some stuff here - the joy of interrupts is that they take care of themselves
}

/* --- doEncoder --- */
void doEncoder() {
/*
  If pinA and pinB are both high or both low, it is spinning
  forward. If they're different, it's going backward.
  For more information on speeding up this process, see
  [Reference/PortManipulation], specifically the PIND register.
*/
  
  if (digitalRead(ENCODER_A_PIN) == digitalRead(ENCODER_B_PIN)) {
    POSITION++;
  }
  else {
    POSITION--;
  }

  Serial.println (POSITION, DEC);
}
