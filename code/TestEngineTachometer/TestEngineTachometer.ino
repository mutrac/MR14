/*
  Test Wheel Tachometer
  Trevor Stanhope

*/
/* --- Libraries --- */
/* --- Declarations --- */
#define SAMPLES 4096
#define SENSOR_ENGINE_PIN 4

volatile unsigned int SENSOR_ENGINE = 0;
volatile unsigned int RATE_ENGINE = 0;
volatile unsigned int INTERVAL = 0;

void setup() {
  Serial.begin(9600);
  attachInterrupt(2, engine, RISING); // Pin 6
}

void loop() {
  SENSOR_ENGINE = 0;
  INTERVAL = 0;
  delay(1000); 
  INTERVAL += 1000;
  RATE_ENGINE = (SENSOR_ENGINE * 60000) / INTERVAL; // converts pulses to RPM (60000 millisec/min)
  Serial.println("--------------");
  Serial.print("PULSES: ");
  Serial.println(SENSOR_ENGINE);
  Serial.print("RPM: ");
  Serial.println(RATE_ENGINE);
}

/* --- Wheel --- */
void engine() {
  SENSOR_ENGINE++;
}
