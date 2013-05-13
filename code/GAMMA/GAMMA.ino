/*
  MR14 GAMMA
  Organization: MuTrac (McGill ASABE Tractor Pull Team), McGill University
  Author: Trevor Stanhope
  Date: 2013-05-04
  Summary: Beta Project for MR14
  - STEERING
  - DYNAMIC BALLAST
  - NO TEMPERATURE
  - KILLSWITCHES AND LOCKSWITCHES
*/

/*&^%$#@! MAIN !@#$%^&*/
/* --- Libraries --- */
#include <SoftwareSerial.h> // needed for RFID module.
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "config.h" // needed for pin setup and global values
#include "DualVNH5019MotorShield.h"

/* --- Declarations --- */

// Serial Communications
LiquidCrystal_I2C lcd(LCD_I2C_PORT, LCD_WIDTH, LCD_HEIGHT); // control object for LCD
DualVNH5019MotorShield motors; // M1 is steering actuator, M2 is ballast motor

// Switches (Locks, Kills, Triggers and Limits)
int KILL_HITCH = LOW;
int KILL_BUTTON = LOW;
int KILL_SEAT = LOW;
int LOCK_LEFTBRAKE = LOW;
int LOCK_RIGHTBRAKE = LOW;
int LOCK_CVT = LOW;
int LIMIT_BALLAST_FAR = LOW;
int LIMIT_BALLAST_NEAR = LOW;
int IGNITION = LOW;

// Ballast and Steering
volatile int BALLAST_SPEED = 0;
volatile int STEERING_POSITION = 0;
volatile int ACTUATOR_POSITION = 0;
volatile int MOTOR_POSITION = 0;
volatile int ACTUATOR_SPEED = 0;

// Sensor Values
volatile unsigned int SENSOR_FUEL = 0;
volatile unsigned int SENSOR_ENGINE = 0;
volatile unsigned int SENSOR_WHEEL = 0;

// Character Buffers
char BUFFER_FUEL[128];
char BUFFER_WHEEL[128];
char BUFFER_ENGINE[128];

// Valid RFID Keys
int ANTOINE[] = {139, 151, 226, 117};
int TREVOR[] = {10, 227, 165, 221};

