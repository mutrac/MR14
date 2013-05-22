/*
  MR14 GAMMA
  Organization: MuTrac (McGill ASABE Tractor Pull Team), McGill University
  Author: Trevor Stanhope
  Date: 2013-05-04
  Summary: Beta Project for MR14
  - STEERING
  - NO LCD
  - DYNAMIC BALLAST
  - NO TEMPERATURE
  - KILLSWITCHES AND LOCKSWITCHES
  - ENGINE
  - WHEEL
  - FUEL
  
  Contents:
  - Boot
    - Libraries
    - Declarations
    - Setup
  - Runtime
    - Loop
    - States
    - Functions
*/

/////////////// BOOT ///////////////
/* --- Libraries --- */
#include <SoftwareSerial.h> // needed for RFID module.
#include <Wire.h> // needed for LCD
#include <LiquidCrystal_I2C.h> // needed for LCD
#include "config.h" // needed for pin setup and global values
#include <DualVNH5019MotorShield.h> // needed for motors

/* --- Declarations --- */
// Spawn serial communication objects.
LiquidCrystal_I2C lcd(LCD_I2C_PORT, LCD_WIDTH, LCD_HEIGHT); // control object for LCD
DualVNH5019MotorShield motors; // M1 is steering actuator, M2 is ballast motor
// Create switch booleans (Locks, Kills, Triggers and Limits).
int KILL_HITCH = 0;
int KILL_BUTTON = 0;
int KILL_SEAT = 0;
int LOCK_LEFTBRAKE = 0;
int LOCK_RIGHTBRAKE = 0;
int LOCK_CVT = 0;
int LIMIT_FAR = 0;
int LIMIT_NEAR = 0;
int IGNITION = 0;
// Create ballast and steering values.
int BALLAST_SPEED = 0;
int MOTOR_SPEED = 0;
int ACTUATOR_SPEED = 0;
volatile int STEERING_POSITION = 0;
volatile int ACTUATOR_POSITION = 0;
int ACTUATOR_FAULT = 0;
int MOTOR_FAULT = 0;
// Create sensor reading values and motor fault booleans.
volatile unsigned int SENSOR_FUEL = 0;
volatile unsigned int SENSOR_ENGINE = 0;
volatile unsigned int SENSOR_WHEEL = 0;
volatile unsigned int RATE_FUEL = 0;
volatile unsigned int RATE_ENGINE = 0;
volatile unsigned int RATE_WHEEL = 0;
// Create character buffers for LCD.
char BUFFER_FUEL[128];
char BUFFER_WHEEL[128];
char BUFFER_ENGINE[128];
char BUFFER_BALLAST[128];
char BUFFER_STEERING[128];
char BUFFER_ACTUATOR[128];
// List of valid RFID keys.
int ANTOINE[] = {139, 151, 226, 117};
int TREVOR[] = {10, 227, 165, 221};
int KEY[] = {0, 0, 0, 0};
// Create time interval.
volatile unsigned int INTERVAL = 0;
/* --- Setup --- */
// Begin MR14 initialization actions.
void setup() {
  /* --- Initialize Serial Objects--- */
  Serial.begin(BAUD); // System
  Serial2.begin(BAUD); // RFID
  motors.setM1Speed(0); // Actuator
  motors.setM2Speed(0); // Ballast
  /* --- Initialize Ballast and Steering --- */
  pinMode(STEERING_A_PIN, INPUT);
  pinMode(STEERING_B_PIN, INPUT);
  digitalWrite(STEERING_A_PIN, HIGH); // turn on pullup resistor
  digitalWrite(STEERING_B_PIN, HIGH); // turn on pullup resistor
  /* --- Relays --- */
  pinMode(RELAY_1_PIN, OUTPUT);
  pinMode(RELAY_2_PIN, OUTPUT);
  pinMode(RELAY_3_PIN, OUTPUT);
  digitalWrite(RELAY_1_PIN, HIGH);
  digitalWrite(RELAY_2_PIN, HIGH);
  digitalWrite(RELAY_3_PIN, HIGH);
  /* --- Power Pins --- */
  pinMode(KILL_BUTTON_POWER, OUTPUT);
  digitalWrite(KILL_BUTTON_POWER, LOW);
  pinMode(KILL_SEAT_POWER, OUTPUT);
  digitalWrite(KILL_SEAT_POWER, LOW);
  pinMode(KILL_HITCH_POWER, OUTPUT);
  digitalWrite(KILL_HITCH_POWER, LOW);
  pinMode(LOCK_LEFTBRAKE_POWER, OUTPUT);
  digitalWrite(LOCK_LEFTBRAKE_POWER, LOW);
  pinMode(LOCK_RIGHTBRAKE_POWER, OUTPUT);
  digitalWrite(LOCK_RIGHTBRAKE_POWER, LOW);
  pinMode(IGNITION_POWER, OUTPUT);
  digitalWrite(IGNITION_POWER, LOW);
  /* --- Input Pins --- */
  pinMode(KILL_BUTTON_PIN, INPUT);
  digitalWrite(KILL_BUTTON_PIN, HIGH);
  pinMode(KILL_SEAT_PIN, INPUT);
  digitalWrite(LOCK_LEFTBRAKE_PIN, HIGH);
  pinMode(LOCK_RIGHTBRAKE_PIN, INPUT);
  digitalWrite(LOCK_RIGHTBRAKE_PIN, HIGH);
  pinMode(IGNITION_PIN, INPUT);
  digitalWrite(IGNITION_PIN, HIGH);
  pinMode(LIMIT_NEAR_PIN, INPUT);
  digitalWrite(LIMIT_NEAR_PIN, HIGH);
  pinMode(LIMIT_FAR_PIN, INPUT);
  digitalWrite(LIMIT_FAR_PIN, HIGH);
  /* --- Initialize Analog Pins --- */
  pinMode(ACTUATOR_FAULT_PIN, INPUT); 
  pinMode(MOTOR_FAULT_PIN, INPUT);
  pinMode(LOCK_CVT_PIN, INPUT);
  pinMode(BALLAST_SPEED_PIN, INPUT);
  /* --- Interrupt Pins --- */
  pinMode(SENSOR_FUEL_PIN, INPUT);
  pinMode(SENSOR_ENGINE_PIN, INPUT);
  pinMode(SENSOR_WHEEL_PIN, INPUT);
  /* --- Boot message --- */
  Serial.println("BOOTING...");
  delay(SHORT);
  Serial.println("MR14");
  Serial.println("MCGILL UNIVERSITY");
  Serial.println("BIORESOURCE ENG");
  delay(LONG);
  Serial.println("READ OWNERS MANUAL");
  Serial.println("BEFORE OPERATION");
  delay(LONG);
}

