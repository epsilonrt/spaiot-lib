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
#define SpaBus SpaIot8266Bus
#define SspSettings "SPAIOT8266SSP"
#define SjbSettings "SPAIOT8266SJB"

#elif defined(ESP32)
//--------------------------------------
// ESP32

#if defined(ARDUINO_LOLIN_S3)
//--------------------------------------
// ESP32 S3
#define SpaButtons SpaIotS3SspButtons
#define SpaMuxA "SpaIotS3MuxA"
#define SpaMuxB "SpaIotS3MuxB"
#define SpaBus SpaIotS3Bus
#define SspSettings "SPAIOTS3SSP"
#define SjbSettings "SPAIOTS3SJB"

#else
#define SpaButtons SpaIot32SspButtons
#define SpaMuxA "SpaIot32MuxA"
#define SpaMuxB "SpaIot32MuxB"
#define SpaBus SpaIot32Bus
#define SspSettings "SPAIOT32SSP"
#define SjbSettings "SPAIOT32SJB"

#endif

#else
#error unsupported platform
#endif
