/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#include "framedecoder_p.h"

namespace SpaIot {

  class FrameDecoder::Private::Engine : public Singleton <Engine> {
      friend class Singleton<Engine>;
    public:
      ~Engine();
      bool begin (const BusSettings & bus, unsigned long waitingTimeMs);
      void end();
      bool isReady() const;
      bool isOpen() const;

    public:
      BusSettings busSettings;

      // Data members of Decoder, updated by interrupt service routines
      volatile uint32_t  frameCounter;
      volatile uint32_t  frameDropped;
      volatile uint16_t  rawStatus;
      mutable volatile uint16_t  errorValue;
      volatile uint16_t  waterTemp;
      volatile uint16_t  desiredTemp;
      volatile uint16_t  sanitizerTime;

      // Working status variables, updated by interrupt service routines
      volatile uint16_t  frameValue;
      volatile uint16_t  frameShift;
      volatile uint16_t  displayValue;
      volatile uint32_t  lastSanitizerFrameCounter;
      volatile uint32_t  lastBlackDisplayFrameCounter;
      volatile bool      isDisplayBlink;
      volatile uint32_t  lastErrorChangeFrameCounter;
      volatile uint16_t  latestLedStatus;
      volatile uint16_t  stableLedStatusCounter;
      volatile uint16_t  latestDisplayValue;
      volatile uint16_t  stableDisplayValueCounter;
      volatile uint16_t  latestDesiredTemp;
      volatile uint16_t  latestWaterTemp;
      volatile uint16_t  stableWaterTempCounter;
      volatile uint16_t  unsetDigits;
      volatile uint8_t   lastTempUnit;
      volatile uint32_t  lastTempUnitChangeFrameCounter;
      volatile uint16_t  counterTempUnitChanged;

    private:
      Engine();
      static IRAM_ATTR void clkRisingInterrupt();
      static IRAM_ATTR void holdRisingInterrupt();
  };
}
