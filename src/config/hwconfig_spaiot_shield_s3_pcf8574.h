#pragma once
//----------------------------------------------------------------------------
//
//                    SpaIoT Shield Hardware Config © ϵIoT
//
// Definition to activate : SPAIOTCFG_SPAIOT_SHIELD_S3_PCF8574
//
// The buttons controller is a PCF8574 connected to 2 multiplexers 4051 as follows:
// - The selection pins {A, B, C} of the 2 multiplexers are connected
//   respectively to {P0, P1, P2} on the PCF8574
// - The INH pin of [A7..A0] multiplexer is connected to P3 on the PCF8574
// - The INH pin of [B7..B0] multiplexer is connected to P4 on the PCF8574
// This gives us the following values for the buttons of a SSP spa:
// |    KEY   | BIT | P4 | P3 | P2 | P1 | P0 | DEC |
// |:--------:|:---:|:--:|:--:|:--:|:--:|:--:|:---:|
// |   Power  |  B2 |  0 |  1 |  0 |  1 |  0 |  10 |
// |  Filter  |  A1 |  1 |  0 |  0 |  0 |  1 |  17 |
// |  Bubble  |  A3 |  1 |  0 |  0 |  1 |  1 |  19 |
// |  Heater  |  B7 |  0 |  1 |  1 |  1 |  1 |  15 |
// |  TempUp  |  B4 |  0 |  1 |  1 |  0 |  0 |  12 |
// | TempDown |  A7 |  1 |  0 |  1 |  1 |  1 |  23 |
// | TempUnit |  B5 |  0 |  1 |  1 |  0 |  1 |  13 |
//----------------------------------------------------------------------------
#include <hardwaresettings.h>
#include <button.h>

namespace SpaIot {

  // My bus configuration :
  // SDATA  -> GPIO23 MOSI GPIO11
  // SCLK   -> GPIO18 SCLK GPIO12
  // nWR    -> GPIO19 MISO GPIO10
  const BusSettings busSettings (11, 12, 10);

  // My button controller
  Pcf8574Mux Mux ("U5", 0x38);  // PCF8574: 0x20 - PCF8574A: 0x38

  /* ------------------------------ SspButtons -----------------------------------
   * POWER        = P
   * FILTER       = F
   * HEATER       = H
   * HEATER_RED   = HR
   * HEATER_GREEN = HG
   * BUBBLE       = B
   * JET          = J
   * SANITIZER    = S
   * DOWN         = D
   * UP           = U
   * FC           = C
   *
   *         |-------------------------------------------------------------------------------|
   *         | B7 | B6 | B5 | B4 | B3 | B2 | B1 | B0 | A7 | A6 | A5 | A4 | A3 | A2 | A1 | A0 |
   * BIT     | 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
   *
   * BUT_SSP |  H |  0 |  C |  U |  0 |  P |  X | BZ |  D |  0 |  0 |  X |  B |  0 |  F |  X |
  *
   * |    KEY    | BIT | P4 | P3 | P2 | P1 | P0 | DEC |
   * |:---------:|:---:|:--:|:--:|:--:|:--:|:--:|:---:|
   * |  Filter   |  A1 |  1 |  0 |  0 |  0 |  1 |  17 |
   * |  Bubble   |  A3 |  1 |  0 |  0 |  1 |  1 |  19 |
   * | TempDown  |  A7 |  1 |  0 |  0 |  0 |  1 |  23 |
   * |   Power   |  B2 |  0 |  1 |  0 |  1 |  0 |  10 |
   * |  TempUp   |  B4 |  0 |  1 |  1 |  0 |  0 |  12 |
   * | TempUnit  |  B5 |  0 |  1 |  1 |  0 |  1 |  13 |
   * |  Heater   |  B7 |  0 |  1 |  1 |  1 |  1 |  15 |
   ---------------------------------------------------------------------------*/
  const std::map<int, ButtonSettings> SspButtons = {
    { Filter,   ButtonSettings (Mux, 16 + 1) },
    { Bubble,   ButtonSettings (Mux, 16 + 3) },
    { TempDown, ButtonSettings (Mux, 16 + 7) },

    { Power,    ButtonSettings (Mux, 8 + 2) },
    { TempUp,   ButtonSettings (Mux, 8 + 4) },
    { TempUnit, ButtonSettings (Mux, 8 + 5) },
    { Heater,   ButtonSettings (Mux, 8 + 7) }
  };

  /* ------------------------------ SjbButtons -----------------------------------
   *         |-------------------------------------------------------------------------------|
   *         | B7 | B6 | B5 | B4 | B3 | B2 | B1 | B0 | A7 | A6 | A5 | A4 | A3 | A2 | A1 | A0 |
   * BIT     | 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
   *
   * BUT_SJB |  H |  0 | *C |  U |  0 |  P |  D | BZ |  F |  0 |  0 |  X | *B |  0 | *J |  S |
   *        * hypothesis that must be verified !
   *
   * |    KEY    | BIT | P4 | P3 | P2 | P1 | P0 | DEC |
   * |:---------:|:---:|:--:|:--:|:--:|:--:|:--:|:---:|
   * | Sanitizer |  A0 |  1 |  0 |  0 |  0 |  0 |  16 |
   * |   Jet     |  A1 |  1 |  0 |  0 |  0 |  1 |  17 |
   * |  Bubble   |  A3 |  1 |  0 |  0 |  1 |  1 |  19 |
   * |  Filter   |  A7 |  1 |  0 |  1 |  1 |  1 |  23 |
   * | TempDown  |  B1 |  0 |  1 |  0 |  0 |  1 |   9 |
   * |   Power   |  B2 |  0 |  1 |  0 |  1 |  0 |  10 |
   * |  TempUp   |  B4 |  0 |  1 |  1 |  0 |  0 |  12 |
   * | TempUnit  |  B5 |  0 |  1 |  1 |  0 |  1 |  13 |
   * |  Heater   |  B7 |  0 |  1 |  1 |  1 |  1 |  15 |
   ---------------------------------------------------------------------------*/
  const std::map<int, ButtonSettings> SjbButtons = {
    { Sanitizer,  ButtonSettings (Mux, 16 + 0) },
    { Jet,        ButtonSettings (Mux, 16 + 1) }, // this will have to be checked
    { Bubble,     ButtonSettings (Mux, 16 + 3) }, // this will have to be checked
    { Filter,     ButtonSettings (Mux, 16 + 7) },

    { TempDown,   ButtonSettings (Mux, 8 + 1) },
    { Power,      ButtonSettings (Mux, 8 + 2) },
    { TempUp,     ButtonSettings (Mux, 8 + 4) },
    { TempUnit,   ButtonSettings (Mux, 8 + 5) }, // this will have to be checked
    { Heater,     ButtonSettings (Mux, 8 + 7) },
  };

  // My configurations
  const HardwareSettings SspConfig (busSettings, SspLeds, SspButtons);
  const HardwareSettings SjbConfig (busSettings, SjbLeds, SjbButtons);
  const HardwareSettings & DefaultConfig = SspConfig;
}