/////////////// RUNTIME ///////////////
/*********** LOOP ***********/
/* --- Loop --- */
void loop() {
  interrupts();
  // Attempt to enter ON mode.
  on();
  // Disable all tractor functions then wait for reboot.
  off();
}
/*********** STATES ***********/
/*
  Void functions which are operation states for MR14.
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
  // Display ON message.
  Serial.println("ON");
  delay(SHORT);
  //  While no Kills, contine to prompt for RFID key.
  while (nokill()) {
    // Display prompt message then test RFID serial.
    Serial.println("SWIPE KEY CARD");
    delay(SHORT);
    Serial2.write(0x02);
    delay(SHORT);
    if (Serial2.available()) {
      // If key is good, enter STANDY mode.
      if (testKey()) {
        standby();
      }
      // Else if key is bad, display error message.
      else {
        Serial.println("INCORRECT KEY");
        delay(SHORT);
      }
    }
  }
}

/* --- Standby --- */
// ON() && TESTKEY() && KILL() --> STANDY()
void standby() {
  // If no Kills enabled, allow partial standby.
  while (nokill()) {
    //  If no Locks or kills enabled, allow full standby.
    while (nolock() && nokill()) {
      // Enable Regulator and Fuel Solenoid and display STANDY message.
      digitalWrite(RELAY_1_PIN, LOW);
      digitalWrite(RELAY_2_PIN, LOW);
      digitalWrite(RELAY_3_PIN, HIGH);
      Serial.println("STANDBY");
      delay(SHORT);
      if (start()) { // activate ignition if start button engaged
        ignition();
        run();
      }
    }
  }
}

