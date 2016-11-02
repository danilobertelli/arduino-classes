#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "CircularBuffer.h"

#define USART_BAUDRATE 9600 // Defines the baud rate
#define BAUD_PRESCALE (((1600000/(USART_BAUDRATE*16UL)))-1) // Calculate the prescale

volatile unsigned long timeInMillis = 0;
volatile char received_char = -1;
CircularBuffer<char, 10> buffer = CircularBuffer<char, 10>();

static const int DELAY_SPACE_SAME_LETTER = 150;
static const unsigned long DELAY_SPACE_LETTER = 450;
static const unsigned long DELAY_SPACE_WORDS = 1050;

// Led state treated as int
const int LED_OFF = 0x00;
const int LED_ON = 0xFF;
volatile unsigned long ledTime = 0;

static char *alpha[] = {
    ".-",   //A
    "-...", //B
    "-.-.", //C
    "-..",  //D
    ".",    //E
    "..-.", //F
    "--.",  //G
    "....", //H
    "..",   //I
    ".---", //J
    "-.-",  //K
    ".-..", //L
    "--",   //M
    "-.",   //N
    "---",  //O
    ".--.", //P
    "--.-", //Q
    ".-.",  //R
    "...",  //S
    "-",    //T
    "..-",  //U
    "...-", //V
    ".--",  //W
    "-..-", //X
    "-.--", //Y
    "--..", //Z
};
static char *num[] = {
    "-----", //0
    ".----", //1
    "..---", //2
    "...--", //3
    "....-", //4
    ".....", //5
    "-....", //6
    "--...", //7
    "---..", //8
    "----.", //9
};

unsigned long getMillis() {
    return timeInMillis;
}

// Turn led state
void turnLedState(int state) {
    PORTB = state;
    if (PORTB == LED_ON) {
        ledTime = getMillis();
    } else {
        ledTime = 0;
    }
}

void delayTime(unsigned long delayTimeMs) {
    while(getMillis() < delayTimeMs){};
}

// Will wait and turn led off
void turnLedOffAfterDelay(unsigned long delayTimeMs) {
    unsigned long wait = ledTime + delayTimeMs;
    delayTime(wait);
    turnLedState(LED_OFF);
}

/**
 * Will blink a dot using 150ms
 */
void dot() {
  turnLedState(LED_ON);
  turnLedOffAfterDelay(150);
}

/**
 * Will blink a dash using 450ms
 */
void dash() {
  turnLedState(LED_ON);
  turnLedOffAfterDelay(450);
}

void decode_as_blink(char character[]) {
  int len = strlen(character);
  for (int i=0; i < len; i++) {
    if (character[i] == '.') {
      while((UCSR0A &(1<<UDRE0))==0){}
      UDR0 = '.';
      dot();
    } else {
      while((UCSR0A &(1<<UDRE0))==0){}
      UDR0 = '-';
      dash();
    }
    long timeToWait = getMillis() + DELAY_SPACE_SAME_LETTER;
    delayTime(timeToWait);
  }
}

void find_morse(){
    for (int i = 0; i < buffer.remain(); i++){
        int delay = -1;
        switch (buffer.pop()) {
          case 'A':
            decode_as_blink(alpha[0]);
            break;
          case 'B':
            decode_as_blink(alpha[1]);
            break;
          case 'C':
            decode_as_blink(alpha[2]);
            break;
          case 'D':
            decode_as_blink(alpha[3]);
            break;
          case 'E':
            decode_as_blink(alpha[4]);
            break;
          case 'F':
            decode_as_blink(alpha[5]);
            break;
          case 'G':
            decode_as_blink(alpha[6]);
            break;
          case 'H':
            decode_as_blink(alpha[7]);
            break;
          case 'I':
            decode_as_blink(alpha[8]);
            break;
          case 'J':
            decode_as_blink(alpha[9]);
            break;
          case 'K':
            decode_as_blink(alpha[10]);
            break;
          case 'L':
            decode_as_blink(alpha[11]);
            break;
          case 'M':
            decode_as_blink(alpha[12]);
            break;
          case 'N':
            decode_as_blink(alpha[13]);
            break;
          case 'O':
            decode_as_blink(alpha[14]);
            break;
          case 'P':
            decode_as_blink(alpha[15]);
            break;
          case 'Q':
            decode_as_blink(alpha[16]);
            break;
          case 'R':
            decode_as_blink(alpha[17]);
            break;
          case 'S':
            decode_as_blink(alpha[18]);
            break;
          case 'T':
            decode_as_blink(alpha[19]);
            break;
          case 'U':
            decode_as_blink(alpha[20]);
            break;
          case 'V':
            decode_as_blink(alpha[21]);
            break;
          case 'W':
            decode_as_blink(alpha[22]);
            break;
          case 'X':
            decode_as_blink(alpha[23]);
            break;
          case 'Y':
            decode_as_blink(alpha[24]);
            break;
          case 'Z':
            decode_as_blink(alpha[25]);
            break;
          case ' ':
            // DELAY_SPACE_LETTER was just receive, do not consider it
            delay = DELAY_SPACE_WORDS - DELAY_SPACE_LETTER;
            break;
          case '0':
            decode_as_blink(num[0]);
            break;
          case '1':
            decode_as_blink(num[1]);
            break;
          case '2':
            decode_as_blink(num[2]);
            break;
          case '3':
            decode_as_blink(num[3]);
            break;
          case '4':
            decode_as_blink(num[4]);
            break;
          case '5':
            decode_as_blink(num[5]);
            break;
          case '6':
            decode_as_blink(num[6]);
            break;
          case '7':
            decode_as_blink(num[7]);
            break;
          case '8':
            decode_as_blink(num[8]);
            break;
          case '9':
            decode_as_blink(num[9]);
            break;
          default:
            break;
        }
        if (delay < 0) {
          // As DELAY_SPACE_SAME_LETTER was already waited inside while, we cannot consider
          delay = DELAY_SPACE_LETTER - DELAY_SPACE_SAME_LETTER;
        }
        long timeToWait = getMillis() + delay;
        delayTime(timeToWait);
      }
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

    // LED
    DDRB = 0xFF;       // Configure PORTB as output
    PORTB = 0x00;      // Set output pins as LOW (LED OFF)

    sei();

    while(1) {
        //TODO Check why this is not working properly at ISR
        // It supposed to be on ISR so when called it push the received char
        if (received_char != -1) {
            buffer.push(received_char);
            received_char = -1;
        }

        if (buffer.remain() > 0) {
            find_morse();
            //char c = buffer.pop();
            //while((UCSR0A &(1<<UDRE0))==0){}
            //UDR0 = c;
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
