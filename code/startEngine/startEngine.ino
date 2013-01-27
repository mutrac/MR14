// startEngine.ino        moveServo.write(360); // dependent on line width
// Trevor Stanhope
// December 16th, 2012
// Start engine (ignition only).

/*
5V to switch
switch to (startPin) AND (10K ohm to Ground)
*/

/* Dependent Libraries */
/* Global Variables */
int startPin = 2; // pin for the Start Engine Button
int relayPin = 3; // pin for relay to engine start
int startVal = LOW; // val for engine ignition state

/* Functions */
// Setup
void setup() {
  pinMode(relayPin, OUTPUT);  // Initialize the digital pin as a relay controller
  pinMode(startPin, INPUT);
}

// Loop
void loop() {
    startVal = digitalRead(startPin);  // read input value
    if (startVal == HIGH) {
        digitalWrite(relayPin, HIGH);  // turn relay ON
    } else {
        digitalWrite(relayPin, LOW);  // turn relay OFF
    }
}
