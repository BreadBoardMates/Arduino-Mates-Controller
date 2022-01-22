#include "MatesController.h"

// -----------------------------------------------------------
// ---------------------- CONSTRUCTORS -----------------------
// -----------------------------------------------------------

MatesController::MatesController(Stream &serial, uint8_t resetPin, uint8_t mode) {
  matesResetPin = resetPin;
  matesResetMode = mode;
  matesSerialType = UNKNOWN_SERIAL;
  matesSerial = &serial;
}

MatesController::MatesController(Stream &serial, Stream &dbSerial, uint8_t resetPin, uint8_t mode) {
  matesResetPin = resetPin;
  matesResetMode = mode;
  matesSerialType = UNKNOWN_SERIAL;
  matesSerial = &serial;
  debugSerial = &dbSerial;
}

MatesController::MatesController(HardwareSerial &serial, uint8_t resetPin, uint8_t mode) {
  matesResetPin = resetPin;
  matesResetMode = mode;
  matesSerialType = HARDWARE_SERIAL;
  hwMatesSerial = &serial;
  matesSerial = &serial;
}

MatesController::MatesController(HardwareSerial &serial, Stream &dbSerial, uint8_t resetPin, uint8_t mode) {
  matesResetPin = resetPin;
  matesResetMode = mode;
  matesSerialType = HARDWARE_SERIAL;
  hwMatesSerial = &serial;
  matesSerial = &serial;
  debugSerial = &dbSerial;
}

#ifdef SoftwareSerial_h
MatesController::MatesController(SoftwareSerial &serial, uint8_t resetPin, uint8_t mode) {
  matesResetPin = resetPin;
  matesResetMode = mode;
  matesSerialType = SOFTWARE_SERIAL;
  swMatesSerial = &serial;
  matesSerial = &serial;
}

MatesController::MatesController(SoftwareSerial &serial, Stream &dbSerial, uint8_t resetPin, uint8_t mode) {
  matesResetPin = resetPin;
  matesResetMode = mode;
  matesSerialType = SOFTWARE_SERIAL;
  swMatesSerial = &serial;
  matesSerial = &serial;
  debugSerial = &dbSerial;
}
#endif

#ifdef AltSoftSerial_h
MatesController::MatesController(AltSoftSerial  &serial, uint8_t resetPin, uint8_t mode) {
  matesResetPin = resetPin;
  matesSerialType = ALTSOFT_SERIAL;
  swMatesSerial = &serial;
  matesSerial = &serial;
}

MatesController::MatesController(AltSoftSerial &serial, Stream &dbSerial, uint8_t resetPin) {
  matesResetPin = resetPin;
  matesSerialType = ALTSOFT_SERIAL;
  sdMatesSerial = &serial;
  matesSerial = &serial;
  debugSerial = &dbSerial;
}
#endif

// -----------------------------------------------------------
// -------------------- PUBLIC FUNCTIONS ---------------------
// -----------------------------------------------------------

bool MatesController::begin(int32_t baudrate, bool resetModule) {

  if (debugSerial != NULL) {
    debugSerial->write("Initializing Serial UART @");
    debugSerial->println(baudrate);
  }  
  pinMode(matesResetPin, OUTPUT);

  switch (matesSerialType) {
    case HARDWARE_SERIAL:
      hwMatesSerial->begin(baudrate);
      break;
#ifdef SoftwareSerial_h
    case SOFTWARE_SERIAL:
      swMatesSerial->begin(baudrate);
      break;
#endif
#ifdef AltSoftSerial_h
    case ALTSOFT_SERIAL:
      asMatesSerial->begin(baudrate);
      break;
#endif
    case UNKNOWN_SERIAL:
      break;
    default:
      return false;
  }

  if (resetModule) {
    return this->reset();
  }

  unsigned long startTime = 0;

  if (debugSerial != NULL) {
    startTime = millis();
  }

  matesReady = WaitForACK(matesBootTimeout);

  if (debugSerial != NULL) {
    debugSerial->write(matesReady ? "Ready after " : "\nTimed out after ");
    debugSerial->print(millis() - startTime);
    debugSerial->write(" ms\n");
  }
  matesReady = true;

  return matesReady;
}

