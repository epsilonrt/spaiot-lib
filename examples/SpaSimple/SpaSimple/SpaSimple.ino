//
// SpaIot Simple Example
//
// This example displays the temperature of the water and resets the filtration
// of the spa, if it is started, to prevent the filtration from stopping
// after 24 hours.
#include <Arduino.h>
#include <SpaIot.h>
#include "MyBoardSettings.h"

using namespace SpaIot;

const unsigned long SerialBaudrate = 115200;
const unsigned long TimerTime = (12 * 60 * 60); // 12 hours in seconds

// My Spa Control Panel, SpaModel is defined in MyBoardSettings.h
ControlPanel spa (SpaModel);

unsigned long timer;
uint16_t waterTemp;

void setup() {

  delay (2000);
  Console.begin (SerialBaudrate);
  Console.println ("\nSpaIot Simple Example");

  spa.begin();  // IMPORTANT LINE!
  if (spa.isOpen() == false) { // check if the connection to the spa has been open
    Console.println ("No spa connection found");
    for (;;); // loop always, to stop
  }

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
