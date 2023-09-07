//
// SpaIot Status Example
//
// This example displays the condition of the spa: water temperature, desired
// temperature and LED status.
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

// My Spa Control Panel
// You can choose another spa model, but you must have the corresponding hardware
// See https://epsilonrt.github.io/spaiot-lib/group___hardware_settings.html
// You can also define your own spa model, See SpaHwCustom example
ControlPanel spa ("SPAIOTS3SSP");


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