bool MatesController::reset(uint16_t waitPeriod) {
  if (debugSerial != NULL) {
    debugSerial->write("Resetting module using Pin ");
    debugSerial->print(matesResetPin);
    debugSerial->write("... ");
  }
  
  digitalWrite(matesResetPin, matesResetMode);
  delay(100);
  digitalWrite(matesResetPin, ~matesResetMode);

  unsigned long startTime = 0;

  if (debugSerial != NULL) {
    startTime = millis();
  }

  matesReady = WaitForACK((waitPeriod == 0) ? matesBootTimeout : waitPeriod);

  if (debugSerial != NULL) {
    debugSerial->write(matesReady ? "\nDone after " : "\nTimed out after ");
    debugSerial->print(millis() - startTime);
    debugSerial->write(" ms\n");
  }
  
  return matesReady;
}

bool MatesController::softReset(uint16_t waitPeriod) {
  if (debugSerial != NULL) {
    debugSerial->write("Resetting module using command... ");
  }
  
  unsigned long startTime = 0;

  WriteCommand(MATES_CMD_SYSTEM_RESET);
  if (debugSerial != NULL) {
    startTime = millis();
  }
  
  matesReady = WaitForACK((waitPeriod == 0) ? matesBootTimeout : waitPeriod);

  if (debugSerial != NULL) {
    debugSerial->write(matesReady ? "Done after " : "Timed out after ");
    debugSerial->print(millis() - startTime);
    debugSerial->write(" ms\n");
  }
  
  return matesReady;
}

bool MatesController::setBacklight(uint8_t value) {
  if (debugSerial != NULL) {
    debugSerial->write("Setting backlight to ");
    debugSerial->print(value);
    debugSerial->write("... ");
  }
  
  bool res = false;
  if (matesReady) {
    WriteCommand(MATES_CMD_SET_BACKLIGHT);
    WriteWord((int16_t)value);    
    res = WaitForACK();
    if ((debugSerial != NULL) && res) debugSerial->write('\n');
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
  }
  return res;
}

bool MatesController::setPage(uint16_t page) {
  if (debugSerial != NULL) {
    debugSerial->write("Navigating to Page");
    debugSerial->print(page);
    debugSerial->write("... ");    
  }

  bool res = false;
  if (matesReady) {
    WriteCommand(MATES_CMD_SET_PAGE);
    WriteWord((int16_t)page);
    res = WaitForACK();
    if ((debugSerial != NULL) && res) debugSerial->write('\n');
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
  }
  return res;
}

int16_t MatesController::getPage() {
  if (debugSerial != NULL) {
    debugSerial->write("Query active page... ");
  }

  if (matesReady) {
    WriteCommand(MATES_CMD_GET_PAGE);
    return ReadResponse();
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
    return -1;
  }
}

bool MatesController::setWidgetValue(int16_t widget, int16_t value) {
  if (debugSerial != NULL) {
    debugSerial->write("Set Widget (0x");
    debugSerial->print(widget, HEX);
    debugSerial->write(") value to ");
    debugSerial->print(value);
    debugSerial->write("... ");
  }

  bool res = false;
  if (matesReady) {
    WriteCommand(MATES_CMD_SET_WIDGET_VALUE);
    WriteWord(widget);
    WriteWord(value);
    res = WaitForACK();
    if ((debugSerial != NULL) && res) debugSerial->write('\n');
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
  }
  return res;
}

int16_t MatesController::getWidgetValue(int16_t widget) {
  if (debugSerial != NULL) {
    debugSerial->write("Query widget (0x");
    debugSerial->print(widget, HEX);
    debugSerial->write(") value... ");
  }

  if (matesReady) {
    WriteCommand(MATES_CMD_GET_WIDGET_VALUE);
    WriteWord(widget);
    return ReadResponse();
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
    return -1;
  }
}

bool MatesController::setWidgetValue(MatesWidget type, uint8_t index, int16_t value) {
  return setWidgetValue((int16_t)((type << 8) | index), value);
}

int16_t MatesController::getWidgetValue(MatesWidget type, uint8_t index) {
  return getWidgetValue((int16_t)((type << 8) | index));
}

