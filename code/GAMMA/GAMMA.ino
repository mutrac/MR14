/*
  MR14 BETA
  Organization: MuTrac (McGill ASABE Tractor Pull Team), McGill University
  Author: Trevor Stanhope
  Date: 2013-05-04
  Summary: Beta Project for MR14
  - STEERING
  - NO TEMPERATURE
  - KILLSWITCHES AND LOCKSWITCHES
*/

/*&^%$#@! MAIN !@#$%^&*/
/* --- Libraries --- */
#include <SoftwareSerial.h> // needed for RFID module.
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "config.h" // needed for pin setup and global values
#include "pitches.h"
#include "DualVNH5019MotorShield.h"

/* --- Declarations --- */
LiquidCrystal_I2C lcd(LCD_I2C_PORT, LCD_WIDTH, LCD_HEIGHT);  
DualVNH5019MotorShield motors;

// Switches (Locks, Kills, Triggers and Limits)
int KILL_HITCH = LOW; // Dashboard kill switch state
int KILL_BUTTON = LOW; // Seat kill switch state
int KILL_SEAT = LOW; // Load kill switch state
int LOCK_BRAKE = LOW; // CVT Guard kill switch state
int LOCK_CVT = LOW; // Engine kill switch state
int START = LOW; // Engine ignition button state

// Ballast and Steering control values
volatile int BALLAST_POSITION = 0;
volatile int STEERING_POSITION = 0;
volatile int ACTUATOR_POSITION = 0;
volatile int MOTOR_POSITION = 0;
volatile int ACTUATOR_SPEED = 0;

// Sensor readings
volatile unsigned int FuelRPM = 0;
volatile unsigned int EngineRPM = 0;
volatile unsigned int WheelRPM = 0;
int flow = 0;

// Character buffers
char TempBuffer[128];
char FuelBuffer[128];
char WheelBuffer[128];
char EngineBuffer[128];
char temperature[16];

// Valid RFID Keys
int antoine[] = {139, 151, 226, 117};
int trevor[] = {10, 227, 165, 221};

/* --- Setup --- */
// Tractor initialization actions
void setup() {
  
  // Initialize Control Panel
  Serial.begin(BAUD); // System
  Serial1.begin(BAUD); // RFID
  pinMode(IGNITION_PIN, INPUT);
  pinMode(STEERING_A_PIN, INPUT);
  pinMode(STEERING_B_PIN, INPUT);
  pinMode(BALLAST_PIN, INPUT);
  digitalWrite(STEERING_A_PIN, HIGH); // turn on pullup resistor
  digitalWrite(STEERING_B_PIN, HIGH); // turn on pullup resistor

  // Initialize Relays in OFF state (OFF = HIGH)
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  digitalWrite(RELAY3_PIN, HIGH);
  
  // Initialize Kill Switches
  pinMode(KILL_BUTTON_PIN, INPUT);
  pinMode(KILL_SEAT_PIN, INPUT);
  pinMode(KILL_HITCH_PIN, INPUT);
  
  // Initialize Lock Switches
  pinMode(LOCK_BRAKE_PIN, INPUT);
  pinMode(LOCK_CVT_PIN, INPUT);
  
  // Initialize Sensors
  pinMode(SENSOR_FUEL_PIN, INPUT);
  pinMode(SENSOR_ENGINE_PIN, INPUT);
  pinMode(SENSOR_WHEEL_PIN, INPUT);
  
  // Initialize Steering Actuator
  pinMode(ACTUATOR_A_PIN, INPUT);
  pinMode(ACTUATOR_B_PIN, INPUT);
  pinMode(ACTUATOR_SPEED_PIN, INPUT);
  
  // Initialize Ballast Motor  
  pinMode(MOTOR_A_PIN, INPUT);
  pinMode(MOTOR_B_PIN, INPUT);
  pinMode(MOTOR_SPEED_PIN, INPUT);
  
  // Attach Interrupts
  attachInterrupt(0, fuel, RISING); // fuel rate
  attachInterrupt(0, engine, RISING); // engine tachometer
  attachInterrupt(0, wheel, RISING); // wheel tachometer
  attachInterrupt(0, steering, CHANGE); // steering encoder
  attachInterrupt(0, ballast, CHANGE); // steering encoder

  // Display Boot Message
  lcd.init();
  lcd.backlight();
  lcd.print("BOOTING");
  delay(SHORT);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MR14");
  lcd.setCursor(0,1);
  lcd.print("MCGILL UNIVERSITY");
  lcd.setCursor(0,2);
  lcd.print("BIORESOURCE ENG");
  delay(LONG);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("READ OWNER MANUAL");
  lcd.setCursor(0,2);
  lcd.print("BEFORE OPERATION");
  delay(LONG);
  lcd.clear();
}

