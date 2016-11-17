#include <avr/interrupt.h>
#include <avr/io.h>

volatile unsigned long count;
volatile bool isTone = false;

// Tone configuration
volatile char* ddrB = (char*) 0x24;
volatile char* portB = (char*) 0x25;

// Enable timer interruptions
void enableTimer() {
    // The Arduino has a clock of 16MHz so: (1/16E6) * 125 * 128 = 1ms
    TCCR2B = 0x00;        //Disable Timer2 while we set it up
    TCNT2  = 130;         //Reset Timer Count to 130 out of 255
    TIFR2  = 0x00;        //Timer2 INT Flag Reg: Clear Timer Overflow Flag
    TIMSK2 = 0x01;        //Timer2 INT Reg: Timer2 Overflow Interrupt Enable
    TCCR2A = 0x00;
    TCCR2B = 0x05;        // Preescaler
}

void disableTimer() {
    TCCR2B = 0x00;        //Disbale Timer2
    TCNT2  = 130;         //Reset Timer Count to 130 out of 255
    TIFR2  = 0x00;        //Timer2 INT Flag Reg: Clear Timer Overflow Flag
    count = 0;
}

void enableTone() {
    *portB |= (1 << 5);
}

void disableTone() {
    *portB &= ~(1 << 5);
}

void tone(long durationMs) {
    enableTone();
    enableTimer();
    count  = durationMs;
}

int main (void) {
    // Configure tone pins
    *ddrB |= (1 << 5); // setup 5 as output
    *ddrB &= ~(1); // sets as input
    *portB &= ~(1 << 5); // turn off tone

    sei();   // Enable Global Interrupts

    tone(5000);
    while(1) {
        // infinite loop
    }
}

//Timer2 Overflow Interrupt Vector, called every 1ms
ISR(TIMER2_OVF_vect) {
    count--;
    TCNT2 = 130;           //Reset Timer
    TIFR2 = 0x00;          //Timer2 INT Flag Reg: Clear Timer Overflow Flag so we can continue at same speed

    if (count <= 0) {
        disableTone();
        disableTimer();
    }
}