bool MatesController::setLedDigitsValue(uint8_t index, int16_t value) {
  return setWidgetValue((int16_t)((MATES_LED_DIGITS << 8) | index), value);
}

bool MatesController::setLedDigitsValue(uint8_t index, int32_t value) {
  return _setWidgetValue((int16_t)((MATES_LED_DIGITS << 8) | index), value);
}

bool MatesController::setLedDigitsValue(uint8_t index, float value) {
  return _setWidgetValue((int16_t)((MATES_LED_DIGITS << 8) | index), value);
}

bool MatesController::setSpectrumValue(int16_t widget, uint8_t gaugeIndex, uint8_t value) {
  return setWidgetValue(widget, (int16_t)((gaugeIndex << 8) | value));
}

bool MatesController::setLedSpectrumValue(uint8_t index, uint8_t gaugeIndex, uint8_t value) {
  return setWidgetValue((int16_t)((MATES_LED_SPECTRUM << 8) | index), (int16_t)((gaugeIndex << 8) | value));
}

bool MatesController::setMediaSpectrumValue(uint8_t index, uint8_t gaugeIndex, uint8_t value) {
  return setWidgetValue((int16_t)((MATES_MEDIA_SPECTRUM << 8) | index), (int16_t)((gaugeIndex << 8) | value));
}

bool MatesController::setWidgetParam(int16_t widget, int16_t param, int16_t value) {
  if (debugSerial != NULL) {
    debugSerial->write("Set widget (0x");
    debugSerial->print(widget, HEX);
    debugSerial->write(") parameter (0x");
    debugSerial->print(param, HEX);
    debugSerial->write(") to ");
    debugSerial->print(value);
    debugSerial->write("... ");
  }

  bool res = false;
  if (matesReady) {
    WriteCommand(MATES_CMD_SET_WIDGET_PARAM);
    WriteWord(widget);
    WriteWord(param);
    WriteWord(value);
    res = WaitForACK();
    if ((debugSerial != NULL) && res) debugSerial->write('\n');
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
  }
  return res;
}

int16_t MatesController::getWidgetParam(int16_t widget, int16_t param) {
  if (debugSerial != NULL) {
    debugSerial->write("Query widget (0x");
    debugSerial->print(widget, HEX);
    debugSerial->write(") parameter (0x");
    debugSerial->print(param, HEX);
    debugSerial->write(")... ");
  }

  if (matesReady) {
    WriteCommand(MATES_CMD_GET_WIDGET_PARAM);
    WriteWord(widget);
    WriteWord(param);
    return ReadResponse();
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
    return -1;
  }
}

bool MatesController::setWidgetParam(MatesWidget type, uint8_t index, int16_t param, int16_t value) {
  return setWidgetParam((int16_t)((type << 8) | index), param, value);
}

int16_t MatesController::getWidgetParam(MatesWidget type, uint8_t index, int16_t param) {
  return getWidgetParam(((int16_t)(type << 8) | index), param);
}

bool MatesController::setBufferSize(uint16_t size) {
  if (size > __MATES_STRING_MAX_BUFFER_SIZE__) return false;
  matesBufferSize = size;
  return true;
}

bool MatesController::clearTextArea(uint16_t index) {
  if (debugSerial != NULL) {
    debugSerial->write("Clear TextArea");
    debugSerial->print(index);
    debugSerial->write("... ");
  }

  bool res = false;
  if (matesReady) {
    WriteCommand(MATES_CMD_UPDATE_TEXT_AREA);
    WriteWord(index);
    WriteByte(0);
    bool res = WaitForACK();
    if ((debugSerial != NULL) && res) debugSerial->write('\n');
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
  }
  return res;
}

bool MatesController::updateTextArea(uint16_t index, const char * format, ...) {
  if (debugSerial != NULL) {
    debugSerial->write("Update TextArea");
    debugSerial->print(index);
    debugSerial->write("... ");
  }
    
  bool res = false;
  if (matesReady) {
    // char buf[matesBufferSize];
    char * buf = (char *) malloc(matesBufferSize);
    va_list args;
    va_start(args, format);
    vsprintf(buf, format, args);
    
    WriteCommand(MATES_CMD_UPDATE_TEXT_AREA);
    WriteWord(index);
    WriteString(buf);
    res = WaitForACK();
    if ((debugSerial != NULL) && res) debugSerial->write('\n');
    free(buf);
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
  }
  return res;
}

