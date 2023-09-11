//
// SpaIot Test 
// Board Settings
#pragma once

#include <Arduino.h>

#if defined(ESP8266)
//--------------------------------------
// ESP8266
#define SpaButtons SpaIot8266SspButtons
#define SpaMuxA "SpaIot8266MuxA"
#define SpaMuxB "SpaIot8266MuxB"

#elif defined(ESP32)
//--------------------------------------
// ESP32

#if defined(ARDUINO_LOLIN_S3)
//--------------------------------------
// ESP32 S3
#define SpaButtons SpaIotS3SspButtons
#define SpaMuxA "SpaIotS3MuxA"
#define SpaMuxB "SpaIotS3MuxB"

#else
#define SpaButtons SpaIot32SspButtons
#define SpaMuxA "SpaIot32MuxA"
#define SpaMuxB "SpaIot32MuxB"
#endif

#else
#error unsupported platform
#endif
