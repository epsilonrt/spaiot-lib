//
// SpaIot Example 
// Board Settings
#pragma once

#include <Arduino.h>

#if defined(ESP8266)
//--------------------------------------
// ESP8266
#define Console Serial
#define SpaModel "SPAIOT8266SSP"

#elif defined(ESP32)
//--------------------------------------
// ESP32

#if defined(ARDUINO_LOLIN_S3)
//--------------------------------------
// ESP32 S3
#if ARDUINO_USB_CDC_ON_BOOT
#define Console Serial0 
#else
#define Console Serial
#endif 
#define SpaModel "SPAIOTS3SSP"
#else
#define Console Serial
#define SpaModel "SPAIOT32SSP"
#endif

#else
#error unsupported platform
#endif
