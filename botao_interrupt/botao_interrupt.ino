
const byte interruptPin = 2;
volatile byte state = LOW;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, FALLING);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, state);
}

void blink() {
  state = !state;
}

