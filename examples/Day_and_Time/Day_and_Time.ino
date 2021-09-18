#include "MatesController.h"

MatesController mates = MatesController(Serial);

// Simulated Timer Variable
unsigned long lastUpdate;

// Test Start Values: Monday, 23:59:47 (will actually start at 48 seconds)
uint8_t lastDay = 1;
uint8_t dy = 1;
int16_t hr = 23;
int16_t mn = 59;
int16_t sc = 47;

// Days of Week Strings
const char * daysOfWeek[] = {
  "SUNDAY",
  "MONDAY",
  "TUESDAY",
  "WEDNESDAY",
  "THURSDAY",
  "FRIDAY",
  "SATURDAY"
};

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

  lastUpdate = millis() - 1000; // Ensures first write
}

void loop() {
  if (millis() - lastUpdate >= 1000) {

    lastUpdate = millis();

    sc++;
    if (sc == 60) {
      sc = 0;
      mn++;
    }
    if (mn == 60) {
      mn = 0;
      hr++;
    }
    if (hr == 24) {
      hr = 0;
      dy++;
    }
    dy %= 7;

    // mates.setWidgetValue(MATES_LED_DIGITS, 0, hr);
    // mates.setWidgetValue(MATES_LED_DIGITS, 1, mn);
    // mates.setWidgetValue(MATES_LED_DIGITS, 2, sc);
    mates.setLedDigitsValue(0, hr);
    mates.setLedDigitsValue(1, mn);
    mates.setLedDigitsValue(2, sc);

    if (lastDay != dy) {
      mates.updateTextArea(0, daysOfWeek[dy]);
      lastDay = dy; // prevents writing the same text to TextArea
    }

  }
}