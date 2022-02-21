#ifndef __MatesController_H__
#define __MatesController_H__

#ifdef AVR
#include <SoftwareSerial.h> // uncomment this line to add direct support for SoftwareSerial
// #include <AltSoftSerial.h> // uncomment this line to add direct support for AltSoftSerial (Note: AltSoftSerial needs to be installed in Arduino IDE)
#endif

#include "includes/MatesErrors.h"
#include "includes/MatesWidgets.h"
#include "includes/MatesCommands.h"
#include "includes/MatesPinConsts.h"
#include "includes/MatesColorConsts.h"
#include "includes/MatesSwipeConsts.h"
#include "includes/MatesWidgetConsts.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#include "WConstants.h"
#endif
#include <stdarg.h>

#define __MATES_STUDIO_COMPATIBILITY_VERSION__  "1.0.9"
#define __MATES_CONTROLLER_LIBRARY_VERSION__    "1.0.5"

#define __MATES_PRT_LENGTH__                    1000 
#define __MATES_STR_LENGTH__                    255 
#define __MATES_STR_BFSIZE__                    (__MATES_STR_LENGTH__ + 1)
#define __MATES_CMD_TIMEOUT__                   1000
#define __MATES_BOOT_TIMEOUT__                  5000

/*
 * RELEASE NOTES
 *   1.0.0: September 16, 2021
 *      Initial release
 *   1.0.1: September 23, 2021
 *      Updated examples
 *      Minor fixes to library
 *   1.0.2: September 24, 2021
 *      Changed library name
 *   1.0.3: September 29, 2021
 *      Added bool updateTextArea(uint16_t index, String str)
 *      Added bool appendToPrintArea(uint16_t index, String str)
 *      Added bool updateDotMatrix(uint16_t index, String str)
 *      Fixed missing bool setLedDigitsValue(uint8_t index, int16_t value)
 *   1.0.4: January 22, 2021
 *      Added int16_t getButtonEventCount()
 *      Added int16_t getNextButtonEvent()
 *      Added int16_t getSwipeEventCount()
 *      Added int16_t getNextSwipeEvent()
 *   1.0.5: February 8, 2021
 *      Fixed print area issue when sending long strings
 */

typedef void (*MatesErrorHandler) (MatesError);

typedef enum {
  NO_SERIAL,
  HARDWARE_SERIAL,
  SOFTWARE_SERIAL,
  ALTSOFT_SERIAL,
  UNKNOWN_SERIAL
} SerialType;

union FrameFloat {
    float value;
    int8_t bytes[4];
};

union Frame32bit {
    int32_t value;
    int8_t bytes[4];
};

union Frame16bit {
    int16_t value;
    int8_t bytes[2];
};

union Frame8bit {
    int8_t value;
    int8_t bytes[1];
};

class MatesController {

  public:

    MatesController(Stream &serial, uint8_t resetPin = 4, uint8_t mode = LOW);
    MatesController(Stream &serial, Stream &dbSerial, uint8_t resetPin = 4, uint8_t mode = LOW);
    MatesController(HardwareSerial &serial, uint8_t resetPin = 4, uint8_t mode = LOW);
    MatesController(HardwareSerial &serial, Stream &dbSerial, uint8_t resetPin = 4, uint8_t mode = LOW);
#ifdef SoftwareSerial_h
    MatesController(SoftwareSerial &serial, uint8_t resetPin = 4, uint8_t mode = LOW);
    MatesController(SoftwareSerial &serial, Stream &dbSerial, uint8_t resetPin = 4, uint8_t mode = LOW);
#endif
#ifdef AltSoftSerial_h
    MatesController(AltSoftSerial &serial, uint8_t resetPin = 4, uint8_t mode = LOW);
    MatesController(AltSoftSerial &serial, Stream &dbSerial, uint8_t resetPin = 4, uint8_t mode = LOW);
#endif

    // HERE ARE THE FUNCTIONS AVAILABLE TO THE USER

    bool begin(int32_t baudrate = 9600, bool resetModule = true);
    bool reset(uint16_t waitPeriod = 0);
    bool softReset(uint16_t waitPeriod = 0);
    void setBootTimeout(uint16_t timeout);
    void resetBootTimeout();
    void attachErrorHandler(MatesErrorHandler handler);
    void detachErrorHandler();

    // Non-widget functions
    bool setBacklight(uint8_t value);
    bool setPage(uint16_t page);
    int16_t getPage();

