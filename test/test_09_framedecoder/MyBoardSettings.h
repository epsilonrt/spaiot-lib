//
// SpaIot Test 
// Board Settings
#pragma once

#include <Arduino.h>

#if defined(ESP8266)
//--------------------------------------
// ESP8266
#define SpaBus SpaIot8266Bus

#elif defined(ESP32)
//--------------------------------------
// ESP32

#if defined(ARDUINO_LOLIN_S3)
//--------------------------------------
// ESP32 S3
#define SpaBus SpaIotS3Bus
#else
#define SpaBus SpaIot32Bus
#endif

#else
#error unsupported platform
#endif