bool MatesController::updateTextArea(uint16_t index, String str) {
  return updateTextArea(index, str.c_str());
}

bool MatesController::clearPrintArea(uint16_t index) {
  if (debugSerial != NULL) {
    debugSerial->write("Clear PrintArea");
    debugSerial->print(index);
    debugSerial->write("... ");
  }  

  bool res = false;
  if (matesReady) {
    WriteCommand(MATES_CMD_CLR_PRINT_AREA);
    WriteWord(index);
    res = WaitForACK();
    if ((debugSerial != NULL) && res) debugSerial->write('\n');
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
  }
  return res;
}

bool MatesController::setPrintAreaColor(uint16_t index, int16_t rgb565) {
  if (debugSerial != NULL) {
    debugSerial->write("Set PrintArea");
    debugSerial->print(index);
    debugSerial->write(" color to 0x");
    debugSerial->print(rgb565, HEX);
    debugSerial->write("... ");
  }

  bool res = false;
  if (matesReady) {
    WriteCommand(MATES_CMD_SET_PRINT_COLOR);
    WriteWord(index);
    WriteWord(rgb565);
    res = WaitForACK();
    if ((debugSerial != NULL) && res) debugSerial->write('\n');
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
  }
  return res; 
}

bool MatesController::setPrintAreaColor(uint16_t index, uint8_t r, uint8_t g, uint8_t b) {
  int16_t rgb565 = 0;
  rgb565 |= (r & 0xF8) << 8;
  rgb565 |= (g & 0xFC) << 3;
  rgb565 |= (b & 0xF8) >> 3;
  return setPrintAreaColor(index, rgb565);
}

bool MatesController::appendToPrintArea(uint16_t index, const int8_t * buf, uint16_t len) {
  if (debugSerial != NULL) {
    debugSerial->write("Append ");
    debugSerial->print(len);
    debugSerial->write(" bytes to PrintArea");
    // debugSerial->write("Append to PrintArea");
    debugSerial->print(index);
    debugSerial->write("... ");
  }

  bool res = false;
  if (matesReady) {
    WriteCommand(MATES_CMD_APPEND_PRINT_AREA);
    WriteWord(index);
    WriteWord(len);
    WriteData(buf, len);
    res = WaitForACK();
    if ((debugSerial != NULL) && res) debugSerial->write('\n');
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
  }
  return res; 
}

bool MatesController::appendToPrintArea(uint16_t index, const char * format, ...) {
  // char buf[matesBufferSize];
  char * buf = (char *) malloc(matesBufferSize);
  va_list args;
  va_start(args, format);
  vsprintf(buf, format, args);
  bool res = appendToPrintArea(index, (int8_t *)buf, strlen(buf));
  free(buf);
  return res;
}

bool MatesController::appendToPrintArea(uint16_t index, String str) {
  return appendToPrintArea(index, str.c_str());
}

bool MatesController::appendToScope(uint16_t index, const int16_t * buf, int16_t len) {
  if (debugSerial != NULL) {
    debugSerial->write("Append to Scope");
    debugSerial->print(index);
    debugSerial->write("... ");
  }

  bool res = false;
  if (matesReady) {
    WriteCommand(MATES_CMD_APPEND_SCOPE_DATA);
    WriteWord(index);
    WriteWord(len);
    WriteData(buf, len);
    res = WaitForACK();
    if ((debugSerial != NULL) && res) debugSerial->write('\n');
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
  }
  return res; 
}

bool MatesController::updateDotMatrix(uint16_t index, const int8_t * buf, uint16_t len) {
  if (debugSerial != NULL) {
    debugSerial->write("Update DotMatrix");
    debugSerial->print(index);
    debugSerial->write("... ");
  }

  bool res = false;
  if (matesReady) {
    WriteCommand(MATES_CMD_UPDATE_DOT_MATRIX);
    WriteWord(index);
    WriteWord(len);
    WriteData(buf, len);
    res = WaitForACK();
    if ((debugSerial != NULL) && res) debugSerial->write('\n');
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
  }
  return res; 
}

