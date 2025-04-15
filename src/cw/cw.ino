#include "morse_decode.h"

// Pin Definitions
const int lPaddlePin = 3;  // Left iambic paddle pin (Dit or "Short" key)
const int rPaddlePin = 2;  // Right iambic paddle pin (Dah or "Long" key)
const int speakerPin = 5; // Pin connected to the speaker

// Timing variables for CW
const int ditDuration = 48;  // Duration of a "Dit" (in milliseconds)
const int dahDuration = ditDuration * 3;  // Duration of a "Dah" (3 times Dit)
const int gapDuration = ditDuration;  // Gap between dits/dahs within a character (standard is "Dit" duration)

// Speaker tone (in Hz) (may have to manipulate based on speaker to get desired "actual" tone)
const int speakerTone = 400;

// Variables to store the current state of the paddles
volatile bool rPaddleState = HIGH;  // Current state of right paddle
volatile bool lPaddleState = HIGH;  // Current state of left paddle

// Tracking last symbol sent for iambic functionality (bool used since only 2 states exist)
volatile bool lastSymbolWasDit = false;

volatile long int lastInputTime = millis();

// Tracking if char has been started (to determine whether to add space at pause)
volatile bool charStarted = false;
volatile bool wordStarted = false;

// Current character tracked
volatile char currentChar[6] = "";

void setup() {
  // Set pin modes for paddles + speakers (and begin serial comms)
  pinMode(lPaddlePin, INPUT_PULLUP);
  pinMode(rPaddlePin, INPUT_PULLUP);
  pinMode(speakerPin, OUTPUT);
  Serial.begin(9600);
}

void playDit() {
  tone(speakerPin, speakerTone);  // Play tone for a "Dit"
  delay(ditDuration);
  noTone(speakerPin);
  delay(gapDuration);
  wordStarted = charStarted = true;
  lastInputTime = millis();
  lastSymbolWasDit = true;
  if (strlen(currentChar) < 6) {
    currentChar[strlen(currentChar)] = '.';
  }
}

void playDah() {
  tone(speakerPin, speakerTone);  // Play tone for a "Dah"
  delay(dahDuration);
  noTone(speakerPin);
  delay(gapDuration);
  wordStarted = charStarted = true;
  lastInputTime = millis();
  lastSymbolWasDit = false;
  if (strlen(currentChar) < 6) {
    currentChar[strlen(currentChar)] = '-';
  }
}

void loop() {
  /*
  Reads paddle states, and triggers iambic keying according to paddle inputs.
  Detected morse characters are written to serial with proper timing/spacing.
  */

  // Read current paddle states
  lPaddleState = digitalRead(lPaddlePin);
  rPaddleState = digitalRead(rPaddlePin);

  if (lPaddleState == LOW && rPaddleState == LOW) { // If both paddles pressed
    if (lastSymbolWasDit) {
      playDah();
    } else {
      playDit();
    }
  } else {
    if (rPaddleState == LOW) { // If right paddle pressed
      playDit();
    } else if (lPaddleState == LOW) { // If left paddle pressed
      playDah();
    } else {
      // Break character after long enough delay
      if (((millis() - lastInputTime) > 2 * ditDuration)) {
        if (charStarted == true) {
          Serial.print(decodeMorse(currentChar));  // Print decoded character
          memset(currentChar,0,strlen(currentChar)); // Null out current encoded character var
          charStarted = false;
        }
        // Break word after long enough delay
        if (((millis() - lastInputTime) > 6 * ditDuration) && wordStarted == true) {
          wordStarted = false;
          Serial.print(' ');  // Print space
        }
      }
    }
  }
}