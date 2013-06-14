/*
  config.h
  Author: Trevor Stanhope
  Date: 2013-05-29
  Summary: Configuration Library for MR14
  0 --> 2
  1 --> 3
  2 --> 21
  3 --> 20
  4 --> 19
  5 --> 18
*/

/*********** INPUT/OUTPUT ***********/
/* --- PWM PINS --- */
// PWM 1: SENSORS
#define SENSOR_FUEL_PIN 3
#define SENSOR_FUEL_INT 1
#define SENSOR_WHEEL_PIN 18
#define SENSOR_WHEEL_INT 5
#define SENSOR_ENGINE_PIN 21
#define SENSOR_ENGINE_INT 2

/* --- COMMUNICATION PINS --- */
// COMMUNICATION 1: RFID, STEERING AND LCD
#define RFID_TX_PIN 16 // Serial2 TX
#define RFID_RX_PIN 17 // Serial2 RX
#define STEERING_A_PIN 19
#define STEERING_B_PIN 20
#define STEERING_INT 4

/* --- START DIGITAL PINS --- */
// DIGITAL 1: KILL, TRIGGER AND LOCK SWITCHES
#define IGNITION_POWER 22
#define IGNITION_PIN 23
#define KILL_BUTTON_POWER 24
#define KILL_BUTTON_PIN 25
#define KILL_HITCH_POWER 26
#define KILL_HITCH_PIN 27
#define KILL_SEAT_POWER 28
#define KILL_SEAT_PIN 29
#define LOCK_LEFTBRAKE_POWER 30
#define LOCK_LEFTBRAKE_PIN 31
#define LOCK_RIGHTBRAKE_POWER 32
#define LOCK_RIGHTBRAKE_PIN 33
#define LIMIT_NEAR_POWER 34
#define LIMIT_NEAR_PIN 35
#define LIMIT_FAR_POWER 36
#define LIMIT_FAR_PIN 37

// DIGITAL 2: ENGINE RELAYS
#define RELAY_1_PIN 39 // pin to disable ENGINE STOP
#define RELAY_2_PIN 43 // pin to enable REGULATION AND FUEL SOLENOID
#define RELAY_3_PIN 45 // pin to enable STARTER

/* --- START ANALOG PINS --- */
// ANALOG 1: POTENTIOMETERS
#define BALLAST_SPEED_PIN 2
#define LOCK_CVT_PIN 3

/*********** VALUES ***********/
/* --- CONSTANTS --- */
#define KEYLENGTH 4
#define BAUD 9600
#define LCD_WIDTH 20
#define LCD_HEIGHT 4
#define CHARS 4
#define MAX 20
#define SENSITIVITY 15
#define THRESHOLD 30000
#define BALLAST_THRESHOLD 25000
#define RATIO 4

/* --- CONVERSIONS --- */
#define CONVERT_WHEEL 15
#define CONVERT_LOAD 180.198
#define CONVERT_ENGINE 60
#define CONVERT_FUEL 25.380

/* --- TIME --- */
#define SHORTEST 40
#define SHORTER 125
#define SHORT 500
#define MEDIUM 1000
#define LONG 2000
#define LONGER 4000
#define INTERVAL 1000

/* --- SERIAL COMMANDS --- */
#define LCD_I2C_PORT 0x3F
#define RFID_CARD 0x02
#define RFID_READ 0x03
#define RFID_WRITE 0x04

// MOTOR CONFIGURATIONS
#define REVERSE -150
#define OFF 0
#define ACTUATOR_SPEED 350
