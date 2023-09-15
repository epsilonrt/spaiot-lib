//
// SpaIot Serial Remote Example with I2C button controller
//
// This example displays the condition of the spa: water temperature, desired
// temperature and LED status.
// It also allows you to press a key on the keypad to control a spa button.
// The buttons controller is a PCF8574 connected to 2 multiplexers 4051 as follows:
// - The selection pins {A, B, C} of the 2 multiplexers are connected
//   respectively to {P0, P1, P2} on the PCF8574
// - The INH pin of [A7..A0] multiplexer is connected to P3 on the PCF8574
// - The INH pin of [B7..B0] multiplexer is connected to P4 on the PCF8574
// This gives us the following values for the buttons of a SSP spa:
// |    KEY   | BIT | P4 | P3 | P2 | P1 | P0 | DEC |
// |:--------:|:---:|:--:|:--:|:--:|:--:|:--:|:---:|
// |   Power  |  B2 |  0 |  1 |  0 |  1 |  0 |  10 |
// |  Filter  |  A1 |  1 |  0 |  0 |  0 |  1 |  17 |
// |  Bubble  |  A3 |  1 |  0 |  0 |  1 |  1 |  19 |
// |  Heater  |  B7 |  0 |  1 |  1 |  1 |  1 |  15 |
// |  TempUp  |  B4 |  0 |  1 |  1 |  0 |  0 |  12 |
// | TempDown |  A7 |  1 |  0 |  1 |  1 |  1 |  23 |
// | TempUnit |  B5 |  0 |  1 |  1 |  0 |  1 |  13 |
#include <Arduino.h>
#include <SpaIot.h>
#include "MyBoardSettings.h"

using namespace SpaIot;
const unsigned long SerialBaudrate = 115200;

// My custom configuration
// You can modify the bus as you wish, but you must have the corresponding hardware
// See https://epsilonrt.github.io/spaiot-lib/group___hardware_settings.html
// MyBus and MyButtons are defined in MyBoardSettings.h
const HardwareSettings MyConfig (MyBus, SspLeds, MyButtons);

ControlPanel spa (MyConfig);

uint16_t waterTemp;
uint16_t desiredTemp = UnsetValue16;
uint16_t rawStatus;
bool isSetupModeTriggered;

void setup() {

  delay (2000);
  Console.begin (SerialBaudrate);
  Console.println ("\nSpaIot Serial Remote Example with I2C button controller Example");

  Wire.begin(); // IMPORTANT LINE! PCF8574 use the default Wire object
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
