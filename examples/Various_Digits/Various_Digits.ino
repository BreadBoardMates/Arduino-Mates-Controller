#include "MatesController.h"

MatesController mates = MatesController(Serial);

int16_t value = 0;
int32_t longValue = 100000;
float floatValue = 3.1416;

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
  // mates.setWidgetValue(MATES_LED_DIGITS, 0, value);
  mates.setLedDigitsValue(0, value);
  mates.setLedDigitsValue(1, longValue);
  mates.setLedDigitsValue(2, floatValue);

  value++;
  longValue += 12345;
  floatValue += 3.1416;
}
