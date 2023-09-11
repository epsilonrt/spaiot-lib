//
// SpaIot Test 
// Board Settings
#pragma once

#include <Arduino.h>

#if defined(ESP8266)
//--------------------------------------
// ESP8266
#define SpaConfig SpaIot8266Ssp

#elif defined(ESP32)
//--------------------------------------
// ESP32

#if defined(ARDUINO_LOLIN_S3)
//--------------------------------------
// ESP32 S3
#define SpaConfig SpaIotS3Ssp

#else
#define SpaConfig SpaIot32Ssp

#endif

#else
#error unsupported platform
#endif
