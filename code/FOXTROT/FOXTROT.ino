/*
  MR14 GAMMA
  Organization: MuTrac (McGill ASABE Tractor Pull Team), McGill University
  Author: Trevor Stanhope
  Date: 2013-05-28
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
#include "config.h" // needed for pin setup and global values
#include <DualVNH5019MotorShield.h> // needed for motors
#include "stdio.h"

/* --- Declarations --- */
// Spawn serial communication objects.
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
int LIMIT_STEERING = 0;
int IGNITION = 0;

// Create ballast and steering values.
int BALLAST_SPEED = 0;
volatile int STEERING_POSITION = 0;
volatile int ACTUATOR_POSITION = 0;
int ACTUATOR_FAULT = 0;
int MOTOR_FAULT = 0;

// Create sensor reading values and motor fault booleans.
volatile int SENSOR_FUEL = 0;
volatile int SENSOR_ENGINE = 0;
volatile int SENSOR_WHEEL = 0;

// Calculate values
char RATE_FUEL[CHARS];
char PERCENTAGE_LOAD[CHARS]; // char because can't print float
int RATE_ENGINE = 0;
int RATE_WHEEL = 0;

// Character buffers.
char BUFFER_FUEL[MAX];
char BUFFER_LOAD[MAX];
char BUFFER_WHEEL[MAX];
char BUFFER_ENGINE[MAX];

// List of valid RFID keys.
int ANTOINE[] = {139, 151, 226, 117};
int TREVOR[] = {10, 227, 165, 221};

// Create time interval.
int INTERVAL = 0;
int INTERVAL_START = 0;
int INTERVAL_END = 0;

/* --- Setup --- */
// Begin MR14 initialization actions.
void setup() {
  /* --- Initialize Serial Objects--- */
  Serial.begin(BAUD); // System
  Serial2.begin(BAUD); // RFID
  Serial3.begin(BAUD); clear(); backlightOn(); cursorOff(); // LCD
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
  
  /* --- Switches --- */
  // Button
  pinMode(KILL_BUTTON_POWER, OUTPUT);
  digitalWrite(KILL_BUTTON_POWER, LOW);
  pinMode(KILL_BUTTON_PIN, INPUT);
  digitalWrite(KILL_BUTTON_PIN, HIGH);
  // Seat
  pinMode(KILL_SEAT_POWER, OUTPUT);
  digitalWrite(KILL_SEAT_POWER, LOW);
  pinMode(KILL_SEAT_PIN, INPUT);
  digitalWrite(KILL_SEAT_PIN, HIGH);
  // Hitch
  pinMode(KILL_HITCH_POWER, OUTPUT);
  digitalWrite(KILL_HITCH_POWER, LOW);
  pinMode(KILL_HITCH_PIN, INPUT);
  digitalWrite(KILL_HITCH_PIN, HIGH);
  // Left Brake
  pinMode(LOCK_LEFTBRAKE_POWER, OUTPUT);
  digitalWrite(LOCK_LEFTBRAKE_POWER, LOW);
  pinMode(LOCK_LEFTBRAKE_PIN, INPUT);
  digitalWrite(LOCK_LEFTBRAKE_PIN, HIGH);
  // Right Brake
  pinMode(LOCK_RIGHTBRAKE_POWER, OUTPUT);
  digitalWrite(LOCK_RIGHTBRAKE_POWER, LOW);
  pinMode(LOCK_RIGHTBRAKE_PIN, INPUT);
  digitalWrite(LOCK_RIGHTBRAKE_PIN, HIGH);
  // Ignition
  pinMode(IGNITION_POWER, OUTPUT);
  digitalWrite(IGNITION_POWER, LOW);
  pinMode(IGNITION_PIN, INPUT);
  digitalWrite(IGNITION_PIN, HIGH);
  // Near Limit
  pinMode(LIMIT_NEAR_POWER, OUTPUT);
  digitalWrite(LIMIT_NEAR_POWER, LOW);
  pinMode(LIMIT_NEAR_PIN, INPUT);
  digitalWrite(LIMIT_NEAR_PIN, HIGH);
  // Far Limit
  pinMode(LIMIT_FAR_POWER, OUTPUT);
  digitalWrite(LIMIT_FAR_POWER, LOW);
  pinMode(LIMIT_FAR_PIN, INPUT);
  digitalWrite(LIMIT_FAR_PIN, HIGH);
  // Steering Limit
  pinMode(LIMIT_STEERING_POWER, OUTPUT);
  digitalWrite(LIMIT_STEERING_POWER, LOW);
  pinMode(LIMIT_STEERING_PIN, INPUT);
  digitalWrite(LIMIT_STEERING_PIN, HIGH);
  
  /* --- Initialize Analog Pins --- */
  pinMode(ACTUATOR_FAULT_PIN, INPUT); 
  pinMode(MOTOR_FAULT_PIN, INPUT);
  pinMode(LOCK_CVT_PIN, INPUT);
  pinMode(BALLAST_SPEED_PIN, INPUT);
  
  /* --- Interrupt Pins --- */
  pinMode(SENSOR_FUEL_PIN, INPUT);
  pinMode(SENSOR_ENGINE_PIN, INPUT);
  pinMode(SENSOR_WHEEL_PIN, INPUT);
  
  /* --- LCD --- */
  tone(227, 60); tone(227, 60); Serial3.print("BOOTING..."); delay(MEDIUM); clear();
  Serial3.print("MR14"); newline(); 
  Serial3.print("MCGILL UNIVERSITY"); newline(); 
  Serial3.print("BIORESOURCE ENG."); delay(LONG); clear();
  Serial3.print("READ OWNERS MANUAL"); newline();
  Serial3.print("BEFORE OPERATION"); delay(LONG); clear();
}

