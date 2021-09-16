#include "MatesController.h"

MatesController mates = MatesController(Serial);

#define BUTTON_PIN      8
#define DEBOUNCE_DELAY  50

int buttonState;
int clickCounter = 0;
int lastButtonState = LOW; 
unsigned long lastDebounceTime;

int errLedStatus = LOW;
void ErrorLed_Toggle() {
  errLedStatus = ~errLedStatus;
  digitalWrite(LED_BUILTIN, errLedStatus);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, errLedStatus);

  if (!mates.begin()) {
    // Display didn't send ready signal in time
    while (1) {
      ErrorLed_Toggle();
      delay(100);
    }
  }
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > 50) {

    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        clickCounter++;
        if (clickCounter >= 10000) clickCounter = 0;
        mates.setWidgetValue(MATES_LED_DIGITS, 0, clickCounter);
      }
    }
  }

  lastButtonState = reading;
}
