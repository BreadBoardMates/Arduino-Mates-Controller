#include "MatesController.h"

MatesController mates = MatesController(Serial);

const int16_t colors[] = {
  (int16_t) 0xFFFF, (int16_t) 0xF800, (int16_t) 0x07E0, (int16_t) 0x001F,
  (int16_t) 0x07FF, (int16_t) 0xF81F, (int16_t) 0xFFE0, (int16_t) 0x39FF
};

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
  static int8_t val = 0;
  static int8_t ctr = 0;

  if (ctr >= 42) {
    mates.clearPrintArea(0);
    ctr = 0;
  }

  mates.setPrintAreaColor(0, colors[ctr % 8]);
  mates.appendToPrintArea(0, &val, 1);
  ctr++;
  val++;

  delay(100);
}
