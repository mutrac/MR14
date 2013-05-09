/*
  config.h
  Author: Trevor Stanhope
  Date: 2013-05-04
  Summary: Configuration Library for MR14
*/

/* --- I/O PINS --- */

// KILL AND LOCK SWITCHES
#define KILL_BUTTON_PIN 22
#define KILL_HITCH_PIN 23
#define KILL_SEAT_PIN 24
#define LOCK_BRAKE_PIN 25
#define LOCK_CVT_PIN 26

// CONTROLS
#define IGNITION_PIN 34
#define BALLAST_PIN 35
#define STEERING_A_PIN 37
#define STEERING_B_PIN 38

// STEERING ACTUATOR
#define ACTUATOR_A_PIN 2
#define ACTUATOR_B_PIN 4
#define ACTUATOR_SPEED_PIN 9

// BALLAST MOTOR
#define MOTOR_A_PIN 7
#define MOTOR_B_PIN 8
#define MOTOR_SPEED_PIN 10

// DISPLAY
#define LCD_SDA_PIN 20
#define LCD_SCL_PIN 21
#define LCD_I2C_PORT 0x3F

// ENGINE RELAYS
#define RELAY1_PIN 30 // pin for the STANDBY mode
#define RELAY2_PIN 31 // pin for relay to engine start
#define RELAY3_PIN 32
#define RELAY4_PIN 33

// SENSORS
#define SENSOR_FUEL_PIN 2
#define SENSOR_ENGINE_PIN 3
#define SENSOR_WHEEL_PIN 4
#define SENSOR_TEMP_PIN 26

/* --- VALUES --- */
// CONSTANTS
#define KEYLENGTH 4
#define BAUD 9600
#define SHORTEST 100
#define SHORTER 300
#define SHORT 800
#define MEDIUM 1000
#define LONG 1500
#define LONGER 2000
#define LCD_WIDTH 16
#define LCD_HEIGHT 2

// LCD COMMANDS
#define I2C_PORT 0x3F

// RFID COMMANDS
#define CARD 0x02
#define READ 0x03
#define WRITE 0x04
