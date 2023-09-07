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
// Define the serial console, depending on the platform
#if defined(ARDUINO_LOLIN_S3)
// Serial  = OTG USB
// Serial0 = UART0 -> Default Pin GPIO18 (RX0) and GPIO17 (TX0), connected to USB-UART (CH340)
// Serial1 = UART1 -> Default Pin GPIO18 (RX1) and GPIO17 (TX1)
#define Console Serial0
#else
#define Console Serial
#endif
const unsigned long TimerTime = 60;

// My bus configuration :
#if defined(ESP8266)
// SDATA  -> GPIO12
// SCLK   -> GPIO14
// nWR    -> GPIO13
const BusSettings MyBus (12, 14, 13);

// My button controllers
Multiplexer MuxA ("U3", {5, 4, 15}, 16); // A->GPIO5, B->GPIO4, C->GPIO15, INH->GPIO16
Multiplexer MuxB ("U4", {5, 4, 15}, 0);  // A->GPIO5, B->GPIO4, C->GPIO15, INH->GPIO0

#elif defined(ARDUINO_LOLIN_S3)
// SDATA  -> GPIO23 MOSI GPIO11
// SCLK   -> GPIO18 SCLK GPIO12
// nWR    -> GPIO19 MISO GPIO10
const BusSettings MyBus (11, 12, 10);

// My button controllers
Multiplexer MuxA ("U3", {4, 5, 6}, 7);  // A->GPIO4, B->GPIO5, C->GPIO6, INH->GPIO7
Multiplexer MuxB ("U4", {4, 5, 6}, 15); // A->GPIO4, B->GPIO5, C->GPIO6, INH->GPIO15

#elif defined(ESP32)
// SDATA  -> GPIO23
// SCLK   -> GPIO18
// nWR    -> GPIO19
const BusSettings MyBus (23, 18, 19);

// My button controllers
Multiplexer MuxA ("U3", {27, 16, 17}, 33); // A->GPIO27, B->GPIO16, C->GPIO17, INH->GPIO33
Multiplexer MuxB ("U4", {27, 16, 17}, 26); // A->GPIO27, B->GPIO16, C->GPIO17, INH->GPIO26

#else
#error unsupported platform
#endif

// My buttons configuration (SSP)
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
const HardwareSettings MyConfig (MyBus, SspLeds, MyButtons);

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
  Console.printf ("Waiting to rearm filter every %lu seconds...", TimerTime);
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