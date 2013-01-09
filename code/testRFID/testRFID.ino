// testRFID.ino
// Trevor Stanhope
// Jan 2, 2013
// Tests 13.56 MHz RFID module

/* Dependent Libraries*/
#include <SoftwareSerial.h> // needed for RFID module

/* Global Objects */
int txPin = 8;
int rxPin = 9;
SoftwareSerial rfidSerial(txPin, rxPin); // for RFID serial communication
int READ = 0x02; // HEX command to set RFID Module to READ-MODE
int KEY[] = {139, 151, 226, 117}; // the valid key
int KEYLENGTH = 4;
int baudrate = 9600;
int interval = 1000;

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
bool testKey() {
    int TMP[KEYLENGTH];
    for (int i = 0; i < KEYLENGTH; i++) {
      TMP[i] = mySerial.read();
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
