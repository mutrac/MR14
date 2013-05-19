#define RELAY_1_PIN 39 // pin to disable ENGINE STOP
#define RELAY_2_PIN 45 // pin to enable REGULATION AND FUEL SOLENOID
#define RELAY_3_PIN 43 // pin to enable STARTER

void setup() {
  // Initialize relays in OFF state.
  pinMode(RELAY_1_PIN, OUTPUT);
  pinMode(RELAY_2_PIN, OUTPUT);
  pinMode(RELAY_3_PIN, OUTPUT);
  digitalWrite(RELAY_1_PIN, HIGH);
  digitalWrite(RELAY_2_PIN, HIGH);
  digitalWrite(RELAY_3_PIN, HIGH);
}

void loop() {
  
  // Relay 1
  digitalWrite(RELAY_1_PIN, LOW);
  digitalWrite(RELAY_2_PIN, HIGH);
  digitalWrite(RELAY_3_PIN, HIGH);
  
  // Relay 2
  digitalWrite(RELAY_1_PIN, HIGH);
  digitalWrite(RELAY_2_PIN, LOW);
  digitalWrite(RELAY_3_PIN, HIGH);
  
  // Relay 3
  digitalWrite(RELAY_1_PIN, HIGH);
  digitalWrite(RELAY_2_PIN, HIGH);
  digitalWrite(RELAY_3_PIN, LOW);
  
  // Position 1
  digitalWrite(RELAY_1_PIN, HIGH);
  digitalWrite(RELAY_2_PIN, HIGH);
  digitalWrite(RELAY_3_PIN, HIGH);
  
  // Position 2a
  digitalWrite(RELAY_1_PIN, LOW);
  digitalWrite(RELAY_2_PIN, HIGH);
  digitalWrite(RELAY_3_PIN, HIGH);
  delay(5000);
  
  // Position 2b
  digitalWrite(RELAY_1_PIN, LOW);
  digitalWrite(RELAY_2_PIN, LOW);
  digitalWrite(RELAY_3_PIN, HIGH);
  delay(5000);
  
  // Position 3
  digitalWrite(RELAY_1_PIN, LOW);
  digitalWrite(RELAY_2_PIN, LOW);
  digitalWrite(RELAY_3_PIN, LOW);
  delay(5000);
}
