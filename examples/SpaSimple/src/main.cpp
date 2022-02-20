//
// SpaIot Simple Example
//
#include <Arduino.h>
#include <SpaIot.h>

using namespace SpaIot;

// It's a SCIP2 controller connected to a PXXXSPA SSP-XXX
// If your configuration is different, see SpaHwCustom Example
const char ConfigName[] = "SCIP2SSP"; 
const unsigned long TimerTime = (12 * 60 * 60); // 12 hours in seconds
const unsigned long SerialBaudrate = 115200;

ControlPanel * panel; // pointer on the control panel
unsigned long timer;
uint16_t waterTemp;

void setup() {

  delay (2000);
  Serial.begin (SerialBaudrate);
  Serial.println ("\nSpaIot Simple Example");

  panel = ControlPanel::getInstance (ConfigName);
  if (panel == nullptr) { // check if the requested configuration has been found
    Serial.println ("Harware settings not found");
    exit(EXIT_FAILURE);
  }

  panel->begin();  // IMPORTANT LINE!
  
  // Wait to read the temperature of the water, it can take 20 seconds ...
  waterTemp = panel->waitForWaterTemp();
  Serial.printf ("waterTemp=%d'C\n", waterTemp);
  Serial.printf ("Waiting to reset filter every %lu seconds...", TimerTime);
}


void loop() {

  uint16_t t = panel->waterTemp(); // Reading the temperature of the water
  if (waterTemp != t) { // If modified, it is displayed
    waterTemp = t;
    Serial.printf ("waterTemp=%d'C\n", waterTemp);
  }

  if (timer >= TimerTime) { // If the time has been reached, 
    if (panel->isFilterOn()) { // you check if the filtering is running, 

      Serial.println ("I rearm the spa filter");
      // if so, it is stopped and restarts.
      panel->setFilter (false);
      delay (1000);
      panel->setFilter (true);
    }
  }

  delay (1000);
}
