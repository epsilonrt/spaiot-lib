/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>
   This file is based on DIYSCIP (c) by Geoffroy HUBERT

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
// #include <mutex>
#include "framedecoderengine_p.h"
#include "defines_p.h"
#include "../spaiotdebug.h"

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                   FrameDecoder::Private::Engine
  //
  //----------------------------------------------------------------------------
  FrameDecoder::Private::Engine::Engine() {

    // Data members of Decoder, updated by interrupt service routines
    frameCounter                  = 0;
    frameDropped                  = 0;
    rawStatus                     = UnsetValue16;
    errorValue                    = 0;
    waterTemp                     = UnsetValue16;
    desiredTemp                   = UnsetValue16;
    sanitizerTime                 = UnsetValue16;
    counterTempUnitChanged        = 0;

    // Working status variables, updated by interrupt service routines
    frameValue                    = 0;
    frameShift                    = 0;
    displayValue                  = UnsetValue16;
    lastSanitizerFrameCounter     = 0;
    lastBlackDisplayFrameCounter  = 0;
    isDisplayBlink                = false;
    lastErrorChangeFrameCounter   = 0;
    latestLedStatus               = UnsetValue16;
    stableLedStatusCounter        = INIT_STABLE_VALUE_COUNTER;
    latestDisplayValue            = UnsetValue16;
    stableDisplayValueCounter     = INIT_STABLE_VALUE_COUNTER;
    latestDesiredTemp             = UnsetValue16;
    latestWaterTemp               = UnsetValue16;
    stableWaterTempCounter        = INIT_STABLE_WATER_COUNTER;
    unsetDigits                   = DISPLAY_ALLDIGITS;
    lastTempUnit                  = 0;
    lastTempUnitChangeFrameCounter = 0;
  }

  //----------------------------------------------------------------------------
  FrameDecoder::Private::Engine::~Engine() {

    end();
  }

  //----------------------------------------------------------------------------
  bool FrameDecoder::Private::Engine::begin (const BusSettings &bus, unsigned long waitingTimeMs) {

    busSettings = bus;
    pinMode (bus.dataPin(), INPUT);
    pinMode (bus.clkPin(), INPUT);
    pinMode (bus.holdPin(), INPUT);

    SPAIOT_DBGP (PSTR ("Configure the CLK pin (%d) in interruption mode on the rising edge"), bus.clkPin());
    attachInterrupt (digitalPinToInterrupt (bus.clkPin()),
                     clkRisingInterrupt, RISING);
    SPAIOT_DBGP (PSTR ("Configure the HOLD pin (%d) in interruption mode on the rising edge"), bus.holdPin());
    attachInterrupt (digitalPinToInterrupt (bus.holdPin()),
                     holdRisingInterrupt, RISING);
    unsigned long timer = 0;
    while ( (rawStatus == UnsetValue16) && (timer < waitingTimeMs)) {

      delay (10);
      timer += 10;
    }
    bool isready = isReady();
    SPAIOT_DBGP (PSTR ("FrameDecoder::Private::Engine::begin: isready: %d - rawStatus: 0x%04X (%lums)"),
                 isready, rawStatus, timer);
    return isready;
  }

  //----------------------------------------------------------------------------
  bool FrameDecoder::Private::Engine::isReady() const {

    return (rawStatus != UnsetValue8);
  }

  //----------------------------------------------------------------------------
  bool FrameDecoder::Private::Engine::isOpen() const {

    return !busSettings.isEmpty();
  }

  //----------------------------------------------------------------------------
  void FrameDecoder::Private::Engine::end() {

    if (isOpen()) {

      detachInterrupt (digitalPinToInterrupt (busSettings.clkPin()));
      detachInterrupt (digitalPinToInterrupt (busSettings.holdPin()));
      busSettings.clear();
    }
  }

//----------------------------------------------------------------------------
void FrameDecoder::Private::Engine::clkRisingInterrupt() {
  SINGLETON_I (Engine);

  SPAIOT_DBGLED_SET();
  i.frameValue = (i.frameValue << 1) + ! digitalRead (i.busSettings.dataPin());
  i.frameShift ++;
  SPAIOT_DBGLED_CLR();
}