bool MatesController::updateDotMatrix(uint16_t index, const char * format, ...) {
  // char buf[matesBufferSize];
  char * buf = (char *) malloc(matesBufferSize);
  va_list args;
  va_start(args, format);
  vsprintf(buf, format, args);
  bool res = updateDotMatrix(index, (int8_t *)buf, strlen(buf));
  free(buf);
  return res;
}

bool MatesController::updateDotMatrix(uint16_t index, String str) {
  return updateDotMatrix(index, str.c_str());
}

uint16_t MatesController::getButtonEventCount() {
  if (debugSerial != NULL) {
    debugSerial->write("Get number of recorded button events... ");
  }
  if (matesReady) {
    WriteCommand(MATES_CMD_BTN_EVENT_COUNT);
    return (uint16_t) ReadResponse();
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
    return -1;
  }
}

int16_t MatesController::getNextButtonEvent() {
  if (debugSerial != NULL) {
    debugSerial->write("Query next button event... ");
  }
  if (matesReady) {
    WriteCommand(MATES_CMD_NEXT_BTN_EVENT);
    return ReadResponse();
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
    return -1;
  }
}

uint16_t MatesController::getSwipeEventCount() {
  if (debugSerial != NULL) {
    debugSerial->write("Get number of recorded swipe events... ");
  }
  if (matesReady) {
    WriteCommand(MATES_CMD_SWP_EVENT_COUNT);
    return (uint16_t) ReadResponse();
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
    return -1;
  }
}

int16_t MatesController::getNextSwipeEvent() {
  if (debugSerial != NULL) {
    debugSerial->write("Query next swipe event... ");
  }
  if (matesReady) {
    WriteCommand(MATES_CMD_NEXT_SWP_EVENT);
    return ReadResponse();
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
    return -1;
  }
}

bool MatesController::pinMode(int16_t pin, int16_t mode) {
  if (debugSerial != NULL) {
    debugSerial->write("Setting pin ");
    debugSerial->print(pin);
    debugSerial->write(" to mode ");
    debugSerial->print(mode);
    debugSerial->write("... ");
  }
  if (matesReady) {
    WriteCommand(MATES_CMD_PIN_MODE);
    WriteWord(pin);
    WriteWord(mode);
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
  }
  return matesReady;
}

bool MatesController::digitalWrite(int16_t pin, int16_t value) {
  if (debugSerial != NULL) {
    debugSerial->write("Setting pin ");
    debugSerial->print(pin);
    debugSerial->write(" to value ");
    debugSerial->print(value);
    debugSerial->write("... ");
  }
  if (matesReady) {
    WriteCommand(MATES_CMD_DIGITAL_WRITE);
    WriteWord(pin);
    WriteWord(value);
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
  }
  return matesReady;
}

int16_t MatesController::digitalRead(int16_t pin) {
  if (debugSerial != NULL) {
    debugSerial->write("Query value of pin ");
    debugSerial->print(pin);
    debugSerial->write("... ");
  }
  if (matesReady) {
    WriteCommand(MATES_CMD_DIGITAL_READ);
    WriteWord(pin);
    return ReadResponse();
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
    return -1;
  }
}

String MatesController::getVersion() {
  return __MATES_CONTROLLER_LIBRARY_VERSION__;
}

String MatesController::getCompatibility() {
  return __MATES_STUDIO_COMPATIBILITY_VERSION__;
}

void MatesController::printVersion() {
  if (debugSerial != NULL) {
    debugSerial->write("Mates Studio - Compatible Version  : ");
    debugSerial->write(__MATES_STUDIO_COMPATIBILITY_VERSION__);
    debugSerial->write('\n');
    debugSerial->write("Mates Controller - Library Version : ");
    debugSerial->write(__MATES_CONTROLLER_LIBRARY_VERSION__);
    debugSerial->write('\n');
  }
}

