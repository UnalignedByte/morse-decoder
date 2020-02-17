#include <LiquidCrystal.h>

// Config
const unsigned long DELAY_LONG_PRESS = 500;
const unsigned long DELAY_DECODE = 500;
const int BUTTON = 2;
const int LED_SHORT = 3;
const int LED_LONG = 4;

const int LCD_RS = 5;
const int LCD_E = 6;
const int LCD_D4 = 7;
const int LCD_D5 = 8;
const int LCD_D6 = 9;
const int LCD_D7 = 10;

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

typedef struct {
  char letter;
  int hash;
} MorseChar;

const int CODES_MAX = 5;
const int MORSE_CHARS_COUNT = 36;

MorseChar morseAlphabet[MORSE_CHARS_COUNT] = {
  {'A', 21},
  {'B', 1112},
  {'C', 1212},
  {'D', 112},
  {'E', 1},
  {'F', 1211},
  {'G', 122},
  {'H', 1111},
  {'I', 11},
  {'J', 2221},
  {'K', 212},
  {'L', 1121},
  {'M', 22},
  {'N', 12},
  {'O', 222},
  {'P', 1221},
  {'Q', 2122},
  {'R', 121},
  {'S', 111},
  {'T', 2},
  {'U', 211},
  {'V', 2111},
  {'W', 221},
  {'X', 2112},
  {'Y', 2212},
  {'Z', 1122},
  {'1', 22221},
  {'2', 22211},
  {'3', 22111},
  {'4', 21111},
  {'5', 11111},
  {'6', 11112},
  {'7', 11122},
  {'8', 11222},
  {'9', 12222},
  {'0', 22222}
};

LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

int hashForSequence(Code *sequence, int length) {
  int hash = 0;
  int multiplier = 1;
  for(int i=0; i<length; i++) {
    int value = 1;
    if(sequence[i] == dash) {
      value = 2;
    }
    hash += value * multiplier;
    multiplier *= 10;
  }
  return hash;
}

char characterForSequence(Code sequence[], int length) {
  // Print print entered sequence
  for(int i=0; i<length; i++) {
    Serial.print(sequence[i] == dot ?  "dot" : "dash");
    if(i == (length - 1)) {
      Serial.println();
    } else {
      Serial.print(", ");
    }
  }
  // Encode the sequence
  int hash = hashForSequence(sequence, length);
  Serial.println(hash);
  // Find letter correponding for the hash value
  char decodedChar = '-';
  for(int i=0; i<MORSE_CHARS_COUNT; i++) {
    if(morseAlphabet[i].hash == hash) {
      decodedChar = morseAlphabet[i].letter;
      break;
    }
  }
  return decodedChar;
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
  lcd.begin(16, 2);
  lcd.cursor();
  lcd.blink();
}

void loop() {
  int buttonState = digitalRead(BUTTON);
  long timeElapsed = millis() - startTime;

  // Press started
  if((state == waiting || state == released) && buttonState == HIGH && codeIndex < CODES_MAX) {
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
    lcd.write(decoded);
    codeIndex = 0;
    delay(200);
    digitalWrite(LED_SHORT, LOW);
    digitalWrite(LED_LONG, LOW);
    state = waiting;
  }
}
