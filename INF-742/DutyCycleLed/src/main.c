#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <uart.h>

#define IDLE_MODE 0
#define POWER_DOWN_MODE 1
#define POWER_SAVE_MODE 2
#define STANDBY_MODE 3
#define EXT_STANDBY_MODE 4

void sleep_cpu() {
    SMCR |= (1<<SE);
    asm("sleep");
}

void set_sleep_mode(char mode) {
    switch (mode) {
        case IDLE_MODE:
           SMCR &= ~(1 << SM0);
           SMCR &= ~(1 << SM1);
           SMCR &= ~(1 << SM2);
        break;
        case POWER_DOWN_MODE:
           SMCR &= ~(1 << SM0);
           SMCR |= (1<<SM1);
        break;
        case POWER_SAVE_MODE:
           SMCR |= (1<<SM1) | (1<<SM0);
        break;
        case STANDBY_MODE:
           SMCR &= ~(1 << SM0);
           SMCR |= (1<<SM2) | (1<<SM1);
        break;
        case EXT_STANDBY_MODE:
           SMCR |= (1<<SM2) | (1<<SM1) | (1<<SM0);
        break;
    }
}

void turnLedStatus() {
    PORTB = PORTB ^ 0xFF;
}

int main (void) {
    // Initialize UART
    uart_init();
    stdout = &uart_output;
    stdin  = &uart_input;

    // Initialize LED
    DDRB = 0xFF;       // Configure PORTB as output
    PORTB = 0x00;      // Set output pins as LOW (LED OFF)

    // Initialize WTD
    /* Clear the reset flag. */
    wdt_reset();
    WDTCSR |= (1<<WDCE) | (1<<WDE);
    WDTCSR = (1<<WDIE) | (0<<WDE) | (0<<WDP3) | (1<<WDP2) | (1<<WDP1) | (0<<WDP0);

    sei();   // Enable Global Interrupts

    printf("Here we go!\n");
    while(1) {
        printf("while!!!!\n");
        set_sleep_mode(POWER_SAVE_MODE);
        sleep_cpu();
    }
}

ISR(WDT_vect) {
    printf("Interrupted!\n");
    turnLedStatus();
}
