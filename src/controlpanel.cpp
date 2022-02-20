/**
 * SpaIot Library (c) by espilonrt - epsilonrt@gmail.com
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
#include "debug.h"

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
  ControlPanel * ControlPanel::getInstance (const std::string & hwSettingsName) {

    if (HardwareSettings::registerContains(hwSettingsName)) {

      m_instance = new ControlPanel (HardwareSettings::getFromRegister(hwSettingsName));
    }
    return m_instance;
  }

  //----------------------------------------------------------------------------
  // protected
  ControlPanel::ControlPanel (const HardwareSettings & hwsettings) :
    FrameDecoder (hwsettings.bus(), hwsettings.leds()),
    m_buttonSettings (hwsettings.buttons()), m_isopened (false) {

    // We create the buttons from their settings...
    for (const auto& [ key, settings ] : hwsettings.buttons()) {

      m_button.emplace (key, Button (settings));
    }
  }

  //----------------------------------------------------------------------------
  const std::map <int, ButtonSettings> ControlPanel::buttonSettings() const {

    return m_buttonSettings;
  }

  //----------------------------------------------------------------------------
  void ControlPanel::begin() {

    if (m_isopened == false)  {
      bool b_isopened = true;

      DBG ("ControlPanel::begin(): opening");
      for (auto& [ k, b ] : m_button) {

        if (b.isOpened() == false) {

          b.begin();
          b_isopened = b_isopened && b.isOpened();
        }
      }

      FrameDecoder::begin();
      m_isopened = b_isopened && FrameDecoder::isOpened();
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
  bool ControlPanel::pushButton (int key)  {
    
    if (hasButton (key)) {
      
      m_button[key].push();
      return true;
    }
    return false;
  }

  //----------------------------------------------------------------------------
  // protected
  // may be used for Power, Filter, Heater, Bubble and Jet
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

    setKeyOn (Heater, v);
    delay (500);
    return isHeaterOn();
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

      if ( (m_rawStatus != UnsetValue16) &&
           ( (m_rawStatus & m_frameLedPower) != 0) && (m_errorValue == 0)) {
        uint16_t  uint16DesiredTemp, pushCounter = 0;

        while ( (desiredTemp() == UnsetValue16) &&
                (pushCounter < PUSH_COUNTER_MAX)) {

          m_button[TempUp].push();
          delay (ButtonIntervalMs);
          pushCounter++;
        }

        while ( (pushCounter < PUSH_COUNTER_MAX) &&
                ( (uint16DesiredTemp = desiredTemp()) != temp)) {

          if (uint16DesiredTemp > temp) {

            m_button[TempDown].push();
          }
          else {

            m_button[TempUp].push();
          }
          delay (ButtonIntervalMs);
          pushCounter++;
        }
      }
      return true;
    }
    return false;
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

}
//------------------------------------------------------------------------------
