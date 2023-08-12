#pragma once

//----------------------------------------------------------------------------
//
//                         SpaIoT Custom Hardware Config
//
// <WARNING> Edit the definitions below so that they correspond to the
//           diagram of your board
//
// Definition to activate : SPAIOTCFG_CUSTOM
//
//----------------------------------------------------------------------------
#include <hardwaresettings.h>
#include <button.h>

namespace SpaIot {

  // My bus configuration :
  // SDATA  -> GPIO23
  // SCLK   -> GPIO18
  // nWR    -> GPIO19
  const BusSettings busSettings (23, 18, 19);

  // My button controllers
  Multiplexer MuxA ("U3", {27, 16, 17}, 33); // A->GPIO27, B->GPIO16, C->GPIO17, INH->GPIO33
  Multiplexer MuxB ("U4", {27, 16, 17}, 26); // A->GPIO27, B->GPIO16, C->GPIO17, INH->GPIO26

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
   ---------------------------------------------------------------------------*/
  const std::map<int, ButtonSettings> SspButtons = {
    { Filter,   ButtonSettings (MuxA, 1) },
    { Bubble,   ButtonSettings (MuxA, 3) },
    { TempDown, ButtonSettings (MuxA, 7) },

    { Power,    ButtonSettings (MuxB, 2) },
    { TempUp,   ButtonSettings (MuxB, 4) },
    { TempUnit, ButtonSettings (MuxB, 5) },
    { Heater,   ButtonSettings (MuxB, 7) }
  };

  /* ------------------------------ SjbButtons -----------------------------------
   *         |-------------------------------------------------------------------------------|
   *         | B7 | B6 | B5 | B4 | B3 | B2 | B1 | B0 | A7 | A6 | A5 | A4 | A3 | A2 | A1 | A0 |
   * BIT     | 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
   *
   * BUT_SJB |  H |  0 | *C |  U |  0 |  P |  D | BZ |  F |  0 |  0 |  X | *B |  0 | *J |  S |
   *        * hypothesis that must be verified !
   ---------------------------------------------------------------------------*/
  const std::map<int, ButtonSettings> SjbButtons = {
    { Sanitizer,  ButtonSettings (MuxA, 0) },
    { Jet,        ButtonSettings (MuxA, 1) }, // this will have to be checked
    { Bubble,     ButtonSettings (MuxA, 3) }, // this will have to be checked
    { Filter,     ButtonSettings (MuxA, 7) },

    { TempDown,   ButtonSettings (MuxB, 1) },
    { Power,      ButtonSettings (MuxB, 2) },
    { TempUp,     ButtonSettings (MuxB, 4) },
    { TempUnit,   ButtonSettings (MuxB, 5) }, // this will have to be checked
    { Heater,     ButtonSettings (MuxB, 7) },
  };

  // My custom configurations
  const HardwareSettings SspConfig (busSettings, SspLeds, SspButtons);
  const HardwareSettings SjbConfig (busSettings, SjbLeds, SjbButtons);
  const HardwareSettings & DefaultConfig = SspConfig;
}
