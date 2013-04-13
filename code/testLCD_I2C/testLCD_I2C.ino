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
LiquidCrystal_I2C lcd(I2C_PORT,WIDTH,HEIGHT);  

void setup() {
  Serial.begin(9600);
  lcd.init(); // initialize the lcd 
  lcd.backlight();  // Backlight ON if under program control
  lcd.print("Begin testing:");
  delay(1000);   
  lcd.clear();
  delay(1000);
}

void loop() {
  for (int i = 0; i < 4; i++) {
    lcd.setCursor(0,i); // print on each line
    lcd.print("Test");
    delay(1000);
    lcd.clear();
    delay(1000);
  }
}
