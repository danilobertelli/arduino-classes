#include <Arduino.h>

void setup() {
    //Definindo o pino buzzer como de saída.
    pinMode(8,OUTPUT);
}

void loop() {
    //Ligando o buzzer com uma frequencia de 1500 hz.
    //tone(8,1500);
    //delay(500);

    tone(8,32);
    delay(500);

    //tone(8,9000);
    //delay(500);

    //Desligando o buzzer.
    noTone(8);
    delay(500);
}
