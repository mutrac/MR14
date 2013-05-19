/*
  TestKillSwitches.ino
  Author: Trevor Stanhope
  Date: 2013-05-04
  
  Through-Button: 5V -> switch -> (KILL_PIN) && (10 KOhm -> GND)
  A-or-B Switch: 
    
*/

/* --- Libraries --- */
/* --- Declarations --- */
#define KILL_POWER 22
#define KILL_PIN 23
int VAL;

/* --- Setup --- */
void setup() {
  pinMode(KILL_PIN, INPUT);
  pinMode(KILL_POWER, OUTPUT);
  digitalWrite(KILL_POWER, LOW);
  digitalWrite(KILL_PIN, HIGH);
  Serial.begin(9600);
}

/* --- Loop --- */
void loop() {
  VAL = digitalRead(KILL_PIN);  // read input valuee
  Serial.println(VAL);
  Serial.println("-------------------");
  delay(100);
}
