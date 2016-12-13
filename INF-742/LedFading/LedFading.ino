int delayMs = 0;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // sets the pin on
  delayMicroseconds(delayMs);        // pauses for delayMs microseconds      
  digitalWrite(LED_BUILTIN, LOW);    // sets the pin off
  delayMicroseconds(2000 - delayMs); // pauses for delayMs microseconds
  delayMs++;
  if (delayMs >= 2000) {
      delayMs = 0;
}
