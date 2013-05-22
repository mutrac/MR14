// testFuelSensor.ino
// Author: Trevor Stanhope
// Updated: 2013-04-13
// Test FS-3400AH Fuel Flow Sensor

/* --- Headers --- */
#define BAUD 9600
#define FUEL_PIN 3

/* --- Declarations --- */
volatile int NbTopsFan; //measuring the rising edges of the signal
int Calc;                               

/* --- Setup --- */
void setup() { 
  pinMode(FUEL_PIN, INPUT); //initializes digital pin 2 as an input
  Serial.begin(BAUD); //This is the setup function where the serial port is initialised,
  attachInterrupt(1, rpm, RISING); //and the interrupt is attached
} 

/* --- Loop --- */
void loop () {
  NbTopsFan = 0; //Set NbTops to 0 ready for calculations
  interrupts(); // Enables interrupts
  delay(1000); // Wait 1 second
  noInterrupts();
  Calc = (NbTopsFan * 60 / 7.5); //(Pulse frequency x 60) / 7.5Q, = flow rate in L/hour 
  Serial.print (Calc, DEC); //Prints the number calculated above
  Serial.print ("L/hour\r\n"); //Prints "L/hour" and returns a  new line
}

/* --- RPM --- */
void rpm () {    //This is the function that the interupt calls  
  NbTopsFan++;  //This function measures the rising and falling edge of the hall effect sensors signal
} 

