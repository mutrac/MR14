/*
  TestPotentiometer.ino
  Author: Trevor Stanhope
  Date: 2013-05-12
  5V -> LINEAR POTENTIOMETER  --> POT_PIN
                              --> 10K --> GND
  
*/

/* --- Libraries --- */
/* --- Declarations --- */
#define POT_PIN 2
int VAL;

/* --- Setup --- */
void setup() {
  pinMode(POT_PIN, INPUT);
  Serial.begin(9600);
}

/* --- Loop --- */
void loop() {
  VAL = analogRead(POT_PIN);  // read input valuee
  Serial.println(VAL);
  Serial.println("-------------------");
  delay(1000);
}
