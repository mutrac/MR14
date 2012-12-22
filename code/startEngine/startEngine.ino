// startEngine.ino
// Trevor Stanhope
// December 16th, 2012
// Start engine (ignition only).

/* Notes */
/* Headers */
/* Global */
int startPin = 2; // pin for the Start Engine Button
int relayPin = 3; // pin for relay to engine start
int startVal = LOW; // val for engine state

/* Setup */
void setup() {
  pinMode(relayPin, OUTPUT);  // Initialize the digital pin as a relay controller.
  pinMode(startPin, INPUT);
}

/* Main */
void loop() {
    startVal = digitalRead(startPin);  // read input value
    if (startVal == HIGH) {
        digitalWrite(relayPin, startVal);  // turn relay ON
    } else {
        digitalWrite(relayPin, startVal);  // turn relay OFF
    }
}
