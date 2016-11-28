#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <uart.h>

#define MAX_PLAYERS 9
#define MAX_NOTES 100
#define DEBUG 1 // set 0 to disable debug prints

volatile long interruptions = 0;  // Controls how much interruptions has fired
volatile long bounceTime = 0;     // debounce control
volatile long bounceTime2 = 0;     // debounce control

// Players array
volatile int players[MAX_PLAYERS]; // number of players
volatile int nro_players = 0;
volatile long player_time = 0;
volatile char notes[MAX_NOTES]; // notes sequence array
volatile int N = 0;
volatile int round = 0;
volatile int current_player = -1;
volatile int current_player_note = 0;

void round_reset();

long get_millis() {
    // First, we need to count how much interruptions we had and multiply it by 16384us
    // after it, get current Timer2 register value and multiply it by 64us (time between each CPU
    // tick)
    long micros = (interruptions * 16384) + (TCNT2 * 64);
    return micros / 1000;
}

// Resets all CURRENT players to 0 so they can play next turn
void reset_players() {
    // before any new round, check players status
    if (DEBUG == 1) {
        printf("\nLast player, reset! \n");
    }
    for (int i = 0; i < nro_players; i++) {
        if(players[i] == 1) {
            players[i] = 0;
        }
    }
}

// Returns how many players do not played yet this turn
int get_not_played_players() {
    int n_played = 0;
    for (int i = 0; i < nro_players; i++) {
        if(players[i] == 0) {
            n_played++;
        }
    }
    return n_played;
}

// Returns how many players are still on the game
int get_active_players() {
    int active = 0;
    for (int i = 0; i < nro_players; i++) {
        if(players[i] != -1) {
            active++;
        }
    }
    return active;
}

// Return who won the game
int find_winner() {
    int winner = 0;
    for (int i = 0; i < nro_players; i++) {
        if(players[i] != -1) {
            winner = i + 1;
            break;
        }
    }
    return winner;
}

void round_turn() {
    // find next player
    for (int i = 0; i < nro_players; i++) {
        if(players[i] == 0) {
            players[i] = 1; // mark as played
            current_player = i;
            break;
        }
    }

    printf("Player %d\n", current_player + 1);
    current_player_note = 0;
    player_time = get_millis();
}

// Prepare for next round
void round_start() {
    int active = get_active_players();

    if (active == 1) {
        // we already have a winner
        printf("\nGame over! Player %d won\n",  find_winner());
        round_reset();
        return;
    } else {
        // Continue gameplay
        if (get_not_played_players() == 0) {
            // there´s no player waiting in line, clear flag
            reset_players();
        }

        // increments round
        round++;
        printf("\nROUND %d\n", round);

        round_turn();
    }
}

// it resets the system
void round_reset() {
    round = 0;
    N = 0;
    current_player = -1;
    current_player_note = 0;
    nro_players = 0;
    interruptions = 0; // clear timer 

    // reset players
    for (int i = 0; i < MAX_PLAYERS; i++) {
        players[i] = -1;
    }
    // reset notes list
    for (int i = 0; i < MAX_NOTES; i++) {
        notes[i] = '0';
    }
    printf("\nWelcome to the follow the leader game! Please enter the number of players: [2-9]\n");
    int count = 0;
    scanf("%d", &count);
    while (count < 2 || count > 9) {
        printf("Enter a valid number of players\n");
        scanf("%d", &count);
    }

    if (DEBUG == 1) {
        printf("Number of players: %d\n", count);
    }

    // active count players
    for (int i = 0; i < count; i++) {
        players[i] = 0;
    }

    nro_players = count;
    round_start();
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
    EIMSK |=  (1 << INT0);

    // Configure button interruptions 3
    DDRD  &= ~(1 << DDD3);    // PD3 (PCINT0 pin) is now an input
    PORTD |=  (1 << PORTD3);  // PD3 is now with pull-up enabled
    EICRA |= (1 << ISC10);
    EIMSK |=  (1 << INT1);

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

    round_reset();
    while(1) {
        // bla
    }
}

// Handle button1 interruption (C notes)
ISR (INT0_vect) {
    long timeNow = get_millis();

    if (bounceTime == 0) {
        bounceTime = timeNow;
        if (DEBUG == 1) {
            printf("C");
        }

        if (current_player_note == N) {
            notes[N] = 'C';
            N++;
            current_player_note++;
            round_start();
        } else if (current_player_note < N) {
            if (notes[current_player_note] == 'C') {
                current_player_note++;
            } else {
                // Player missed the note.. game over for him
                players[current_player] = -1;
                printf("Incorrect sequence! You have been eliminated.\n");
                round_start();
            }
        }
    }
}

// Handle button2 interruption (F notes)
ISR (INT1_vect) {
    long timeNow = get_millis();

    if (bounceTime2 == 0) {
        player_time = timeNow;
        bounceTime2 = timeNow;
        if (DEBUG == 1) {
            printf("F");
        }

        if (current_player_note == N) {
            notes[N] = 'F';
            N++;
            current_player_note++;
            round_start();
        } else if (current_player_note < N) {
            if (notes[current_player_note] == 'F') {
                current_player_note++;
            } else {
                // Player missed the note.. game over for him
                players[current_player] = -1;
                printf("Incorrect sequence! You have been eliminated.\n");
                round_start();
            }
        }
    }
}

ISR(TIMER2_OVF_vect) {
    // We reach the overflow each 16320us based on Timer2 configuration
    interruptions++;

    long timeNow = ((interruptions * 16384) + (TCNT2 * 64)) / 1000;

    // handle bounce control on button 1
    if (bounceTime > 0 && timeNow - bounceTime > 300) {
        bounceTime = 0;
    }

    // handle bounce control on button 2
    if (bounceTime2 > 0 && timeNow - bounceTime2 > 300) {
        bounceTime2 = 0;
    }

    // handle player timeout
    if (player_time > 0 && timeNow - player_time > 5000) {
        player_time = 0;
        players[current_player] = -1;
        printf("Time expired. You have been eliminated!\n");
        round_start();
    }
}
