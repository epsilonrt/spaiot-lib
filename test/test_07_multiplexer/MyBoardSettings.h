//
// SpaIot Test 
// Board Settings
#pragma once

#include <Arduino.h>

#if defined(ESP8266)
//--------------------------------------
// ESP8266
#define SpaMuxA SpaIot8266MuxA
#define SpaMuxB SpaIot8266MuxB
#define SpaButtons SpaIot8266SspButtons

#elif defined(ESP32)
//--------------------------------------
// ESP32

#if defined(ARDUINO_LOLIN_S3)
//--------------------------------------
// ESP32 S3
#define SpaMuxA SpaIotS3MuxA
#define SpaMuxB SpaIotS3MuxB
#define SpaButtons SpaIotS3SspButtons

#else
#define SpaMuxA SpaIot32MuxA
#define SpaMuxB SpaIot32MuxB
#define SpaButtons SpaIot32SspButtons

#endif

#else
#error unsupported platform
#endif