/////////////// RUNTIME ///////////////
/*********** LOOP ***********/
/* --- Loop --- */
void loop() {
  
  // Attempt to enter ON mode.
  on();
  
  // Disable all tractor functions then wait for reboot.
  restart();
  
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
  clear(); Serial3.print("ON"); delay(MEDIUM); clear(); Serial.println("ON");
  
  //  While no Kills, contine to prompt for RFID key.
  while (nokill()) {
    // Display prompt message then test RFID serial.
    clear(); Serial3.print("SWIPE KEY CARD"); delay(SHORT); clear(); Serial.println("SWIPE KEY CARD");
    Serial2.write(0x02);
    delay(SHORTER);
    if (Serial2.available()) {
      // If key is good, enter STANDY mode.
      if (testKey()) {
        standby(); /// <<<<<<<<<<<<<<<<<<<<
        // Reset temporary key.
      }
      // Else if key is bad, display error message.
      else {
        clear(); Serial3.print("INCORRECT KEY"); delay(SHORT); clear(); Serial.println("INCORRECT KEY");
      }
    }
  }
}

/* --- Standby --- */
// ON() && TESTKEY() && KILL() --> STANDY()
void standby() {
  
  //  If no Locks or kills enabled, allow full standby.
  while (nolock() && nokill()) {
    clear(); Serial3.print("STANDBY"); delay(MEDIUM); clear(); Serial.println("STANDBY");
    digitalWrite(RELAY_1_PIN, LOW);
    digitalWrite(RELAY_2_PIN, LOW);
    digitalWrite(RELAY_3_PIN, HIGH);
  
    // Activate ignition if button is pressed.
    if (start()) {
      ignition();
      run();
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
  Serial3.print("IGNITION"); delay(SHORT); clear(); Serial.println("IGNITION");
  
  // While ignition button is enaged, attempt ignition.
  while (start() && nokill()) {
    clear(); Serial3.print("IGNITION"); delay(SHORTER); clear(); Serial.println("IGNITION");
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
  clear(); Serial3.print("RUNNING"); delay(MEDIUM); clear(); Serial.println("RUNNING");
  
  // Attach interrupt functions to interrupt pins.
  attachInterrupt(1, fuel, RISING); // Pin 3
  attachInterrupt(2, engine, RISING); // Pin 5
  attachInterrupt(4, wheel, RISING); // Pin 18
  attachInterrupt(5, encoder, CHANGE); // Pin 19
  
  // While the engine is running, monitor all sensors and controls. 
  while (nokill()) {
    // Get ballast speed()
    ballast();
    
    // Reset counters
    Serial.println("--------------");
    SENSOR_FUEL = 0;
    SENSOR_ENGINE = 0;
    SENSOR_WHEEL = 0;
    
    // Active interval
    INTERVAL_START = millis(); // milliseconds/interval
    // Interval 1
    steering();
    delay(SHORTER);
    if (!nokill()) {
      break;
    }
    // Interval 2
    steering();
    delay(SHORTER);
    if (!nokill()) {
      break;
    }
    // Interval 3
    steering();
    delay(SHORTER);
    if (!nokill()) {
      break;
    }
    // Interval 4
    steering();
    delay(SHORTER);
    if (!nokill()) {
      break;
    }
    INTERVAL_END = millis();
    
    // Display to serial.
    Serial.println(INTERVAL);
    Serial.println(SENSOR_FUEL);
    Serial.println(SENSOR_ENGINE);
    Serial.println(SENSOR_WHEEL);
    
    // Calculate rates
    INTERVAL = INTERVAL_END - INTERVAL_START;
    dtostrf(LPH(), 3, 1, RATE_FUEL);
    dtostrf(Load(), 3, 1, PERCENTAGE_LOAD);
    RATE_ENGINE = engineRPM();
    RATE_WHEEL = wheelRPM();
    
    // Package sensor readings as character buffers, then display them.
    sprintf(BUFFER_FUEL, "FUEL (L/H): %s", RATE_FUEL);
    sprintf(BUFFER_LOAD, "LOAD (%%): %s", PERCENTAGE_LOAD);
    sprintf(BUFFER_ENGINE, "ENGINE (RPM): %d", RATE_ENGINE);
    sprintf(BUFFER_WHEEL, "WHEEL (RPM): %d", RATE_WHEEL);
    clear();
    Serial3.print(BUFFER_FUEL); newline(); Serial.println(RATE_FUEL);
    Serial3.print(BUFFER_LOAD); newline(); Serial.println(PERCENTAGE_LOAD);
    Serial3.print(BUFFER_ENGINE); newline(); Serial.println(RATE_ENGINE);
    Serial3.print(BUFFER_WHEEL); newline(); Serial.println(RATE_WHEEL);
  }
}

/* --- Restart --- */
// LOOP() --> OFF()
void restart() {
  
  // Engine Position 1
  digitalWrite(RELAY_1_PIN, HIGH); Serial.print(1);
  digitalWrite(RELAY_2_PIN, HIGH); Serial.print(1);
  digitalWrite(RELAY_3_PIN, HIGH); Serial.print(1);
  
  // Display OFF message
  Serial.println("RESTARTING");
  Serial3.print("RESTARTING..."); delay(MEDIUM); clear();
  
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
      // Actuator and Ballast
      motors.setM1Speed(OFF);
      motors.setM2Speed(OFF);
      // Error message
      Serial.println("DRIVER NOT ON SEAT");
      clear(); tone(220, 100); Serial3.print("DRIVER NOT ON SEAT"); delay(LONGER); clear();
    }
  }
  
  // If hitch is removed
  if (KILL_HITCH) {
    // Engine Position 1
    digitalWrite(RELAY_1_PIN, HIGH);
    digitalWrite(RELAY_2_PIN, HIGH);
    digitalWrite(RELAY_3_PIN, HIGH);
    // Actuator and Ballast
    motors.setM1Speed(OFF);
    motors.setM2Speed(OFF);
    // Error message
    Serial.println("HITCH DETACHED");
    clear(); tone(220, 100); Serial3.print("HITCH DETACHED"); delay(LONGER); clear();
  }
  
  // If Kill button is pressed
  if (KILL_BUTTON) {
     // Engine Position 1
    digitalWrite(RELAY_1_PIN, HIGH);
    digitalWrite(RELAY_2_PIN, HIGH);
    digitalWrite(RELAY_3_PIN, HIGH);
    // Actuator and Ballast
    motors.setM1Speed(OFF);
    motors.setM2Speed(OFF);
    // Error message
    Serial.println("KILL BUTTON PRESSED");
    clear(); tone(220, 100); Serial3.print("KILL BUTTON PRESSED"); delay(LONGER); clear();
  }
  
  // Return kill state
  if (KILL_SEAT || KILL_BUTTON || KILL_HITCH) {
    return false;
  }
  else {
    return true;
  }
}

/* --- Lock Switches --- */
// MR14 will not allow ignition if any of the locks are engaged
boolean nolock() {
  
  // get interlock states.
  LOCK_LEFTBRAKE = digitalRead(LOCK_LEFTBRAKE_PIN);
  LOCK_RIGHTBRAKE = digitalRead(LOCK_RIGHTBRAKE_PIN);
  LOCK_CVT = analogRead(LOCK_CVT_PIN);
  
  // If left brake is not pressed
  if (LOCK_LEFTBRAKE) {
    Serial.println("ENGAGE LEFT BRAKE");
    Serial3.print("ENGAGE LEFT BRAKE"); delay(SHORT); clear();
  }
  
  // If right brake is not pressed
  if (LOCK_RIGHTBRAKE) {
    Serial.println("ENGAGE RIGHT BRAKE");
    Serial3.print("ENGAGE RIGHT BRAKE"); delay(SHORT); clear();
  }
  
  // If CVT guard is removed
  if (LOCK_CVT > 10) {
    Serial.println("CVT GUARD OFF");
    Serial3.print("CVT GUARD OFF"); delay(SHORT); clear();
  }
  
  // Return lock state.
  if ((LOCK_CVT > 10) || LOCK_LEFTBRAKE || LOCK_RIGHTBRAKE) {
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
  int KEY[] = {0,0,0,0};
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
    Serial3.print("HELLO ANTOINE"); delay(MEDIUM); clear();
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
    Serial3.print("HELLO TREVOR"); delay(MEDIUM); clear();
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
  
  // If limits are engaged, disable the DBS motor.
  if (LIMIT_NEAR || LIMIT_FAR) {
    motors.setM2Speed(OFF);
    Serial.println("BALLAST LIMIT");
    tone(220, 100); Serial3.print("BALLAST LIMIT"); delay(SHORT); clear();
  }
  
  // If limits are not engaged, activate motor.
  else {
    if (BALLAST_SPEED > 0) {
      if (BALLAST_SPEED < 100) {
        motors.setM2Speed(OFF); // no speed at first
      }
      else {
        motors.setM2Speed((BALLAST_SPEED*400)/1023); // from 0 to 400
      }
    }
    else {
      motors.setM2Speed(REVERSE);
    }
  }
  
  // Set motor and return values.
  return BALLAST_SPEED;
}

/* --- Steering --- */
int steering() {
  
  // Get steering limit
  LIMIT_STEERING = digitalRead(LIMIT_STEERING_PIN);

  // If at steering limit, disable actuator.
  if (LIMIT_STEERING) {
    motors.setM1Speed(OFF);
    Serial.println("STEERING LIMIT");
    Serial3.print("STEERING LIMIT"); delay(SHORT); clear();
  }
  
  // Otherwise, enable actuator.
  else {
    // Until actuator is left of steering wheel, adjust right.
    if (ACTUATOR_POSITION < STEERING_POSITION) {
      while (ACTUATOR_POSITION < STEERING_POSITION) {
        motors.setM1Speed(ACTUATOR_SPEED);
        ACTUATOR_POSITION++;
        delay(SHORTEST);
      }
    }
    
    // Until actuator is right of steering wheel, adjust left.
    else if (ACTUATOR_POSITION > STEERING_POSITION) {
      while (ACTUATOR_POSITION > STEERING_POSITION) {
        motors.setM1Speed(-ACTUATOR_SPEED);
        ACTUATOR_POSITION--;
        delay(SHORTEST);
      }
    }
      
    // After actuator is adjusted, disable it.
    else {
        motors.setM1Speed(OFF);
    }
  }
  
  // Return value.
  return ACTUATOR_POSITION;
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

/*********** LCD ***********/
/*
  Void functions that are called frequently to adjust the LCD.
*/
void clear() {
  Serial3.write(12);
}

void newline() {
  Serial3.write(13);
}

void backlightOn() {
  Serial3.write(17);
}

void backlightOff() {
  Serial3.write(18);
}

void displayOff() {
  Serial3.write(20);
}

void displayOn() {
  Serial3.write(21);
}

void cursorOff() {
  Serial3.write(22);
}

void cursorOn() {
  Serial3.write(23);
}

void tone(int note, int duration) {
  if ((note > 218) && (note < 232)) {
    Serial3.write(note);
    delay(duration);
  }
}

/*********** SENSOR CALCULATIONS ***********/
float LPH() {
  float val = 0;
  val = (SENSOR_FUEL * 1800) / INTERVAL; // convert pulses to Liters/Hr (1800 L*Hrs/pulse)
  return val;
}

int engineRPM() {
  int val = 0;
  val = (SENSOR_ENGINE * 60000) / INTERVAL; // converts pulses to RPM (60000 millisec/min)
  return val;
}

int wheelRPM() {
  int val = 0;
  val = (SENSOR_WHEEL * 60000) / INTERVAL; // converts pulses to RPM (60000 millisec/min)
  return val;
}

float Load() {
  float val = 0;
  val = ((SENSOR_FUEL * 25380) / INTERVAL); // convert pulses to Liters/Hr (1800 L*Hrs/pulse)
  return val;
}
