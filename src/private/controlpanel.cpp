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
#include <Arduino.h>
#include "controlpanel_p.h"
#include "framedecoderengine_p.h"
#include "defines_p.h"
#include "../spaiotdebug.h"

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                            ControlPanel
  //
  //----------------------------------------------------------------------------


  //----------------------------------------------------------------------------
  // Private implementation

  //----------------------------------------------------------------------------
  ControlPanel::Private::Private (ControlPanel *q) : FrameDecoder::Private (q), isopen (false)
  {}

  //----------------------------------------------------------------------------
  ControlPanel::Private::Private (ControlPanel *q, const HardwareSettings &hwsettings) :
    FrameDecoder::Private (q, hwsettings.bus(), hwsettings.leds()),
    btnsettings (hwsettings.buttons()), isopen (false)
  {}

  //----------------------------------------------------------------------------
  bool ControlPanel::Private::makeButtons() {
    bool b_isopened = true;

    // We create the buttons from their settings...
    for (const auto &elmt : btnsettings) {
      int key = elmt.first;
      const ButtonSettings &settings = elmt.second;
      Button b (settings);

      button.emplace (key, b);
      if (b.isOpen() == false) {

        b.begin();
        b_isopened = b_isopened && b.isOpen();
      }
    }
    return b_isopened;
  }

  //----------------------------------------------------------------------------
  // may be used for Power, Filter, Bubble and Jet
  uint8_t ControlPanel::Private::setKeyOn (int key, bool v) {

    if (isopen) {
      PIMPL_Q (ControlPanel);
      uint8_t b = q->isLedOn (key);

      if (q->hasButton (key) && b != UnsetValue8) {

        if (v ^ b) {

          button[key].push();
        }
        return q->isLedOn (key);
      }
    }
    return UnsetValue8;
  }

  //----------------------------------------------------------------------------
  const std::map <int, Button> &ControlPanel::Private::buttons() const {

    return button;
  }

  //----------------------------------------------------------------------------
  // Protected constructor with private implementation
  ControlPanel::ControlPanel (Private &dd) : FrameDecoder (dd)
  {}

  //----------------------------------------------------------------------------
  // API

  //----------------------------------------------------------------------------
  // Default constructor
  // Call the protected constructor with private implementation
  ControlPanel::ControlPanel() :
    ControlPanel (*new Private (this))
  {}

  //----------------------------------------------------------------------------
  ControlPanel::ControlPanel (const HardwareSettings &hwsettings) :
    ControlPanel (*new Private (this, hwsettings))
  {}

  //----------------------------------------------------------------------------
  ControlPanel::ControlPanel (const String &hwSettingsName) :
    ControlPanel (*new Private (this, HardwareSettings::getFromRegister (hwSettingsName)))
  {}
  
  //----------------------------------------------------------------------------
  void ControlPanel::end() {

    if (isOpen()) {
      PIMPL_D (ControlPanel);

      for (auto &elmt : d->button) {
        Button &b = elmt.second;

        if (b.isOpen()) {

          b.ctrl().end();
        }
      }
      FrameDecoder::end();
    }
  }

  //----------------------------------------------------------------------------
  void ControlPanel::begin (const HardwareSettings &hwsettings,
                            unsigned long waitingTimeMs) {

    if (!isOpen())  {
      PIMPL_D (ControlPanel);

      SPAIOT_DBG ("ControlPanel::begin(): opening");

      d->btnsettings = hwsettings.buttons();
      FrameDecoder::begin (hwsettings.bus(), hwsettings.leds(), waitingTimeMs);
      d->isopen = d->makeButtons() && FrameDecoder::isOpen();
    }

  }

  //----------------------------------------------------------------------------
  void ControlPanel::begin (const String &hwSettingsName,
                            unsigned long waitingTimeMs) {

    if (HardwareSettings::registerContains (hwSettingsName)) {

      begin (HardwareSettings::getFromRegister (hwSettingsName), waitingTimeMs);
    }
    else {

      SPAIOT_DBG ("%s:%d: <Fatal Error> Unable to find the '%s' "
                  "HardwareSettings in the register, check his name !",
                  __PRETTY_FUNCTION__, __LINE__,
                  hwSettingsName.c_str());
    }

  }

  //----------------------------------------------------------------------------
  void ControlPanel::begin (unsigned long waitingTimeMs) {

    if (!isOpen())  {
      PIMPL_D (ControlPanel);

      SPAIOT_DBG ("ControlPanel::begin(): opening");

      FrameDecoder::begin (waitingTimeMs);
      d->isopen = d->makeButtons() && FrameDecoder::isOpen();
    }
  }

  //----------------------------------------------------------------------------
  bool ControlPanel::isOpen() const {
    PIMPL_D (const ControlPanel);

    return d->isopen;
  }

  //----------------------------------------------------------------------------
  bool ControlPanel::hasButton (int key) const {
    PIMPL_D (const ControlPanel);

    return d->button.count (key) == 1;
  }

  //----------------------------------------------------------------------------
  Button &ControlPanel::button (int key) {
    PIMPL_D (ControlPanel);

    return d->button.at (key);
  }

  //----------------------------------------------------------------------------
  bool ControlPanel::pushButton (int key)  {

    if (hasButton (key)) {
      PIMPL_D (ControlPanel);

      d->button[key].push();
      return true;
    }
    return false;
  }


  //----------------------------------------------------------------------------
  uint8_t ControlPanel::setPower (bool v) {
    PIMPL_D (ControlPanel);

    return d->setKeyOn (Power, v);
  }

  //----------------------------------------------------------------------------
  uint8_t ControlPanel::setFilter (bool v) {
    PIMPL_D (ControlPanel);

    return d->setKeyOn (Filter, v);
  }

  //----------------------------------------------------------------------------
  uint8_t ControlPanel::setHeater (bool v) {

    if (isOpen() && hasButton (Heater)) {
      uint8_t b = isHeaterOn ();

      if (b != UnsetValue8) {

        if (v ^ b) {
          PIMPL_D (ControlPanel);
          int count = 0;

          d->button[Heater].push();
          while ( (isHeaterOn() != v) && (count < 20)) {

            delay (100);
            count++;
          }
        }
        return isHeaterOn();
      }
    }
    return UnsetValue8;
  }

  //----------------------------------------------------------------------------
  uint8_t ControlPanel::setBubble (bool v) {
    PIMPL_D (ControlPanel);

    return d->setKeyOn (Bubble, v);
  }

  //----------------------------------------------------------------------------
  uint8_t ControlPanel::setJet (bool v) {
    PIMPL_D (ControlPanel);

    return d->setKeyOn (Jet, v);
  }

  //----------------------------------------------------------------------------
  bool ControlPanel::setSanitizerTime (uint16_t time) {

    if (hasButton (Sanitizer)) {

      if (isOpen()) {
        PIMPL_D (ControlPanel);

        if ( (d->engine->rawStatus != UnsetValue16) &&
             ( (d->engine->rawStatus & d->frameLedPower) != 0) && (d->engine->errorValue == 0)) {
          uint16_t pushCounter = 0;

          if (time == 0) {

            if ( (d->engine->rawStatus != UnsetValue16) && (d->engine->rawStatus & d->frameLedSanitizer)) {

              do {

                d->button[Sanitizer].push();
                delay (ButtonIntervalMs);

                pushCounter ++;

                // push button till 8H
              }
              while ( (DIGITS2UINT (d->engine->sanitizerTime) != 8) &&
                      (pushCounter < PUSH_COUNTER_MAX));
              // then push a last time to cancel
              d->button[Sanitizer].push();

            } // else already off

          }
          else if ( (time == 3) || (time == 5) || (time == 8)) {

            do {

              d->button[Sanitizer].push();
              delay (ButtonIntervalMs);

              pushCounter ++;

            }
            while ( (pushCounter < PUSH_COUNTER_MAX) &&
                    (DIGITS2UINT (d->engine->sanitizerTime) != time));

          }
          else {   // Invalid value

            return false;
          }
        }
        return true;
      }
    }
    else {

      return true;
    }
    return false;
  }

  //----------------------------------------------------------------------------
  bool ControlPanel::setDesiredTemp (uint16_t temp) {

    if ( (temp >= DesiredTempMin) &&
         (temp <= DesiredTempMax) &&
         hasButton (TempUp) && hasButton (TempDown) && isOpen()) {
      PIMPL_D (ControlPanel);

      if ( (waitForDesiredTemp() != UnsetValue16) && (d->engine->errorValue == 0)) {
        uint16_t  uint16DesiredTemp = desiredTemp();
        int  pushCounter = temp - uint16DesiredTemp;

        SPAIOT_DBG ("%s:%d: desiredTemp: %d'C -> %d'C > %+d'C",
                    __PRETTY_FUNCTION__, __LINE__,
                    desiredTemp(), temp, pushCounter);

        if (pushCounter != 0) {

          // faster version
          while (pushCounter != 0) {

            if (pushCounter > 0) {

              d->button[TempUp].push();
              pushCounter--;
            }
            else {

              d->button[TempDown].push();
              pushCounter++;
            }
            delay (HoldPressedMs);
          }

          // Adjust if necessery
          pushCounter = 0;
          while ( (pushCounter < PUSH_COUNTER_MAX) &&
                  ( (uint16DesiredTemp = desiredTemp()) != temp)) {

            if (uint16DesiredTemp > temp) {

              d->button[TempDown].push();
            }
            else {

              d->button[TempUp].push();
            }
            delay (ButtonIntervalMs);
            SPAIOT_DBG ("%s:%d: pushCounter %d - desiredTemp %d'C",
                        __PRETTY_FUNCTION__, __LINE__,
                        pushCounter, desiredTemp());
            pushCounter++;
          }

          if ( (pushCounter >= PUSH_COUNTER_MAX) ||
               ( (uint16DesiredTemp = desiredTemp()) != temp)) {

            SPAIOT_DBG ("%s:%d: Unable to set desiredTemp to %d'C: pushCounter %d - desiredTemp %d'C",
                        __PRETTY_FUNCTION__, __LINE__,
                        temp, pushCounter, desiredTemp());
            return false;
          }
        }
      }
      else {

        SPAIOT_DBG ("%s:%d: Unable to read desiredTemp to %d'C",
                    __PRETTY_FUNCTION__, __LINE__,
                    desiredTemp());
        return false;
      }
    }
    else {

      SPAIOT_DBG ("%s:%d: Unable to set desiredTemp to %d'C: is_opened %d - min %d'C - max %d'C",
                  __PRETTY_FUNCTION__, __LINE__,
                  temp, isOpen(), DesiredTempMin, DesiredTempMax);
      return false;
    }
    return true;
  }

  //----------------------------------------------------------------------------
  uint16_t ControlPanel::waitForDesiredTemp (unsigned long  MaxWaitingTimeMs) {

    if (isOpen() && (desiredTemp() == UnsetValue16)) {
      PIMPL_D (ControlPanel);
      const unsigned long looptime = 100;

      d->button[TempUp].push();
      while ( (isDisplayBlink() != true) && (MaxWaitingTimeMs > looptime)) {

        delay (looptime);
        MaxWaitingTimeMs -= looptime;
      }
      if (isDisplayBlink() == true) {

        while ( (desiredTemp() == UnsetValue16) && (MaxWaitingTimeMs > looptime)) {

          delay (looptime);
          MaxWaitingTimeMs -= looptime;
        }
      }
    }
    return desiredTemp();
  }

  //----------------------------------------------------------------------------
  const std::map <int, ButtonSettings> ControlPanel::buttonSettings() const {
    PIMPL_D (const ControlPanel);

    return d->btnsettings;
  }

}
//------------------------------------------------------------------------------