//----------------------------------------------------------------------------
void FrameDecoder::Private::Engine::holdRisingInterrupt() {
  SINGLETON_I (Engine);

  SPAIOT_DBGLED_SET();
  i.frameCounter++;

  if (i.frameShift == FRAME_BITS_SIZE) {
    i.frameShift = 0;

    if (i.frameValue != FRAME_CUE) {

      if (i.frameValue & FRAME_DISPLAY) {
        byte  digit;

        switch (i.frameValue & FRAME_DISPLAY_DIGIT_MASK) {

          case FRAME_DISPLAY_OFF:
            digit         = DIGITOFF_VALUE;
            i.unsetDigits   = 0;
            i.displayValue  = DISPLAY_OFF;
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

        if (i.frameValue & FRAME_DISPLAY_1) {

          i.displayValue = (i.displayValue & 0x0FFF) + (digit << 12);
          i.unsetDigits &= ~DISPLAY_DIGIT1;

          if (digit == 0xE) { // Display error, digit4 is not set

            i.displayValue = (i.displayValue & 0xFFF0);
            i.unsetDigits &= ~DISPLAY_DIGIT4;
          }

        }
        else if (i.frameValue & FRAME_DISPLAY_2) {

          i.displayValue = (i.displayValue & 0xF0FF) + (digit << 8);
          i.unsetDigits &= ~DISPLAY_DIGIT2;

        }
        else if (i.frameValue & FRAME_DISPLAY_3) {

          i.displayValue = (i.displayValue & 0xFF0F) + (digit << 4);
          i.unsetDigits &= ~DISPLAY_DIGIT3;

        }
        else if (i.frameValue & FRAME_DISPLAY_4) {

          i.displayValue = (i.displayValue & 0xFFF0) + digit;
          i.unsetDigits &= ~DISPLAY_DIGIT4;
        }

        if (i.unsetDigits == 0) {
          i.unsetDigits = DISPLAY_ALLDIGITS;

          if (i.displayValue == i.latestDisplayValue) {

            i.stableDisplayValueCounter--;
            if (i.stableDisplayValueCounter == 0) {

              i.stableDisplayValueCounter = INIT_STABLE_VALUE_COUNTER;

              if (i.displayValue == DISPLAY_OFF) {

                i.lastBlackDisplayFrameCounter  = i.frameCounter;
                i.isDisplayBlink = true;

                if (i.latestDesiredTemp != UnsetValue16) {

                  i.desiredTemp = i.latestDesiredTemp;
                }

              }
              else {

                if ( (i.frameCounter - i.lastBlackDisplayFrameCounter) > BLINK_RESET_FRAME_MIN) { // blinking is over

                  i.isDisplayBlink    = false;
                  i.latestDesiredTemp = UnsetValue16;
                }

                if (NO_ERROR_ON_DISPLAY (i.displayValue)) {

                  if (TIMING_ON_DISPLAY (i.displayValue)) { // sanitizer time

                    i.sanitizerTime             = i.displayValue;
                    i.lastSanitizerFrameCounter = i.frameCounter;
                  }
                  else if (TEMP_ON_DISPLAY (i.displayValue)) {

                    if (i.isDisplayBlink && (i.errorValue == 0)) { // blinking but not in error !

                      // when blink finished, it displays water temp that should not be confused
                      // with desired temp !
                      // So desired temp is read just after a black screen and set at next black screen

                      if ( (i.frameCounter - i.lastBlackDisplayFrameCounter) < BLINK_DESIRED_FRAME_MAX) {

                        i.latestDesiredTemp = i.displayValue;
                      }

                    }
                    else {   // not blinking

                      if (i.displayValue == i.latestWaterTemp) {

                        i.stableWaterTempCounter--;
                        if (i.stableWaterTempCounter == 0) {

                          i.waterTemp = i.displayValue;
                          i.stableWaterTempCounter = INIT_STABLE_WATER_COUNTER;
                        }

                      }
                      else {

                        i.latestWaterTemp = i.displayValue;
                        i.stableWaterTempCounter = INIT_STABLE_WATER_COUNTER;
                      }

                      if (DISPLAY_UNIT (i.displayValue) != i.lastTempUnit) {

                        if ( (i.frameCounter - i.lastTempUnitChangeFrameCounter) < SetupTrigUnitChangeFrameCounterMax) {

                          i.counterTempUnitChanged++;
                        }
                        else {

                          i.counterTempUnitChanged = 0;
                        }

                        i.lastTempUnitChangeFrameCounter = i.frameCounter;
                        i.lastTempUnit = DISPLAY_UNIT (i.displayValue);
                      }
                    }
                  }

                }
                else {   // error on display

                  i.errorValue = DISPLAY2ERROR (i.displayValue);
                  i.lastErrorChangeFrameCounter = i.frameCounter;
                }
              }
            }

          }
          else {   // i.displayValue not stable

            // While error, there is a black screen after error display
            // not visible by eye but must not break the stable counter

            if (NO_ERROR_ON_DISPLAY (i.latestDisplayValue) || (i.displayValue != DISPLAY_OFF)) {

              i.latestDisplayValue = i.displayValue;
              i.stableDisplayValueCounter = INIT_STABLE_VALUE_COUNTER;
            }
          }

        } // else all digits not yet set

      }
      else if (i.frameValue & FRAME_LED) {

        if (i.frameValue == i.latestLedStatus) {

          i.stableLedStatusCounter--;
          if (i.stableLedStatusCounter == 0) {

            i.rawStatus = i.frameValue;
            i.stableLedStatusCounter = INIT_STABLE_VALUE_COUNTER;
          }
        }
        else {

          i.latestLedStatus = i.frameValue;
          i.stableLedStatusCounter = INIT_STABLE_VALUE_COUNTER;
        }
      }
    } // else cue frame

  }
  else {   // esp misses some bits in frame (performance issue !?)

    i.frameDropped ++;
    i.frameShift = 0;
  }
  SPAIOT_DBGLED_CLR();
}

}
//------------------------------------------------------------------------------
