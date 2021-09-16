#include "MatesController.h"

MatesController mates = MatesController(Serial);

const int16_t values[] = {
  40, 50, 59, 67, 73, 77, 79, 77, 73, 67, 59, 50,
  40, 29, 20, 12, 6, 2, 1, 2, 6, 12, 20, 29
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
}

void loop() {
  // Computed version START
  /*
  // static uint16_t deg = 270;
    deg += 15;
  if (deg >= 360) {
    deg %= 360;
  }

  // 180 / PI = 57.29578
  // value = (int16_t) (39 * sin(deg * 3.1416 / 180)) + 40;
  value = (int16_t) (39 * sin(deg / 57.29578)) + 40;
  // Float computations can take a significant time

  mates.setWidgetValue(MATES_SCOPE, 0, value);
  */
  // Computed version END

  // Pre-computed version START
  static int16_t value = 18;

  value++;
  if (value >= 24) value = 0;
  mates.setWidgetValue(MATES_SCOPE, 0, values[value]);
  // Pre-computed version END
}
