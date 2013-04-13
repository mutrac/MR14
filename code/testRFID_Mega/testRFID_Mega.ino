// testRFID_Mega.ino
// Author: Trevor Stanhope
// Updated: 2013-04-12
// Tests 13.56 MHz RFID module

/* --- Headers --- */
#define TX_PIN 15
#define RX_PIN 14
#define KEYLENGTH 4
#define BAUDRATE 9600
#define INTERVAL 1000
#define READ 0x02

/* --- Declarations --- */
int KEY[] = {139, 151, 226, 117}; // the valid key

/* --- Setup --- */
void setup() {
  Serial.begin(BAUDRATE);
  Serial1.begin(BAUDRATE);
}

/* --- Loop --- */
void loop() {
  Serial1.write(READ); // Send the command to RFID, please refer to RFID manual
  delay(50);
  if (Serial1.available() > 0) {
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
    TMP[i] = Serial1.read();
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

