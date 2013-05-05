/*
  Test Quadrature Encoder with Interrupt
  Author: Trevor Stanhope
  Reference: Max Wolf
  Date: 2013-05-05
  
  uses Arduino pullups on A & B channel outputs
  turning on the pullups saves having to hook up resistors 
  to the A & B channel outputs
  
  Schematic:
    COMMON --> GROUND
    A --> ENCODER_A_PIN
    B --> ENCODER_B_PIN
*/ 

/* --- Libraries --- 
/* --- Declarations --- */
#define ENCODER_A_PIN  2
#define ENCODER_B_PIN  4
#define BAUD 9600
volatile int POSITION = 0;

/* --- Setup --- */
void setup() { 
  
  // Initialize I/O Pins
  pinMode(ENCODER_A_PIN, INPUT); 
  pinMode(ENCODER_B_PIN, INPUT); 
  digitalWrite(ENCODER_A_PIN, HIGH); // turn on pullup resistor
  digitalWrite(ENCODER_B_PIN, HIGH); // turn on pullup resistor

  // Initialize serials
  attachInterrupt(0, doEncoder, CHANGE); // encoder pin on interrupt 0 - pin 2
  Serial.begin (BAUD);
  Serial.println("start"); // a personal quirk
} 

/* --- Loop --- */
void loop(){
  // do some stuff here - the joy of interrupts is that they take care of themselves
}

/* --- doEncoder --- */
void doEncoder() {
/*
  If pinA and pinB are both high or both low, it is spinning
  forward. If they're different, it's going backward.
  For more information on speeding up this process, see
  [Reference/PortManipulation], specifically the PIND register.
*/
  
  if (digitalRead(ENCODER_A_PIN) == digitalRead(ENCODER_B_PIN)) {
    POSITION++;
  }
  else {
    POSITION--;
  }

  Serial.println (POSITION, DEC);
}



/*
  See this expanded function to get a better understanding of the
  meanings of the four possible (pinA, pinB) value pairs:
*/

/* --- doEncoder Expanded --- */
void doEncoder_Expanded() {
/*
  See this expanded function to get a better understanding of the
  meanings of the four possible (pinA, pinB) value pairs:
*/

  if (digitalRead(ENCODER_A_PIN) == HIGH) {   // found a low-to-high on channel A
    if (digitalRead(ENCODER_B_PIN) == LOW) {  // check channel B to see which way encoder is turning
      POSITION--; // CCW
    } 
    else {
      POSITION++; // CW
    }
  }
  
  else {                                       // found a high-to-low on channel A 
    if (digitalRead(ENCODER_B_PIN) == LOW) {   // check channel B to see which way encoder is turning  
      POSITION++; // CW
    } 
    else {
      POSITION--; // CCW
    }
  }
  
  
  Serial.println (POSITION, DEC);          // debug - remember to comment out
                                              // before final program run
  // you don't want serial slowing down your program if not needed
}

/*
  to read the other two transitions - just use another attachInterrupt()
  in the setup and duplicate the doEncoder function into say, 
  doEncoderA and doEncoderB. 
  You also need to move the other encoder wire over to pin 3 (interrupt 1). 
*/ 
