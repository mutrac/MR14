/*
  TestKillSwitches.ino
  Author: Trevor Stanhope
  Date: 2013-05-04
  
  Through-Button: 5V -> switch -> (KILL_PIN) && (10 KOhm -> GND)
  A-or-B Switch: 
    
*/

/* --- Libraries --- */
/* --- Declarations --- */
#define KILL_PIN 22
int VAL;

/* --- Setup --- */
void setup() {
  pinMode(KILL_PIN, INPUT);
  digitalWrite(KILL_PIN, LOW);
  Serial.begin(9600);
}

/* --- Loop --- */
void loop() {
  VAL = digitalRead(KILL_PIN);  // read input valuee
  Serial.println(VAL);
  Serial.println("-------------------");
  delay(1000);
}
