/*
  TestSwitches
*/

//  KILL, TRIGGER AND LOCK SWITCHES
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

// 
int KILL_HITCH = 0;
int KILL_BUTTON = 0;
int KILL_SEAT = 0;
int LOCK_LEFTBRAKE = 0;
int LOCK_RIGHTBRAKE = 0;
int LIMIT_FAR = 0;
int LIMIT_NEAR = 0;
int IGNITION = 0;

void setup() {
  // Power Pins
  pinMode(KILL_BUTTON_POWER, OUTPUT);
  digitalWrite(KILL_BUTTON_POWER, LOW);
  pinMode(KILL_SEAT_POWER, OUTPUT);
  digitalWrite(KILL_SEAT_POWER, LOW);
  pinMode(KILL_HITCH_POWER, OUTPUT);
  digitalWrite(KILL_HITCH_POWER, LOW);
  pinMode(LOCK_LEFTBRAKE_POWER, OUTPUT);
  digitalWrite(LOCK_LEFTBRAKE_POWER, LOW);
  pinMode(LOCK_RIGHTBRAKE_POWER, OUTPUT);
  digitalWrite(LOCK_RIGHTBRAKE_POWER, LOW);
  pinMode(IGNITION_POWER, OUTPUT);
  digitalWrite(IGNITION_POWER, LOW);
  
  // Input Pins
  pinMode(KILL_BUTTON_PIN, INPUT);
  digitalWrite(KILL_BUTTON_PIN, HIGH);
  pinMode(KILL_SEAT_PIN, INPUT);
  digitalWrite(LOCK_LEFTBRAKE_PIN, HIGH);
  pinMode(LOCK_RIGHTBRAKE_PIN, INPUT);
  digitalWrite(LOCK_RIGHTBRAKE_PIN, HIGH);
  pinMode(IGNITION_PIN, INPUT);
  digitalWrite(IGNITION_PIN, HIGH);
  pinMode(LIMIT_NEAR_PIN, INPUT);
  digitalWrite(LIMIT_NEAR_PIN, HIGH);
  pinMode(LIMIT_FAR_PIN, INPUT);
  digitalWrite(LIMIT_FAR_PIN, HIGH);
  
  Serial.begin(9600);
}

void loop() {
  
  // Get Values
  KILL_HITCH = digitalRead(KILL_HITCH_PIN);
  KILL_BUTTON = digitalRead(KILL_BUTTON_PIN);
  KILL_SEAT = digitalRead(KILL_SEAT_PIN);
  LOCK_LEFTBRAKE = digitalRead(LOCK_LEFTBRAKE_PIN);
  LOCK_RIGHTBRAKE = digitalRead(LOCK_RIGHTBRAKE_PIN);
  LIMIT_FAR = digitalRead(LIMIT_NEAR_PIN);
  LIMIT_NEAR = digitalRead(LIMIT_NEAR_PIN);
  IGNITION = digitalRead(IGNITION_PIN);
  
  // Display values
  Serial.print("Hitch: ");
  Serial.println(KILL_HITCH);
  Serial.print("Seat: ");
  Serial.println(KILL_SEAT);
  Serial.print("Button: ");
  Serial.println(KILL_BUTTON);
  Serial.print("Left Brake: ");
  Serial.println(LOCK_LEFTBRAKE);
  Serial.print("Right Brake: ");
  Serial.println(LOCK_RIGHTBRAKE);
  Serial.print("Near Limit: ");
  Serial.println(LIMIT_NEAR);
  Serial.print("Far Limit: ");
  Serial.println(LIMIT_FAR);
  
  // Wait
  delay(1000);
}
