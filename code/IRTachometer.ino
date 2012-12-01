// IRTachometer.ino
// Trevor Stanhope
// December 1st, 2012
// An infra-red tachometer.

/* Global Variables */
int receiverPin = 13; // red
int emitterPin = 12; // yellow
unsigned long duration; // time shutter is open
int duration; 

/* Setup */
void setup() {
  pinMode(receiverPin, INPUT);
  Serial.begin(9600);
}

/* Main */
void loop() {
  digitalWrite(emitterPin, HIGH);
  duration = pulseIn(receiverPin, LOW);
  if(duration != 0 ) {
    Serial.println(duration);
  }
}
