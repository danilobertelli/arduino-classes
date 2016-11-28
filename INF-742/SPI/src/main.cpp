#include <Arduino.h>
#include <SPI.h>

// Button interrupt pin
const byte interruptPin = 2;

void blink();

// the setup function runs once when you press reset or power the board
void setup() {
    digitalWrite(SS, HIGH);  // Ensure SS stays high for now
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV16);
    Serial.begin(9600);

    attachInterrupt(digitalPinToInterrupt(interruptPin), blink, FALLING);
}

// the loop function runs over and over again forever
void loop() {
    // Do nothing
}

void blink() {
    // enable Slave Select
    digitalWrite(SS, LOW);

    // Send message
    SPI.transfer('x');
    Serial.println("Sending..");

    // Disable slave select
    digitalWrite(SS, HIGH);
}
