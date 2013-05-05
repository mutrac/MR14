/*
  TestKillSwitches.ino
  Author: Trevor Stanhope
  Date: 2013-05-04
  
  Through-Button: 5V -> switch -> (KILL_PIN) && (10 KOhm -> GND)
  A-or-B Switch: 
    
*/

/* --- Libraries --- */
/* --- Declarations --- */
#define KILL1_PIN 22
#define KILL2_PIN 23
#define KILL3_PIN 24
#define KILL4_PIN 25
#define KILL5_PIN 26
int VAL1;
int VAL2;
int VAL3;
int VAL4;
int VAL5;

/* --- Setup --- */
void setup() {
  pinMode(KILL1_PIN, INPUT);
  pinMode(KILL2_PIN, INPUT);
  pinMode(KILL3_PIN, INPUT);
  pinMode(KILL4_PIN, INPUT);
  pinMode(KILL5_PIN, INPUT);
  Serial.begin(9600);
}

/* --- Loop --- */
void loop() {
  VAL1 = digitalRead(KILL1_PIN);  // read input value
  VAL2 = digitalRead(KILL2_PIN);  // read input value
  VAL3 = digitalRead(KILL3_PIN);  // read input value
  VAL4 = digitalRead(KILL4_PIN);  // read input value
  VAL5 = digitalRead(KILL5_PIN);  // read input value
  Serial.println(VAL1);
  Serial.println(VAL2);
  Serial.println(VAL3);
  Serial.println(VAL4);
  Serial.println(VAL5);
  Serial.println("-------------------");
  delay(1000);
}
