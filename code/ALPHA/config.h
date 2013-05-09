/* CONFIG FILE FOR MR14 */
/* PINS */
// USER INPUT (0 to 7)
#define rfidTXPin 2 
#define rfidRXPin 3   
#define startButtonPin 5 // pin for the Start Engine Button
#define killSwitchPin 4

// TRACTOR CONTROL (14 to 21)
#define standbyRelayPin 14 // pin for the STANDBY mode
#define startRelayPin 15 // pin for relay to engine start

// DISPLAY (8 to 13)
#define lcd14Pin 8
#define lcd13Pin 9
#define lcd12Pin 10
#define lcd11Pin 11
#define lcdEPin 12
#define lcdRSPin 13

// DAQ (22 to 53)
#define fuelGaugePin 22

/* VALUES */
// CONSTANTS
#define KEYLENGTH 4
#define BAUD 9600
#define INTERVAL 1000
#define WIDTH 16
#define HEIGHT 2

// RFID COMMANDS
#define CARD 0x02
#define READ 0x03
#define WRITE 0x04
