# spaiot-lib
Supervision and Control library for Intxx PxxxSPA with an ESP8266 or ESP32

---

This projet aims to add remote supervision and control ability to the Intxx PxxxSPA without altering the initial product. 

The spaiot library must be associated with an electronic board which is placed between the control panel and the motor block, through its specific 5 pins connector.

The spaiot library:  
- is designed in C ++ for Esp8266 and ESP32 chips,  
- uses the Arduino framework,  
- is part of the PLATFORMIO register,  
- has been modularly designed to adapt to a large number of hardware and software choices.

Personally I use the IDE Visual Studio Code with Platformio, but it is quite possible to use spaiot-lib in the IDE Arduino.

spaiot-lib allows the integration of your Intxx PxxxSPA into your home automation (Internet of things).

For example, associated with Sinricpro, spaiot-Lib will order its spa in the voice using a Google Assistant, Amazon Alexa...

<a href="https://creativecommons.org/licenses/by-nc-sa/4.0/">
  <img src="https://mirrors.creativecommons.org/presskit/buttons/88x31/png/by-nc-sa.png" alt="by-nc-sa.png" align="right" valign="top">
</a>

spaiot-lib is based on Geoffroy Hubert's work on the DIYSCIP project. 
It is published with the same license: Creative Commons Attribution - No commercial use - sharing under the same conditions.

> :warning: **disclaimer:** This project is not affiliated with Intxx. 
It is distributed in the hope it will be useful but WITHOUT ANY WARRANTY. 
Any damaged on your spa or lost of original product warranty is in your own responsibility, including any consequences of using this project.

spaiot-lib provides the following features:  
- decoding frames on the connection between the control block and the engine block to monitor the condition of the spa,  
- remote control of push buttons

It is documented using doxygen and delivered with examples. Unit tests provide continuous integration and diffusion (CI / CD)

### decoding frames

Decoding requires only 3 GPIO pins of the ESP chip. These pins are connected to the SDATA, SCLK clock and nWR write signals of the link between the control block and the engine block.
These 3 signals constitutes a synchronous serial bus (similar to the SPI bus) that transmits frames consisting of 16-bit words.
The words are transmitted higher weight first (MSB on the left) and consisting of 2 bytes, that of higher weight is noted b (on the left) and the weak weight has (right).

We can see in the image below the frame that corresponds to the command of the LEDs:

![ledframe](images/ledframe.png)

Decoding is implemented by the FrameDecoder class. This class makes it possible to recover mainly the following information:
- LED status using FrameDecoder::isPowerOn(), FrameDecoder::isFilterOn() ...
- measured water and desired temperature using the FrameDecoder::waterTemp() et FrameDecoder::desiredTemp(),
- error code displayed using the FrameDecoder::error() function

The GPIO pins connected to the SCLK and NWR signals are used in interruption. That's why the Framedecoder class is a singleton.
It is designed to be a base class and can not be instantiated directly. It will be used thanks to its ControlPanel derived class.

### Control of push buttons

The general idea is to "connect" an electronic switch to bypass the button on the control panel.
Of course, this is not what is done because it would be necessary in this case to intervene on the control panel.

To find out how to do it, simply study the diagram below:

![sch1_sregister](images/sch1_sregister.png)

It is possible to see 2 integrated circuits, U1 and U2 which will store the 16 bits of the frame and apply their logic level to the correditional pins of U1 / U2.
U1 memorizes low weight byte, u2 of high weight.

To play a push button:
1. The button must be selected by transmitting a 16-bit word that applies a low level to a pin of this push button,
2. Then she put nWR in the top state,
3. Activates a pull-up resistance on SDATA,
4. Toggle the signal SDATA input,
5. Then after a delay of a few microseconds, to read the state of SDATA, which is connected to the other pin of the button.
    
If SDATA is in the low state, the button selected in step 1 is pressed.
For example, if we want to test if the S1-Filter button is pressed, you have to send a word with bit A1 to 0.

To be able to emulate pressing a button, it is necessary to reproduce this scheme on a board and replace the buttons with electronic switches.

MOSFET transistors can be used, multiplexer circuits...
In version 2 of the board, the DIYSCIP project uses 2 multiplexer circuits 1 to 8 (4051) that allow to take into account any button connected to A0..A7 or B0..B7

spaiot-lib implements the button control with the Button class.
Button uses the Abstract ButtonController class to make the connection between the button of the button and SDATA (SCOM actually).
Thus, the developer will be able to create a derived class of ButtonController to implement his solution.

### Configuration of the library according to the electronic part and the spa model

spaiot-lib comes with hardware configurations of DIYSCIP boards and SSP-XXX and SJB-XXX SPA models.
But it possible to adapt the hardware configuration with the HardwareSettings class which is an aggregation of classes:
- BusSettings that defines the configuration of the synchronous serial link,
- LedSettings that defines the configuration of the LEDs of the spa,
- ButtonSettings that defines the configuration of the buttons.

### Example

To do.

### Documentation

To do.