/* --- Ignition --- */
// STANDBY() && START() && KILL() && LOCK() --> IGNITION()
void ignition() {
  // Enable Starter, disable Fuel Solenoid and display ignition message.
  digitalWrite(RELAY_1_PIN, LOW);
  digitalWrite(RELAY_2_PIN, LOW);
  digitalWrite(RELAY_3_PIN, LOW);
  Serial.println("IGNITION");
  delay(SHORT);
  // While ignition button is enaged, attempt ignition.
  while (start() && nokill() && nolock()) {
    delay(SHORT);
  }
}

/* --- Run --- */
// STANDBY() && START() && KILL() && LOCK() --> RUN()
void run() {
  // Enable engine RUN mode
  digitalWrite(RELAY_1_PIN, LOW);
  digitalWrite(RELAY_2_PIN, LOW);
  digitalWrite(RELAY_3_PIN, HIGH);
  // Display RUNNING message
  Serial.println("RUNNING");
  delay(MEDIUM);
  // Attach interrupt functions to interrupt pins.
  attachInterrupt(1, fuel, RISING); // Pin 3
  attachInterrupt(2, engine, RISING); // Pin 5
  attachInterrupt(3, wheel, RISING); // Pin 6
  attachInterrupt(4, encoder, CHANGE); // Pin 18
  // While the engine is running, monitor all sensors and controls. 
  while (nokill()) {
    Serial.println("--------------");
    // Get Ballast and steering
    ballast();
    steering();
    // Reset interrupt counters, then wait for next interval.
    SENSOR_FUEL = 0;
    SENSOR_ENGINE = 0;
    SENSOR_WHEEL = 0;
    INTERVAL = 0; // milliseconds/interval
    delay(MEDIUM);
    INTERVAL += MEDIUM;
    // Calculate rates
    RATE_FUEL = (SENSOR_FUEL * 1800) / INTERVAL; // convert pulses to Liters/Hr (1800 L*Hrs/pulse)
    RATE_ENGINE = (SENSOR_ENGINE * 60000) / INTERVAL; // convert pulses to RPM (60000 millisec/min)
    RATE_WHEEL = (SENSOR_WHEEL * 60000) / INTERVAL; // converts pulses to RPM (60000 millisec/min)
    // Package sensor readings as character buffers, then display them.
    sprintf(BUFFER_FUEL, "FUEL (L/H):   %d    ", RATE_FUEL);
    sprintf(BUFFER_ENGINE, "ENGINE (RPM): %d    ", RATE_ENGINE);
    sprintf(BUFFER_WHEEL, "WHEEL (RPM):  %d    ", RATE_WHEEL);
    sprintf(BUFFER_BALLAST, "BALLAST:  %d    ", MOTOR_SPEED);
    sprintf(BUFFER_STEERING, "STEERING:  %d    ", STEERING_POSITION);
    sprintf(BUFFER_ACTUATOR, "ACTUATOR:  %d    ", ACTUATOR_POSITION);
    Serial.println(BUFFER_FUEL);
    Serial.println(BUFFER_ENGINE);
    Serial.println(BUFFER_WHEEL);
    Serial.println(BUFFER_BALLAST);
    Serial.println(BUFFER_STEERING);
    Serial.println(BUFFER_ACTUATOR);
  }
}

