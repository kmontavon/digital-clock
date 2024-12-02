#ifndef Settings_h
#define Settings_h

#include "Arduino.h"
#include "Globals.h"

#include "Secrets.h"                   // This file holds the AP WiFi name and password

// All the extra downloaded libraries go here (current code tested with the following versions)
#include <WiFiNINA.h>                  // v1.8.13 WiFi Library - https://github.com/arduino-libraries/WiFiNINA
#include <WiFiUdp.h>                   // v1.8.13 WiFi UDP library - https://github.com/arduino-libraries/WiFiNINA
#include <NTPClient.h>                 // v3.2.1  NTP Client Library - https://github.com/arduino-libraries/NTPClient
#include <FlashStorage.h>              // v1.0.0  Use Flash Memory as EEPROM - Nano 33 has no EEPROM :( - https://github.com/cmaglie/FlashStorage
#include <RTClib.h>                    // v2.0.3  DS3231 RTC Library - https://github.com/adafruit/RTClib
#include <AceButton.h>                 // v1.9.2  Button library - https://github.com/bxparks/AceButton
#include <MQTT.h>                      // v2.5.2  MQTT - https://github.com/256dpi/arduino-mqtt

// Required by AceButton.h
using namespace ace_button;

// Create a structures to store the WiFi credentials and all other settings into flash
// NOTE: This is lost when a new sketch is uploaded, you can write to flash only 10,000 times before failure occurs
typedef struct {bool valid; char flash_SSID[100]; char flash_PASS[100];} savedWiFi;         // WiFi credentials
typedef struct {bool validBrightness; byte flashBrightness;} savedBrightness;               // Nixie PWM Brightness
typedef struct {bool validNTP; bool flashNTP;} savedNTP;                                    // Enable/Disable NTP
typedef struct {bool validNTPPool; byte flashNTPPool;} savedNTPPool;                        // NTP pool selection
typedef struct {bool validOnOffHour; byte flashOnHour; byte flashOffHour;} savedOnOffHour;  // On/Off Hour
typedef struct {bool validUSB; bool flashUSB;} savedUSB;                                    // USB/12V DC Toggle
typedef struct {bool validFont; byte flashFont;} savedFont;                                 // WebUI Font selection
typedef struct {bool validBackground; byte flashBackground;} savedBackground;               // WebUI Background selection
typedef struct {bool validUTCOffset; int flashUTCOffset;} savedUTCOffset;                   // UTC Offset
typedef struct {bool validColon; byte flashColon;} savedColon;                              // Colon PWM level
typedef struct {bool validLED; byte flashLED1; byte flashLED2; byte flashLED3;} savedLED;   // Switch LED PWM level
typedef struct {bool validSpin; byte flashSpin;} savedSpin;                                 // Number of spins every minute
typedef struct {bool validLux; byte flashLux;} savedLux;                                    // Low level light trigger value
typedef struct {bool validName; char flashTitle[50]; char flashName[50];} savedName;        // WebUI Title and name

class Settings {
  public:
    Settings();

    const char* fwVersion = "1.00";                           // Firmware version for display purposes
    float fwVersion2 = 1.00;                                  // Firmware version for display purposes

    // Changeable global variables
    const unsigned long eventTime_Time = 200;                 // Event time for time functions
    const unsigned long eventTime_Light = 3001;               // Event time for light functions
    const unsigned long eventTime_Server = 2003;              // Event time for Server functions
    const unsigned long eventTime_OnOffHour = 6000;           // Event time for auto on/off

    unsigned long previousTime_Time = 0;                      // Event start time for time functions
    unsigned long previousTime_Light = 0;                     // Event start time for light functions
    unsigned long previousTime_Server = 0;                    // Event start time for Server functions
    unsigned long previousTime_OnOffHour = 0;                 // Event start time for auto on/off


    const char* webTitle = "Arduino Nano 33 IoT Nixie Clock"; // WebUI Header Title First Part
    const char* webName = "Living Room";                      // WebUI Header Title Second Part / Room Name
    const char* webFont = "Audiowide";                        // WebUI Initial Font

    const int longpressduration = 5000;                       // Time in ms for button long press duration (used by AceButton.h)

    // Global variables
    unsigned long currentTime;                                // Used by millis

    bool I2C_CODE[3];                                         // Hold startup status of I2C devices
    byte hour, minute, second, day, month, year;              // Store RTC data

    // Flash EEPROM initial save values, all these settings are changed/saved via the WebUI
    String flash_SSID;                                        // Store SSID retrieved from the WebUI
    String flash_PASS;                                        // Store Password retrieved from the WebUI
    String flashTitle;                                        // Store WebUI Title retrieved from the WebUI
    String flashName;                                         // Store WebUI Name retrieved from the WebUI
    byte flashBrightness = 255;                               // Store PWM HV5530 brightness level 1 - 255 (Note: 0 would turn off the Nixies but leave the HV on)
    bool flashNTP = 1;                                        // Store Enable/Disable NTP boolean
    byte flashNTPPool = 3;                                    // Store NTP pool address (1 = africa, 2 = asia, 3 = europe, 4 = north america, 5 = oceania, 6 = south america)
    byte flashOnHour = 6;                                     // Store display On Hour
    byte flashOffHour = 23;                                   // Store display Off hour
    bool flashUSB = 1;                                        // Set power supply mode, setting to 1 enables the 5v to 12v booster for the HV5530
    byte flashFont = 1;                                       // The WebUI font
    byte flashBackground = 4;                                 // The WebUI background
    byte flashUTCOffset = +1;                                  // UTC offset in hours
    byte flashColon = 15;                                     // Colon LED Brightness via PWM
    byte flashLED1 = 0;                                     // Switch 1 LED Brightness via PWM (note these values can be the same if the correct resistors are used for the LEDs)
    byte flashLED2 = 0;                                      // Switch 2 LED Brightness via PWM (note these values can be the same if the correct resistors are used for the LEDs)
    byte flashLED3 = 0;                                     // Switch 3 LED Brightness via PWM (note these values can be the same if the correct resistors are used for the LEDs)
    byte flashSpin = 1;                                       // Nixie Spin Cycles per minute
    byte flashLux = 1;                                        // Low Lux Level Threshold

    const char* ntpServerName = "europe.pool.ntp.org";        // Stores the NTP pool to query, changed via flashNTPPool
    
    const char* ssid;                                         // Hold the SSID
    const char* pass;                                         // Hold the SSID Password

    bool noSSID = 0;                                          // Holds WiFi failure state

    void begin();
    void rwSettings(byte setting, bool save);
    void OnOff();
    void debug(byte n);

  private:
};

#endif
