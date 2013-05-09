// MR14
// Trevor Stanhope
// Jan 9, 2013
// Full Project for MR14

/******************************************************/
/* Header Libraries */
#include <SoftwareSerial.h> // needed for RFID module.
#include <LiquidCrystal.h> // needed for LCD screen
#include "pitches.h" // needed for start tone.
#include "config.h" // needed for pin setup and global values

/******************************************************/
/* Global Objects */
SoftwareSerial rfidSerial(rfidTXPin, rfidRXPin); // so the RFID can be read
LiquidCrystal lcd(lcdRSPin, lcdEPin, lcd11Pin, lcd12Pin, lcd13Pin, lcd14Pin);
int key[] = {139, 151, 226, 117};
int killVal = LOW;
int startVal = LOW;

/******************************************************/
/* Primary Functions */
void setup() {
    // Set Pins
    pinMode(startButtonPin, INPUT);
    pinMode(standbyRelayPin, OUTPUT);
    pinMode(startRelayPin, OUTPUT);
    pinMode(killSwitchPin, INPUT);
    pinMode(fuelGaugePin, INPUT);
    // Initialize Serials
    Serial.begin(BAUD);
    rfidSerial.begin(BAUD);
    // Initialize Relays
    digitalWrite(standbyRelayPin, HIGH);
    digitalWrite(startRelayPin, HIGH);
    // Start LCD
    lcd.begin(WIDTH, HEIGHT);
}

void loop() {
  while (killSwitch()) {
    scan();
  }
  off();
  delay(INTERVAL);
}

/******************************************************/
/* Loop-State Functions */
// Scan continously for key
void scan() {
  lcd.clear();
  lcd.print("SCAN            ");
  lcd.print("                ");
  delay(INTERVAL);
  rfidSerial.write(0x02);
  delay(INTERVAL);
  if (rfidSerial.available()) {
    if (testKey()) {
      lcd.clear();
      lcd.print("WELCOME         ");
      lcd.print("                ");
      delay(INTERVAL);
      standby();
    }
    else {
      lcd.clear();
      lcd.print("BAD CARD        ");
      lcd.print("                ");
      delay(INTERVAL);
    }
  }
  else {
    lcd.clear();
    lcd.print("NO CARD         ");
    lcd.print("                ");
    delay(INTERVAL);
    off();
    delay(INTERVAL);
  }
  delay(INTERVAL); // scan every second
}

// Standby for start
void standby() {
  while (killSwitch()) {
    lcd.clear();
    lcd.print("STANDBY         ");
    lcd.print("                ");
    digitalWrite(standbyRelayPin, LOW);
    digitalWrite(startRelayPin, HIGH);
    if (startButton()) { // activate starter if start engaged
      start();
      on();
    }
    delay(INTERVAL);
  }
  off(); // kill if kill switch is on, wait for new card
}

// start phase 
void start() {
  while (startButton()) {
    lcd.clear();
    lcd.print("START           ");
    lcd.print("                ");
    digitalWrite(standbyRelayPin, LOW);
    digitalWrite(startRelayPin, LOW);
  }
}

// Main operating mode
void on() {
  lcd.clear();
  lcd.print("ON              ");
  lcd.print("                ");
  digitalWrite(standbyRelayPin, LOW);
  digitalWrite(startRelayPin, HIGH);
  int i = 0;
  // DAQ Loopstate
  while (killSwitch()) {
    i++;
    lcd.clear();
    lcd.print(i);
    delay(INTERVAL); 
  }
  off();
}

// OFF state, tractor is 
void off() {
  lcd.clear();
  lcd.print("OFF             ");
  lcd.print("                ");
  digitalWrite(standbyRelayPin, HIGH);
  digitalWrite(startRelayPin, HIGH);
}

/******************************************************/
/* Auxiliary Functions (Action Functions) */
// Get Kill Switch state
boolean killSwitch() {
  if (digitalRead(killSwitchPin)) { // if kill switch is not engaged
    return true;
  }
  return false;
}

// Get start Button state
boolean startButton() {
  if (digitalRead(startButtonPin)) { // if start button is pressed
    return true;
  }
  else {
    return false;
  }
}

// Tests if RFID key is valid
boolean testKey() {
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
