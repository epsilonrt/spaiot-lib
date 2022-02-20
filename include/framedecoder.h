/**
 * SpaIot Library (c) by espilonrt - epsilonrt@gmail.com
 * This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>
 * This file is based on DIYSCIP (c) by Geoffroy HUBERT
 *
 * SpaIot library is licensed under a
 * Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
 *
 * You should have received a copy of the license along with this
 * work. If not, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.
 *
 * SpaIot library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY;
 */
#pragma once

#include <map>
#include "bussettings.h"

namespace SpaIot {

  /**
   * @class FrameDecoder
   * @brief This class decodes information from the technical block of the spa
   *
     Cette classe décode les trames de façon à récupérer l'état des leds et la
     valeur présente sur l'afficheur à 7 segments.
     Cette classe ne détecte pas les appuis sur les boutons !
   *
   * Base class that can not be instantiated !
   */
  class FrameDecoder {
    public:
      void begin();
      bool isOpened() const;
      bool hasLed (int key) const;
      const BusSettings & busSettings() const;
      const std::map <int, LedSettings> ledSettings() const;

      uint16_t  rawStatus() const;

      uint8_t   isLedOn (int key) const;
      inline uint8_t   isPowerOn() const;
      inline uint8_t   isFilterOn() const;
      inline uint8_t   isBubbleOn() const;
      inline uint8_t   isHeatReached() const;
      inline uint8_t   isJetOn() const;
      inline uint8_t   isSanitizerOn() const;

      // Heater||HeatReached
      uint8_t   isHeaterOn() const;

      uint16_t  waterTemp() const;
      uint16_t  desiredTemp() const;
      uint16_t  sanitizerTime() const;

      uint32_t  frameCounter() const;
      uint32_t  frameDropped() const;

      uint16_t  error();

      /**
       * @brief AP request triggered
       *
       * Press the FC units change button for 6 times to switch to AP mode to
       * reconfigure the settings.
       * @return true if triggered
       */
      bool isSetupModeTriggered() const;

      uint8_t isDisplayBlink() const;
      uint8_t waitUntilDisplayBlink (unsigned long MaxWaitingTimeMs = 5000) const;
      uint16_t waitForWaterTemp (unsigned long MaxWaitingTimeMs = 25000) const;

    protected:
      FrameDecoder (const BusSettings & bus,
                  const std::map <int, LedSettings> & leds);
      static uint16_t  convertDisplayToCelsius (uint16_t m_displayValue);

    protected:
      const BusSettings & m_busSettings;
      const std::map <int, LedSettings> & m_ledSettings;

      // Parameters set by the constructor
      static int  m_dataPin;
      static uint16_t m_frameLedPower;
      static uint16_t m_frameLedFilter;
      static uint16_t m_frameLedHeater;
      static uint16_t m_frameLedHeaterReached;
      static uint16_t m_frameLedBubble;
      static uint16_t m_frameLedJet;
      static uint16_t m_frameLedSanitizer;

      // Data members of Decoder, updated by interrupt service routines
      static volatile uint32_t  m_frameCounter;
      static volatile uint32_t  m_frameDropped;
      static volatile uint16_t  m_rawStatus;
      static volatile uint16_t  m_errorValue;
      static volatile uint16_t  m_waterTemp;
      static volatile uint16_t  m_desiredTemp;
      static volatile uint16_t  m_sanitizerTime;

      // Working status variables, updated by interrupt service routines
      static volatile uint16_t  m_frameValue;
      static volatile uint16_t  m_frameShift;
      static volatile uint16_t  m_displayValue;
      static volatile uint32_t  m_lastSanitizerFrameCounter;
      static volatile uint32_t  m_lastBlackDisplayFrameCounter;
      static volatile bool      m_isDisplayBlink;
      static volatile uint32_t  m_lastErrorChangeFrameCounter;
      static volatile uint16_t  m_latestLedStatus;
      static volatile uint16_t  m_stableLedStatusCounter;
      static volatile uint16_t  m_latestDisplayValue;
      static volatile uint16_t  m_stableDisplayValueCounter;
      static volatile uint16_t  m_latestDesiredTemp;
      static volatile uint16_t  m_latestWaterTemp;
      static volatile uint16_t  m_stableWaterTempCounter;
      static volatile uint16_t  m_unsetDigits;
      static volatile uint8_t   m_lastTempUnit;
      static volatile uint32_t  m_lastTempUnitChangeFrameCounter;
      static volatile uint16_t  m_counterTempUnitChanged;

      static const unsigned long FramePeriodUs = 320;
      static const unsigned long SetupTrigUnitChangeFrameCounterMax =
        (SetupTrigUnitChangeStepMaxMs * 1000) / FramePeriodUs;

    private:
      bool m_isopened;
      static IRAM_ATTR void clkRisingInterrupt();
      static IRAM_ATTR void holdRisingInterrupt();
  };


  //----------------------------------------------------------------------------
  inline uint8_t FrameDecoder::isPowerOn() const {

    return isLedOn (Power);
  }

  //----------------------------------------------------------------------------
  inline uint8_t FrameDecoder::isFilterOn() const {

    return isLedOn (Filter);
  }

  //----------------------------------------------------------------------------
  inline uint8_t FrameDecoder::isBubbleOn() const {

    return isLedOn (Bubble);
  }

  //----------------------------------------------------------------------------
  inline uint8_t FrameDecoder::isHeatReached() const {

    return isLedOn (HeatReached);
  }

  //----------------------------------------------------------------------------
  inline uint8_t FrameDecoder::isJetOn() const {

    return isLedOn (Jet);
  }

  //----------------------------------------------------------------------------
  inline uint8_t FrameDecoder::isSanitizerOn() const {

    return isLedOn (Sanitizer);
  }

}
