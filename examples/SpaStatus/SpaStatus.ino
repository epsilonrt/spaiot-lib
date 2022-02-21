//
// SpaIot Status Example
//
// This example displays the condition of the spa: water temperature, desired 
// temperature and LED status.
#include <Arduino.h>
#include <SpaIot.h>

using namespace SpaIot;

// It's a SCIP2 controller connected to a PXXXSPA SSP-XXX
// If your configuration is different, see SpaHwCustom Example
const char ConfigName[] = "SCIP2SSP";
const unsigned long SerialBaudrate = 115200;

ControlPanel * spa; // pointer on the control spa
uint16_t waterTemp;
uint16_t desiredTemp;
uint16_t rawStatus;

void setup() {

  delay (2000);
  Serial.begin (SerialBaudrate);
  Serial.println ("\nSpaIot Status Example");

  spa = ControlPanel::getInstance (ConfigName);
  if (spa == nullptr) { // check if the requested configuration has been found
    Serial.println ("Harware settings not found");
    for(;;); // loop always, to stop 
  }

  spa->begin();  // IMPORTANT LINE!
  if (spa->isOpened() == false) { // check if the connection to the spa has been open
    Serial.println ("No spa connection found");
    for(;;); // loop always, to stop 
  }
  spa->setPower (true); // Comment this line if the spa is already started

  // Wait to read the temperature of the water, it can take 20 seconds ...
  waterTemp = spa->waitForWaterTemp();
  Serial.printf ("waterTemp=%d'C\n", waterTemp);
  desiredTemp = spa->waitForDesiredTemp();
  Serial.printf ("desiredTemp=%d'C\n", desiredTemp);
}


void loop() {
  uint16_t w;

  w = spa->waterTemp(); // Reading the temperature of the water
  if (waterTemp != w) { // If modified, it is displayed
    waterTemp = w;
    Serial.printf ("waterTemp=%d'C\n", waterTemp);
  }

  w = spa->desiredTemp(); // Reading the desired temperature
  if (desiredTemp != w) { // If modified, it is displayed
    desiredTemp = w;
    Serial.printf ("desiredTemp=%d'C\n", desiredTemp);
  }

  w = spa->rawStatus();
  if (rawStatus != w) { // If modified, it is displayed
    rawStatus = w;
    Serial.println ("PFBHR");
    Serial.printf ("%d%d%d%d%d\n",
                   spa->isPowerOn(),
                   spa->isFilterOn(),
                   spa->isBubbleOn(),
                   spa->isHeaterOn(),
                   spa->isHeatReached());
  }

  delay (1000);
}