/* --- Off --- */
// LOOP() --> OFF()
void off() {
  // Engine Position 1
  digitalWrite(RELAY_1_PIN, HIGH);
  digitalWrite(RELAY_2_PIN, HIGH);
  digitalWrite(RELAY_3_PIN, HIGH);
  // Display OFF message
  Serial.println("OFF");
  delay(MEDIUM);
}

/*********** FUNCTIONS ***********/
/*
  Secondary tasks executed by state functions which return values.
    kill()
    lock()
    testKey()
    ballast()
    steering()
*/

/* --- Kill Switches --- */
// Tractor will not operate if any of the killswitches are engaged
boolean nokill() {
  // Get kill switch states
  KILL_SEAT = digitalRead(KILL_SEAT_PIN);
  KILL_BUTTON = digitalRead(KILL_BUTTON_PIN);
  KILL_HITCH = digitalRead(KILL_HITCH_PIN);
  // If driver is off of seat
  if (KILL_SEAT) {
    delay(SHORT);
    // Re-read to determine if driver is still off of the seat
    if (digitalRead(KILL_SEAT_PIN)) {
      // Engine Position 1
      digitalWrite(RELAY_1_PIN, HIGH);
      digitalWrite(RELAY_2_PIN, HIGH);
      digitalWrite(RELAY_3_PIN, HIGH);
      // Error message
      Serial.println("DRIVER NOT ON SEAT");
      delay(SHORT);
      return false;
    }
    else {
      return true;
    }

  }
  // If hitch is removed
  else if (KILL_HITCH) {
    // Engine Position 1
    digitalWrite(RELAY_1_PIN, HIGH);
    digitalWrite(RELAY_2_PIN, HIGH);
    digitalWrite(RELAY_3_PIN, HIGH);
    // Error message
    Serial.println("HITCH DETACHED");
    delay(SHORT);
    return false;
  }
  // If Kill button is pressed
  else if (KILL_BUTTON) {
     // Engine Position 1
    digitalWrite(RELAY_1_PIN, HIGH);
    digitalWrite(RELAY_2_PIN, HIGH);
    digitalWrite(RELAY_3_PIN, HIGH);
    // Error message
    Serial.println("KILL BUTTON PRESSED");
    delay(SHORT);
    return false;
  }
  // If all kill switches are not engaged
  else {
    return true;
  }
}

/* --- Lock Switches --- */
// MR14 will not allow ignition if any of the locks are engaged
boolean nolock() {
  // get Brake switches
  LOCK_LEFTBRAKE = digitalRead(LOCK_LEFTBRAKE_PIN);
  LOCK_RIGHTBRAKE = digitalRead(LOCK_RIGHTBRAKE_PIN);
  // If brakes are not pressed
  if (LOCK_LEFTBRAKE || LOCK_RIGHTBRAKE) {
    Serial.println("BRAKES NOT ENGAGED");
    delay(SHORT);
  }
  // Get CVT state
  LOCK_CVT = analogRead(LOCK_CVT_PIN);
  // If CVT guard is removed
  if (LOCK_CVT > 10) {
    Serial.println("CVT GUARD OFF");
    delay(SHORT);
  }
  // Return value.
  if (LOCK_CVT || LOCK_LEFTBRAKE || LOCK_RIGHTBRAKE) {
    return false;
  }
  else {
    return true;
  }
}

/* --- Start Ignition --- */
boolean start() {
  IGNITION = digitalRead(IGNITION_PIN);
  if (IGNITION) {
    return false;
  }
  else {
    return true;
  }
}

