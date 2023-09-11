//
// SpaIot Test 
// Board Settings
#pragma once

#include <Arduino.h>

#if defined(ESP8266)
//--------------------------------------
// ESP8266
#define SpaConfig SpaIot8266Ssp
#define SpaButtons SpaIot8266SspButtons

#elif defined(ESP32)
//--------------------------------------
// ESP32

#if defined(ARDUINO_LOLIN_S3)
//--------------------------------------
// ESP32 S3
#define SpaConfig SpaIotS3Ssp
#define SpaButtons SpaIotS3SspButtons

#else
#define SpaConfig SpaIot32Ssp
#define SpaButtons SpaIot32SspButtons

#endif

#else
#error unsupported platform
#endif
