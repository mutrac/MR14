// mr14.ino
// Trevor Stanhope
// Jan 9, 2013
// Full Project for MR14

/******************************************************/
/* Header braries */
#include <SoftwareSerial.h> // needed for RFID module.
#include "pitches.h" // needed for start tone.
#include "config.h" // needed for pin setup and global values

/******************************************************/
/* Global Objects */
SoftwareSerial rfidSerial(rfidTXPin, rfidRXPin); // so the RFID can be read
int key[] = {139, 151, 226, 117};

/******************************************************/
/* Primary Functions */
void setup() {
    // Set Pins
    pinMode(ignitionButtonPin, INPUT);
    pinMode(standbyRelayPin, OUTPUT);
    pinMode(ignitionRelayPin, OUTPUT);
    pinMode(killSwitchPin, INPUT);
    pinMode(fuelGaugePin, INPUT);
    // Initialize Serials
    Serial.begin(BAUD);
    rfidSerial.begin(BAUD);
    // Initialize Relays
    digitalWrite(standbyRelayPin, HIGH);
    digitalWrite(ignitionRelayPin, HIGH);
}

void loop() {
  scan();
}

/******************************************************/
/* State Functions */
// sting state
void off() {
  Serial.println("OFF");
  digitalWrite(standbyRelayPin, HIGH);
  digitalWrite(ignitionRelayPin, HIGH);
}

// Scan continously for key
void scan() {
  Serial.println("SCANNING");
  rfidSerial.write(READ);
  if (rfidSerial.available() && testKey()) {
    standby();
  }
  else {
    off();
  }
  delay(INTERVAL); // scan every second
}

// Standby for ignition
void standby() {
  while (killSwitch() == false) {
    Serial.println("STANDBY");
    digitalWrite(standbyRelayPin, LOW);
    digitalWrite(ignitionRelayPin, HIGH);
    if (ignitionButton() == true) { // activate starter if ignition engaged
      ignition();
      on();
    }
  }
  off(); // kill if kill switch is on, wait for new card
}

// Ignition phase 
void ignition() {
  Serial.println("IGNITION");
  digitalWrite(standbyRelayPin, LOW);
  digitalWrite(ignitionRelayPin, LOW);
}

// Main operating mode
void on() {
  Serial.println("ON");
  digitalWrite(standbyRelayPin, LOW);
  digitalWrite(ignitionRelayPin, HIGH);
  while (killSwitch() == false) {
    getFuelRate();
  }
  off();
}

/******************************************************/
/* Auxiliary Functions (Action Functions) */
// Get Kill Switch state
boolean killSwitch() {
  if (1 == 1) { // if kill switch is not engaged
    return false;
  }
  return true;
}

// Get Ignition Button state
boolean ignitionButton() {
  if (1 == 1) { // if ignition button is pressed
    return true;
  }
  else {
    return false;
  }
}

// Tests if RFID key is valid
boolean testKey() {
  rfidSerial.write(READ); // Send the command to RFID, please refer to RFID manual
  int temp[KEYLENGTH]; // length of key is 4;
  for (int i = 0; i < KEYLENGTH; i++) {
    temp[i] = rfidSerial.read();
  }
  for (int j = 0; j < KEYLENGTH; j++) {
    if (key[j] == temp[j]) {
      continue;
    }
    else {
      return false;
    }
  }
  return true;
}

// Get Fuel Gauge Rate in L/h
float getFuelRate() {
    float flow = 0;
    Serial.println("FUEL RATE");
    Serial.println(flow);
    return flow;
}
