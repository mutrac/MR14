//Code to read data from Parallax RFID reader/writer 28440 via Arduino
//Program reads data from the old EM4100-based tags and prints their value in the serial monitor.

//Writen by vgrhcp, uberdude, sebflippers and sixeyes

#include <SoftwareSerial.h>
#define txPin 6
#define rxPin 8
#define RFID_LEGACY 0x0F


SoftwareSerial mySerial(rxPin, txPin);
int  val = 0; 
char code[11];     //Note this is 11 for the extra null char?
int bytesread = 0;

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(txPin, OUTPUT);     //pin 6
  pinMode(rxPin, INPUT);      //pin 8

  Serial.println("RFID Read/Write Test");
}

void loop()
{
  mySerial.print("!RW");
  mySerial.write(byte(RFID_LEGACY));

  //mySerial.print(32, BYTE);

  if(mySerial.available() > 0) {          // if data available from reader 

    if((val = mySerial.read()) == 10) {   // check for header 
      bytesread = 0; 
      while(bytesread<10) {              // read 10 digit code 
        if( mySerial.available() > 0) { 
          val = mySerial.read(); 
          if((val == 10)||(val == 13)) { // if header or stop bytes before the 10 digit reading 
            break;                       // stop reading 
          } 
          code[bytesread] = val;         // add the digit           
          bytesread++;                   // ready to read next digit  
        } 
      } 

      if(bytesread == 10) {              // if 10 digit read is complete 
        Serial.print("TAG code is: ");   // possibly a good TAG 
        Serial.println(code);            // print the TAG code 
      } 
      bytesread = 0;

      delay(500);                       // wait for a 1/2 second 
    } 
  } 
}

