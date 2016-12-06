#include "rgb_lcd.h"
#include <math.h>

const int pinMoisture = A1;
const int pinLight = A2;
const int pinUV = A3;
const int pinButton = 0;
rgb_lcd lcd;

volatile int ptr = 0;

void buttonListener() {
  lcd.setRGB(255, 0, 0);
  ptr++;
}

void setup() {
  // LCD
  lcd.begin(16, 2);
  lcd.setRGB(127, 127, 127);
  lcd.clear();

  attachInterrupt(pinButton, buttonListener, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  switch (ptr) {
    case 0:
      printUV();
      break;
    case 1:
      printLight();
      break;
    case 2:
      printMoisture();
      break;
  }
  delay(1000);
}

void printUV() {
  char str[80];
  sprintf(str, "UV: %d", getUV());
  lcd.clear();
  lcd.print(str);
}

int getUV() {
  return 307 * analogRead(pinUV);
}

void printLight() {
  char str[80];
  sprintf(str, "Light: %d", getLight());
  lcd.clear();
  lcd.print(str);
}

int getLight() {
  int light = analogRead(pinLight);
  int R = ((1023 - light) * 10) / light;
  int lux = 350 * pow(R, -1.43);
  return lux;
}

void printMoisture() {
  char str[80];
  sprintf(str, "Moisture: %d", getMoisture());
  lcd.clear();
  lcd.print(str);
}

int getMoisture() {
  return analogRead(pinMoisture);
}

//is button pressed
int isButtonPressed() {
  int state = 0;
  if (digitalRead(pinButton)) {
    while (digitalRead(pinButton));
    state = 1;
  }
  return state;
}



