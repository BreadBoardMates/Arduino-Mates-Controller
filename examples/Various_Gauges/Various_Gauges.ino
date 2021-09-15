#include "MatesController.h"

MatesController mates = MatesController(Serial);

int16_t value = 0;
int8_t inc = 1;

int errLedStatus = LOW;
void ErrorLed_Toggle() {
  errLedStatus != errLedStatus;
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
  value += inc;
  if (value == 100) inc = -1;
  if (value == 0) inc = 1;

  mates.setWidgetValue(MATES_GAUGE_A, 0, value);
  mates.setWidgetValue(MATES_LED_DIGITS, 0, value);
  mates.setWidgetValue(MATES_MEDIA_GAUGE_B, 0, value);
}
