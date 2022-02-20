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

#include <map>
#include <vector>
#include <initializer_list>
#include "global.h"

namespace SpaIot {

  /**
   * @class BusSettings
   * @brief
   */
  class BusSettings {
    public:
      BusSettings();
      BusSettings (const std::initializer_list<int>& pins);
      BusSettings (const int dataPin, const int clkPin, const int holdPin);
      int dataPin() const;
      int clkPin() const;
      int holdPin() const;
      bool isNull() const;
      bool operator== (const BusSettings &other) const;
      bool operator!= (const BusSettings &other) const;
      void setDataPin (int pin);
      void setClkPin (int pin);
      void setHoldPin (int pin);
    private:
      std::vector<int> m_pin;
  };

  /**
   * @class LedSettings
   * @brief
   */
  class LedSettings {

    public:
      LedSettings ();
      LedSettings (const uint8_t order);
      uint8_t order() const;
      uint16_t frame() const;
      void setOrder (const uint8_t order);
      bool isNull() const;
      bool operator== (const LedSettings &other) const;
      bool operator!= (const LedSettings &other) const;

    private:
      uint8_t m_order;
  };

  extern const BusSettings DiyScipBus;
  extern const std::map<int, LedSettings> SspLeds;
  extern const std::map<int, LedSettings> SjbLeds;
}
