volatile char* ddrB = (char*) 0x24;
volatile char* pinB = (char*) 0x23;
volatile char* portB = (char*) 0x25;

bool pressed;

void setup() {
  // put your setup code here, to run once:
  *ddrB |= (1 << 5); // setup 5 as output
  *ddrB &= ~(1); // sets as input
  *portB &= ~(1 << 5); // turn off led
  Serial.begin(9600);
}

void loop() {
  pressed = *pinB & (1);
  if (pressed) {
    *portB |= (1 << 5); // turn on led
  } else {
    *portB &= ~(1 << 5); // turn off led
  }
}
