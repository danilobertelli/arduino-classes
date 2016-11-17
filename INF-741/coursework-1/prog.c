/*
    Danilo Arthur Bertelli
    Reaction Game using Timer2 (8-bits)
    We can count up to 255 so if we preload the counter with 130, this leaves 125 cycles left to count.
    To reach the 255 value at the Arduino 16Mhz clock speed, gives us 1ms each overflow.
*/

#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include "simple_usart/uart.h"

// Just enable more logs
const bool DEBUG = false;

// Led state treated as int
const int LED_OFF = 0x00;
const int LED_ON = 0xFF;

volatile unsigned long count = 0; // stores ms from system start
volatile bool reseted = true; // system state

volatile int generatedRand = 0;
volatile unsigned long bounceTime = 0;
volatile unsigned long targetTime = 0;
volatile unsigned long ledTime = 0;

// Enable timer interruptions
void enableTimer() {
    // The Arduino has a clock of 16MHz so: (1/16E6) * 125 * 128 = 1ms
    TCCR2B = 0x00;        //Disable Timer2 while we set it up
    TCNT2  = 130;         //Reset Timer Count to 130 out of 255
    TIFR2  = 0x00;        //Timer2 INT Flag Reg: Clear Timer Overflow Flag
    TIMSK2 = 0x01;        //Timer2 INT Reg: Timer2 Overflow Interrupt Enable
    TCCR2A = 0x00;
    TCCR2B = 0x05;        // Preescaler
    count  = 0;
}

void disableTimer() {
    TCCR2B = 0x00;        //Disbale Timer2 
    TCNT2  = 130;         //Reset Timer Count to 130 out of 255
    TIFR2  = 0x00;        //Timer2 INT Flag Reg: Clear Timer Overflow Flag
    count = 0;
}

// return milliseconds from timer
unsigned long get_millis() {
	return count;
}

// return the microseconds from timer
unsigned long get_micros() {
    return get_millis() * 1000 + TCNT2 * 8; // our TCNT2 is laoaded an need 125 cycles, so it's 1/8 from 10000
}

/*
    This return a number between 1 and 3
*/
unsigned int get_random() {
    return rand() % 3 + 1;
}

// Change LED status
void turnLedStatus(int state) {
    PORTB = state;
    if (PORTB == LED_ON) {
        ledTime = get_millis();
    } else {
        ledTime = 0;
    }
}

int main (void) {
    // Initialize UART
    uart_init();
    stdout = &uart_output;
    stdin  = &uart_input;

    // Print welcome message!
    printf("Press button to start\n");

    // Configure button interruptions
    DDRD  &= ~(1 << DDD2);    // PD2 (PCINT0 pin) is now an input
    PORTD |=  (1 << PORTD2);  // PD2 is now with pull-up enabled
    EICRA &= ~(1 << ISC00);   // set INT0 to trigger on
    EICRA |=  (1 << ISC01);   // FALLING edge (ISC0 = 01)
    EIMSK |=  (1 << INT0);    // Allow INT0 interrupts

    // LED
    DDRB = 0xFF;       // Configure PORTB as output
    PORTB = 0x00;      // Set output pins as LOW (LED OFF)

    // Enable timer interruption
    enableTimer();

    sei();   // Enable Global Interrupts

    while(1) {
        // SHOULD WE KEEP THIS HERE OR CAN WE CONTINUE TO HANDLE IN THE TIMER INTERRUPTION?
        
        // unsigned long time = get_millis();
        // //handle bounce control
        // if (bounceTime > 0 && time - bounceTime > 400) {
            // if (DEBUG) {
                // printf("reseting bounce\n");
            // }
            // bounceTime = 0;
        // }
        
        // // handle led turn on timeout
        // if(!reseted && PORTB == LED_OFF && time >= targetTime) {
            // if (DEBUG) {
                // printf("Target time achieved, turn led on\n");
            // }
            // // turn on the LED
            // bounceTime = 0; // ensure that we can press
            // generatedRand = 0;
            // turnLedStatus(LED_ON);
            // // Set timeout target time in case of user is dead
            // targetTime = get_millis() + 5000; // actual time + 5 seconds
        // }

        // // handle timeout after pressed (5 seconds)
        // if(!reseted && PORTB == LED_ON && time >= targetTime) {
            // printf("Game Over! Time out\n\nPress a button to start");
            // reseted = true;
            // turnLedStatus(LED_OFF);
            // bounceTime = 0;
            // targetTime = 0;
            // count = 0;  //Resets the interrupt counter
        // }
        
    }
}

// Handle button interruption
ISR (INT0_vect) {
    if (DEBUG) {
        printf("button pressed %lu \n", bounceTime);
    }

    if (bounceTime == 0) {
        bounceTime = get_millis();
        if (DEBUG) {
             printf("button pressed in millis %lu \n", bounceTime);
        }

        if (reseted) {
            reseted = false; // system is running

            // Get RANDOM number
            targetTime = get_millis() + (get_random() * 1000);
            if (DEBUG) {
                printf("targetTime adquired: %lu \n", targetTime);
            }
        } else {
            if (DEBUG) {
                printf("PUSHED LED IS: %d \n", PORTB);
            }
            if (PORTB == LED_ON) {
                unsigned long userTime = get_millis();
                printf("Congrats, reaction time = %lu ms, new round! \n", userTime - ledTime);

                turnLedStatus(LED_OFF); // turn off led

                // get a new target time
                targetTime = get_millis() + (get_random() * 1000);
                if (DEBUG) {
                    printf("targetTime adquired: %lu \n", targetTime);
                }
            } else {
                printf("Beeeh, wrong time, game Over!\n\n");
                reseted = true;
                turnLedStatus(LED_OFF);
                bounceTime = 0;
                targetTime = 0;
                count = 0;  //Resets the interrupt counter

                printf("Press button to start\n");
            }
        }
    }
}

//Timer2 Overflow Interrupt Vector, called every 1ms
ISR(TIMER2_OVF_vect) {
    count++; //Increments the interrupt counter

    //handle bounce control
    if (bounceTime > 0 && count - bounceTime > 200) {
        if (DEBUG) {
            printf("reseting bounce\n");
        }
        bounceTime = 0;
    }
        
    // handle led turn on timeout
    if(!reseted && PORTB == LED_OFF && count >= targetTime) {
        if (DEBUG) {
            printf("Target time achieved, turn led on\n");
        }
        // turn on the LED
        bounceTime = 0; // ensure that we can press
        generatedRand = 0;
        turnLedStatus(LED_ON);
        // Set timeout target time in case of user is dead
        targetTime = get_millis() + 5000; // actual time + 5 seconds
    }

    // handle timeout after pressed (5 seconds)
    if(!reseted && PORTB == LED_ON && count >= targetTime) {
        printf("Game Over! Time out\n\nPress a button to start");
        reseted = true;
        turnLedStatus(LED_OFF);
        bounceTime = 0;
        targetTime = 0;
        count = 0;  //Resets the interrupt counter
    }

    TCNT2 = 130;           //Reset Timer
    TIFR2 = 0x00;          //Timer2 INT Flag Reg: Clear Timer Overflow Flag so we can continue at same speed
}; 