/*
 * SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
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
#include <Arduino.h>
#include <framedecoder.h>
#include "defines_p.h"
#include "spaiotdebug.h"

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                            FrameDecoder
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // protected constructor
  FrameDecoder::FrameDecoder () : m_isopened (false) {}

  //----------------------------------------------------------------------------
  // protected constructor
  FrameDecoder::FrameDecoder (const BusSettings & settings,
                              const std::map <int, LedSettings> & leds) :
    m_busSettings (settings), m_ledSettings (leds), m_isopened (false) {}

  //----------------------------------------------------------------------------
  const BusSettings & FrameDecoder::busSettings() const {

    return m_busSettings;
  }

  //----------------------------------------------------------------------------
  const std::map <int, LedSettings> FrameDecoder::ledSettings() const {

    return m_ledSettings;
  }

  //----------------------------------------------------------------------------
  void FrameDecoder::begin (const BusSettings & settings,
                            const std::map <int, LedSettings> & leds,
                            unsigned long waitingTimeMs) {
    m_busSettings = settings;
    m_ledSettings = leds;
    begin (waitingTimeMs);
  }

  //----------------------------------------------------------------------------
  void FrameDecoder::begin (unsigned long waitingTimeMs) {

    if (m_isopened == false)  {
      SPAIOT_DBG ("FrameDecoder::begin(): opening");

      m_dataPin = m_busSettings.dataPin();

      for (auto & led : m_ledSettings) {

        switch (led.first) {

          case Power:
            m_frameLedPower = led.second.frame();
            break;
          case Filter:
            m_frameLedFilter = led.second.frame();
            break;
          case HeatReached:
            m_frameLedHeaterReached = led.second.frame();
            break;
          case Heater:
            m_frameLedHeater = led.second.frame();
            break;
          case Bubble:
            m_frameLedBubble = led.second.frame();
            break;
          case Jet:
            m_frameLedJet = led.second.frame();
            break;
          case Sanitizer:
            m_frameLedSanitizer = led.second.frame();
            break;
          default:
            break;
        }
      }
      pinMode (m_busSettings.dataPin(), INPUT);
      pinMode (m_busSettings.clkPin(), INPUT);
      pinMode (m_busSettings.holdPin(), INPUT);

      SPAIOT_DBG ("Configure the CLK pin (%d) in interruption mode on the rising edge", m_busSettings.clkPin());
      attachInterrupt (digitalPinToInterrupt (m_busSettings.clkPin()),
                       FrameDecoder::clkRisingInterrupt, RISING);
      SPAIOT_DBG ("Configure the HOLD pin (%d) in interruption mode on the rising edge", m_busSettings.holdPin());
      attachInterrupt (digitalPinToInterrupt (m_busSettings.holdPin()),
                       FrameDecoder::holdRisingInterrupt, RISING);
      unsigned long timer = 0;
      while ( (m_rawStatus == UnsetValue16) && (timer < waitingTimeMs)) {

        delay (10);
        timer += 10;
      }
      m_isopened = (m_rawStatus != UnsetValue8);

      SPAIOT_DBG ("FrameDecoder::begin(): m_isopened: %d - rawStatus(): 0x%04X (%lums)",
                  m_isopened, rawStatus(), timer);
    }
  }

  //----------------------------------------------------------------------------
  void FrameDecoder::end() {
    if (m_isopened) {
      detachInterrupt (digitalPinToInterrupt (m_busSettings.clkPin()));
      detachInterrupt (digitalPinToInterrupt (m_busSettings.holdPin()));
      m_isopened = false;
    }
  }

  //----------------------------------------------------------------------------
  FrameDecoder::~FrameDecoder() {
    end();
  }

  //----------------------------------------------------------------------------
  bool FrameDecoder::isOpened() const {

    return m_isopened;
  }

  //----------------------------------------------------------------------------
  bool FrameDecoder::hasLed (int key) const {

    return m_ledSettings.count (key) == 1;
  }

  //----------------------------------------------------------------------------
  unsigned int FrameDecoder::frameCounter() const {

    return m_frameCounter;
  }

  //----------------------------------------------------------------------------
  unsigned int FrameDecoder::frameDropped() const {

    return m_frameDropped;
  }

  //----------------------------------------------------------------------------
  uint16_t FrameDecoder::waterTemp() const {

    return (m_waterTemp != UnsetValue16) ?
           convertDisplayToCelsius (m_waterTemp) : UnsetValue16;
  }

  //----------------------------------------------------------------------------
  uint16_t FrameDecoder::desiredTemp() const {

    return (m_desiredTemp != UnsetValue16) ?
           convertDisplayToCelsius (m_desiredTemp) : UnsetValue16;
  }

  //----------------------------------------------------------------------------
  uint16_t FrameDecoder::error() {

    if ( (m_errorValue != 0) &&
         ( (m_frameCounter - m_lastErrorChangeFrameCounter) > ResetErrorFrameCounter)) {
      // no error displayed since ResetErrorFrameCounter, so error has disappeared
      m_errorValue = 0;
    }

    return m_errorValue;
  }

  //----------------------------------------------------------------------------
  uint16_t  FrameDecoder::rawStatus() const {

    return (m_rawStatus != UnsetValue16) ? m_rawStatus & ~FRAME_BEEP_BIT : UnsetValue16;
  }

  //----------------------------------------------------------------------------
  uint8_t FrameDecoder::isLedOn (int key) const {

    return ( (m_rawStatus != UnsetValue16) && hasLed (key)) ?
           ( (m_rawStatus & m_ledSettings.at (key).frame()) ? true : false) :
           UnsetValue8;
  }

  //----------------------------------------------------------------------------
  uint8_t FrameDecoder::isHeaterOn() const {

    return ( (m_rawStatus != UnsetValue16) && hasLed (Heater)) ?
           ( (m_rawStatus & (m_frameLedHeater | m_frameLedHeaterReached)) ? true : false) :
           UnsetValue8;
  }

  //----------------------------------------------------------------------------
  uint16_t FrameDecoder::sanitizerTime() const {

    if ( (m_rawStatus != UnsetValue16) && (m_sanitizerTime != UnsetValue16) &&
         ( (m_frameCounter - m_lastSanitizerFrameCounter) > MIN_SANITIZER_FRAME_COUNTER)) {

      // sanitizer must have been set for at least MIN_SANITIZER_FRAME_COUNTER
      // because m_rawStatus may not have been set yet

      if (m_rawStatus & m_frameLedSanitizer) {

        uint16_t time = DIGITS2UINT (m_sanitizerTime);
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

    return (m_counterTempUnitChanged >= SetupTrigUnitChangeMin);
  }

  //----------------------------------------------------------------------------
  uint8_t FrameDecoder::isDisplayBlink() const {

    return m_isDisplayBlink;
  }

  //----------------------------------------------------------------------------
  uint8_t FrameDecoder::waitUntilDisplayBlink (unsigned long MaxWaitingTimeMs) const {

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

    if (isOpened() && (waterTemp() == UnsetValue16)) {
      const unsigned long looptime = 100;

      while ( (waterTemp() == UnsetValue16) && (MaxWaitingTimeMs > looptime)) {

        delay (looptime);
        MaxWaitingTimeMs -= looptime;
      }
    }
    return waterTemp();
  }

  //----------------------------------------------------------------------------
  //
  //                              Private
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  void FrameDecoder::clkRisingInterrupt() {

    SPAIOT_DBGLED_SET();
    m_frameValue = (m_frameValue << 1) + ! digitalRead (m_dataPin);
    m_frameShift ++;
    SPAIOT_DBGLED_CLR();
  }

  //----------------------------------------------------------------------------
  void FrameDecoder::holdRisingInterrupt() {

    SPAIOT_DBGLED_SET();
    m_frameCounter ++;

    if (m_frameShift == FRAME_BITS_SIZE) {
      m_frameShift = 0;

      if (m_frameValue != FRAME_CUE) {

        if (m_frameValue & FRAME_DISPLAY) {
          byte  digit;

          switch (m_frameValue & FRAME_DISPLAY_DIGIT_MASK) {

            case FRAME_DISPLAY_OFF:
              digit         = DIGITOFF_VALUE;
              m_unsetDigits   = 0;
              m_displayValue  = DISPLAY_OFF;
              break;

            case FRAME_DISPLAY_DIGIT0:
              digit = 0x0;
              break;

            case FRAME_DISPLAY_DIGIT1:
              digit = 0x1;
              break;

            case FRAME_DISPLAY_DIGIT2:
              digit = 0x2;
              break;

            case FRAME_DISPLAY_DIGIT3:
              digit = 0x3;
              break;

            case FRAME_DISPLAY_DIGIT4:
              digit = 0x4;
              break;
            case FRAME_DISPLAY_DIGIT5:
              digit = 0x5;
              break;

            case FRAME_DISPLAY_DIGIT6:
              digit = 0x6;
              break;

            case FRAME_DISPLAY_DIGIT7:
              digit = 0x7;
              break;

            case FRAME_DISPLAY_DIGIT8:
              digit = 0x8;
              break;

            case FRAME_DISPLAY_DIGIT9:
              digit = 0x9;
              break;

            case FRAME_DISPLAY_DIGITA:
              digit = 0xA;
              break;

            case FRAME_DISPLAY_DIGITC:
              digit = 0xC;
              break;

            case FRAME_DISPLAY_DIGITE:
              digit = 0xE;
              break;

            case FRAME_DISPLAY_DIGITF:
              digit = 0xF;
              break;

            case FRAME_DISPLAY_DIGITH:
              digit = DIGITH_VALUE; // happens on display4 when sanitizer time.
              break;

            default:
              return ;
          }

          if (m_frameValue & FRAME_DISPLAY_1) {

            m_displayValue = (m_displayValue & 0x0FFF) + (digit << 12);
            m_unsetDigits &= ~DISPLAY_DIGIT1;

            if (digit == 0xE) { // Display error, digit4 is not set

              m_displayValue = (m_displayValue & 0xFFF0);
              m_unsetDigits &= ~DISPLAY_DIGIT4;
            }

          }
          else if (m_frameValue & FRAME_DISPLAY_2) {

            m_displayValue = (m_displayValue & 0xF0FF) + (digit << 8);
            m_unsetDigits &= ~DISPLAY_DIGIT2;

          }
          else if (m_frameValue & FRAME_DISPLAY_3) {

            m_displayValue = (m_displayValue & 0xFF0F) + (digit << 4);
            m_unsetDigits &= ~DISPLAY_DIGIT3;

          }
          else if (m_frameValue & FRAME_DISPLAY_4) {

            m_displayValue = (m_displayValue & 0xFFF0) + digit;
            m_unsetDigits &= ~DISPLAY_DIGIT4;
          }

          if (m_unsetDigits == 0) {
            m_unsetDigits = DISPLAY_ALLDIGITS;

            if (m_displayValue == m_latestDisplayValue) {

              m_stableDisplayValueCounter--;
              if (m_stableDisplayValueCounter == 0) {

                m_stableDisplayValueCounter = INIT_STABLE_VALUE_COUNTER;

                if (m_displayValue == DISPLAY_OFF) {

                  m_lastBlackDisplayFrameCounter  = m_frameCounter;
                  m_isDisplayBlink = true;

                  if (m_latestDesiredTemp != UnsetValue16) {

                    m_desiredTemp = m_latestDesiredTemp;
                  }

                }
                else {

                  if ( (m_frameCounter - m_lastBlackDisplayFrameCounter) > BLINK_RESET_FRAME_MIN) { // blinking is over

                    m_isDisplayBlink    = false;
                    m_latestDesiredTemp = UnsetValue16;
                  }

                  if (NO_ERROR_ON_DISPLAY (m_displayValue)) {

                    if (TIMING_ON_DISPLAY (m_displayValue)) { // sanitizer time

                      m_sanitizerTime             = m_displayValue;
                      m_lastSanitizerFrameCounter = m_frameCounter;
                    }
                    else if (TEMP_ON_DISPLAY (m_displayValue)) {

                      if (m_isDisplayBlink && (m_errorValue == 0)) { // blinking but not in error !

                        // when blink finished, it displays water temp that should not be confused
                        // with desired temp !
                        // So desired temp is read just after a black screen and set at next black screen

                        if ( (m_frameCounter - m_lastBlackDisplayFrameCounter) < BLINK_DESIRED_FRAME_MAX) {

                          m_latestDesiredTemp = m_displayValue;
                        }

                      }
                      else {   // not blinking

                        if (m_displayValue == m_latestWaterTemp) {

                          m_stableWaterTempCounter--;
                          if (m_stableWaterTempCounter == 0) {

                            m_waterTemp = m_displayValue;
                            m_stableWaterTempCounter = INIT_STABLE_WATER_COUNTER;
                          }

                        }
                        else {

                          m_latestWaterTemp = m_displayValue;
                          m_stableWaterTempCounter = INIT_STABLE_WATER_COUNTER;
                        }

                        if (DISPLAY_UNIT (m_displayValue) != m_lastTempUnit) {

                          if ( (m_frameCounter - m_lastTempUnitChangeFrameCounter) < SetupTrigUnitChangeFrameCounterMax) {

                            m_counterTempUnitChanged++;
                          }
                          else {

                            m_counterTempUnitChanged = 0;
                          }

                          m_lastTempUnitChangeFrameCounter = m_frameCounter;
                          m_lastTempUnit = DISPLAY_UNIT (m_displayValue);
                        }
                      }
                    }

                  }
                  else {   // error on display

                    m_errorValue = DISPLAY2ERROR (m_displayValue);
                    m_lastErrorChangeFrameCounter = m_frameCounter;
                  }
                }
              }

            }
            else {   // m_displayValue not stable

              // While error, there is a black screen after error display
              // not visible by eye but must not break the stable counter

              if (NO_ERROR_ON_DISPLAY (m_latestDisplayValue) || (m_displayValue != DISPLAY_OFF)) {

                m_latestDisplayValue = m_displayValue;
                m_stableDisplayValueCounter = INIT_STABLE_VALUE_COUNTER;
              }
            }

          } // else all digits not yet set

        }
        else if (m_frameValue & FRAME_LED) {

          if (m_frameValue == m_latestLedStatus) {

            m_stableLedStatusCounter--;
            if (m_stableLedStatusCounter == 0) {

              m_rawStatus = m_frameValue;
              m_stableLedStatusCounter = INIT_STABLE_VALUE_COUNTER;
            }
          }
          else {

            m_latestLedStatus = m_frameValue;
            m_stableLedStatusCounter = INIT_STABLE_VALUE_COUNTER;
          }
        }
      } // else cue frame

    }
    else {   // esp misses some bits in frame (performance issue !?)

      m_frameDropped ++;
      m_frameShift = 0;
    }
    SPAIOT_DBGLED_CLR();
  }

  //----------------------------------------------------------------------------
  //
  //                              Protected
  //
  //----------------------------------------------------------------------------
  // Parameters set by the constructor
  int      FrameDecoder::m_dataPin = -1;
  uint16_t FrameDecoder::m_frameLedPower = 0;
  uint16_t FrameDecoder::m_frameLedFilter = 0;
  uint16_t FrameDecoder::m_frameLedHeater = 0;
  uint16_t FrameDecoder::m_frameLedHeaterReached = 0;
  uint16_t FrameDecoder::m_frameLedBubble = 0;
  uint16_t FrameDecoder::m_frameLedJet = 0;
  uint16_t FrameDecoder::m_frameLedSanitizer = 0;

  // Data members of Decoder, updated by interrupt service routines
  volatile uint32_t FrameDecoder::m_frameCounter   = 0;
  volatile uint32_t FrameDecoder::m_frameDropped   = 0;
  volatile uint16_t FrameDecoder::m_errorValue     = 0;
  volatile uint16_t FrameDecoder::m_rawStatus      = UnsetValue16;
  volatile uint16_t FrameDecoder::m_waterTemp      = UnsetValue16;
  volatile uint16_t FrameDecoder::m_desiredTemp    = UnsetValue16;
  volatile uint16_t FrameDecoder::m_sanitizerTime  = UnsetValue16;
  volatile uint16_t FrameDecoder::m_counterTempUnitChanged = 0;

  // Working status variables, updated by interrupt service routines
  volatile uint16_t FrameDecoder::m_frameShift     = 0;
  volatile uint16_t FrameDecoder::m_frameValue     = 0;
  volatile uint16_t FrameDecoder::m_displayValue   = UnsetValue16;
  volatile uint16_t FrameDecoder::m_unsetDigits   = DISPLAY_ALLDIGITS;
  volatile uint32_t FrameDecoder::m_lastSanitizerFrameCounter          = 0;
  volatile uint32_t FrameDecoder::m_lastBlackDisplayFrameCounter       = 0;
  volatile bool     FrameDecoder::m_isDisplayBlink                     = false;
  volatile uint32_t FrameDecoder::m_lastErrorChangeFrameCounter        = 0;
  volatile uint16_t FrameDecoder::m_latestLedStatus                    = UnsetValue16;
  volatile uint16_t FrameDecoder::m_stableLedStatusCounter             = INIT_STABLE_VALUE_COUNTER;
  volatile uint16_t FrameDecoder::m_latestDisplayValue                 = UnsetValue16;
  volatile uint16_t FrameDecoder::m_stableDisplayValueCounter          = INIT_STABLE_VALUE_COUNTER;
  volatile uint16_t FrameDecoder::m_latestDesiredTemp                  = UnsetValue16;
  volatile uint16_t FrameDecoder::m_latestWaterTemp                    = UnsetValue16;
  volatile uint16_t FrameDecoder::m_stableWaterTempCounter             = INIT_STABLE_WATER_COUNTER;
  volatile uint8_t  FrameDecoder::m_lastTempUnit                       = 0;
  volatile uint32_t FrameDecoder::m_lastTempUnitChangeFrameCounter     = 0;

  //----------------------------------------------------------------------------
  // protected
  uint16_t FrameDecoder::convertDisplayToCelsius (uint16_t value) {

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
}
//------------------------------------------------------------------------------
