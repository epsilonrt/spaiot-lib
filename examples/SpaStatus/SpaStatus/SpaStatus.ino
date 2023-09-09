//
// SpaIot Status Example
//
// This example displays the condition of the spa: water temperature, desired
// temperature and LED status.
#include <Arduino.h>
#include <SpaIot.h>
#include "MyBoardSettings.h"

using namespace SpaIot;

const unsigned long SerialBaudrate = 115200;

// My Spa Control Panel, SpaModel is defined in MyBoardSettings.h
ControlPanel spa (SpaModel);

uint16_t waterTemp;
uint16_t desiredTemp;
uint16_t rawStatus;

void setup() {

  delay (2000);
  Console.begin (SerialBaudrate);
  Console.println ("\nSpaIot Status Example");

  spa.begin();  // IMPORTANT LINE!
  if (spa.isOpen() == false) { // check if the connection to the spa has been open
    Console.println ("No spa connection found");
    for (;;); // loop always, to stop
  }
}


void loop() {
  uint16_t w;

  w = spa.waterTemp(); // Reading the temperature of the water
  if ( (w != UnsetValue16) && (waterTemp != w)) { // If modified, it is displayed
    waterTemp = w;
    Console.printf ("waterTemp=%d'C\n", waterTemp);
  }

  w = spa.desiredTemp(); // Reading the desired temperature
  if ( (w != UnsetValue16) && (desiredTemp != w)) { // If modified, it is displayed
    desiredTemp = w;
    Console.printf ("desiredTemp=%d'C\n", desiredTemp);
  }

  w = spa.rawStatus();
  if (rawStatus != w) { // If modified, it is displayed
    rawStatus = w;
    Console.println ("PFBHR");
    Console.printf ("%d%d%d%d%d\n",
                   spa.isPowerOn(),
                   spa.isFilterOn(),
                   spa.isBubbleOn(),
                   spa.isHeaterOn(),
                   spa.isHeatReached());
  }

  delay (1000);
}
