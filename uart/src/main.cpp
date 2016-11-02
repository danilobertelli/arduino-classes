#include <avr/io.h>
#include <avr/interrupt.h>
#include <CircularBuffer.h>

#define USART_BAUDRATE 9600 // Defines the baud rate
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1) // Calculate the prescale

volatile unsigned long timeInMillis = 0;
volatile char received_char = -1;
CircularBuffer<char, 10> buffer = CircularBuffer<char, 10>();

unsigned long getMillis() {
    return timeInMillis;
}

int main(void) {

    // Enable UART
    UCSR0B |=(1<<RXEN0)|(1<<TXEN0); // Enable RX and TX
    UCSR0C |=(1<<UCSZ00)|(1<<UCSZ01); // Use 8 bit char size
    UBRR0H = (BAUD_PRESCALE >>8); // Baud rate at high
    UBRR0L = BAUD_PRESCALE; // Baud rate at low
    UCSR0B |= (1 << RXEN0 ) | (1 << RXCIE0); // Enable interrupt

    // Enable timer2 interrupt
    // The Arduino has a clock of 16MHz so: (1/16E6) * 125 * 128 = 1ms
    TCCR2B = 0x00;        //Disable Timer2 while we set it up
    TCNT2  = 130;         //Reset Timer Count to 130 out of 255
    TIFR2  = 0x00;        //Timer2 INT Flag Reg: Clear Timer Overflow Flag
    TIMSK2 = 0x01;        //Timer2 INT Reg: Timer2 Overflow Interrupt Enable
    TCCR2A = 0x00;
    TCCR2B = 0x05;        // Preescaler

    sei();

    while(1) {
        //TODO Check why this is not working properly at ISR
        // It supposed to be on ISR so when called it push the received char
        if (received_char != -1) {
            buffer.push(received_char);
            received_char = -1;
        }

        if (buffer.remain() > 0) {
            char c = buffer.pop();
            while((UCSR0A &(1<<UDRE0))==0){}
            UDR0 = c;
        }
    }

    return 0;
}

// UART interruption
ISR(USART_RX_vect) {
    buffer.push(UDR0);
}

// Timer interruption called 1ms at time
ISR(TIMER2_OVF_vect) {
    timeInMillis++;
}
