#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
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

long get_millis() {
    // First, we need to count how much interruptions we had and multiply it by 16320us
    // after it, get current Timer2 register value and multiply it by 64us (time between each CPU
    // tick)
    long micros = (interruptions * 16320) + (TCNT2 * 64);
    return micros / 1000;
}

// It waits doing nothing for delayInMs time
void delay(long delayInMs) {
    long currentTime = get_millis();
    while (get_millis() - currentTime <= delayInMs) {
        // Happily do nothings
    }
}

void sleep_cpu() {
    SMCR |= (1<<SE);
    delay(100); // delay time before sleep so we can flush uart buffer
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
void increment_sleep_time() {
    if (sleepTime > 0) {
        long totalTimeSleeping = get_millis() - sleepTime;
        totalSleepTime = totalTimeSleeping + totalSleepTime;
    }
    sleepTime = 0; // reset value to wait next bed time
}

int main (void) {
    // Initialize UART
    uart_init();
    stdout = &uart_output;
    stdin  = &uart_input;

    // Configure button interruptions
    DDRD  &= ~(1 << DDD2);    // PD2 (PCINT0 pin) is now an input
    PORTD |=  (1 << PORTD2);  // PD2 is now with pull-up enabled
    EICRA &= ~(1 << ISC00);   // set INT0 to trigger on
    EICRA |=  (1 << ISC01);   // FALLING edge (ISC0 = 01)
    EIMSK |=  (1 << INT0);    // Allow INT0 interrupts

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

    printf("Here we go!\n");
    while(1) {
        printf("main() \n");
        set_sleep_mode(POWER_SAVE_MODE);
        sleep_cpu();
    }
}

// Handle button interruption
ISR (INT0_vect) {
    increment_sleep_time(); // as the interruption woke up the CPU, get time sleeping

    if (bounceTime == 0) {
        bounceTime = get_millis();
        printf("Time since system is booted: %ld \n", bounceTime);
        // The active time can be calculated by the difference between total time and sleep time.
        long activeTime = bounceTime - totalSleepTime;
        printf("Total active time: %ld \n", activeTime);
    }
}

ISR(TIMER2_OVF_vect) {
    increment_sleep_time(); // as the interruption woke up the CPU, get time sleeping

    // We reach the overflow each 16320us based on Timer2 configuration
    interruptions++;

    // handle bounce control
    if (bounceTime > 0 && get_millis() - bounceTime > 200) {
        bounceTime = 0;
    }
}
