/* PINS */
// USER INPUT (0 to 7)
#define TX_PIN 2 
#define RX_PIN 3   
#define IGNITION_PIN 25
#define KILL_PIN 24

// TRACTOR CONTROL (14 to 21)
#define RELAY1_PIN 22 // pin for the STANDBY mode
#define RELAY2_PIN 23 // pin for relay to engine start

// DISPLAY
#define SDA_PIN 20
#define SCL_PIN 21
#define I2C_PORT 0x3F

// DAQ (22 to 53)
#define FUEL_PIN 22

/* VALUES */
// CONSTANTS
#define KEYLENGTH 4
#define BAUD 9600
#define SHORT 100
#define MEDIUM 1000
#define LONG 2000
#define WIDTH 16
#define HEIGHT 2

// RFID COMMANDS
#define CARD 0x02
#define READ 0x03
#define WRITE 0x04
