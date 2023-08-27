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

using namespace SpaIot;

const unsigned long SerialBaudrate = 115200;

// My bus configuration :
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

// My button controllers
Pcf8574Mux MyBtnCtrl ("U5", 0x38); // i2c slave address, PCF8574: 0x20 - PCF8574A: 0x38

// My buttons configuration (SSP)
const std::map<int, ButtonSettings> MyButtons = {
  { Power,    ButtonSettings (MyBtnCtrl, 10) },
  { Filter,   ButtonSettings (MyBtnCtrl, 17) },
  { Bubble,   ButtonSettings (MyBtnCtrl, 19) },
  { Heater,   ButtonSettings (MyBtnCtrl, 15) },
  { TempUp,   ButtonSettings (MyBtnCtrl, 12) },
  { TempDown, ButtonSettings (MyBtnCtrl, 23) },
  { TempUnit, ButtonSettings (MyBtnCtrl, 13) }
};
// My custom configuration
const HardwareSettings MyConfig (MyBus, SspLeds, MyButtons);

ControlPanel spa (MyConfig);

uint16_t waterTemp;
uint16_t desiredTemp = UnsetValue16;
uint16_t rawStatus;
bool isSetupModeTriggered;

void setup() {

  delay (2000);
  Serial.begin (SerialBaudrate);
  Serial.println ("\nSpaIot Serial Remote Example with I2C button controller Example");

  Wire.begin(); // IMPORTANT LINE! PCF8574 use the default Wire object

  spa.begin();  // IMPORTANT LINE!
  if (spa.isOpen() == false) { // check if the connection to the spa has been open
    Serial.println ("No spa connection found");
    for (;;); // loop always, to stop
  }
  spa.setPower (false);

  Serial.println ("You can press a key to press a spa button:");
  Serial.println ("Key   Button");
  Serial.println ("P     Power");
  Serial.println ("F     Filter");
  Serial.println ("B     Bubble");
  Serial.println ("H     Heater");
  Serial.println ("U     Temp. Up");
  Serial.println ("D     Temp. Down");
  Serial.println ("C     Temp. Unit");
  Serial.println ("Waiting for reading temperatures, it can take up to 20 seconds...");

  // Wait to read the temperature of the water, it can take 20 seconds ...
  waterTemp = spa.waitForWaterTemp();
  Serial.printf ("waterTemp=%d'C\n", waterTemp);
}


void loop() {
  uint16_t w;
  bool b;

  w = spa.waterTemp(); // Reading the temperature of the water
  if (waterTemp != w) { // If modified, it is displayed
    waterTemp = w;
    Serial.printf ("waterTemp=%d'C\n", waterTemp);
  }

  w = spa.desiredTemp(); // Reading the desired temperature
  if ( (desiredTemp != w) && (spa.isPowerOn())) { // If modified, it is displayed
    desiredTemp = w;
    Serial.printf ("desiredTemp=%d'C\n", desiredTemp);
  }

  b = spa.isSetupModeTriggered(); // Check if setup mode triggered
  if (isSetupModeTriggered != b) {
    isSetupModeTriggered = b;
    Serial.printf ("isSetupModeTriggered=%d\n", isSetupModeTriggered);
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

  if (Serial.available() > 0) {
    // read the incoming byte:
    int c = Serial.read();

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
