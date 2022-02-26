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
#pragma once

#include <Arduino.h>
#include "bussettings.h"
#include "buttonsettings.h"

namespace SpaIot {

  /**
   * @class HardwareSettings
   * @brief
   */
  class HardwareSettings {
    public:
      HardwareSettings();
      HardwareSettings (const BusSettings & bus,
                        const std::map <int, LedSettings> & leds,
                        const std::map <int, ButtonSettings> & buttons);
      bool isNull() const;
      const BusSettings  & bus() const;
      const std::map <int, LedSettings> & leds() const;
      const std::map <int, ButtonSettings> & buttons() const;
      bool operator== (const HardwareSettings &other) const;
      bool operator!= (const HardwareSettings &other) const;
#ifndef NODEBUG_SPAIOT
      void print(Print &out) const;
#endif
      static bool addToRegister (const std::string & name, const HardwareSettings & settings);
      static const HardwareSettings & getFromRegister (const std::string & name);
      static bool registerContains (const std::string & name);

    private:
      const BusSettings * m_bus;
      const std::map <int, LedSettings> * m_leds;
      const std::map <int, ButtonSettings> * m_buttons;
      static std::map<std::string, const HardwareSettings *> Register;
  };

  extern const HardwareSettings Scip2Ssp;
  extern const HardwareSettings Scip2Sjb;
}
