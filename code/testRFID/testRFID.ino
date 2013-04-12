// testRFID.ino
// Trevor Stanhope
// Jan 2, 2013
// Tests 13.56 MHz RFID module

/* Dependent Libraries*/
#include <SoftwareSerial.h>

/* Global Objects */
// Input/Output
int txPin = 7;
int rxPin = 6 ;
int READ = 0x02;

// For the RFID Module
SoftwareSerial rfidSerial(txPin, rxPin); // so the RFID can be read
int KEY[] = {139, 151, 226, 117}; // the valid key
int KEYLENGTH = 4;
int baudrate = 9600;
int interval = 1000;

// For STANDBY Mode (Valid RFID)

/* Functions*/
// Setup
void setup() {
  Serial.begin(baudrate);
  rfidSerial.begin(baudrate);
}

// Loop Actions
void loop() {
  rfidSerial.write(READ); // Send the command to RFID, please refer to RFID manual
  if (rfidSerial.available()) {
    Serial.println("CARD DETECTED");
    if (testKey()) {
      Serial.println("SYSTEM ACTIVATED");
    }
    else {
      Serial.println("BAD KEY");
    }
  }
  else {
    Serial.println("SWIPE KEY CARD");
  }
  delay(interval); // check for card every on interval
}

// Test RFID Key Card
boolean testKey() {
  int TMP[KEYLENGTH];
  for (int i = 0; i < KEYLENGTH; i++) {
    TMP[i] = rfidSerial.read();
    Serial.println(TMP[i]);
  }
  for (int j = 0; j < KEYLENGTH; j++) {
    if (KEY[j] == TMP[j]) {
      continue;
    }
    else {
      return false;
    }
  }
  return true;
}

