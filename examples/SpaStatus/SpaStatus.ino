//
// SpaIot Status Example
//
// This example displays the condition of the spa: water temperature, desired
// temperature and LED status.
// We use a DIY board connected to a SSP spa, the configuration  is as follows:
//
//  const BusSettings MyBus (12, 14, 13); // for ESP8266
//  const BusSettings MyBus (23, 18, 19); // for ESP32
//
//  const std::map<int, LedSettings> SspLeds = {
//    { Power,          LedSettings (0) },
//    { Heater,         LedSettings (7) },
//    { HeatReached,    LedSettings (9) },
//    { Bubble,         LedSettings (10) },
//    { Filter,         LedSettings (12) }
//  };
#include <Arduino.h>
#include <SpaIot.h>

using namespace SpaIot;

const unsigned long SerialBaudrate = 115200;

// My bus configuration :
// If your configuration is different, see SpaHwCustom Example

#if defined(ESP8266)
// SDATA  -> GPIO12
// SCLK   -> GPIO14
// nWR    -> GPIO13
const BusSettings MyBus (12, 14, 13);

#elif defined(ARDUINO_LOLIN_S3)
// SDATA  -> GPIO23 MOSI GPIO11
// SCLK   -> GPIO18 SCLK GPIO12
// nWR    -> GPIO19 MISO GPIO10
const BusSettings MyBus (11, 12, 10);

#elif defined(ESP32)
// SDATA  -> GPIO23
// SCLK   -> GPIO18
// nWR    -> GPIO19
const BusSettings MyBus (23, 18, 19);

#else
#error unsupported platform
#endif

// My buttons : no buttons !
const std::map<int, ButtonSettings> NoButtons;

// My custom configuration
const HardwareSettings MyConfig (MyBus, SspLeds, NoButtons);


ControlPanel spa (MyConfig);

uint16_t waterTemp;
uint16_t desiredTemp;
uint16_t rawStatus;

void setup() {

  delay (2000);
  Serial.begin (SerialBaudrate);
  Serial.println ("\nSpaIot Status Example");

  spa.begin();  // IMPORTANT LINE!
  if (spa.isOpened() == false) { // check if the connection to the spa has been open
    Serial.println ("No spa connection found");
    for (;;); // loop always, to stop
  }
}


void loop() {
  uint16_t w;

  w = spa.waterTemp(); // Reading the temperature of the water
  if ( (w != UnsetValue16) && (waterTemp != w)) { // If modified, it is displayed
    waterTemp = w;
    Serial.printf ("waterTemp=%d'C\n", waterTemp);
  }

  w = spa.desiredTemp(); // Reading the desired temperature
  if ( (w != UnsetValue16) && (desiredTemp != w)) { // If modified, it is displayed
    desiredTemp = w;
    Serial.printf ("desiredTemp=%d'C\n", desiredTemp);
  }

  w = spa.rawStatus();
  if (rawStatus != w) { // If modified, it is displayed
    rawStatus = w;
    Serial.println ("PFBHR");
    Serial.printf ("%d%d%d%d%d\n",
                   spa.isPowerOn(),
                   spa.isFilterOn(),
                   spa.isBubbleOn(),
                   spa.isHeaterOn(),
                   spa.isHeatReached());
  }

  delay (1000);
}
