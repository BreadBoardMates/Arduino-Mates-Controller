#include "MatesController.h"

MatesController mates = MatesController(Serial);

#define BUTTON_PIN      8
#define DEBOUNCE_DELAY  50

int buttonState;
int ledState = 0;
bool blinkLed = false;
int lastButtonState = LOW;
unsigned long previousMillis = 0;
unsigned long lastDebounceTime = 0;

int errLedStatus = LOW;
void ErrorLed_Toggle() {
  errLedStatus = ~errLedStatus;
  digitalWrite(LED_BUILTIN, errLedStatus);
}

void setup()
{
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

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {

    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        blinkLed = !blinkLed;
        if (blinkLed) {
          previousMillis = millis();
          ledState = 1;
        } else {
          ledState = 0;
        }
        mates.setWidgetValue(MATES_MEDIA_LED, 0, ledState);
      }
    }
  }

  lastButtonState = reading;

  if (blinkLed && (millis() - previousMillis >= 500)) {
    // save the last time you blinked the LED
    previousMillis = millis();

    // if the LED is off turn it on and vice-versa:
    if (ledState == 0) {
      ledState = 1;
    } else {
      ledState = 0;
    }

    // set the LED with the ledState of the variable:
    mates.setWidgetValue(MATES_MEDIA_LED, 0, ledState);
  }
}
