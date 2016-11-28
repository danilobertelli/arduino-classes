#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <uart.h>

#define IDLE_MODE 0
#define POWER_DOWN_MODE 1
#define POWER_SAVE_MODE 2
#define STANDBY_MODE 3
#define EXT_STANDBY_MODE 4

volatile long interruptions = 0;  // Controls how much interruptions has fired
volatile long bounceTime = 0;     // debounce control
volatile long sleepTime = 0;      // Time before sleeping
volatile long totalSleepTime = 0; // Total Time sleeping
volatile long delayTime = 0;
volatile bool shouldSleep = true;

long get_millis() {
    // First, we need to count how much interruptions we had and multiply it by 16384us
    // after it, get current Timer2 register value and multiply it by 64us (time between each CPU
    // tick)
    long micros = (interruptions * 16384) + (TCNT2 * 64);
    return micros / 1000;
}

void sleep_cpu() {
    SMCR |= (1<<SE);
    sleepTime = get_millis(); // Get current time before goes to sleep
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

// It increments the total time that CPU was sleeping
void increment_sleep_time(long timeInMs) {
    if (sleepTime > 0) {
        long totalTimeSleeping = timeInMs - sleepTime;
        totalSleepTime = totalTimeSleeping + totalSleepTime;
    }
    sleepTime = 0; // reset value to wait next bed time
}

int main (void) {
    // Initialize UART
    uart_init();
    stdout = &uart_output;
    stdin  = &uart_input;

    // Configure button interruptions 2
    DDRD  &= ~(1 << DDD2);    // PD2 (PCINT0 pin) is now an input
    PORTD |=  (1 << PORTD2);  // PD2 is now with pull-up enabled
    EICRA |= (1 << ISC00);
    EIMSK |=  (1 << INT0);    // Allow INT0 interrupts

    // Configure button interruptions 3
    DDRD  &= ~(1 << DDD3);    // PD3 (PCINT0 pin) is now an input
    PORTD |=  (1 << PORTD3);  // PD3 is now with pull-up enabled
    EICRA |= (1 << ISC10);
    EIMSK |=  (1 << INT1);    // Allow INT0 interrupts

    // Don't know why but the led is starting ON so turn it OFF as a workaround
    DDRB = 0xFF;       // Configure PORTB as output
    PORTB = 0x00;      // Set output pins as LOW (LED OFF)

    // Initialize Timer1
    TCCR2B = 0x00;                               // Disable Timer2 while we set it up
    TCNT2  = 0;                                  // Reset Timer Count to 0 out of 255
    TIFR2  = 0x00;                               // Timer2 INT Flag Reg: Clear Timer Overflow Flag
    TIMSK2 = 0x01;                               // Timer2 INT Reg: Timer2 Overflow Interrupt Enable
    TCCR2A = 0x00;
    TCCR2B = (1<<CS22) | (1<<CS21) | (1<<CS20);  // Preescaler 1024 (or 0x07)

    sei(); // Enable Global Interrupts

    while(1) {
        if (shouldSleep) {
            // set_sleep_mode(POWER_SAVE_MODE);
            // sleep_cpu();
        }
    }
}

// Handle button interruption
ISR (INT0_vect) {
    printf("INT0:");
}

ISR (INT1_vect) {
    printf("INT1:");
}

ISR(TIMER2_OVF_vect) {
    // We reach the overflow each 16320us based on Timer2 configuration
    interruptions++;

    long timeNow = ((interruptions * 16384) + (TCNT2 * 64)) / 1000;
    increment_sleep_time(timeNow); // as the interruption woke up the CPU, get time sleeping

    // handle bounce control
    if (bounceTime > 0 && timeNow - bounceTime > 100) {
        bounceTime = 0;
    }

    // handle delay to sleep after print
    if (delayTime > 0 && timeNow - delayTime > 30) {
        delayTime = 0;
        shouldSleep = true;
    }
}
