#include "mario.h"
#define melodyPin 3

void setup() {
  // put your setup code here, to run once:
  pinMode(melodyPin, OUTPUT);//buzzer

}

void loop() {
  // put your main code here, to run repeatedly:
    play_melody();

}
