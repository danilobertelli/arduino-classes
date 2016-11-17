static const int DELAY_SPACE_SAME_LETTER = 150;
static const int DELAY_SPACE_LETTER = 450;
static const int DELAY_SPACE_WORDS = 1050;

static const char *alpha[] = {
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
static const char *num[] = {
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

String msg = "HELLO IOT WORLD";
boolean finished = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!finished) {
    find_morse();
  }
}

/**
 * Will blink a dot using 150ms
 */
void dot() {
  Serial.println("dot()");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(150);
  digitalWrite(LED_BUILTIN, LOW);  
}

/**
 * Will blink a dash using 450ms
 */
void dash() {
  Serial.println("dash()");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(450);
  digitalWrite(LED_BUILTIN, LOW);
}

void delayTime(int ms) {
  Serial.print("delayTime of ");
  Serial.println(ms);
  delay(ms);
}

void decode_as_blink(char character[]) {
  int len = strlen(character);
  for (int i=0; i < len; i++) {
    if (character[i] == '.') {
      dot();
    } else {
      dash();
    }
    delayTime(DELAY_SPACE_SAME_LETTER);
  }
}

void find_morse(){
  for (int i = 0; i < msg.length(); i++){
    int delay = -1;
    switch (toupper(msg[i])) {
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
        Serial.println("space");
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
        Serial.println("default");
        break;
    }
    if (delay < 0) {
      // As DELAY_SPACE_SAME_LETTER was already waited inside while, we cannot consider
      delay = DELAY_SPACE_LETTER - DELAY_SPACE_SAME_LETTER;
    }
    delayTime(delay);
  }
  finished = true;
}

