// readRFID.ino
// Trevor Stanhope
// December 20th, 2012
// Reads 13.56 MHz RFID card tag.

/* Headers */
#include <SoftwareSerial.h>
#define RFID_READ 0x01
#define txPin 6
#define rxPin 8

/* Globals */
SoftwareSerial mySerial(rxPin, txPin);
int val;
int runs = 0;

/* Functions */
void setup() {
    Serial.begin(9600);
    Serial.println("RFID Read/Write Test");
    mySerial.begin(9600);
    pinMode(txPin, OUTPUT);    
    pinMode(rxPin, INPUT);      
}

// suppresses the "null result" from being printed if no RFID tag is present
void suppressAll() {                               
    if(mySerial.available() > 0) { 
        mySerial.read();
        suppressAll();
    }
}

void loop() {
    int val;
    
    mySerial.print("!RW")
    mySerial.write(byte(RFID_READ));
    mySerial.write(byte(32));

    /* Conditions */
    if (mySerial.available() > 0) {      
        val = mySerial.read();                        
        if (val != 1) {suppressAll();} // if error, suppress it.                       
    }      
    if (mySerial.available() > 0) {      
        val = mySerial.read();
        Serial.print("1st:");
        Serial.println(val, HEX);
    }
    if (mySerial.available() > 0) {        
        val = mySerial.read();
        Serial.print("2nd:");
        Serial.println(val, HEX);
    }
    if (mySerial.available() > 0) {      
        val = mySerial.read();
        Serial.print("3rd:");
        Serial.println(val, HEX);
    }
    if (mySerial.available() > 0) {          
        val = mySerial.read();
        Serial.print("4th:");
        Serial.println(val, HEX);
        
    }
    
    Serial.println("-----------------");
    delay(750);
}
