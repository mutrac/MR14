// displayLCD.ino
// Trevor Stanhope
// Jan 29, 2013
// Display Text on 16x2 LCD

/* Schematic */
// LCD2 to 5V
// LCD1 to 3
// LCD4 to D12
// LCD5 to GND
// LCD6 to D11
// LCD11 to D5
// LCD12 to D4
// LCD13 to D3
// LCD14 to D4
// LCD15 to 5V
// LCD16 to GND

/* Headers */
#include <LiquidCrystal.h>

/* Globals */
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

/* Setup */
void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

/* Main */
void loop() {
  lcd.noDisplay(); // Turn off the display:
  delay(500);
  lcd.display(); // Turn on the display:
  delay(500);
}
