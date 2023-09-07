//
// SpaIot Serial Remote Example
//
// This example displays the condition of the spa: water temperature, desired
// temperature and LED status.
// It also allows you to press a key on the keypad to control a spa button.
#include <Arduino.h>
#include <SpaIot.h>

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

using namespace SpaIot;

uint16_t waterTemp;
uint16_t desiredTemp = UnsetValue16;
uint16_t rawStatus;
bool isSetupModeTriggered;

// My Spa Control Panel
// You can choose another spa model, but you must have the corresponding hardware
// See https://epsilonrt.github.io/spaiot-lib/group___hardware_settings.html
// You can also define your own spa model, See SpaHwCustom example
ControlPanel spa ("SPAIOTS3SSP");

void setup() {

  delay (2000);
  Console.begin (SerialBaudrate);
  Console.println ("\nSpaIot Serial Remote Example");

  spa.begin();  // IMPORTANT LINE!
  if (spa.isOpen() == false) { // check if the connection to the spa has been open
    Console.println ("No spa connection found");
    for (;;); // loop always, to stop
  }
  spa.setPower (false);

  Console.println ("You can press a key to press a spa button:");
  Console.println ("Key   Button");
  Console.println ("P     Power");
  Console.println ("F     Filter");
  Console.println ("B     Bubble");
  Console.println ("H     Heater");
  Console.println ("U     Temp. Up");
  Console.println ("D     Temp. Down");
  Console.println ("C     Temp. Unit");
  Console.println ("Waiting for reading temperatures, it can take up to 20 seconds...");

  // Wait to read the temperature of the water, it can take 20 seconds ...
  waterTemp = spa.waitForWaterTemp();
  Console.printf ("waterTemp=%d'C\n", waterTemp);
}


void loop() {
  uint16_t w;
  bool b;

  w = spa.waterTemp(); // Reading the temperature of the water
  if (waterTemp != w) { // If modified, it is displayed
    waterTemp = w;
    Console.printf ("waterTemp=%d'C\n", waterTemp);
  }

  w = spa.desiredTemp(); // Reading the desired temperature
  if ( (desiredTemp != w) && (spa.isPowerOn())) { // If modified, it is displayed
    desiredTemp = w;
    Console.printf ("desiredTemp=%d'C\n", desiredTemp);
  }

  b = spa.isSetupModeTriggered(); // Check if setup mode triggered
  if (isSetupModeTriggered != b) {
    isSetupModeTriggered = b;
    Console.printf ("isSetupModeTriggered=%d\n", isSetupModeTriggered);
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

  if (Console.available() > 0) {
    // read the incoming byte:
    int c = Console.read();

    if ( (c == 'P') || (c == 'p')) {
      spa.pushButton (Power);
    }
    else if ( (c == 'F') || (c == 'f')) {
      spa.pushButton (Filter);
    }
    else if ( (c == 'B') || (c == 'b')) {
      spa.pushButton (Bubble);
    }
    else if ( (c == 'H') || (c == 'h')) {
      spa.pushButton (Heater);
    }
    else if ( (c == 'U') || (c == 'u')) {
      spa.pushButton (TempUp);
    }
    else if ( (c == 'D') || (c == 'd')) {
      spa.pushButton (TempDown);
    }
    else if ( (c == 'C') || (c == 'c')) {
      spa.pushButton (TempUnit);
    }
  }

  delay (1000);
}
