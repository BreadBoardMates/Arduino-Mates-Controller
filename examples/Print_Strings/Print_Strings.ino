#include "MatesController.h"
#include <SoftwareSerial.h>

SoftwareSerial matesSerial(2,3);
MatesController mates = MatesController(matesSerial, Serial);

char str[2];
const char * msg = "Mates Studio offers a variety of widgetswhich includes this Print Area. For moreinfo, please refer  to our manuals.";
unsigned long lastUpdate;

int errLedStatus = LOW;
void ErrorLed_Toggle() {
  errLedStatus = ~errLedStatus;
  digitalWrite(LED_BUILTIN, errLedStatus);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, errLedStatus);

  matesSerial.begin(9600);
  Serial.begin(115200);
  if (!mates.begin()) {
    // Display didn't send ready signal in time
    while (1) {
      ErrorLed_Toggle();
      delay(100);
    }
  }

  mates.appendToPrintArea(0, msg);
  
  lastUpdate = millis();
}

void loop() {
  if (millis() - lastUpdate >= 3000) {

    mates.clearPrintArea(0);

    for (uint8_t i = 0; i < strlen(msg); i++) {
      str[0] = msg[i];
      str[1] = 0;
      mates.appendToPrintArea(0, str);
      delay(50);
    }

    lastUpdate = millis();
  }
}