/* --- Setup --- */
// Tractor initialization actions
void setup() {
  
  // Initialize Control Panel
  Serial.begin(BAUD); // System
  Serial1.begin(BAUD); // RFID
  pinMode(IGNITION_PIN, INPUT);
  pinMode(BALLAST_SPEED_PIN, INPUT);
  
  // Initialize Steering
  pinMode(STEERING_A_PIN, INPUT);
  pinMode(STEERING_B_PIN, INPUT);
  digitalWrite(STEERING_A_PIN, HIGH); // turn on pullup resistor
  digitalWrite(STEERING_B_PIN, HIGH); // turn on pullup resistor

  // Initialize Relays in OFF state (OFF = HIGH)
  pinMode(RELAY_1_PIN, OUTPUT);
  pinMode(RELAY_2_PIN, OUTPUT);
  pinMode(RELAY_3_PIN, OUTPUT);
  digitalWrite(RELAY_1_PIN, HIGH);
  digitalWrite(RELAY_2_PIN, HIGH);
  digitalWrite(RELAY_3_PIN, HIGH);
  
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
  pinMode(ACTUATOR_FAULT_PIN, INPUT);
  
  // Initialize Ballast Motor  
  pinMode(MOTOR_A_PIN, INPUT);
  pinMode(MOTOR_B_PIN, INPUT);
  pinMode(MOTOR_SPEED_PIN, INPUT);
  pinMode(MOTOR_FAULT_PIN, INPUT);
  
  // Attach Interrupts
  attachInterrupt(0, fuel, RISING);
  attachInterrupt(1, fuel, RISING);
  attachInterrupt(2, engine, RISING);
  attachInterrupt(3, wheel, RISING);
  attachInterrupt(4, steering, CHANGE);
  attachInterrupt(5, steering, CHANGE);

  // Display BOOT Message
  lcd.init();
  lcd.backlight();
  lcd.print("BOOTING...");
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
  lcd.setCursor(0,0);
  lcd.print("READ OWNERS MANUAL");
  lcd.setCursor(0,1);
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
    
    // Read RFID serial
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
  digitalWrite(RELAY_1_PIN, LOW);
  digitalWrite(RELAY_2_PIN, LOW);
  digitalWrite(RELAY_3_PIN, HIGH);

  // Run Loop
  while (kill() && lock()) {
      if (digitalRead(IGNITION_PIN)) { // activate ignition if start button engaged
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
  while (digitalRead(IGNITION_PIN)) {
    digitalWrite(RELAY_1_PIN, LOW);
    digitalWrite(RELAY_2_PIN, LOW);
    digitalWrite(RELAY_3_PIN, LOW);
  }
}

/* --- Run --- */
// STANDBY() && START() && KILL() && LOCK() --> RUN()
void run() {
  
  // Engine Position 2
  digitalWrite(RELAY_1_PIN, LOW);
  digitalWrite(RELAY_2_PIN, LOW);
  digitalWrite(RELAY_3_PIN, HIGH);
  
  // Display RUNNING message
  lcd.clear();
  lcd.print("RUNNING");
  delay(MEDIUM);
  lcd.clear();
  
  // Tractor is now running
  while (kill()) {
    
    // Disable interrupts
    noInterrupts();
    
    // Set ballast speed
    ballast();
    
    // Reset interrupts
    interrupts();
    delay(SHORTER); // pulses per 300ms = L/hour
    
    // Package readings as character buffers
    sprintf(BUFFER_FUEL, "FUEL (L/H):   %d    ", SENSOR_FUEL);
    sprintf(BUFFER_ENGINE, "ENGINE (RPM): %d    ", SENSOR_ENGINE);
    sprintf(BUFFER_WHEEL, "WHEEL (RPM):  %d    ", SENSOR_WHEEL);
    
    // Display readings
    lcd.setCursor(0,0); 
    lcd.print(BUFFER_FUEL);
    lcd.setCursor(0,1);
    lcd.print(BUFFER_ENGINE);
    lcd.setCursor(0,2);
    lcd.print(BUFFER_WHEEL);
    
    // Reset interrupt counters
    SENSOR_FUEL = 0;
    SENSOR_ENGINE = 0;
    SENSOR_WHEEL = 0;
  }
}

/* --- Off --- */
// LOOP() --> OFF()
void off() {
  
  // Disable all interrupts
  noInterrupts();
  
  // Display OFF message
  lcd.clear();
  lcd.print("OFF");

  // Engine Position 1
  digitalWrite(RELAY_1_PIN, HIGH);
  digitalWrite(RELAY_2_PIN, HIGH);
  digitalWrite(RELAY_3_PIN, HIGH);
}

/*&^%$#@! FUNCTIONS !@#$%^&*/
/*
  Secondary tasks executed by state functions:
    kill()
    lock()
    testKey()
    start()
    fuel()
    engine()
    wheel()
    steering()
    ballast()
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
  
  // If driver is off of seat
  if (KILL_SEAT) {
    lcd.clear();
    lcd.print("DRIVER NOT ON SEAT");
    delay(SHORT);
    return false;
  }
  
  // If hitch is removed
  else if (KILL_HITCH) {
    lcd.clear();
    lcd.print("HITCH DETACHED");
    delay(SHORT);
    return false;
  }
  
  // If Kill button is pressed
  else if (KILL_BUTTON) {
    lcd.clear();
    lcd.print("KILL BUTTON PRESSED");
    delay(SHORT);
    return false;
  }
  
  // If all kill switches not engaged
  else {
    return true;
  }
}

/* --- Lock Switches --- */
// MR14 will not allow ignition if any of the locks are engaged
boolean lock() {
  
  // If brakes are not pressed
  if (LOCK_LEFTBRAKE || LOCK_RIGHTBRAKE) {
    lcd.clear();
    lcd.print("BRAKES NOT ENGAGED");
    delay(SHORT);
    return false;
  }
  
  // If CVT guard is removed
  else if (LOCK_CVT) {
    lcd.clear();
    lcd.print("CVT GUARD OFF");
    delay(SHORT);
    return false;
  }
  
  // If all locks are proper
  else {
    return true;
  }
  
}

/* --- Test Key --- */
// Tests if RFID key is valid
boolean testKey() {
  
  // Get RFID key
  int temp[KEYLENGTH]; // length of key is 4;
  for (int i = 0; i < KEYLENGTH; i++) {
    temp[i] = Serial1.read();
  }
  
  // Antoine's key
  if (temp[0] == 139) {
    for (int j = 0; j < KEYLENGTH; j++) {
      if (ANTOINE[j] == temp[j]) { // compare key to valid key
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
  
  // Trevor's key
  else if (temp[0] == 10) {
    for (int j = 0; j < KEYLENGTH; j++) {
      if (TREVOR[j] == temp[j]) { // compare key to valid key
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
  
  // Bad key
  else {
    return false;
  }
  
  // Reset key
  temp[4] = 0;
  
  // If key passed return true
  return true;
}

/* --- Fuel --- */
void fuel() { 
  SENSOR_FUEL++;
}

/* --- Engine --- */
void engine() {
  SENSOR_ENGINE++;
}

/* --- Wheel --- */
void wheel() {
  SENSOR_WHEEL++;
}

/* --- Steering --- */
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
  BALLAST_SPEED = analogRead(BALLAST_SPEED_PIN);
  
  // Read limit states
  LIMIT_BALLAST_NEAR = digitalRead(LIMIT_BALLAST_NEAR_PIN);
  LIMIT_BALLAST_FAR = digitalRead(LIMIT_BALLAST_FAR_PIN);
  
  // If limit switches not activated
  if (!LIMIT_BALLAST_NEAR || !LIMIT_BALLAST_FAR) {
    
    // SPEED -2
    if (BALLAST_SPEED < 575) {
      motors.setM2Speed(-100);
    }
    // SPEED -1
    else if ((BALLAST_SPEED > 575) && (BALLAST_SPEED < 650)) {
      motors.setM2Speed(-50);
    }
    // SPEED 0
    else if ((BALLAST_SPEED > 650) && (BALLAST_SPEED < 750)) {
      motors.setM2Speed(0);
    }
    // SPEED 1
    else if ((BALLAST_SPEED > 750) && (BALLAST_SPEED < 825)) {
      motors.setM2Speed(50);
    }
    // SPEED 2
    else if (BALLST_SPEED > 825) {
      motors.setM2Speed(100); // no ballast if 
    }
  }
  
  // If limit switches triggered
  else {
    motors.setM2Speed(0); // no ballast if at limit
  }
}
