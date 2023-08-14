/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>
   This file is based on DIYSCIP (c) by Geoffroy HUBERT

   SpaIot library is licensed under a
   Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.

   You should have received a copy of the license along with this
   work. If not, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#include <Arduino.h>
#include "framedecoder_p.h"
#include "framedecoderengine_p.h"
#include "defines_p.h"
#include "../spaiotdebug.h"

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                            FrameDecoder
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // Private implementation

  //----------------------------------------------------------------------------
  FrameDecoder::Private::Private (FrameDecoder *q) : q_ptr (q) {

    engine = & Engine::instance();
    isopened = false;
    frameLedPower = 0;
    frameLedFilter = 0;
    frameLedHeater = 0;
    frameLedHeaterReached = 0;
    frameLedBubble = 0;
    frameLedJet = 0;
    frameLedSanitizer = 0;
  }

  //----------------------------------------------------------------------------
  FrameDecoder::Private::Private (FrameDecoder *q, const BusSettings &settings,
                                  const std::map <int, LedSettings> &leds) :
    Private(q) {
    busSettings = settings;
    ledSettings = leds;
  }

  //----------------------------------------------------------------------------
  uint16_t FrameDecoder::Private::convertDisplayToCelsius (uint16_t value) {

    uint16_t celsiusValue = DIGITS2UINT (value);

    if (DISPLAY_UNIT (value) == DISPLAY_UNIT_F) { // convert °F to °C
      double fValue = (double) celsiusValue;

      celsiusValue = (uint16_t) round ( ( (fValue - 32) * 5) / 9);

    }
    else if (DISPLAY_UNIT (value) != DISPLAY_UNIT_C) {

      return UnsetValue16;
    }

    return (celsiusValue >= 0) && (celsiusValue <= 60) ? celsiusValue : UnsetValue16;
  }

  //----------------------------------------------------------------------------
  // Protected constructor with private implementation
  FrameDecoder::FrameDecoder (Private &dd) : d_ptr (&dd) {}


  //----------------------------------------------------------------------------
  // API

  //----------------------------------------------------------------------------
  // Default constructor
  // Call the protected constructor with private implementation
  FrameDecoder::FrameDecoder() :
    FrameDecoder (*new Private(this)) {}

  //----------------------------------------------------------------------------
  FrameDecoder::FrameDecoder (const BusSettings &settings,
                              const std::map <int, LedSettings> &leds) :
    FrameDecoder (*new Private (this, settings, leds)) {}

  //----------------------------------------------------------------------------
  // Destructor
  // No default implementation, unique_ptr will call the destructor of the
  // private implementation
  FrameDecoder::~FrameDecoder() {
    end();
  }


  //----------------------------------------------------------------------------
  const BusSettings &FrameDecoder::busSettings() const {
    PIMPL_D (const FrameDecoder);

    return d->busSettings;
  }

  //----------------------------------------------------------------------------
  const std::map <int, LedSettings> FrameDecoder::ledSettings() const {
    PIMPL_D (const FrameDecoder);

    return d->ledSettings;
  }

  //----------------------------------------------------------------------------
  void FrameDecoder::begin (const BusSettings &settings,
                            const std::map <int, LedSettings> &leds,
                            unsigned long waitingTimeMs) {
    PIMPL_D (FrameDecoder);

    d->busSettings = settings;
    d->ledSettings = leds;
    begin (waitingTimeMs);
  }

  //----------------------------------------------------------------------------
  void FrameDecoder::begin (unsigned long waitingTimeMs) {

    if (!isOpened())  {
      PIMPL_D (FrameDecoder);

      SPAIOT_DBG ("FrameDecoder::begin(): opening");
      for (auto &led : d->ledSettings) {

        switch (led.first) {

          case Power:
            d->frameLedPower = led.second.frame();
            break;
          case Filter:
            d->frameLedFilter = led.second.frame();
            break;
          case HeatReached:
            d->frameLedHeaterReached = led.second.frame();
            break;
          case Heater:
            d->frameLedHeater = led.second.frame();
            break;
          case Bubble:
            d->frameLedBubble = led.second.frame();
            break;
          case Jet:
            d->frameLedJet = led.second.frame();
            break;
          case Sanitizer:
            d->frameLedSanitizer = led.second.frame();
            break;
          default:
            break;
        }
      }

      d->isopened = d->engine->begin (d->busSettings, waitingTimeMs);
    }
  }

  //----------------------------------------------------------------------------
  void FrameDecoder::end() {
    if (isOpened()) {
      PIMPL_D (FrameDecoder);
      
      d->engine->end();
      d->isopened = false;
    }
  }

  //----------------------------------------------------------------------------
  bool FrameDecoder::isOpened() const {
    PIMPL_D (const FrameDecoder);

    return d->isopened;
  }

  //----------------------------------------------------------------------------
  bool FrameDecoder::hasLed (int key) const {
    PIMPL_D (const FrameDecoder);

    return d->ledSettings.count (key) == 1;
  }

  //----------------------------------------------------------------------------
  unsigned int FrameDecoder::frameCounter() const {
    PIMPL_D (const FrameDecoder);

    return d->engine->frameCounter;
  }

  //----------------------------------------------------------------------------
  unsigned int FrameDecoder::frameDropped() const {
    PIMPL_D (const FrameDecoder);

    return d->engine->frameDropped;
  }

  //----------------------------------------------------------------------------
  uint16_t FrameDecoder::waterTemp() const {
    PIMPL_D (const FrameDecoder);

    return (d->engine->waterTemp != UnsetValue16) ?
           d->convertDisplayToCelsius (d->engine->waterTemp) : UnsetValue16;
  }

  //----------------------------------------------------------------------------
  uint16_t FrameDecoder::desiredTemp() const {
    PIMPL_D (const FrameDecoder);

    return (d->engine->desiredTemp != UnsetValue16) ?
           d->convertDisplayToCelsius (d->engine->desiredTemp) : UnsetValue16;
  }

  //----------------------------------------------------------------------------
  uint16_t FrameDecoder::error() const {
    PIMPL_D (const FrameDecoder);

    if ( (d->engine->errorValue != 0) &&
         ( (d->engine->frameCounter - d->engine->lastErrorChangeFrameCounter) > ResetErrorFrameCounter)) {
      // no error displayed since ResetErrorFrameCounter, so error has disappeared
      d->engine->errorValue = 0;
    }

    return d->engine->errorValue;
  }

  //----------------------------------------------------------------------------
  uint16_t  FrameDecoder::rawStatus() const {
    PIMPL_D (const FrameDecoder);

    return (d->engine->rawStatus != UnsetValue16) ? d->engine->rawStatus & ~FRAME_BEEP_BIT : UnsetValue16;
  }

  //----------------------------------------------------------------------------
  uint8_t FrameDecoder::isLedOn (int key) const {
    PIMPL_D (const FrameDecoder);

    return ( (d->engine->rawStatus != UnsetValue16) && hasLed (key)) ?
           ( (d->engine->rawStatus & d->ledSettings.at (key).frame()) ? true : false) :
           UnsetValue8;
  }

  //----------------------------------------------------------------------------
  uint8_t FrameDecoder::isHeaterOn() const {
    PIMPL_D (const FrameDecoder);

    return ( (d->engine->rawStatus != UnsetValue16) && hasLed (Heater)) ?
           ( (d->engine->rawStatus & (d->frameLedHeater | d->frameLedHeaterReached)) ? true : false) :
           UnsetValue8;
  }

  //----------------------------------------------------------------------------
  uint16_t FrameDecoder::sanitizerTime() const {
    PIMPL_D (const FrameDecoder);

    if ( (d->engine->rawStatus != UnsetValue16) && (d->engine->sanitizerTime != UnsetValue16) &&
         ( (d->engine->frameCounter - d->engine->lastSanitizerFrameCounter) > MIN_SANITIZER_FRAME_COUNTER)) {

      // sanitizer must have been set for at least MIN_SANITIZER_FRAME_COUNTER
      // because d->engine->rawStatus may not have been set yet

      if (d->engine->rawStatus & d->frameLedSanitizer) {

        uint16_t time = DIGITS2UINT (d->engine->sanitizerTime);
        // May happen sanitize time has fake value due to values multiplexing
        return ( (time <= 8) && (time > 0)) ? time : UnsetValue16;
      }
      else {

        return 0;
      }

    }
    else {

      return UnsetValue16;
    }
  }

  //----------------------------------------------------------------------------
  bool FrameDecoder::isSetupModeTriggered() const {
    PIMPL_D (const FrameDecoder);

    return (d->engine->counterTempUnitChanged >= SetupTrigUnitChangeMin);
  }

  //----------------------------------------------------------------------------
  uint8_t FrameDecoder::isDisplayBlink() const {
    PIMPL_D (const FrameDecoder);

    return d->engine->isDisplayBlink;
  }

  //----------------------------------------------------------------------------
  uint8_t FrameDecoder::waitUntilDisplayBlink (unsigned long MaxWaitingTimeMs) const {
    PIMPL_D (const FrameDecoder);

    if (isOpened()) {
      const unsigned long looptime = 100;

      while ( (isDisplayBlink() == true) && (MaxWaitingTimeMs > looptime)) {

        delay (looptime);
        MaxWaitingTimeMs -= looptime;
      }
    }
    return isDisplayBlink();
  }

  //----------------------------------------------------------------------------
  uint16_t FrameDecoder::waitForWaterTemp (unsigned long MaxWaitingTimeMs) const {
    PIMPL_D (const FrameDecoder);

    if (isOpened() && (waterTemp() == UnsetValue16)) {
      const unsigned long looptime = 100;

      while ( (waterTemp() == UnsetValue16) && (MaxWaitingTimeMs > looptime)) {

        delay (looptime);
        MaxWaitingTimeMs -= looptime;
      }
    }
    return waterTemp();
  }

}
//------------------------------------------------------------------------------
