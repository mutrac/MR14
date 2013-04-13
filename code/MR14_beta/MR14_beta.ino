// MR14
// Trevor Stanhope
// Jan 9, 2013
// Full Project for MR14

/******************************************************/
/* Header Libraries */
#include <SoftwareSerial.h> // needed for RFID module.
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "pitches.h" // needed for start tone.
#include "config.h" // needed for pin setup and global values

/* --- Global Objects --- */
LiquidCrystal_I2C lcd(I2C_PORT, WIDTH, HEIGHT);  
int key[] = {139, 151, 226, 117};
int killVal = LOW;
int startVal = LOW;

/* --- Setup --- */
void setup() {
  pinMode(IGNITION_PIN, INPUT);
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(KILL_PIN, INPUT);
  pinMode(FUEL_PIN, INPUT);
  Serial.begin(BAUD);
  Serial1.begin(BAUD);
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  lcd.init();
  lcd.backlight();
  lcd.print("MR14");
  lcd.setCursor(0,2);
  lcd.print("MCGILL UNIVERSITY");
  delay(LONG);
  lcd.clear();
  lcd.setCursor(0,2);
  lcd.print("READ OWNER MANUAL");
  lcd.setCursor(0,3);
  lcd.print("BEFORE OPERATION");
  delay(LONG);
  lcd.clear();
}

/* --- Loop --- */
void loop() {
  if (killSwitch()) {
    scan();
  }
  else { 
    off();
  }
  delay(MEDIUM);
}

/* --- Scan --- */
// Scan continously for key
void scan() {
  lcd.clear();
  lcd.print("SWIPE KEY CARD");
  Serial1.write(0x02);
  delay(MEDIUM);
  if (Serial1.available()) {
    if (testKey()) {
      lcd.clear();
      lcd.print("KEY AUTHENTICATED");
      delay(MEDIUM);
      standby();
    }
    else {
      lcd.clear();
      lcd.print("INCORRECT KEY");
      delay(MEDIUM);
    }
  }
  lcd.clear();
  delay(SHORT); // scan every second
}

/* --- Standby --- */
// Standby for start
void standby() {
  while (killSwitch()) {
    lcd.clear();
    lcd.print("STANDBY");
    digitalWrite(RELAY1_PIN, LOW);
    digitalWrite(RELAY2_PIN, HIGH);
    if (startButton()) { // activate starter if start engaged
      start();
      on();
    }
    delay(SHORT);
  }
  off(); // kill if kill switch is on, wait for new card
}

/* --- Start --- */
// start phase 
void start() {
  while (startButton()) {
    lcd.clear();
    lcd.print("IGNITION");
    digitalWrite(RELAY1_PIN, LOW);
    digitalWrite(RELAY2_PIN, LOW);
    delay(SHORT);
  }
}

/* --- On --- */
// Main operating mode
void on() {
  lcd.clear();
  lcd.print("RUNNING");
  digitalWrite(RELAY1_PIN, LOW);
  digitalWrite(RELAY2_PIN, HIGH);
  int i = 0;
  while (killSwitch()) {
    i++;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Fuel:");
    lcd.setCursor(0,1);
    lcd.print("Speed:");
    lcd.setCursor(0,2);
    lcd.print("RPM:");
    delay(MEDIUM); 
  }
  off();
}

/* --- Off --- */
// OFF state, tractor is 
void off() {
  lcd.clear();
  lcd.print("OFF");
  delay(LONG);
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  lcd.clear();
  delay(MEDIUM);
}

/* --- Kill Switch --- */
// Get Kill Switch state
boolean killSwitch() {
  if (digitalRead(KILL_PIN)) { // if kill switch is not engaged
    return true;
  }
  return false;
}

/* --- Start Button --- */
// Get start Button state
boolean startButton() {
  if (digitalRead(IGNITION_PIN)) { // if start button is pressed
    return true;
  }
  else {
    return false;
  }
}

/* --- Test Key --- */
// Tests if RFID key is valid
boolean testKey() {
  int temp[KEYLENGTH]; // length of key is 4;
  for (int i = 0; i < KEYLENGTH; i++) {
    temp[i] = Serial1.read();
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

/* --- Get Fuel Rate --- */
// Get Fuel Gauge Rate in L/h
float getFuelRate() {
  float flow = 0;
  Serial.println("FUEL RATE");
  Serial.println(flow);
  return flow;
}

