//
// SpaIot Example 
// Board Settings
#pragma once

#include <Arduino.h>
#include <SpaIot.h>

using namespace SpaIot;

// My bus configuration :
#if defined(ESP8266)
#define Console Serial
// SDATA  -> GPIO12
// SCLK   -> GPIO14
// nWR    -> GPIO13
const BusSettings MyBus (12, 14, 13);

// My button controllers
Multiplexer MuxA ("U3", {5, 4, 15}, 16); // A->GPIO5, B->GPIO4, C->GPIO15, INH->GPIO16
Multiplexer MuxB ("U4", {5, 4, 15}, 0);  // A->GPIO5, B->GPIO4, C->GPIO15, INH->GPIO0

#elif defined(ARDUINO_LOLIN_S3)
#if ARDUINO_USB_CDC_ON_BOOT
#define Console Serial0 
#else
#define Console Serial
#endif
// SDATA  -> GPIO23 MOSI GPIO11
// SCLK   -> GPIO18 SCLK GPIO12
// nWR    -> GPIO19 MISO GPIO10
const BusSettings MyBus (11, 12, 10);

// My button controllers
Multiplexer MuxA ("U3", {4, 5, 6}, 7);  // A->GPIO4, B->GPIO5, C->GPIO6, INH->GPIO7
Multiplexer MuxB ("U4", {4, 5, 6}, 15); // A->GPIO4, B->GPIO5, C->GPIO6, INH->GPIO15

#elif defined(ESP32)
#define Console Serial
// SDATA  -> GPIO23
// SCLK   -> GPIO18
// nWR    -> GPIO19
const BusSettings MyBus (23, 18, 19);

// My button controllers
Multiplexer MuxA ("U3", {27, 16, 17}, 33); // A->GPIO27, B->GPIO16, C->GPIO17, INH->GPIO33
Multiplexer MuxB ("U4", {27, 16, 17}, 26); // A->GPIO27, B->GPIO16, C->GPIO17, INH->GPIO26

#else
#error unsupported platform
#endif

