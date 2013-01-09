// startTone.ino
// Trevor Stanhope
// December 16th, 2012
// Tractor ON tone (pre-ignition only).

/* Dependent Libraries */
#include "pitches.h"

/* Global Objects */
// notes in the start melody and repeat tone
int melody[] = {NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};
int repeat[] = {NOTE_G5, NOTE_G5};

// note durations: 4 = quarter note, 8 = eighth note, etc...
int melodyDurations[] = {4,8,8,4,4,4,4,4 };
int repeatDurations[] = {4, 4};

/* Functions */
// setup/0; Plays start melody.
void setup() {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/melodyDurations[thisNote];
    tone(8, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
  delay(1000);
}

// loop/0; Plays repeat waiting tone.
void loop() {
  for (int thisNote = 0; thisNote < 2; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/repeatDurations[thisNote];
    tone(8, repeat[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
  }
  delay(1000);
}
