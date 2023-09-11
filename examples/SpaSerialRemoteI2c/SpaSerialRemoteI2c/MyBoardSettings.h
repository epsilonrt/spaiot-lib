//
// SpaIot Example 
// Board Settings
#pragma once

#include <Arduino.h>

#if defined(ESP8266)
//--------------------------------------
// ESP8266
#define Console Serial
// You can choose another spa bus, but you must have the corresponding hardware
// See https://epsilonrt.github.io/spaiot-lib/group___hardware_settings.html
// You can also define your own spa bus, See SpaHwCustom example
#define MyBus SpaIot8266Bus
#define MyButtons SpaIot8266SspButtons

#elif defined(ARDUINO_LOLIN_S3)
//--------------------------------------
// ESP32 S3
#define Console Serial0
#define MyBus SpaIotS3Bus
#define MyButtons SpaIotS3SspButtons

#elif defined(ESP32)
//--------------------------------------
// ESP32
#define Console Serial
#define MyBus SpaIot32Bus
#define MyButtons SpaIot32SspButtons

#else
#error unsupported platform
#endif