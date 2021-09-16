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
}

void loop() {
  static uint8_t col = 0;
  
  col++;
  if (col == 6) col = 0;

  // mates.setSpectrumValue(MATES_MEDIA_SPECTRUM, 0, (uint8_t) (rand() % 101));
  mates.setMediaSpectrumValue(0, col, (uint8_t) (rand() % 101));
}