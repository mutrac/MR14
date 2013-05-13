/*
  config.h
  Author: Trevor Stanhope
  Date: 2013-05-04
  Summary: Configuration Library for MR14
*/

/* --- PWM PINS --- */
// STEERING ACTUATOR
#define ACTUATOR_A_PIN 2
#define ACTUATOR_B_PIN 4
#define ACTUATOR_SPEED_PIN 9
#define ACTUATOR_FAULT_PIN 6

// BALLAST MOTOR
#define MOTOR_A_PIN 7
#define MOTOR_B_PIN 8
#define MOTOR_SPEED_PIN 10
#define MOTOR_FAULT_PIN 12

// SENSORS
#define SENSOR_FUEL_PIN 3
#define SENSOR_WHEEL_PIN 5
#define SENSOR_ENGINE_PIN 11 // TOO MANY PWM PINS
/* --- END PWM PINS --- */

/* --- START COMMUNICATION PINS --- */
// RFID
#define RFID_TX_PIN 14
#define RFID_RX_PIN 15

// LCD
#define LCD_SDA_PIN 20
#define LCD_SCL_PIN 21
/* --- END COMMUNICATION PINS --- */

/* --- START DIGITAL PINS --- */
// KILL AND LOCK SWITCHES
#define KILL_BUTTON_PIN 22
#define KILL_HITCH_PIN 23
#define KILL_SEAT_PIN 24
#define LOCK_BRAKE_PIN 25
#define LOCK_CVT_PIN 26
#define LIMIT_BALLAST_FAR_PIN 27
#define LIMIT_BALLAST_NEAR_PIN 28

// CONTROLS
#define IGNITION_PIN 29

// ENGINE RELAYS
#define RELAY_1_PIN 30 // pin for the STANDBY mode
#define RELAY_2_PIN 31 // pin for relay to engine start
#define RELAY_3_PIN 32
#define RELAY_4_PIN 33

// STEERING
#define STEERING_A_PIN 45 // these can act as PWM
#define STEERING_B_PIN 46 // these can act as PWM
/* --- END DIGITAL PINS --- */

/* --- START ANALOG PINS --- */
#define ACTUATOR_CURRENT_PIN 0
#define MOTOR_CURRENT_PIN 1
#define BALLAST_SPEED_PIN 2
/* --- END ANALOG PINS

/* --- START VALUES --- */
// CONSTANTS
#define KEYLENGTH 4
#define BAUD 9600
#define SHORTEST 100
#define SHORTER 300
#define SHORT 800
#define MEDIUM 1000
#define LONG 1500
#define LONGER 2000
#define LCD_WIDTH 20
#define LCD_HEIGHT 4

// LCD COMMANDS
#define LCD_I2C_PORT 0x3F

// RFID COMMANDS
#define CARD 0x02
#define READ 0x03
#define WRITE 0x04
/* --- END VALUES --- */
