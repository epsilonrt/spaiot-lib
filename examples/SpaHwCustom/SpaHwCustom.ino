//
// SpaIot Simple Example with custom hardware settings
//
// This example displays the temperature of the water and resets the filtration
// of the spa, if it is started, to prevent the filtration from stopping
// after 24 hours.
#include <Arduino.h>
#include <SpaIot.h>

using namespace SpaIot;

const unsigned long SerialBaudrate = 115200;
const unsigned long TimerTime = 60;
//const unsigned long TimerTime = (12 * 60 * 60); // 12 hours in seconds

// My bus configuration :
#if defined(ESP8266)
// SDATA  -> GPIO12
// SCLK   -> GPIO14
// nWR    -> GPIO13
const BusSettings MyBus (12, 14, 13);

// My button controllers
Cd4051 BtnCtrlA ("U3", 5, 4, 15, 16); // A->GPIO5, B->GPIO4, C->GPIO15, INH->GPIO16
Cd4051 BtnCtrlB ("U4", 5, 4, 15, 0);  // A->GPIO5, B->GPIO4, C->GPIO15, INH->GPIO0

#elif defined(ESP32)
// SDATA  -> GPIO23
// SCLK   -> GPIO18
// nWR    -> GPIO19
const BusSettings MyBus (23, 18, 19);

// My button controllers
Cd4051 BtnCtrlA ("U3", 27, 16, 17, 25); // A->GPIO27, B->GPIO16, C->GPIO17, INH->GPIO25
Cd4051 BtnCtrlB ("U4", 27, 16, 17, 26); // A->GPIO27, B->GPIO16, C->GPIO17, INH->GPIO26

#else
#error unsupported platform
#endif

// My buttons configuration (SSP)
const std::map<int, ButtonSettings> MyButtons = {
  { Filter,   ButtonSettings (BtnCtrlA, 1) },  // Filter   -> A1
  { Bubble,   ButtonSettings (BtnCtrlA, 3) },  // Bubble   -> A3
  { TempDown, ButtonSettings (BtnCtrlA, 7) },  // TempDown -> A7

  { Power,    ButtonSettings (BtnCtrlB, 2) },  // Power    -> B2
  { TempUp,   ButtonSettings (BtnCtrlB, 4) },  // TempUp   -> B4
  { TempUnit, ButtonSettings (BtnCtrlB, 5) },  // TempUnit -> B5
  { Heater,   ButtonSettings (BtnCtrlB, 7) }   // Heater   -> B7
};
// My custom configuration
const HardwareSettings MyConfig (MyBus, SspLeds, MyButtons);

ControlPanel & spa = ControlPanel::singleton (MyConfig);

unsigned long timer;
uint16_t waterTemp;

void setup() {

  delay (2000);
  Serial.begin (SerialBaudrate);
  Serial.println ("\nSpaIot Simple Example with custom hardware settings");

  spa.begin();  // IMPORTANT LINE!
  if (spa.isOpened() == false) { // check if the connection to the spa has been open
    Serial.println ("No spa connection found");
    for (;;); // loop always, to stop
  }
  spa.setPower (true); // Comment this line if the spa is already started

  // Wait to read the temperature of the water, it can take 20 seconds ...
  waterTemp = spa.waitForWaterTemp();
  Serial.printf ("waterTemp=%d'C\n", waterTemp);
  Serial.printf ("Waiting to rearm filter every %lu seconds...", TimerTime);
}

void loop() {

  uint16_t t = spa.waterTemp(); // Reading the temperature of the water
  if (waterTemp != t) { // If modified, it is displayed
    waterTemp = t;
    Serial.printf ("waterTemp=%d'C\n", waterTemp);
  }

  if (spa.isFilterOn()) { // you check if the filtering is running,

    if (timer++ >= TimerTime) { // If the time has been reached,

      Serial.println ("I rearm the spa filter");
      // if so, it is stopped and restarts.
      spa.setFilter (false);
      delay (1000);
      spa.setFilter (true);
      timer = 0;
    }
  }
  else {

    timer = 0;
  }

  delay (1000);
}
