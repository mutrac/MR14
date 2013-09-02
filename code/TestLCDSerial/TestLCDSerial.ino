#include <SoftwareSerial.h>

void setup() {
  Serial3.begin(9600);
  backlightOn();
  clear();
}

void loop() {
  Elow(); delay(63);
  Elow(); delay(63);
  Elow(); delay(250);
  Clow(); delay(125);
  Elow(); delay(125);
  Ghigh(); delay(250);
  Glow(); delay(250);
  delay(2000);
}

/*********** LCD ***********/
void backlightOn() {
  Serial3.write(17);
}

void newline() {
  Serial3.write(13);
}

void clear() {
  Serial3.write(12);
}

void backlightOff() {
  Serial3.write(18);
}

void displayOff() {
  Serial3.write(21);
}

void displayOn() {
  Serial3.write(22);
}

void Alow() {
  Serial3.write(219);
}

void Blow() {
  Serial3.write(220);
}

void Clow() {
  Serial3.write(221);
}

void Dlow() {
  Serial3.write(222);
}

void Elow() {
  Serial3.write(223);
}

void Flow() {
  Serial3.write(224);
}

void Glow() {
  Serial3.write(225);
}

void Ahigh() {
  Serial3.write(226);
}

void Bhigh() {
  Serial3.write(227);
}

void Chigh() {
  Serial3.write(228);
}

void Dhigh() {
  Serial3.write(229);
}

void Ehigh() {
  Serial3.write(230);
}

void Fhigh() {
  Serial3.write(231);
}

void Ghigh() {
  Serial3.write(232);
}

void cursorOff() {
  Serial3.write(22);
}

void cursorOn() {
  Serial3.write(23);
}

