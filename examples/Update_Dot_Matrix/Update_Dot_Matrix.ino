#include "MatesController.h"

MatesController mates = MatesController(Serial);

// The Dot Matrix can contain 24 characters
char myBuffer[25]; 
// "DotMatrxVal1 XXXVal2 YYY" is the reference string
//    where XXX is first value
//    and YYY is second value
// This should appear as:
//    DotMatrx
//    Val1 XXX
//    Val2 YYY

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

  sprintf(myBuffer, "DotMatrxVal1 %03dVal2 %03d", val1, val2);

  mates.updateDotMatrix(0, myBuffer);

  val1++;
  val2--;
}
