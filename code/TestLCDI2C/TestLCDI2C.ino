// testLCD_I2C.ino
// Author: Trevor Stanhope
// Updated: 2013-04-12
// Display Text on 20x4 I2C LCD

/* Schematic */
// SDA to SDA
// SDL to SDL
// 5V to 5V
// GND to GND

/* Headers */
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define I2C_PORT 0x3F
#define WIDTH 20
#define HEIGHT 4

/* Declarations */
LiquidCrystal_I2C lcd(I2C_PORT,16,2);  

void setup() {
  Serial.begin(9600);
  lcd.init(); // initialize the lcd 
  lcd.blink();
  lcd.noDisplay();
  lcd.display();
  lcd.backlight();  // Backlight ON if under program control
  lcd.print("Begin testing:");
  delay(1000);   
  lcd.clear();
  delay(1000);
}

void loop() {
  lcd.init();
  lcd.print("TEST");
  delay(500);
  lcd.clear();
}
