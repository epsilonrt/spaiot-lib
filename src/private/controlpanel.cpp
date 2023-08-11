/*
 * SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
 * This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>
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
#include <controlpanel.h>
#include "defines_p.h"
#include <spaiotdebug.h>

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                            ControlPanel
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // static
  ControlPanel * ControlPanel::m_instance;

  //----------------------------------------------------------------------------
  // static
  ControlPanel * ControlPanel::getInstance (const HardwareSettings & hwsettings) {

    if (m_instance == 0) {

      m_instance = new ControlPanel (hwsettings);
    }
    return m_instance;
  }

  //----------------------------------------------------------------------------
  // static
  ControlPanel * ControlPanel::getInstance () {

    if (m_instance == 0) {

      m_instance = new ControlPanel ();
    }
    return m_instance;
  }

  //----------------------------------------------------------------------------
  // static
  ControlPanel& ControlPanel::singleton (const HardwareSettings& hwsettings) {

    return * getInstance (hwsettings);
  }

  //----------------------------------------------------------------------------
  // static
  ControlPanel * ControlPanel::getInstance (const String & hwSettingsName) {

    if (HardwareSettings::registerContains (hwSettingsName)) {

      m_instance = new ControlPanel (HardwareSettings::getFromRegister (hwSettingsName));
    }
    else {

      SPAIOT_DBG ("%s:%d: <Fatal Error> Unable to find the '%s' "
                  "HardwareSettings in the register, check his name !",
                  __PRETTY_FUNCTION__, __LINE__,
                  hwSettingsName.c_str());
    }
    return m_instance;
  }

  //----------------------------------------------------------------------------
  // static
  ControlPanel& ControlPanel::singleton (const String& hwSettingsName) {
    // throw exception if hwSettingsName not found !
    return * getInstance (hwSettingsName);
  }

  //----------------------------------------------------------------------------
  // static
  ControlPanel& ControlPanel::singleton () {
    return * getInstance ();
  }

  //----------------------------------------------------------------------------
  // protected
  ControlPanel::ControlPanel (const HardwareSettings & hwsettings) :
    FrameDecoder (hwsettings.bus(), hwsettings.leds()),
    m_btnsettings (hwsettings.buttons()), m_isopened (false) {

  }

  //----------------------------------------------------------------------------
  // protected
  ControlPanel::ControlPanel () :
    FrameDecoder (), m_isopened (false) {

  }

  //----------------------------------------------------------------------------
  void ControlPanel::end() {

    if (m_isopened) {
      
      for (auto & elmt : m_button) {
        Button & button = elmt.second;

        if (button.isOpened()) {

          button.ctrl().end();
        }
      }
      FrameDecoder::end();
      m_isopened = false;
    }
  }

  //----------------------------------------------------------------------------
  ControlPanel::~ControlPanel() {

    end();
    delete m_instance;
    m_instance = nullptr;
  }

  //----------------------------------------------------------------------------
  // protected
  bool ControlPanel::makeButtons() {
    bool b_isopened = true;

    // We create the buttons from their settings...
    for (const auto & elmt : m_btnsettings) {
      int key = elmt.first;
      const ButtonSettings & settings = elmt.second;
      Button button (settings);

      m_button.emplace (key, button);
      if (button.isOpened() == false) {

        button.begin();
        b_isopened = b_isopened && button.isOpened();
      }
    }
    return b_isopened;
  }

  //----------------------------------------------------------------------------
  void ControlPanel::begin (const HardwareSettings & hwsettings,
                            unsigned long waitingTimeMs) {

    if (m_isopened == false)  {

      SPAIOT_DBG ("ControlPanel::begin(): opening");

      m_btnsettings = hwsettings.buttons();
      FrameDecoder::begin (hwsettings.bus(), hwsettings.leds(), waitingTimeMs);
      m_isopened = makeButtons() && FrameDecoder::isOpened();
    }

  }

  //----------------------------------------------------------------------------
  void ControlPanel::begin (const String & hwSettingsName,
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

    if (m_isopened == false)  {

      SPAIOT_DBG ("ControlPanel::begin(): opening");

      FrameDecoder::begin (waitingTimeMs);
      m_isopened = makeButtons() && FrameDecoder::isOpened();
    }
  }

  //----------------------------------------------------------------------------
  bool ControlPanel::isOpened() const {

    return m_isopened;
  }

  //----------------------------------------------------------------------------
  bool ControlPanel::hasButton (int key) const {

    return m_button.count (key) == 1;
  }

  //----------------------------------------------------------------------------
  const std::map <int, Button> & ControlPanel::buttons() const {

    return m_button;
  }

  //----------------------------------------------------------------------------
  Button & ControlPanel::button (int key) {

    return m_button.at (key);
  }

  //----------------------------------------------------------------------------
  bool ControlPanel::pushButton (int key)  {

    if (hasButton (key)) {

      m_button[key].push();
      return true;
    }
    return false;
  }

  //----------------------------------------------------------------------------
  // protected
  // may be used for Power, Filter, Bubble and Jet
  uint8_t ControlPanel::setKeyOn (int key, bool v) {

    if (isOpened()) {
      uint8_t b = isLedOn (key);

      if (hasButton (key) && b != UnsetValue8) {

        if (v ^ b) {

          m_button[key].push();
        }
        return isLedOn (key);
      }
    }
    return UnsetValue8;
  }

  //----------------------------------------------------------------------------
  uint8_t ControlPanel::setPower (bool v) {

    return setKeyOn (Power, v);
  }

  //----------------------------------------------------------------------------
  uint8_t ControlPanel::setFilter (bool v) {

    return setKeyOn (Filter, v);
  }

  //----------------------------------------------------------------------------
  uint8_t ControlPanel::setHeater (bool v) {

    if (isOpened() && hasButton (Heater)) {
      uint8_t b = isHeaterOn ();

      if (b != UnsetValue8) {

        if (v ^ b) {
          int count = 0;

          m_button[Heater].push();
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

    return setKeyOn (Bubble, v);
  }

  //----------------------------------------------------------------------------
  uint8_t ControlPanel::setJet (bool v) {

    return setKeyOn (Jet, v);
  }

  //----------------------------------------------------------------------------
  bool ControlPanel::setSanitizerTime (uint16_t time) {

    if (hasButton (Sanitizer)) {

      if (isOpened()) {

        if ( (m_rawStatus != UnsetValue16) &&
             ( (m_rawStatus & m_frameLedPower) != 0) && (m_errorValue == 0)) {
          uint16_t pushCounter = 0;

          if (time == 0) {

            if ( (m_rawStatus != UnsetValue16) && (m_rawStatus & m_frameLedSanitizer)) {

              do {

                m_button[Sanitizer].push();
                delay (ButtonIntervalMs);

                pushCounter ++;

                // push button till 8H
              }
              while ( (DIGITS2UINT (m_sanitizerTime) != 8) &&
                      (pushCounter < PUSH_COUNTER_MAX));
              // then push a last time to cancel
              m_button[Sanitizer].push();

            } // else already off

          }
          else if ( (time == 3) || (time == 5) || (time == 8)) {

            do {

              m_button[Sanitizer].push();
              delay (ButtonIntervalMs);

              pushCounter ++;

            }
            while ( (pushCounter < PUSH_COUNTER_MAX) &&
                    (DIGITS2UINT (m_sanitizerTime) != time));

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
         hasButton (TempUp) && hasButton (TempDown) && isOpened()) {

      if ( (waitForDesiredTemp() != UnsetValue16) && (m_errorValue == 0)) {
        uint16_t  uint16DesiredTemp = desiredTemp();
        int  pushCounter = temp - uint16DesiredTemp;

        SPAIOT_DBG ("%s:%d: desiredTemp: %d'C -> %d'C > %+d'C",
                    __PRETTY_FUNCTION__, __LINE__,
                    desiredTemp(), temp, pushCounter);

        if (pushCounter != 0) {

          // faster version
          while (pushCounter != 0) {

            if (pushCounter > 0) {

              m_button[TempUp].push();
              pushCounter--;
            }
            else {

              m_button[TempDown].push();
              pushCounter++;
            }
            delay (HoldPressedMs);
          }

          // Adjust if necessery
          pushCounter = 0;
          while ( (pushCounter < PUSH_COUNTER_MAX) &&
                  ( (uint16DesiredTemp = desiredTemp()) != temp)) {

            if (uint16DesiredTemp > temp) {

              m_button[TempDown].push();
            }
            else {

              m_button[TempUp].push();
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
                  temp, isOpened(), DesiredTempMin, DesiredTempMax);
      return false;
    }
    return true;
  }

  //----------------------------------------------------------------------------
  uint16_t ControlPanel::waitForDesiredTemp (unsigned long  MaxWaitingTimeMs) {

    if (isOpened() && (desiredTemp() == UnsetValue16)) {
      const unsigned long looptime = 100;

      m_button[TempUp].push();
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

    return m_btnsettings;
  }

}
//------------------------------------------------------------------------------
