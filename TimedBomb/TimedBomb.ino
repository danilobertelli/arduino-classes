const int RATE = 20;
int TOTAL_TIME = 500;
int delayMs = 0;
int state;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  delayMs = TOTAL_TIME;
  state = 1;
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  // Check if we should increase of decrease
  if(delayMs == 0) {
    state = 0;
  } else if (delayMs == TOTAL_TIME) {
    state = 1;
  }
  
  Serial.println("delay of ");       // prints in a new line a message
  Serial.println(delayMs);           // prints in a new line the delay in ms
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(delayMs);                    // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(delayMs);                    // wait for a second

  if (state == 0) {
    delayMs = delayMs + RATE;
  } else {
    delayMs = delayMs - RATE;
  }
}