/* --- Test Key --- */
// Tests if RFID key is valid
boolean testKey() {
  // Reset temporary key.
  for (int i = 0; i < KEYLENGTH; i++) {
    KEY[i] = 0;
  }
  // Get RFID key
  for (int i = 0; i < KEYLENGTH; i++) {
    KEY[i] = Serial2.read();
  }
  // Test if Antoine's key.
  if (KEY[0] == 139) {
    for (int j = 0; j < KEYLENGTH; j++) {
      if (ANTOINE[j] == KEY[j]) { // compare key to valid key
        continue;
      }
      else {
        return false;
      }
    }
    Serial.println("HELLO ANTOINE");
    delay(MEDIUM);
  }
  // Test if Trevor's key
  else if (KEY[0] == 10) {
    for (int j = 0; j < KEYLENGTH; j++) {
      if (TREVOR[j] == KEY[j]) { // compare key to valid key
        continue;
      }
      else {
        return false;
      }
    }
    Serial.println("HELLO TREVOR");
    delay(MEDIUM);
  }
  // Bad key
  else {
    return false;
  }
  // If key passed, return true
  return true;
}

/* --- Ballast --- */
// 5V --> POTENTIOMETER --> (GND || BALLAST_SPEED_PIN)
int ballast() {
  // Get Ballast control input and well as far/near limit switches.
  BALLAST_SPEED = analogRead(BALLAST_SPEED_PIN);
  LIMIT_NEAR = digitalRead(LIMIT_NEAR_PIN);
  LIMIT_FAR = digitalRead(LIMIT_FAR_PIN);
  // If motor is stable, enable motor.
  //MOTOR_FAULT = digitalRead(MOTOR_FAULT_PIN);
  MOTOR_FAULT = 0;
  if (!MOTOR_FAULT) {
    // If limits are not engaged, set the proper motor speed.
    if (LIMIT_NEAR || LIMIT_FAR) {
      Serial.println("LIMIT REACHED");
      MOTOR_SPEED = OFF;
    }
    else {
      if (BALLAST_SPEED > 0) {
        MOTOR_SPEED = BALLAST_SPEED;
      }
      else {
        MOTOR_SPEED = REVERSE;
      }
      Serial.println("LIMIT REACHED");
      MOTOR_SPEED = OFF;
    }
  }
  // If motor fault, disable motor.
  else {
    Serial.println("MOTOR FAULT");
    MOTOR_SPEED = OFF;
  }
  // Set motor and return values.
  motors.setM2Speed(MOTOR_SPEED);
  return MOTOR_SPEED;
}

/* --- Steering --- */
int steering() {
  // Get actuator fault reading.
  //ACTUATOR_FAULT = digitalRead(ACTUATOR_FAULT_PIN);
  ACTUATOR_FAULT = 0;
  // If actuator is stable, enable actuator.
  if (!ACTUATOR_FAULT) {
    // Until actuator is left of steering wheel, adjust right.
    if (ACTUATOR_POSITION < STEERING_POSITION) {
      while (ACTUATOR_POSITION < STEERING_POSITION) {
        motors.setM1Speed(MODERATE);
        ACTUATOR_POSITION++;
        delay(SHORTEST);
        INTERVAL += SHORTEST; // increment time of interval
      }
    }
    // Until actuator is right of steering wheel, adjust left.
   else if (ACTUATOR_POSITION > STEERING_POSITION) {
      while (ACTUATOR_POSITION > STEERING_POSITION) {
        motors.setM1Speed(-MODERATE);
        ACTUATOR_POSITION--;
        delay(SHORTEST);
        INTERVAL += SHORTEST; // increment time of interval
      }
    }
    // Otherwise, disable actuator.
    else {
      motors.setM1Speed(OFF);
    }
  }
  // Otherwise, disable actuator;
  else {
    Serial.println("ACTUATOR FAULT");
    motors.setM1Speed(OFF);
  }
  // Return value.
  return STEERING_POSITION;
}

/*********** INTERRUPTS ***********/
/*
  Void functions that are called frequently as counter incrementers.
    fuel()
    engine()
    wheel()
    encoder()
*/
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

/* --- Encoder --- */
void encoder() {
  // Get the current encoder position.
  if (digitalRead(STEERING_A_PIN) == digitalRead(STEERING_B_PIN)) {
    STEERING_POSITION++;
  }
  else {
    STEERING_POSITION--;
  }
}
