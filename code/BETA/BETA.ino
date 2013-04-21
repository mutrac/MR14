/*
  MR14 BETA
  Trevor Stanhope
  Beta Project for MR14
*/

/* --- Libraries --- */
#include <SoftwareSerial.h> // needed for RFID module.
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include "config.h" // needed for pin setup and global values

/* --- Declarations --- */
LiquidCrystal_I2C lcd(I2C_PORT, WIDTH, HEIGHT);  
OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);
int key[] = {139, 151, 226, 117};
int killVal = LOW;
int startVal = LOW;
volatile unsigned int iterator1 = 0;
volatile unsigned int iterator2 = 0;
volatile unsigned int iterator3 = 0;
int flow;
char temp[128];
char fuel[128];
char wrpm[128];
char erpm[128];
char buffer[16];


/* --- Setup --- */
void setup() {
  // Interfaces
  Serial.begin(BAUD); // System
  Serial1.begin(BAUD); // RFID
  sensors.begin();
  pinMode(IGNITION_PIN, INPUT); // a.k.a.start button pin
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(KILL_PIN, INPUT);
  pinMode(FUEL_PIN, INPUT);
  
  // I/O
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  attachInterrupt(0, pulses, RISING); // attach fuel rate interrupt
  attachInterrupt(0, sparks, RISING); // attach fuel rate interrupt
  attachInterrupt(0, rotations, RISING); // attach fuel rate interrupt

  // Initialize
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("MR14");
  lcd.setCursor(0,1);
  lcd.print("MCGILL UNIVERSITY");
  lcd.setCursor(0,2);
  lcd.print("BIORESOURCE");
  delay(LONGER);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("READ OWNER MANUAL");
  lcd.setCursor(0,2);
  lcd.print("BEFORE OPERATION");
  delay(LONGER);
  lcd.clear();
}

/* --- Loop --- */
void loop() {
  scan();
  off();
  delay(MEDIUM);
}
  
/* --- Scan --- */
// Scan continously for key
void scan() {
  while (killSwitch()) {
    lcd.clear();
    delay(SHORT);
    lcd.print("SWIPE KEY CARD");
    Serial1.write(0x02);
    delay(SHORT);
    if (Serial1.available()) {
      if (testKey()) {
        lcd.clear();
        lcd.print("KEY AUTHENTICATED");
        delay(MEDIUM); // time for display
        standby();
      }
      else {
        lcd.clear();
        lcd.print("INCORRECT KEY");
        delay(SHORT);
      }
    }
  }
}

/* --- Standby --- */
// STANDY mode
void standby() {
  lcd.clear();
  lcd.print("STANDBY");
  while (killSwitch()) {
    digitalWrite(RELAY1_PIN, LOW);
    digitalWrite(RELAY2_PIN, HIGH);
    if (startButton()) { // activate starter if start engaged
      ignition();
      run();
    }
  }
  off(); // turn off if kill switch is on, wait for new card
}

/* --- Ignition --- */
// IGNITION mode
void ignition() {
  lcd.clear();
  lcd.print("IGNITION");
  while (startButton()) {
    digitalWrite(RELAY1_PIN, LOW);
    digitalWrite(RELAY2_PIN, LOW);
  }
}

/* --- Run --- */
// RUN mode
void run() {
  lcd.clear();
  lcd.print("ENGINE STARTED");
  digitalWrite(RELAY1_PIN, LOW);
  digitalWrite(RELAY2_PIN, HIGH);
  delay(MEDIUM);
  lcd.clear();
  while (killSwitch()) {
    noInterrupts();
    interrupts();
    sensors.requestTemperatures();
    dtostrf(sensors.getTempCByIndex(0), 4, 2, buffer);
    delay(SHORTER); // pulses per 300ms = L/hour
    lcd.setCursor(0,0);
    sprintf(fuel, "FUEL (L/H):   %d    ", iterator1);
    sprintf(temp, "TEMP (C):     %s  ", buffer);
    sprintf(erpm, "ENGINE (RPM): %d    ", iterator2);
    sprintf(wrpm, "WHEEP (RPM):  %d    ", iterator3);
    lcd.setCursor(0,0); 
    lcd.print(fuel);
    lcd.setCursor(0,1);
    lcd.print(temp);
    lcd.setCursor(0,2);
    lcd.print(erpm);
    lcd.setCursor(20,1);
    lcd.print(wrpm);
    iterator1 = 0;
    iterator2 = 0;
    iterator3 = 0;
  }
}

/* --- Off --- */
// OFF state, tractor is 
void off() {
  lcd.clear();
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
}

/* --- Kill Switch --- */
// Get killswitch states
boolean killSwitch() {
  if (digitalRead(KILL1_PIN) && digitalRead(KILL2_PIN) && digitalRead(KILL3_PIN)) { // if killswitches is not engaged
    return true;
  }
  else {
    return false;
  }
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
boolean testKey() {
  int temp[KEYLENGTH]; // length of key is 4;
  for (int i = 0; i < KEYLENGTH; i++) {
    temp[i] = Serial1.read();
  }
  for (int j = 0; j < KEYLENGTH; j++) {
    if (key[j] == temp[j]) { // compare key to valid key
      continue;
    }
    else {
      return false;
    }
  }
  return true;
}

/* --- RPM --- */
void pulses() { 
  iterator1++;
}
void sparks() {
  iterator2++;
}
void rotations() {
  iterator3++;
}
