#ifndef I2C_h
#define I2C_h

#include "Arduino.h"
#include "Globals.h"
#include "Settings.h"
#include "NixieDisplay.h"
#include "HV.h"

class I2C {
  public:
    I2C(NixieDisplay* nixie, RTC_DS3231* rtc, Settings* settings, HV* hv);
    void rtcBegin();
    void readTime();
    void readDate();
    void adjustEpoch(unsigned long epoch);
    void adjustDateTime(byte dt);
    void readLight();
  private:
    NixieDisplay* _nixie;
    RTC_DS3231* _rtc;
    Settings* _settings;
    HV* _hv;
};

#endif
