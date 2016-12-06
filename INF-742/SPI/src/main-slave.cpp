#include <SPI.h>
#include <Arduino.h>

volatile boolean ledOn = true;

void setup (void)
{
  Serial.begin (9600);

  pinMode(MISO, OUTPUT);
  pinMode(8, OUTPUT);

  SPCR |= _BV(SPE);

  Serial.print("Setup");
  SPI.attachInterrupt();
}


ISR (SPI_STC_vect) {
  if (ledOn){
    tone(8, 31, 200);
  } else {
    tone(8, 31, 200);
  }
  ledOn = !ledOn;
}

void loop (void)
{

}
