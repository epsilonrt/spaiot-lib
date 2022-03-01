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
      /**
       * @brief 
       */
      BusSettings();
      /**
       * @brief 
       * @param pins
       */
      BusSettings (const std::initializer_list<int>& pins);
      /**
       * @brief 
       * @param dataPin
       * @param clkPin
       * @param holdPin
       */
      BusSettings (const int dataPin, const int clkPin, const int holdPin);
      /**
       * @brief 
       * @return 
       */
      int dataPin() const;
      /**
       * @brief 
       * @return 
       */
      int clkPin() const;
      /**
       * @brief 
       * @return 
       */
      int holdPin() const;
      /**
       * @brief 
       * @return 
       */
      bool isNull() const;
      /**
       * @brief 
       * @param other
       */
      bool operator== (const BusSettings &other) const;
      /**
       * @brief 
       * @param other
       */
      bool operator!= (const BusSettings &other) const;
      /**
       * @brief 
       * @param pin
       */
      void setDataPin (int pin);
      /**
       * @brief 
       * @param pin
       */
      void setClkPin (int pin);
      /**
       * @brief 
       * @param pin
       */
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
      /**
       * @brief 
       */
      LedSettings ();
      /**
       * @brief 
       * @param order
       */
      LedSettings (const uint8_t order);
      /**
       * @brief 
       * @return 
       */
      uint8_t order() const;
      /**
       * @brief 
       * @return 
       */
      uint16_t frame() const;
      /**
       * @brief 
       * @param order
       */
      void setOrder (const uint8_t order);
      /**
       * @brief 
       * @return 
       */
      bool isNull() const;
      /**
       * @brief 
       * @param other
       */
      bool operator== (const LedSettings &other) const;
      /**
       * @brief 
       * @param other
       */
      bool operator!= (const LedSettings &other) const;

    private:
      uint8_t m_order;
  };

  /**
   * @brief 
   */
  extern const BusSettings ScipBus;
  /**
   * @brief 
   */
  extern const std::map<int, LedSettings> SspLeds;
  /**
   * @brief 
   */
  extern const std::map<int, LedSettings> SjbLeds;
}
