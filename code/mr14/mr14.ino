// mr14.ino
// Trevor Stanhope
// Jan 9, 2013
// Full Project for MR14.

/* Dependent Libraries*/
#include <SoftwareSerial.h> // needed for RFID module.
#include "pitches.h" // needed for start tone.
#include "config.h" // needed for pin setup

/* Global Objects */
// For RFID Module
SoftwareSerial rfidSerial(txPin, rxPin); // so the RFID can be read
int KEY[] = {139, 151, 226, 117}; // the valid key
int KEYLENGTH = 4;
int baudrate = 9600;
int interval = 1000;

// For STANDBY Mode
int ignitionVal = LOW; // Tractor starts off of course

// For LCD

// For ON Mode

/* Functions*/
// Setup
void setup() {
  Serial.begin(baudrate);
  rfidSerial.begin(baudrate);
  
  // Initialize Pins
  pinMode(startPin, INPUT);
  pinMode(relayStandby, OUTPUT);
  pinMode(relayIgnition, OUTPUT);
}

// Main Loop
void loop() {
  rfidSerial.write(READ); // Send the command to RFID, please refer to RFID manual
  if (rfidSerial.available()) {
    if (testKey()) {
      Serial.println("SYSTEM ACTIVATED");
      startTone();
      digitalWrite(relayStandby, HIGH); // START STANDBY MODE
      waitStart();
      on();
    }
    else {
      Serial.println("BAD KEY");
      digitalWrite(relayStandby, LOW);
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

// Wait for Ignition
waitStart() {
  while (startIgnition() == LOW) {
    digitalWrite(relayIgnition, LOW); // kill ignition on button release
    waitTone();
  } 
  else {
    digitalWrite(relayIgnition, HIGH);  // turn relay ON
    while (500 > engineTachometer()) {
      continue;
    }
  }
}

// Calls to Ignition Button
int startIgnition() {
  ignitionVal = digitalRead(startPin);  // read input value
}

// Start Tone
startTone() {
  // notes in the start tone
  int melody[] = {NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};
  int melodyDurations[] = {4,8,8,4,4,4,4,4};  // note durations: 4 = quarter note, 8 = eighth note, etc...
  // Plays Start Melody.
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/melodyDurations[thisNote];
    tone(speakerPin, melody[thisNote],noteDuration);
    // to distinguish the notes, set a minimum time between them.
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(speakerPin);
  }
  delay(1000)
}

// Wait Tone
waitTone() {
  // notes in the repeat tone
  int repeat[] = {NOTE_G5, NOTE_G5};
  int repeatDurations[] = {4, 4};
  // Plays Start Melody.
  for (int thisNote = 0; thisNote < 2; thisNote++) {
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/repeatDurations[thisNote];
    tone(speakerPin, repeat[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(speakerPin);
  }
  delay(1000);
}

// Reverse Tone
reverseTone() {
  int reverse[] = {NOTE_G6};
  int reverseDurations[] = {4};
  // Plays Start Melody.
  for (int thisNote = 0; thisNote < 2; thisNote++) {
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/reverseDurations[thisNote];
    tone(speakerPin, reverse[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(speakerPin);
  }
  delay(1000);
}

bool isReversing() {
  if (digitalRead(reversePin) == HIGH); {
    return true;
  }
  else {
    return false;
  }
}

on() {
  dataLogger();
  while (isReversing) {
    reverseTone();
  }
}