/* --- Loop --- */
void loop() {
  on();
  off();
  delay(MEDIUM);
}

/*&^%$#@! STATES !@#$%^&*/
/*
  Operation states for the MR14:
    on()
    standby()
    ignition()
    run()
    off()
*/

/* --- On --- */
// LOOP() --> ON()
// Tractor is on and scans continously for key.
void on() {
  
  // Disable interrupts
  noInterrupts();
  
  // Display ON message
  lcd.clear();
  lcd.print("ON");
  
  // Run Loop
  while (kill()) {
    
    // Blink the prompt for RFID key
    lcd.clear();
    delay(SHORT);
    lcd.print("SWIPE KEY CARD");
    
    // Read serial
    Serial1.write(0x02);
    delay(SHORT);
    if (Serial1.available()) {
      if (testKey()) {
        standby();
      }
      else {
        lcd.clear();
        lcd.print("INCORRECT KEY");
        delay(SHORT);
      }
    }
    break;
  }
}

/* --- Standby --- */
// ON() && TESTKEY() && KILL() --> STANDY()
void standby() {
  
  // Disable interrupts
  noInterrupts();
  
  // Display STANDY message
  lcd.clear();
  lcd.print("STANDBY");
  
  // Enable 'Position 2' Relays
  digitalWrite(RELAY1_PIN, LOW);
  digitalWrite(RELAY2_PIN, LOW);
  digitalWrite(RELAY3_PIN, HIGH);

  // Run Loop
  while (kill() && lock()) {
      if (start()) { // activate ignition if start button engaged
        ignition();
        run();
      }
  }
}

/* --- Ignition --- */
// STANDBY() && START() && KILL() && LOCK() --> IGNITION()
void ignition() {
  
  // Disable interrupts
  noInterrupts();
  
  // Display IGNITION message
  lcd.clear();
  lcd.print("IGNITION");
  
  // Enable 'Position 3' Relays
  while (start()) {
    digitalWrite(RELAY1_PIN, LOW);
    digitalWrite(RELAY2_PIN, LOW);
    digitalWrite(RELAY3_PIN, LOW);
  }
}

/* --- Run --- */
// STANDBY() && START() && KILL() && LOCK() --> RUN()
void run() {
  
  // Disable interrupts
  noInterrupts();
  
  // Engage 'Position 2' Relays
  digitalWrite(RELAY1_PIN, LOW);
  digitalWrite(RELAY2_PIN, LOW);
  digitalWrite(RELAY3_PIN, HIGH);
  
  // Display RUNNING message
  lcd.clear();
  lcd.print("RUNNING");
  delay(MEDIUM);
  lcd.clear();
  
  // Tractor is now running
  while (kill()) {
    
    // Reset interrupts
    noInterrupts();
    interrupts();
    delay(SHORTER); // pulses per 300ms = L/hour
    
    // Package readings as character buffers
    sprintf(FuelBuffer, "FUEL (L/H):   %d    ", FuelRPM);
    sprintf(TempBuffer, "TEMP (C):     %s  ", temperature);
    sprintf(EngineBuffer, "ENGINE (RPM): %d    ", EngineRPM);
    sprintf(WheelBuffer, "WHEEL (RPM):  %d    ", WheelRPM);
    
    // Display readings
    lcd.setCursor(0,0); 
    lcd.print(FuelBuffer);
    lcd.setCursor(0,1);
    lcd.print(WheelBuffer);
    lcd.setCursor(0,2);
    lcd.print(EngineBuffer);
    
    // Reset interrupt counters
    FuelRPM = 0;
    EngineRPM = 0;
    WheelRPM = 0;
  }
}

/* --- Off --- */
// LOOP() --> OFF()
void off() {
  
  // Disable interrupts
  noInterrupts();
  
  // Display OFF message
  lcd.clear();
  lcd.print("OFF");

  // Disable all relays
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  digitalWrite(RELAY3_PIN, HIGH);
}

/*&^%$#@! FUNCTIONS !@#$%^&*/
/*
  Secondary tasks executed by state functions:
    kill()
    lock()
    testKey()
    start()
*/

