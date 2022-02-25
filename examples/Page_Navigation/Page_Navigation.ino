#include "MatesController.h"

MatesController mates = MatesController(Serial);

void Day_and_Time_Animation(void) {

  // Days of Week Strings
  const char * daysOfWeek[] = {
    "SUNDAY",
    "MONDAY",
    "TUESDAY",
    "WEDNESDAY",
    "THURSDAY",
    "FRIDAY",
    "SATURDAY"
  };

  // Test Start Values: Monday, 23:59:47 (will actually start at 48 seconds)
  uint8_t lastDay = 1;
  uint8_t day = 1;
  int16_t hrs = 23;
  int16_t mnt = 59;
  int16_t sec = 47;

  mates.updateTextArea(0, daysOfWeek[day]);

  // Simulated Timer Variable
  unsigned long lastUpdate = millis() - 1000; // Ensures first write

  // stop at Day 2, Tuesday, 00:00:0  7
  while (day != 2 || hrs != 0 || mnt != 0 || sec != 7) {

    if (millis() - lastUpdate >= 1000) {

      lastUpdate = millis();

      sec++;
      if (sec == 60) {
        sec = 0;
        mnt++;
      }
      if (mnt == 60) {
        mnt = 0;
        hrs++;
      }
      if (hrs == 24) {
        hrs = 0;
        day++;
      }
      day %= 7;

      // mates.setWidgetValue(MATES_LED_DIGITS, 0, hrs);
      // mates.setWidgetValue(MATES_LED_DIGITS, 1, mnt);
      // mates.setWidgetValue(MATES_LED_DIGITS, 2, sec);
      mates.setLedDigitsValue(0, hrs);
      mates.setLedDigitsValue(1, mnt);
      mates.setLedDigitsValue(2, sec);
      if (lastDay != day) {
        mates.updateTextArea(0, daysOfWeek[day]);
        lastDay = day; // prevents writing the same text to TextArea
      }

    }

  }
}

void Numbered_LEDs_Animation(void) {

  mates.setWidgetValue(MATES_MEDIA_LED, 0, 1);

  uint8_t ledOff = 0;
  uint8_t ledOn = 1;

  for (int i = 0; i < 20; i++) {

    delay(500);

    mates.setWidgetValue(MATES_MEDIA_LED, ledOff, 0);
    mates.setWidgetValue(MATES_MEDIA_LED, ledOn, 1);

    ledOff = ledOn;
    ledOn++;
    ledOn %= 8;

  }

  mates.setWidgetValue(MATES_MEDIA_LED, ledOff, 0);

}

void Various_Gauges_Animation(void) {
  int16_t value = 0;
  int8_t inc = 1;

  unsigned long lastUpdate = millis();

  while (millis() - lastUpdate <= 10000) {

    value += inc;
    if (value == 100) inc = -1;
    if (value == 0) inc = 1;

    mates.setWidgetValue(MATES_GAUGE_A, 0, value);
    mates.setWidgetValue(MATES_LED_DIGITS, 3, value);
    mates.setWidgetValue(MATES_MEDIA_GAUGE_B, 0, value);

  }
}

void Various_Digits_Animation(void) {
  int16_t value = 0;
  int32_t longValue = 100000;
  float floatValue = 3.1416;

  unsigned long lastUpdate = millis();

  while (millis() - lastUpdate <= 5000) {

    // mates_setWidgetValue(MATES_LED_DIGITS, 4, value);
    mates.setLedDigitsValue(4, value);
    mates.setLedDigitsValue(5, longValue);
    mates.setLedDigitsValue(6, floatValue);

    value++;
    longValue += 12345;
    floatValue += 3.1416;

  }
}

void Print_Strings_Animation(void) {

  const char * msg = "Mates Studio offers a variety of widgetswhich includes this Print Area. For moreinfo, please refer  to our manuals.";

  uint8_t len = (uint8_t) strlen(msg);

  char str[2];

  mates.appendToPrintArea(0, msg);

  delay(2000);

  mates.clearPrintArea(0);

  for (uint8_t i = 0; i < len; i++) {
    str[0] = msg[i];
    str[1] = 0;
    mates.appendToPrintArea(0, str);
    delay(50);
  }

}

void Print_Hex_Values_Animation(void) {
  const int16_t colors[] = {
    (int16_t) 0xFFFF, (int16_t) 0xF800, (int16_t) 0x07E0, (int16_t) 0x001F,
    (int16_t) 0x07FF, (int16_t) 0xF81F, (int16_t) 0xFFE0, (int16_t) 0x39FF
  };

  int8_t val = 0;
  int8_t ctr = 0;

  unsigned long lastUpdate = millis();

  while (millis() - lastUpdate <= 5000) {

    if (ctr >= 42) {
      mates.clearPrintArea(0);
      ctr = 0;
    }

    mates.setPrintAreaColor(1, colors[ctr % 8]);
    mates.appendToPrintArea(1, &val, 1);
    ctr++;
    val++;

    delay(100);

  }
}

void Audio_Spectrum_Animation(void) {
  uint8_t col = 1;

  unsigned long lastUpdate = millis();

  while (millis() - lastUpdate <= 5000) {

    // mates.setSpectrumValue(MATES_MEDIA_SPECTRUM, 0, (uint8_t) (rand() % 101));
    mates.setMediaSpectrumValue(0, col, (uint8_t) (rand() % 101));

    col++;
    if (col == 6) col = 0;

  }
}

void Updating_Scope_Animation(void) {
  // uint16_t deg = 270; // Use for computed version
  int16_t value = 18;

  const int16_t values[] = {
    40, 50, 59, 67, 73, 77, 79, 77, 73, 67, 59, 50,
    40, 29, 20, 12, 6, 2, 1, 2, 6, 12, 20, 29
  };

  unsigned long lastUpdate = millis();

  while (millis() - lastUpdate <= 5000) {

    // Computed version START
    /*
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

    value++;
    if (value >= 24) value = 0;
    mates.setWidgetValue(MATES_SCOPE, 0, values[value]);

    // Pre-computed version END

  }
}

void Update_Dot_Matrix(void) {
  char buffer[25] = "DotMatrxVal1 XXXVal2 XXX"; // 24 characters + null terminator (8 * 3 + 1 = 25)
  char * val1ptr = buffer + 13;
  char * val2ptr = buffer + 21;

  uint8_t val1 = 0;
  uint8_t val2 = 255;

  unsigned long lastUpdate = millis();

  while (millis() - lastUpdate <= 5000) {

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

    mates.updateDotMatrix(0, buffer);

  }
}

void (* animations[])(void) = {
  Day_and_Time_Animation,
  Numbered_LEDs_Animation,
  Various_Gauges_Animation,
  Various_Digits_Animation,
  Print_Strings_Animation,
  Print_Hex_Values_Animation,
  Audio_Spectrum_Animation,
  Updating_Scope_Animation,
  Update_Dot_Matrix
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
  static uint16_t page = 0;
  (*animations[page])();
  page++;
  page %= 9;
  mates.setPage(page);
}