MatesError MatesController::getError() {
  return matesError;
}

// -----------------------------------------------------------
// ------------------- PRIVATE FUNCTIONS ---------------------
// -----------------------------------------------------------

void MatesController::WriteCommand(MatesCommand value) {
  matesSerial->write(MATES_CMD_START_BYTE);
  matesSerial->write(value >> 8);
  matesSerial->write(value);
}

void MatesController::WriteByte(int8_t value) {
  matesSerial->write(value);
}

void MatesController::WriteWord(int16_t value) {
  matesSerial->write(value >> 8);
  matesSerial->write(value);
}

void MatesController::WriteLong(int32_t value) {
  matesSerial->write(value >> 24);
  matesSerial->write(value >> 16);
  matesSerial->write(value >> 8);
  matesSerial->write(value);
}

void MatesController::WriteFloat(float value) {
  long * longPtr = (long*) &value ; // cast float to long
  WriteLong(*longPtr);
}

void MatesController::WriteString(const char * str) {
  do {
    matesSerial->write(*str);
  } while (*str++);
}

void MatesController::WriteData(const int8_t * buf, int len) {
  for (int i = 0; i < len; i++) {
    matesSerial->write(buf[i]);
  }
}

void MatesController::WriteData(const int16_t * buf, int len) {
  for (int i = 0; i < len; i++) {
    matesSerial->write(buf[i] >> 8);
    matesSerial->write(buf[i]);
  }
}

int16_t MatesController::ReadWord() {
  int16_t value = matesSerial->read();
  value = (value << 8) | (matesSerial->read());
  return value;
}

int16_t MatesController::ReadResponse(uint16_t timeout) {
  if (!WaitForACK()) return -1;
  unsigned long startTime = millis();
  while (matesSerial->available() < 2) {
    if (millis() - startTime >= timeout) {
      if (debugSerial != NULL) debugSerial->write("RSP Timeout\n");
      matesError = MATES_ERROR_RESPONSE_TIMEOUT;
      return -1;
    }
  }
  int16_t res = ReadWord();
  if (debugSerial != NULL) debugSerial->println(res);
  return res;
}

bool MatesController::WaitForACK(uint16_t timeout) {
  unsigned long startTime = millis();
  while (matesSerial->available() < 1) {
    if (millis() - startTime >= timeout) {
      matesError = MATES_ERROR_COMMAND_TIMEOUT;
      if (debugSerial != NULL) debugSerial->write("CMD Timeout\n");
      return false;
    }
  }
  bool res = (matesSerial->read()) == 0x06;
  matesError = res ? MATES_ERROR_NONE : MATES_ERROR_COMMAND_FAILED;
  if (debugSerial != NULL) debugSerial->write(res ? "Success" : "Failed\n");
  return res;
}

bool MatesController::_setWidgetValue(int16_t widget, int32_t value) {
  if (debugSerial != NULL) {
    debugSerial->write("Set Widget (0x");
    debugSerial->print(widget, HEX);
    debugSerial->write(") value to ");
    debugSerial->print(value);
    debugSerial->write("... ");
  }

  bool res = false;
  if (matesReady) {
    WriteCommand(MATES_CMD_SET_WIDGET_32VAL);
    WriteWord(widget);
    WriteLong(value);
    res = WaitForACK();
    if ((debugSerial != NULL) && res) debugSerial->write('\n');
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
  }
  return res;
}

bool MatesController::_setWidgetValue(int16_t widget, float value) {
  if (debugSerial != NULL) {
    debugSerial->write("Set Widget (0x");
    debugSerial->print(widget, HEX);
    debugSerial->write(") value to ");
    debugSerial->print(value);
    debugSerial->write("... ");
  }

  bool res = false;
  if (matesReady) {
    WriteCommand(MATES_CMD_SET_WIDGET_32VAL);
    WriteWord(widget);
    WriteFloat(value);
    res = WaitForACK();
    if ((debugSerial != NULL) && res) debugSerial->write('\n');
  } else {
    matesError = MATES_ERROR_NOT_INITIALIZED;
    if (debugSerial != NULL) debugSerial->write('Not Ready\n');
  }
  return res;
}