/* --- Kill Switches --- */
// Tractor will not operate if any of the killswitches are engaged
boolean kill() {
  
  // Disable interrupts
  noInterrupts();
  
  // Get kill switch states
  KILL_SEAT = digitalRead(KILL_SEAT_PIN);
  KILL_BUTTON = digitalRead(KILL_BUTTON_PIN);
  KILL_HITCH = digitalRead(KILL_HITCH_PIN);
  
  // Return boolean
  if (KILL_SEAT) {
    lcd.clear();
    lcd.print("DRIVER NOT ON SEAT");
    delay(SHORT);
    return false;
  }
  else if (KILL_HITCH) {
    lcd.clear();
    lcd.print("HITCH DETACHED");
    delay(SHORT);
    return false;
  }
  else if (KILL_BUTTON) {
    lcd.clear();
    lcd.print("KILL BUTTON PRESSED");
    delay(SHORT);
    return false;
  }
  else {
    return true;
  }
}

/* --- Lock Switches --- */
// MR14 will not allow ignition if any of the locks are engaged
boolean lock() {
  if (LOCK_BRAKE) {
    lcd.clear();
    lcd.print("BRAKE NOT ENGAGED");
    delay(SHORT);
    return false;
  }
  else if (LOCK_CVT) {
    lcd.clear();
    lcd.print("CVT GUARD OFF");
    delay(SHORT);
    return false;
  }
  else {
    return true;
  }
  
}

/* --- Start Button --- */
// Get start Button state
boolean start() {
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
  if (temp[0] == 139) {
    for (int j = 0; j < KEYLENGTH; j++) {
      if (antoine[j] == temp[j]) { // compare key to valid key
        continue;
      }
      else {
        return false;
      }
    }
    lcd.clear();
    lcd.print("HELLO ANTOINE");
    delay(MEDIUM);
  }
  else if (temp[0] == 10) {
    for (int j = 0; j < KEYLENGTH; j++) {
      if (trevor[j] == temp[j]) { // compare key to valid key
        continue;
      }
      else {
        return false;
      }
    }
    lcd.clear();
    lcd.print("HELLO TREVOR");
    delay(MEDIUM);
  }
  else {
    return false;
  }
  temp[4] = 0;
  return true;
}

/*&^%$#@! INTERRUPTS !@#$%^&*/
/*
  Interrupt functions for reading sensors instantaneously: 
    fuel()
    engine()
    wheel()
    steering()
    ballast()
*/

/* --- Fuel --- */
void fuel() { 
  FuelRPM++;
}

/* --- Engine --- */
void engine() {
  EngineRPM++;
}

/* --- Wheel --- */
void wheel() {
  WheelRPM++;
}

/* --- Steering --- */
// 1 rotation = 220 positions
// 2 rotations in either direction
void steering() {
  
  // Get encoder position
  if (digitalRead(STEERING_A_PIN) == digitalRead(STEERING_B_PIN)) {
    STEERING_POSITION++;
    if (STEERING_POSITION > 220) {
      STEERING_POSITION = 220 ; // limit right steer to 440
    }
  }
  else {
    STEERING_POSITION--;
    if (STEERING_POSITION < -220) {
    STEERING_POSITION = -220; // limit left steer to -440
    }
  }
  
  // If actuator is left of steering wheel
  while (ACTUATOR_POSITION < STEERING_POSITION) {
    ACTUATOR_SPEED = abs(STEERING_POSITION - ACTUATOR_POSITION);
    motors.setM1Speed(ACTUATOR_SPEED);
    ACTUATOR_POSITION++;
    delay(1);
  }
  motors.setM1Speed(0);
  
  // While actuator is right of steering wheel, adjust log-wise
  while (ACTUATOR_POSITION > STEERING_POSITION) {
    ACTUATOR_SPEED = abs(STEERING_POSITION - ACTUATOR_POSITION);
    motors.setM1Speed(-ACTUATOR_SPEED);
    ACTUATOR_POSITION--;
    delay(1);
  }
  motors.setM1Speed(0);
  
}

/* --- Ballast --- */
void ballast() {
  
  // Read Ballast control input
  BALLAST_POSITION = analogRead(BALLAST_PIN);
  
  // Set Ballast speed
  if (abs(BALLAST_POSITION) < 128) {
    motors.setM2Speed(0);
  }
  else {
    motors.setM2Speed(BALLAST_POSITION);
  }
}