    // Common widget functions
    bool setWidgetValue(int16_t widget, int16_t value);
    int16_t getWidgetValue(int16_t widget);
    bool setWidgetValue(MatesWidget type, uint8_t index, int16_t value);
    int16_t getWidgetValue(MatesWidget type, uint8_t index);

    // Widget-specific functions
    bool setLedDigitsValue(uint8_t index, int16_t value);
    bool setLedDigitsValue(uint8_t index, int32_t value);
    bool setLedDigitsValue(uint8_t index, float value);
    bool setSpectrumValue(int16_t widget, uint8_t gaugeIndex, uint8_t value);
    bool setLedSpectrumValue(uint8_t index, uint8_t gaugeIndex, uint8_t value);
    bool setMediaSpectrumValue(uint8_t index, uint8_t gaugeIndex, uint8_t value);

    // Non-Image (GCI) common widget functions
    bool setWidgetParam(int16_t widget, int16_t param, int16_t value);
    int16_t getWidgetParam(int16_t widget, int16_t param);
    bool setWidgetParam(MatesWidget type, uint8_t index, int16_t param, int16_t value);
    int16_t getWidgetParam(MatesWidget type, uint8_t index, int16_t param);

    // PrintArea support functions
    bool setBufferSize(uint16_t size);

    // TextArea functions    
    bool clearTextArea(uint16_t index);
    bool updateTextArea(uint16_t index, const char * format, ...);
    bool updateTextArea(uint16_t index, String str);

    // PrintArea functions
    bool clearPrintArea(uint16_t index);
    bool setPrintAreaColor(uint16_t index, int16_t rgb565);
    bool setPrintAreaColor(uint16_t index, uint8_t r, uint8_t g, uint8_t b);
    bool appendToPrintArea(uint16_t index, const int8_t * buf, uint16_t len);
    bool appendToPrintArea(uint16_t index, const char * format, ...);
    bool appendToPrintArea(uint16_t index, String str);

    // Scope functions
    bool appendToScope(uint16_t index, const int16_t * buf, uint16_t len);

    // Dot Matrix functions
    bool updateDotMatrix(uint16_t index, const char * format, ...);
    bool updateDotMatrix(uint16_t index, String str);

    // Button functions
    uint16_t getButtonEventCount();
    int16_t getNextButtonEvent();

    // Swipe functions
    uint16_t getSwipeEventCount();
    int16_t getNextSwipeEvent();

    // GPIO functions
    bool pinMode(int16_t pin, int16_t mode);
    bool digitalWrite(int16_t pin, int16_t value);
    int16_t digitalRead(int16_t pin);

    // Utility functions
    String getVersion();
    String getCompatibility();
    void printVersion();
    MatesError getError();

  private:
  
    SerialType matesSerialType;
    Stream * matesSerial = NULL;
    Stream * debugSerial = NULL;

    bool matesReady = false;
    MatesError matesError = MATES_ERROR_NONE;
    MatesErrorHandler matesErrorHandler = NULL;
    uint16_t matesBufferSize = __MATES_STR_LENGTH__;
    uint16_t matesBootTimeout = __MATES_BOOT_TIMEOUT__;
    uint16_t matesTxDelay = 0;
    uint8_t matesResetPin = 4;
    uint8_t matesResetMode = LOW;

    HardwareSerial * hwMatesSerial = NULL;

#ifdef SoftwareSerial_h
    SoftwareSerial * swMatesSerial = NULL;
#endif

#ifdef AltSoftSerial_h
    AltSoftSerial  * asMatesSerial = NULL;
#endif

    void SetError(MatesError error);
    void WriteCommand(MatesCommand value);
    void WriteByte(int8_t value);
    void WriteWord(int16_t value);
    void WriteLong(int32_t value);
    void WriteFloat(float value);
    void WriteString(const char * str);
    void WriteData(const int8_t * buf, uint16_t len, bool invert = false);
    void WriteData(const int16_t * buf, uint16_t len);

    bool WaitForACK(uint16_t timeout = __MATES_CMD_TIMEOUT__);
    int16_t ReadWord();
    int16_t ReadResponse(uint16_t timeout = __MATES_CMD_TIMEOUT__);

    // Widget Support Functions
    bool _setWidgetValue(int16_t widget, int32_t value);
    bool _setWidgetValue(int16_t widget, float value);
    bool updateDotMatrix(uint16_t index, const int8_t * buf, uint16_t len);

};

#endif
// __MatesController_H__
