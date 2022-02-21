//
// SpaIot Simple Example with custom hardware settings
//
// This example displays the temperature of the water and resets the filtration
// of the spa, if it is started, to prevent the filtration from stopping
// after 24 hours.
#include <Arduino.h>
#include <SpaIot.h>

using namespace SpaIot;


const unsigned long TimerTime = (12 * 60 * 60); // 12 hours in seconds
const unsigned long SerialBaudrate = 115200;

// My bus configuration :
// SCLK   -> GPIO12
// SDATA  -> GPIO14
// nWR    -> GPIO13
const BusSettings MyBus (12, 14, 13);
// My leds configuration (SSP)
const std::map<int, LedSettings> MyLeds = {
  { Power,          LedSettings (0) },  // Power        -> A0
  { Heater,         LedSettings (7) },  // Heater       -> A7
  { HeatReached,    LedSettings (9) },  // HeatReached  -> B1
  { Bubble,         LedSettings (10) }, // Bubble       -> B2
  { Filter,         LedSettings (12) }  // Filter       -> B4
};
// My button controllers
Cd4051 CustomCtrlA (5, 4, 15, 16); // S0->GPIO5, S1->GPIO4, S2->GPIO15, En->GPIO16
Cd4051 CustomCtrlB (5, 4, 15, 0);  // S0->GPIO5, S1->GPIO4, S2->GPIO15, En->GPIO0
// My buttons configuration (SSP)
const std::map<int, ButtonSettings> MyButtons = {
  { Filter,   ButtonSettings ("MyCtrlA", 1) },  // Filter   -> A1
  { Bubble,   ButtonSettings ("MyCtrlA", 3) },  // Bubble   -> A3
  { TempDown, ButtonSettings ("MyCtrlA", 7) },  // TempDown -> A7

  { Power,    ButtonSettings ("MyCtrlB", 2) },  // Power    -> B2
  { TempUp,   ButtonSettings ("MyCtrlB", 4) },  // TempUp   -> B4
  { TempUnit, ButtonSettings ("MyCtrlB", 5) },  // TempUnit -> B5
  { Heater,   ButtonSettings ("MyCtrlB", 7) }   // Heater   -> B7
};
// My custom configuration
const HardwareSettings MyConfig (MyBus, MyLeds, MyButtons);

ControlPanel * spa; // pointer on the control panel
unsigned long timer;
uint16_t waterTemp;

void setup() {

  delay (2000);
  Serial.begin (SerialBaudrate);
  Serial.println ("\nSpaIot Simple Example with custom hardware settings");

  // The button controllers must be registered before getInstance() call
  ButtonController::addToRegister ("MyCtrlA", CustomCtrlA);
  ButtonController::addToRegister ("MyCtrlB", CustomCtrlB);

  spa = ControlPanel::getInstance (MyConfig);

  spa->begin();  // IMPORTANT LINE!
  if (spa->isOpened() == false) { // check if the connection to the spa has been open
    Serial.println ("No spa connection found");
    for(;;); // loop always, to stop 
  }
  spa->setPower (true); // Comment this line if the spa is already started

  // Wait to read the temperature of the water, it can take 20 seconds ...
  waterTemp = spa->waitForWaterTemp();
  Serial.printf ("waterTemp=%d'C\n", waterTemp);
  Serial.printf ("Waiting to reset filter every %lu seconds...", TimerTime);
}


void loop() {

  uint16_t t = spa->waterTemp(); // Reading the temperature of the water
  if (waterTemp != t) { // If modified, it is displayed
    waterTemp = t;
    Serial.printf ("waterTemp=%d'C\n", waterTemp);
  }

  if (timer >= TimerTime) { // If the time has been reached,
    if (spa->isFilterOn()) { // you check if the filtering is running,

      Serial.println ("I rearm the spa filter");
      // if so, it is stopped and restarts.
      spa->setFilter (false);
      delay (1000);
      spa->setFilter (true);
    }
  }

  delay (1000);
}
