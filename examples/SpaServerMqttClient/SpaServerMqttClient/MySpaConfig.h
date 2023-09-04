//
// SpaIot Example - MySpaConfig.h
//
#pragma once

#include <SpaIot.h>
using namespace SpaIot;

// My bus configuration :
#if defined(ESP8266)
// SDATA  -> GPIO12
// SCLK   -> GPIO14
// nWR    -> GPIO13
const BusSettings MyBus (12, 14, 13);

// My button controllers
Multiplexer MuxA ("U3", {5, 4, 15}, 16); // A->GPIO5, B->GPIO4, C->GPIO15, INH->GPIO16
Multiplexer MuxB ("U4", {5, 4, 15}, 0);  // A->GPIO5, B->GPIO4, C->GPIO15, INH->GPIO0

#elif defined(ARDUINO_LOLIN_S3)
// SDATA  -> GPIO23 MOSI GPIO11
// SCLK   -> GPIO18 SCLK GPIO12
// nWR    -> GPIO19 MISO GPIO10
const BusSettings MyBus (11, 12, 10);

// My button controllers
Multiplexer MuxA ("U3", {4, 5, 6}, 7);  // A->GPIO4, B->GPIO5, C->GPIO6, INH->GPIO7
Multiplexer MuxB ("U4", {4, 5, 6}, 15); // A->GPIO4, B->GPIO5, C->GPIO6, INH->GPIO15

#elif defined(ESP32)
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

// My buttons configuration (SSP)
const std::map<int, ButtonSettings> MyButtons = {
  { Filter,   ButtonSettings (MuxA, 1) },  // Filter   -> A1
  { Bubble,   ButtonSettings (MuxA, 3) },  // Bubble   -> A3
  { TempDown, ButtonSettings (MuxA, 7) },  // TempDown -> A7

  { Power,    ButtonSettings (MuxB, 2) },  // Power    -> B2
  { TempUp,   ButtonSettings (MuxB, 4) },  // TempUp   -> B4
  { TempUnit, ButtonSettings (MuxB, 5) },  // TempUnit -> B5
  { Heater,   ButtonSettings (MuxB, 7) }   // Heater   -> B7
};
