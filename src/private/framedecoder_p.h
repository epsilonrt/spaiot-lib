/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SpaIot library is licensed under a
   Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.

   You should have received a copy of the license along with this
   work. If not, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#include "../framedecoder.h"

namespace SpaIot {

  class FrameDecoder::Private {
    public:
      Private (FrameDecoder *q);
      Private (FrameDecoder *q, const BusSettings &settings, const std::map <int, LedSettings> &leds);
      static uint16_t  convertDisplayToCelsius (uint16_t displayValue);

    public:
      class Engine;
      FrameDecoder *const q_ptr;
      Engine *engine;

      // Parameters set by the constructor
      uint16_t frameLedPower;
      uint16_t frameLedFilter;
      uint16_t frameLedHeater;
      uint16_t frameLedHeaterReached;
      uint16_t frameLedBubble;
      uint16_t frameLedJet;
      uint16_t frameLedSanitizer;

      BusSettings busSettings;
      std::map <int, LedSettings> ledSettings;

      static const unsigned long FramePeriodUs = 320;
      static const uint32_t SetupTrigUnitChangeFrameCounterMax = (SetupTrigUnitChangeStepMaxMs * 1000) / FramePeriodUs;
      static const uint32_t ResetErrorFrameCounter = (ResetErrorTimeMs * 1000) / FramePeriodUs;
    private:
      PIMPL_DECLARE_PUBLIC (FrameDecoder);
  };

}
