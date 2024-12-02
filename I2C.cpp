#include "I2C.h"

I2C::I2C(NixieDisplay* nixie, RTC_DS3231* rtc, Settings* settings, HV* hv) {
  _nixie = nixie;
  _rtc = rtc;
  _settings = settings;
  _hv = hv;
}

void I2C::rtcBegin() {
  Wire.begin();

  // Start the DS3231 RTC
  if (!_rtc->begin()) {
    _settings->I2C_CODE[0] = 1;
  } else {
    _settings->I2C_CODE[0] = 0;
  }
}

// Read time from DS3231
void I2C::readTime() {
  DateTime now = _rtc->now();
  _settings->hour = now.hour();
  _settings->minute = now.minute();
  _settings->second = now.second();
}

// Read date from DS3231
void I2C::readDate() {
  DateTime now = _rtc->now();
  _settings->day = now.day();
  _settings->month = now.month();
  _settings->year = now.year() % 100;
}

// Adjust DS3231 date and time using epoch
void I2C::adjustEpoch(unsigned long epoch) {
  _rtc->adjust(DateTime(epoch));
  SP("Setting DS3231 Date and Time using EPOCH");
}

// Adjust DS3231 date/time using user input values
void I2C::adjustDateTime(byte dt) {
  if (dt == 1) { // If dt = 1, then only set date
    readTime();
  } else if (dt == 2) {  // If dt = 2, then only set time
    readDate();
  }
  _rtc->adjust(DateTime(_settings->year, _settings->month, _settings->day, _settings->hour, _settings->minute, _settings->second));
  SPL("Setting date/time manually updated");
}
