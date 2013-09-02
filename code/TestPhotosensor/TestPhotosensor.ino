/*
  Test Photosensor
  Trevor Stanhope
  2013-05-19
*/
#define LOCK_CVT_PIN 3
#define BAUD 9600
int VAL = 0;

void setup() {
  Serial.begin(BAUD);
  pinMode(LOCK_CVT_PIN, INPUT);
}

void loop() {
  delay(1000);
  Serial.println("---------------");
  VAL = analogRead(LOCK_CVT_PIN);
  Serial.println(VAL);
}
