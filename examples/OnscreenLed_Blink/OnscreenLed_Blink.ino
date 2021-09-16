#include "MatesController.h"

MatesController mates = MatesController(Serial);

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

void loop()
{
  mates.setWidgetValue(MATES_MEDIA_LED, 0, 1);  // turn the on screen Led ON
  delay(500);                                   // wait for 500ms
  mates.setWidgetValue(MATES_MEDIA_LED, 0, 0);  // turn the on screen Led OFF
  delay(500);                                   // wait for 500ms
}
