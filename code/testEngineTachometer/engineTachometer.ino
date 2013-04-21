/*
  engineTachometer.ino
  Trevor Stanhope
  Jan 9, 2013
  Measures frequency of engine.
  http://tushev.org/articles/electronics/43-measuring-frequency-with-arduino
*/
/* --- Libraries --- */
/* --- Declarations --- */
#define SAMPLES 4096

long getFrequency(int pin) {
  long freq = 0;
  for (unsigned int j=0; j<SAMPLES; j++) {
    freq += 500000/pulseIn(pin, HIGH, 250000);
  }
  return (freq / SAMPLES);
}
