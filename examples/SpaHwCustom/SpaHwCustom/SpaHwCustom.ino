//
// SpaIot Simple Example with custom hardware settings
//
// This example displays the temperature of the water and resets the filtration
// of the spa, if it is started, to prevent the filtration from stopping
// after 24 hours.
#include <Arduino.h>
#include <SpaIot.h>
#include "MyBoardSettings.h"

using namespace SpaIot;

const unsigned long SerialBaudrate = 115200;
const unsigned long TimerTime = 60;

// My buttons configuration (SSP)
// MuxA and MuxB are defined in MyBoardSettings.h
const std::map<int, ButtonSettings> MyButtons = {
  { Filter,   ButtonSettings (MuxA, 1) },  // Filter   -> A1
  { Bubble,   ButtonSettings (MuxA, 3) },  // Bubble   -> A3
  { TempDown, ButtonSettings (MuxA, 7) },  // TempDown -> A7

  { Power,    ButtonSettings (MuxB, 2) },  // Power    -> B2
  { TempUp,   ButtonSettings (MuxB, 4) },  // TempUp   -> B4
  { TempUnit, ButtonSettings (MuxB, 5) },  // TempUnit -> B5
  { Heater,   ButtonSettings (MuxB, 7) }   // Heater   -> B7
};

// My custom configuration
// MyBus is defined in MyBoardSettings.h
// SspLeds is a SpaIot object that manages the LEDs of the SSP
const HardwareSettings MyConfig (MyBus, SspLeds, MyButtons);

// My Spa Control Panel
ControlPanel spa (MyConfig);

unsigned long timer;
uint16_t waterTemp;

void setup() {

  delay (2000);
  Console.begin (SerialBaudrate);
  Console.println ("\nSpaIot Simple Example with custom hardware settings");

  spa.begin();  // IMPORTANT LINE!
  if (spa.isOpen() == false) { // check if the connection to the spa has been open
    Console.println ("No spa connection found");
    for (;;); // loop always, to stop
  }
  spa.setPower (true); // Comment this line if the spa is already started

  // Wait to read the temperature of the water, it can take 20 seconds ...
  waterTemp = spa.waitForWaterTemp();
  Console.printf ("waterTemp=%d'C\n", waterTemp);
  Console.printf ("Waiting to rearm filter every %lu seconds...\n", TimerTime);
}

void loop() {

  uint16_t t = spa.waterTemp(); // Reading the temperature of the water
  if (waterTemp != t) { // If modified, it is displayed
    waterTemp = t;
    Console.printf ("waterTemp=%d'C\n", waterTemp);
  }

  if (spa.isFilterOn()) { // you check if the filtering is running,

    if (timer++ >= TimerTime) { // If the time has been reached,

      Console.println ("I rearm the spa filter");
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
