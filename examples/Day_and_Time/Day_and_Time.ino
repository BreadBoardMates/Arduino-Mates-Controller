#include "MatesController.h"

MatesController mates = MatesController(Serial);

// Simulated Timer Variable
unsigned long lastUpdate;

// Test Start Values: Monday, 23:59:47 (will actually start at 48 seconds)
uint8_t lastDay = 1;
uint8_t day = 1;
int16_t hrs = 23;
int16_t min = 59;
int16_t sec = 47;

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

    sec++;
    if (sec == 60) {
      sec = 0;
      min++;
    }
    if (min == 60) {
      min = 0;
      hrs++;
    }
    if (hrs == 24) {
      hrs = 0;
      day++;
    }
    day %= 7;

    // mates.setWidgetValue(MATES_LED_DIGITS, 0, hrs);
    // mates.setWidgetValue(MATES_LED_DIGITS, 1, min);
    // mates.setWidgetValue(MATES_LED_DIGITS, 2, sec);
    mates.setLedDigitsValue(0, hrs);
    mates.setLedDigitsValue(1, min);
    mates.setLedDigitsValue(2, sec);

    if (lastDay != day) {
      mates.updateTextArea(0, daysOfWeek[day]);
      lastDay = day; // prevents writing the same text to TextArea
    }

  }
}
