// killSwitch.ino
// Trevor Stanhope
// December 16th, 2012
// Master Kill Switch

/*
5V to switch
switch to (startPin) AND (10K ohm to Ground)
*/

/* Dependent Libraries */
/* Global Variables */
int killSwitchPin = 4; // pin for the Start Engine Button
int val = LOW; // val for engine ignition state

/* Functions */
// Setup
void setup() {
  pinMode(killSwitchPin, INPUT);
  Serial.begin(9600);
}

// Loop
void loop() {
  val = digitalRead(killSwitchPin);  // read input value
  Serial.println(val);
}
