#include "rgb_lcd.h"

const int ENCODER1_PIN = 2;
const int ENCODER2_PIN = 3;
const int SERVO = 6;

rgb_lcd lcd;
volatile int position = 0;

void rodou() {
  while (!digitalRead(ENCODER2_PIN));
  if (digitalRead(ENCODER1_PIN) == HIGH) {
    if (position < 20) {
      position++;
    }
  } else {
    if (position > 0 ) {
      position--;
    }
  }
}

void setup() {
  Serial.begin(9600);
  // LCD
  lcd.begin(16, 2);
  lcd.setRGB(0, 255, 0);
  lcd.clear();
  char str[80];
  sprintf(str, "Position: %d", position);
  lcd.print(str);

  // Encoder
  pinMode(ENCODER1_PIN, INPUT);
  pinMode(ENCODER2_PIN, INPUT);

  // Servo
  pinMode(SERVO, OUTPUT);

  // Sets interruption for motion detector
  attachInterrupt(ENCODER2_PIN, rodou, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  float newPos = position * 9;
  servo(newPos * 11);
  printLcd();
}

// prints on LCD
void printLcd() {
  lcd.setRGB(0, 255, 0);
  lcd.clear();
  char str[80];
  sprintf(str, "Position: %d", position);
  lcd.print(str);
  Serial.println(position);
  Serial.println(position * 9);
  //delay(10);
}

// Rotate servo
void servo(float ms) {
  digitalWrite(SERVO, HIGH);
  delayMicroseconds(ms + 500);
  digitalWrite(SERVO, LOW);
  delayMicroseconds(20000 - ms - 500);
}
