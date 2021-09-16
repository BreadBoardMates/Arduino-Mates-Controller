#include "MatesController.h"

MatesController mates = MatesController(Serial);

char myBuffer[25] = "DotMatrxVal1 XXXVal2 XXX"; // 24 characters + null terminator (8 * 3 + 1 = 25)
char * val1ptr = myBuffer + 13;
char * val2ptr = myBuffer + 21;

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
  static uint8_t val1 = 0;
  static uint8_t val2 = 255;

  // START of Simulated Values
  val1++;
  val2--;

  if (val1 < 100) {
    val1ptr[0] = ' ';
  } else {
    val1ptr[0] = '0' + (val1 / 100);
  }

  if (val1 < 10) {
    val1ptr[1] = ' ';
  } else {
    val1ptr[1] = '0' + ((val1 % 100) / 10);
  }

  val1ptr[2] = '0' + (val1 % 10);

  if (val2 < 100) {
    val2ptr[0] = ' ';
  } else {
    val2ptr[0] = '0' + (val2 / 100);
  }

  if (val2 < 10) {
    val2ptr[1] = ' ';
  } else {
    val2ptr[1] = '0' + ((val2 % 100) / 10);
  }

  val2ptr[2] = '0' + (val2 % 10);
  // END of Simulated Values

  mates.updateDotMatrix(0, myBuffer);
}
