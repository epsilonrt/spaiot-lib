//
// SpaIot Serial Remote Example
//
// This example displays the condition of the spa: water temperature, desired
// temperature and LED status.
// It also allows you to press a key on the keypad to control a spa button.
#include <Arduino.h>
#include <SpaIot.h>
#include <debug.h>
using namespace SpaIot;

// My bus configuration :
#if defined(ESP8266)
// SDATA  -> GPIO12
// SCLK   -> GPIO14
// nWR    -> GPIO13
const BusSettings MyBus (12, 14, 13);
// My button controllers
Cd4051 CustomCtrlA (5, 4, 15, 16); // S0->GPIO5, S1->GPIO4, S2->GPIO15, En->GPIO16
Cd4051 CustomCtrlB (5, 4, 15, 0);  // S0->GPIO5, S1->GPIO4, S2->GPIO15, En->GPIO0

#elif defined(ESP32)
// SDATA  -> GPIO23
// SCLK   -> GPIO18
// nWR    -> GPIO19
const BusSettings MyBus (23, 18, 19);
// My button controllers
Cd4051 CustomCtrlA (27, 16, 17, 25); // S0->GPIO27, S1->GPIO16, S2->GPIO17, En->GPIO25
Cd4051 CustomCtrlB (27, 16, 17, 26); // S0->GPIO27, S1->GPIO16, S2->GPIO17, En->GPIO26
#else
#error unsupported platform
#endif

// My buttons configuration (SSP)
const std::map<int, ButtonSettings> MyButtons = {
  { Filter,   ButtonSettings ("MyCtrlA", 1) },  // Filter   -> A1
  { Bubble,   ButtonSettings ("MyCtrlA", 3) },  // Bubble   -> A3
  { TempDown, ButtonSettings ("MyCtrlA", 7) },  // TempDown -> A7

  { Power,    ButtonSettings ("MyCtrlB", 2) },  // Power    -> B2
  { TempUp,   ButtonSettings ("MyCtrlB", 4) },  // TempUp   -> B4
  { TempUnit, ButtonSettings ("MyCtrlB", 5) },  // TempUnit -> B5
  { Heater,   ButtonSettings ("MyCtrlB", 7) }   // Heater   -> B7
};
// My custom configuration
const HardwareSettings MyConfig (MyBus, SspLeds, MyButtons);

const unsigned long SerialBaudrate = 115200;

ControlPanel * spa; // pointer on the control spa
uint16_t waterTemp;
uint16_t desiredTemp = UnsetValue16;
uint16_t rawStatus; 
bool isSetupModeTriggered;

void setup() {

  delay (2000);
  Serial.begin (SerialBaudrate);
  Serial.println ("\nSpaIot Serial Remote Example");

  // The button controllers must be registered before getInstance() call
  ButtonController::addToRegister ("MyCtrlA", CustomCtrlA);
  ButtonController::addToRegister ("MyCtrlB", CustomCtrlB);

  spa = ControlPanel::getInstance (MyConfig);
  if (spa == nullptr) { // check if the requested configuration has been found
    Serial.println ("Harware settings not found");
    for (;;); // loop always, to stop
  }

  spa->begin();  // IMPORTANT LINE!
  if (spa->isOpened() == false) { // check if the connection to the spa has been open
    Serial.println ("No spa connection found");
    for (;;); // loop always, to stop
  }
  spa->setPower (false);

  Serial.println("You can press a key to press a spa button:");
  Serial.println("Key   Button");
  Serial.println("P     Power");
  Serial.println("F     Filter");
  Serial.println("B     Bubble");
  Serial.println("H     Heater");
  Serial.println("U     Temp. Up");
  Serial.println("D     Temp. Down");
  Serial.println("C     Temp. Unit");
  Serial.println("Waiting for reading temperatures, it can take up to 20 seconds...");
  
  // Wait to read the temperature of the water, it can take 20 seconds ...
  waterTemp = spa->waitForWaterTemp();
  Serial.printf ("waterTemp=%d'C\n", waterTemp);
}


void loop() {
  uint16_t w;
  bool b;

  w = spa->waterTemp(); // Reading the temperature of the water
  if (waterTemp != w) { // If modified, it is displayed
    waterTemp = w;
    Serial.printf ("waterTemp=%d'C\n", waterTemp);
  }

  w = spa->desiredTemp(); // Reading the desired temperature
  if ((desiredTemp != w) && (spa->isPowerOn())) { // If modified, it is displayed
    desiredTemp = w;
    Serial.printf ("desiredTemp=%d'C\n", desiredTemp);
  }

  b = spa->isSetupModeTriggered(); // Check if setup mode triggered
  if (isSetupModeTriggered != b) {
    isSetupModeTriggered = b;
     Serial.printf ("isSetupModeTriggered=%d\n", isSetupModeTriggered);
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
