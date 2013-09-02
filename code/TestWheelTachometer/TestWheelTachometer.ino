/*
  Test Wheel Tachometer
  Trevor Stanhope

*/
/* --- Libraries --- */
/* --- Declarations --- */
#define SAMPLES 4096
#define SENSOR_WHEEL_PIN 3

volatile unsigned int SENSOR_WHEEL = 0;
volatile unsigned int RATE_WHEEL = 0;
volatile unsigned int INTERVAL = 0;

void setup() {
  Serial.begin(9600);
  attachInterrupt(1, wheel, CHANGE); // Pin 6
}

void loop() {
  SENSOR_WHEEL = 0;
  INTERVAL = 0;
  delay(1000); 
  INTERVAL += 1000;
  RATE_WHEEL = (SENSOR_WHEEL * 15000) / INTERVAL; // converts pulses to RPM (60000 millisec/min)
  Serial.println(SENSOR_WHEEL);
  Serial.println(RATE_WHEEL);
}

/* --- Wheel --- */
void wheel() {
  SENSOR_WHEEL++;
}
