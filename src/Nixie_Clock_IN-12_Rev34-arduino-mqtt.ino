#include "./Wifi/WiFiTask.h"

// Real time clock
RTC_DS3231 rtc;

// DC booster enable pin control
HV hv;

// Variables and settings
Settings settings;

// HV shift registers
NixieDisplay nixie(&settings);

// PWM fade functions
Fade fade(&settings);

// I2C devices
I2C i2c(&nixie, &rtc, &settings, &hv);

// Tasks that run periodically
TimeTask timetask(&nixie, &i2c, &settings, &hv);

// HTTP Server Port
WiFiServer server(80);

// Initialise UDP
WiFiUDP ntpUDP;

// Initialise NTP Client
NTPClient timeclient(ntpUDP);

// WiFi functions
WiFiTask wifitask(&nixie, &timetask, &i2c, &timeclient, &server, &settings, &fade, &hv);

void setup() {

  // Start Serial
  Serial.begin(57600);
  delay(2000);

  // Generate random seed for random number generation
  randomSeed(analogRead(RandomSeed));

  // Load all default/saved settings from Flash
  settings.debug(0);
  settings.begin();

  // Initialise the Colon and Switch LEDs
  //settings.debug(0);
  fade.start();
  
  // Initiate the HV5532 outputs
  settings.debug(1);
  nixie.begin();

  // Fire up the 170V and 5 to 12V booster if needed
  settings.debug(2);
  hv.begin(settings.flashUSB);

  // Take the Nixie tubes for a quick spin
  nixie.runSlotMachine(1);

  // Display the firmware version
  nixie.displayFW();

  // Initialise DS3231 RTC
  settings.debug(3);
  i2c.rtcBegin();
  if (settings.I2C_CODE[0]) {
    settings.debug(4);
  } else {
    settings.debug(5);
  }

  // Connect to wifi SSID
  settings.debug(10);
  wifitask.connectWiFi();

  // Start the builtin AP if Wi-Fi connection has failed
  if (wifitask.WiFiFail) {
    wifitask.startAP();
  }

  // Start the NTP client task
  settings.debug(11);
  wifitask.ntpBegin();

  // Start the Web Server, update the RTC via NTP and display the IP address of the device
  if (!wifitask.WiFiFail) {
    settings.debug(12);
    wifitask.serverBegin();

    settings.debug(13);
    wifitask.getNTP();
  
    settings.debug(14);
    wifitask.displayIP(); 
  }

  // Start MQTT Client
  wifitask.connectMQTT();
}

int t = 0;
unsigned long lastMillis = 0;

void loop() {

  // Using millis to call timed functions, you can use interrupts if more accuracy is required
  settings.currentTime = millis();

  // publish a message roughly every second.
  if ((unsigned long)(settings.currentTime - lastMillis) >= 1000) {
    lastMillis = settings.currentTime;
    wifitask.displaystatusMQTT();
  }
  // receive MQTT Commands
  wifitask.MQTTmessage();

  // Main time display function
  if (settings.currentTime - settings.previousTime_Time >= settings.eventTime_Time) {
    timetask.task();
    settings.previousTime_Time = settings.currentTime;
  }
  // WiFi Server function
  if (settings.currentTime - settings.previousTime_Server >= settings.eventTime_Server) {
    wifitask.clientServer();
    settings.previousTime_Server = settings.currentTime;
  }
}
