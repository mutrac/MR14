/*
  TestFuelSensor
  Author: Trevor Stanhope
  Updated: 2013-04-13
  Tests the FS-3400AH Fuel Flow Sensor
*/

/* --- Headers --- */
#define BAUD 9600
#define SENSOR_FUEL_PIN 3

/* --- Declarations --- */
volatile int SENSOR_FUEL; //measuring the rising edges of the signal
volatile int INTERVAL;
int RATE_FUEL;

/* --- Setup --- */
void setup() { 
  pinMode(SENSOR_FUEL_PIN, INPUT); //initializes digital pin 2 as an input
  Serial.begin(BAUD); //This is the setup function where the serial port is initialised,
  attachInterrupt(1, fuel, RISING); //and the interrupt is attached
} 

/* --- Loop --- */
void loop () {
  SENSOR_FUEL = 0; //Set NbTops to 0 ready for calculations
  INTERVAL = 0;
  delay(1000);
  INTERVAL += 1000; // Wait 1 second
  RATE_FUEL = (SENSOR_FUEL * 1800) / INTERVAL;
  Serial.print(RATE_FUEL, DEC); //P rints the number calculated above
  Serial.print("L/hour\r\n"); // Prints "L/hour" and returns a  new line
}

/* --- Fuel --- */
void fuel() {    //This is the function that the interupt calls  
  SENSOR_FUEL++;  //This function measures the rising and falling edge of the hall effect sensors signal
} 

