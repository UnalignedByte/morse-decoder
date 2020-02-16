typedef enum {
  dot,
  dash
} Code;

typedef enum {
  waiting,
  shortPressed,
  longPressed,
  released
} State;

const unsigned long DELAY_LONG_PRESS = 1000;
const unsigned long DELAY_DECODE = 1000;
const int BUTTON = 2;
const int LED_SHORT = 3;
const int LED_LONG = 4;
const int CODES_MAX = 5;

char characterForSequence(Code *sequence, int length) {
  for(int i=0; i<length; i++) {
    Serial.print(sequence[i] == dot ?  "dot" : "dash");
    if(i == (length - 1)) {
      Serial.println();
    } else {
      Serial.print(", ");
    }
  }
  return 'A';
}

State state = waiting;
unsigned long startTime;
Code currentCode;
Code codes[CODES_MAX];
int codeIndex = 0;

void setup() {
  pinMode(BUTTON, INPUT);
  pinMode(LED_SHORT, OUTPUT);
  pinMode(LED_LONG, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int buttonState = digitalRead(BUTTON);
  long timeElapsed = millis() - startTime;

  // Press started
  if((state == waiting || state == released) && buttonState == HIGH && codeIndex <= CODES_MAX) {
    Serial.println("shortPressed");
    state = shortPressed;
    currentCode = dot;
    digitalWrite(LED_SHORT, HIGH);
    startTime = millis();
  // Button held
  } else if(state == shortPressed && timeElapsed >= DELAY_LONG_PRESS) {
    Serial.println("longPressed");
    state = longPressed;
    currentCode = dash;
    digitalWrite(LED_LONG, HIGH);
  // Button released
  } else if((state == shortPressed || state == longPressed) && buttonState == LOW) {
    Serial.println("released");
    state = released;
    codes[codeIndex] = currentCode;
    codeIndex++;
    digitalWrite(LED_SHORT, LOW);
    digitalWrite(LED_LONG, LOW);
    delay(10); // Let's the button stabilize after release
    startTime = millis();
  // Decode delay
  } else if(state == released && timeElapsed >= DELAY_DECODE) {
    Serial.println("waiting");
    digitalWrite(LED_SHORT, HIGH);
    digitalWrite(LED_LONG, HIGH);
    char decoded = characterForSequence(codes, codeIndex);
    Serial.print("Decoded: ");
    Serial.println(decoded);
    codeIndex = 0;
    delay(200);
    digitalWrite(LED_SHORT, LOW);
    digitalWrite(LED_LONG, LOW);
    state = waiting;
  }
}
