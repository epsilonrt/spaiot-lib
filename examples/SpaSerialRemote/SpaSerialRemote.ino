//
// SpaIot Serial Remote Example
//
// This example displays the condition of the spa: water temperature, desired
// temperature and LED status.
// It also allows you to press a key on the keypad to control a spa button.
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
  Serial.println ("\nSpaIot Serial Remote Example");

  spa = ControlPanel::getInstance (ConfigName);
  if (spa == nullptr) { // check if the requested configuration has been found
    Serial.println ("Harware settings not found");
    for (;;); // loop always, to stop
  }

  spa->begin();  // IMPORTANT LINE!
  if (spa->isOpened() == false) { // check if the connection to the spa has been open
    Serial.println ("No spa connection found");
    for (;;); // loop always, to stop
  }
  spa->setPower (true); // Comment this line if the spa is already started

  // Wait to read the temperature of the water, it can take 20 seconds ...
  waterTemp = spa->waitForWaterTemp();
  Serial.printf ("waterTemp=%d'C\n", waterTemp);
  desiredTemp = spa->waitForDesiredTemp();
  Serial.printf ("desiredTemp=%d'C\n", desiredTemp);
  Serial.println("You can press a key to press a spa button:");
  Serial.println("Key   Button");
  Serial.println("P     Power");
  Serial.println("F     Filter");
  Serial.println("B     Bubble");
  Serial.println("H     Heater");
  Serial.println("U     Temp. Up");
  Serial.println("D     Temp. Down");
  Serial.println("C     Temp. Unit");
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

  if (Serial.available() > 0) {
    // read the incoming byte:
    int c = Serial.read();
    
    if ((c == 'P')||(c == 'p')) {
      spa->pushButton(Power);
    }
    else if ((c == 'F')||(c == 'f')) {
      spa->pushButton(Filter);
    }
    else if ((c == 'B')||(c == 'b')) {
      spa->pushButton(Bubble);
    }
    else if ((c == 'H')||(c == 'h')) {
      spa->pushButton(Heater);
    }
    else if ((c == 'U')||(c == 'u')) {
      spa->pushButton(TempUp);
    }
    else if ((c == 'D')||(c == 'd')) {
      spa->pushButton(TempDown);
    }
    else if ((c == 'C')||(c == 'c')) {
      spa->pushButton(TempUnit);
    }
  }

  delay (1000);
}
