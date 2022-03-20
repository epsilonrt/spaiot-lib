# spaiot-lib
Supervision and Control library for Intxx PxxxSPA with an ESP8266 or ESP32

[![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/epsilonrt/spaiot-lib?include_prereleases)](https://github.com/epsilonrt/spaiot-lib/releases)
[![Build](https://github.com/epsilonrt/spaiot-lib/actions/workflows/build.yml/badge.svg)](https://github.com/epsilonrt/spaiot-lib/actions/workflows/build.yml)
[![BuildTest](https://github.com/epsilonrt/spaiot-lib/actions/workflows/build-test.yml/badge.svg)](https://github.com/epsilonrt/spaiot-lib/actions/workflows/build-test.yml)
[![UnitTest](https://github.com/epsilonrt/spaiot-lib/actions/workflows/unit-test.yml/badge.svg)](https://github.com/epsilonrt/spaiot-lib/actions/workflows/unit-test.yml)

[![PlatformIO Registry](https://badges.registry.platformio.org/packages/epsilonrt/library/spaiot-lib.svg)](https://registry.platformio.org/libraries/epsilonrt/spaiot-lib)
[![Platform ESP8266](https://img.shields.io/badge/Platform-Espressif8266-orange)](#)
[![Platform ESP32](https://img.shields.io/badge/Platform-Espressif32-orange)](#)
[![Framework](https://img.shields.io/badge/Framework-Arduino-blue)](https://www.arduino.cc/)

---

This projet aims to add remote supervision and control ability to the Intxx PxxxSPA without altering the initial product. 

The spaiot library must be associated with an electronic board which is placed between the control panel and the motor block, through its specific 5 pins connector.

The spaiot library:  
- is designed in C++ using the Arduino framework for ESP8266 and ESP32 chips,  
- has been modularly designed to adapt to a large number of hardware and software choices,  
- available in the [PlatformIO Library Manager](https://registry.platformio.org/libraries/epsilonrt/spaiot-lib) and as a ZIP file for the Arduino IDE.

spaiot-lib allows the integration of your Intxx PxxxSPA into your home automation (Internet of things).
For example, associated with [SinricPro](https://sinric.pro/), spaiot-lib will order its spa in the voice using a Google Assistant, Amazon Alexa...

<a href="https://creativecommons.org/licenses/by-nc-sa/4.0/">
  <img src="https://raw.githubusercontent.com/epsilonrt/spaiot-lib/master/extras/images/by-nc-sa-small.png" alt="by-nc-sa.png" align="right" valign="top">
</a>

spaiot-lib is based on Geoffroy Hubert's work on the DIYSCIP project. 
It is published with the same license: Creative Commons Attribution - No commercial use - sharing under the same conditions.

> :warning: **disclaimer:** This project is not affiliated with Intxx. 
It is distributed in the hope it will be useful but WITHOUT ANY WARRANTY. 
Any damaged on your spa or lost of original product warranty is in your own responsibility, including any consequences of using this project.

spaiot-lib provides the following features:  
- decoding frames on the connection between the control panel and the motor block to monitor the status of the spa,  
- remote control of push buttons

It is documented using [doxygen](https://www.doxygen.nl/index.html) and delivered with [examples](https://github.com/epsilonrt/spaiot-lib/tree/master/examples). 
[Unit tests](https://github.com/epsilonrt/spaiot-lib/tree/master/test) provide continuous integration and delivery ([CI/CD](https://en.wikipedia.org/wiki/CI/CD))

---

## Installation

### VS Code & PlatformIO  
1. Install [VS Code](https://code.visualstudio.com/)  
2. Install [PlatformIO](https://platformio.org/platformio-ide)  
3. Install **spaiot-lib** by using [Library Manager](https://docs.platformio.org/en/latest/librarymanager/)  
4. Use included [platformio.ini](https://github.com/epsilonrt/spaiot-lib/blob/master/examples/platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automaticly.

![spaiot-lib library manager](https://raw.githubusercontent.com/epsilonrt/spaiot-lib/master/extras/images/platformio-install-spaiot.png)

### ArduinoIDE  
1. Install [Arduino IDE](https://www.arduino.cc/en/software)  
2. Install Arduino core for [ESP8266](https://github.com/esp8266/Arduino#installing-with-boards-manager) or [ESP32](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)  
3. Download from GitHub, you should navigate to the top level of the [spaiot-lib project](https://github.com/epsilonrt/spaiot-lib) and then a green "Code" download button will be visible on the right. 
Choose the Download ZIP option from the Code pull-down menu.  
4. Since you have downloaded the ZIP file, open your Arduino IDE, click on Sketch > Include Library > Add .ZIP Library. 
Choose the ZIP file you just downloaded，and if the library install correct, you will see Library added to your libraries in the notice window. Which means the library is installed successfully.  

Then let's check if the library install correctly.

When you add the library successfully, there will be a demo in the Example. In this case, click on File > Example > spaiot-lib > SpaSimple to open an example.  
Choose the highest CPU frequency (160MHz for ESP8266, 240MHz for ESP32) from the Tools > CPU Frequency menu, click on the Verify button, if there's no error, congratulation, the library is installed perfectly.

![arduino ide](https://raw.githubusercontent.com/epsilonrt/spaiot-lib/master/extras/images/arduino-example.png)

---

## Full user documentation

The first-level API consists of the [SpaIot::ControlPanel](https://epsilonrt.github.io/spaiot-lib/class_spa_iot_1_1_control_panel.html) class.
For simply use, so just read the [documentation](https://epsilonrt.github.io/spaiot-lib/class_spa_iot_1_1_control_panel.html) of this class.

For more advanced development, full documentation is available.  
Please see here for [full user documentation](https://epsilonrt.github.io/spaiot-lib/)

---

## Examples
See [examples](https://github.com/epsilonrt/spaiot-lib/tree/master/examples) on GitHub

---

## Usage  

1. Include SpaIot-Library (`SpaIot.h`) and use the SpaIot namespace

  ```cpp
  #include <SpaIot.h>

  using namespace SpaIot;
  ```

2. Declare a global reference on the `spa` control panel to get the singleton object

  ```cpp
  // Get the singleton object with your SPA configuration (here SCIP2SSP)
  ControlPanel & spa = ControlPanel::singleton ("SCIP2SSP");
  ```

  :warning: **disclaimer:** only one control panel [singleton](https://en.wikipedia.org/wiki/Singleton_pattern) may exist.

3. In `setup()`

  ```cpp
    // Start the control panel
    spa.begin();  // IMPORTANT LINE!
  ```

4. In `loop()`, use API of the [ControlPanel](https://epsilonrt.github.io/spaiot-lib/class_spa_iot_1_1_control_panel.html) class to do or read what you want

  ```cpp
    uint16_t w;

    w = spa.waterTemp(); // Reading the temperature of the water
    if (waterTemp != w) { // If modified, it is displayed
      waterTemp = w;
      Serial.printf ("waterTemp=%d'C\n", waterTemp);
    }

    if (Serial.available() > 0) { // If the key pressed...
      // read the incoming byte:
      int c = Serial.read();

      if ( (c == 'P') || (c == 'p')) { // If the 'P' key pressed, push the power button
        spa.pushButton (Power);
      }
    }
  ```

---

## Configure the library according to the electronic part and the spa model

spaiot-lib comes with hardware configurations of DIYSCIP boards and SSP-XXX and SJB-XXX SPA models :  
1. **SCIP2SSP** > DIYSCIP v2 and SSP-XXX spa
2. **SCIP2SJB** > DIYSCIP v2 and SJB-XXX spa  

But it possible to adapt the hardware configuration with the HardwareSettings class which is an aggregation of classes:
- BusSettings that defines the configuration of the synchronous serial link,
- LedSettings that defines the configuration of the LEDs of the spa,
- ButtonSettings that defines the configuration of the buttons.

You can see how to create your own configuration with the [SpaHwCustom example](https://github.com/epsilonrt/spaiot-lib/blob/master/examples/SpaHwCustom/SpaHwCustom.ino)

Before declare a global pointer on the spa control panel:  

1. Describe the pins used by the bus using a BusSettings constant 

  ```cpp
  // My bus configuration :
  // SDATA  -> GPIO12
  // SCLK   -> GPIO14
  // nWR    -> GPIO13
  const BusSettings MyBus (12, 14, 13);
  ```

2. Describe the bits of the frame used by LEDs using a list of LedSettings type constants

  ```cpp
  // My leds configuration (SSP)
  const std::map<int, LedSettings> MyLeds = {
    { Power,          LedSettings (0) },  // Power        -> A0
    { Heater,         LedSettings (7) },  // Heater       -> A7
    { HeatReached,    LedSettings (9) },  // HeatReached  -> B1
    { Bubble,         LedSettings (10) }, // Bubble       -> B2
    { Filter,         LedSettings (12) }  // Filter       -> B4
  };
  ```

3. Describe buttons controllers and their pins with the corresponding class (here Cd4051)  

  ```cpp
  // My button controllers
  Cd4051 BtnCtrlA ("U3", 5, 4, 15, 16); // A->GPIO5, B->GPIO4, C->GPIO15, INH->GPIO16
  Cd4051 BtnCtrlB ("U4", 5, 4, 15, 0);  // A->GPIO5, B->GPIO4, C->GPIO15, INH->GPIO0
  ```

4. Describe the (bits used in the frame and controller) using a List of ButtonSettings constants  

  ```cpp
  // My buttons configuration (SSP)
  const std::map<int, ButtonSettings> MyButtons = {
    { Filter,   ButtonSettings (BtnCtrlA, 1) },  // Filter   -> A1
    { Bubble,   ButtonSettings (BtnCtrlA, 3) },  // Bubble   -> A3
    { TempDown, ButtonSettings (BtnCtrlA, 7) },  // TempDown -> A7

    { Power,    ButtonSettings (BtnCtrlB, 2) },  // Power    -> B2
    { TempUp,   ButtonSettings (BtnCtrlB, 4) },  // TempUp   -> B4
    { TempUnit, ButtonSettings (BtnCtrlB, 5) },  // TempUnit -> B5
    { Heater,   ButtonSettings (BtnCtrlB, 7) }   // Heater   -> B7
  };
  ```

5. Finally Create the hardware configuration  

  ```cpp
  // My custom configuration
  const HardwareSettings MyConfig (MyBus, MyLeds, MyButtons);
  ```

---

## How does SpaIot work ?

### Decoding frames

Decoding requires only 3 GPIO pins of the ESP chip. These pins are connected to the SDATA, SCLK and nWR signals of the bus between the control panel and the motor block.
These 3 signals constitutes a synchronous serial bus (similar to the SPI bus) that transmits frames consisting of 16-bit words.
The words are composed of 2 bytes A and B. B is the most significant byte and it is transmitted first. Bytes are transmitted with the most significant bit first (left).

We can see in the image below the frame that corresponds to the command of the LEDs:

![ledframe](https://raw.githubusercontent.com/epsilonrt/spaiot-lib/master/extras/images/ledframe.png)

Decoding is implemented by the FrameDecoder class. This class makes it possible to recover mainly the following information:
- LED status using FrameDecoder::isPowerOn(), FrameDecoder::isFilterOn() ...
- measured water and desired temperature using the FrameDecoder::waterTemp() et FrameDecoder::desiredTemp(),
- error code displayed using the FrameDecoder::error() function

The GPIO pins connected to the SCLK and nWR signals are used in interruption. That's why the Framedecoder class is a singleton.
It is designed to be a base class and can not be instantiated directly. It will be used thanks to its ControlPanel derived class.

### Control of push buttons

The general idea is to "connect" an electronic switch to bypass the button on the control panel.
Of course, this is not what is done because it would be necessary in this case to intervene on the control panel.

To find out how to do it, simply study the diagram below:

![sch1_sregister](https://raw.githubusercontent.com/epsilonrt/spaiot-lib/master/extras/images/sch1_sregister.png)

It is possible to see 2 integrated circuits, U1 and U2 which will store the 16 bits of the frame and apply their logic level to the correditional pins of U1 / U2.
U1 memorizes least significant byte A, U2 the most significant byte B.

To play a push button:
1. The button must be selected by transmitting a 16-bit word that applies a low level to a pin of this push button,
2. Then put nWR in the top state,
3. Activates a pull-up resistance on SDATA,
4. Toggle the signal SDATA input,
5. Then after a delay of a few microseconds, to read the state of SDATA, which is connected to the other pin of the button.
    
If SDATA is in the low level, the button selected in step 1 is pressed.
For example, if we want to test if the S1-Filter button is pressed, you have to send a word with bit A1 to 0.

To be able to emulate pressing a button, it is necessary to reproduce this scheme on a board and replace the buttons with electronic switches.

MOSFET transistors can be used, multiplexer circuits...
In version 2 of the board, the DIYSCIP project uses 2 multiplexer circuits 1 to 8 (4051) that allow to take into account any button connected to A0..A7 or B0..B7

spaiot-lib implements the button control with the Button class.
Button uses the Abstract ButtonController class to make the connection between the button of the button and SDATA (SCOM actually).
Thus, the developer will be able to create a derived class of ButtonController to implement his solution.

---
