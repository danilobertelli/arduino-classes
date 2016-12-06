#include <Arduino.h>
#include <Wire.h>

#define DEVICE_ID 8

volatile bool send = false;

// Button interrupt pin
const byte interruptPin = 2;

void sendMessage();

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Wire.begin(); // join i2c bus (address optional for master)

    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(interruptPin), sendMessage, FALLING);
}
byte x = 0;
void loop() {
    if (send) {
        send = false;
        Wire.beginTransmission(DEVICE_ID); // transmit to device #8
        Wire.write("Eat this bytes ");
        Wire.write(x);
        Wire.endTransmission();    // stop transmitting

        x++;
    }
}

void sendMessage() {
    Serial.println("send message");
    send = true;
}
