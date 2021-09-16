#include "MatesController.h"

MatesController mates = MatesController(Serial);

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

  mates.setWidgetValue(MATES_MEDIA_LED, 0, 1);
}

void loop() {
  static uint8_t ledOff = 0;
  static uint8_t ledOn = 1;
  
  delay(500);

  mates.setWidgetValue(MATES_MEDIA_LED, ledOff, 0);
  mates.setWidgetValue(MATES_MEDIA_LED, ledOn, 1);

  ledOff = ledOn;
  ledOn++;
  ledOn %= 8;
}
