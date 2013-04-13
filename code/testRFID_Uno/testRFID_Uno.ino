// testRFID_Uno.ino
// Author: Trevor Stanhope
// Updated: 2013-04-12
// Tests 13.56 MHz RFID module

/* --- Headers --- */
#include <SoftwareSerial.h>
#define TX_PIN 15
#define RX_PIN 14
#define KEYLENGTH 4
#define BAUDRATE 9600
#define INTERVAL 1000
#define READ 0x02

/* --- Declarations --- */
SoftwareSerial rfid(RX_PIN, TX_PIN); // so the RFID can be read
int KEY[] = {139, 151, 226, 117}; // the valid key

/* --- Setup --- */
void setup() {
  Serial.begin(BAUDRATE);
  rfid.begin(BAUDRATE);
  rfid.write(READ);
}

/* --- Loop --- */
void loop() {
  if (rfid.available() > 0) {
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
  delay(INTERVAL); // check for card every on interval
}

// Test RFID Key Card
boolean testKey() {
  int TMP[KEYLENGTH];
  for (int i = 0; i < KEYLENGTH; i++) {
    TMP[i] = rfid.read();
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


