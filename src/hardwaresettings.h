/*
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
      /**
       * @brief 
       */
      HardwareSettings();
      /**
       * @brief 
       * @param bus
       * @param leds
       * @param buttons
       */
      HardwareSettings (const BusSettings & bus,
                        const std::map <int, LedSettings> & leds,
                        const std::map <int, ButtonSettings> & buttons);
      
      /**
       * @brief 
       * @return 
       */
      bool isNull() const;
      /**
       * @brief 
       * @return 
       */
      const BusSettings  & bus() const;
      /**
       * @brief 
       * @return 
       */
      const std::map <int, LedSettings> & leds() const;
      /**
       * @brief 
       * @return 
       */
      const std::map <int, ButtonSettings> & buttons() const;
      /**
       * @brief 
       * @param other
       */
      bool operator== (const HardwareSettings &other) const;
      /**
       * @brief 
       * @param other
       */
      bool operator!= (const HardwareSettings &other) const;
#ifndef NODEBUG_SPAIOT
      /**
       * @brief 
       * @param out
       */
      void print(Print &out) const;
#endif
      /**
       * @brief 
       * @param name
       * @param settings
       * @return 
       */
      static bool addToRegister (const std::string & name, const HardwareSettings & settings);
      /**
       * @brief 
       * @param name
       * @return 
       */
      static const HardwareSettings & getFromRegister (const std::string & name);
      /**
       * @brief 
       * @param name
       * @return 
       */
      static bool registerContains (const std::string & name);

    private:
      const BusSettings * m_bus;
      const std::map <int, LedSettings> * m_leds;
      const std::map <int, ButtonSettings> * m_buttons;
      static std::map<std::string, const HardwareSettings *> Register;
  };

  /**
   * @brief 
   */
  extern const HardwareSettings Scip2Ssp;
  /**
   * @brief 
   */
  extern const HardwareSettings Scip2Sjb;
}
