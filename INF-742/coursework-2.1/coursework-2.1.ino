#include "rgb_lcd.h"

const int BUZZER_PIN = 4;
const int MOTION_PIN = 7;
const int ENCODER1_PIN = 2;
const int ENCODER2_PIN = 3;

rgb_lcd lcd;
volatile boolean motion_detected = false;
volatile boolean buzzing = false;
volatile int volume = 0;

void motionListener() {
  int sensorValue = digitalRead(MOTION_PIN);
  if (sensorValue == HIGH) {
    motion_detected = true;
    return;
  }
  motion_detected = false;
}

void voluminho() {
  while (!digitalRead(ENCODER2_PIN));
  if (digitalRead(ENCODER1_PIN) == HIGH) {
    if (volume < 10) {
      volume++;
    }
  } else {
    if (volume > 0 ) {
      volume--;
    }
  }
}

void setup() {
  // Motion Sensor
  pinMode(MOTION_PIN, INPUT);

  // Buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  // LCD
  lcd.begin(16, 2);
  lcd.setRGB(0, 255, 0);
  lcd.clear();
  char str[80];
  sprintf(str, "SAFE: %d", volume);
  lcd.print(str);

  // Encoder
  pinMode(ENCODER1_PIN, INPUT);
  pinMode(ENCODER2_PIN, INPUT);

  // Sets interruption for motion detector
  attachInterrupt(MOTION_PIN, motionListener, CHANGE);
  attachInterrupt(ENCODER2_PIN, voluminho, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (motion_detected) {
    motionDetected();
  } else {
    clearMotionDetection();
  }
}

// "Listener" callback function
void motionDetected() {
  lcd.setRGB(255, 0, 0);
  lcd.clear();
  char str[80];
  sprintf(str, "INTRUDER: %d", volume);
  lcd.print(str);
  buzzIt(true);
  delay(volume * 100);
  buzzIt(false);
}

void clearMotionDetection() {
  lcd.setRGB(0, 255, 0);
  lcd.clear();
  char str[80];
  sprintf(str, "SAFE: %d", volume);
  lcd.print(str);
  buzzIt(false);
  delay(100);
}

// It buzz it; or stop it
void buzzIt(bool state) {
  buzzing = state;
  if (state) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
}

