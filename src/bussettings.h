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
   * @brief SPI 2840X Bus settings
   */
  class BusSettings {
    public:
      /**
       * The constructor for the BusSettings class takes three parameters, and stores
       * them in the private member variables
       *
       * @param dataPin The pin number of the data pin of the MCP23017 chip.
       * @param clkPin The pin number of the clock pin.
       * @param holdPin The pin that is used to hold the data line high.
       */
      BusSettings (const int dataPin, const int clkPin, const int holdPin);
      /**
       * The constructor takes a list of pins and stores them in the member variable
       * m_pin
       *
       * @param pins A list of pins to use for the bus.
       */
      BusSettings (const std::initializer_list<int>& pins);
      /**
       * The default constructor
       * creates a BusSettings object and initializes the m_pin member to
       * (3, -1)
       */
      BusSettings();
      /**
       * This function returns the data pin number
       *
       * @return The pin number of the GPIO pin that the bus is connected to.
       */
      int dataPin() const;
      /**
       * This function returns the clock pin number
       *
       * @return The pin number of the GPIO pin that the bus is connected to.
       */
      int clkPin() const;
      /**
       * This function returns the pin number of the hold pin
       *
       * @return The pin number of the GPIO pin that the bus is connected to.
       */
      int holdPin() const;
      /**
       * This function returns true if any of the pins are set to -1
       *
       * @return The return value is a boolean.
       */
      bool isNull() const;
      /**
       * The function returns true if the two objects are equal
       *
       * @param other the other BusSettings object to compare to
       *
       * @return The return value is a boolean value.  It is true if the two objects are
       * equal, and false if they are not equal.
       */
      bool operator== (const BusSettings &other) const;
      /**
       * The function returns true if the two objects are not equal
       *
       * @param other the other BusSettings object to compare to
       *
       * @return The return value is a boolean value.  It is true if the two objects are
       * not equal, and false if they are equal.
       */
      bool operator!= (const BusSettings &other) const;
      /**
       * Set the data pin to the given value.
       *
       * @param pin The pin number of the GPIO pin that the bus is connected to.
       */
      void setDataPin (int pin);
      /**
       * Set the clock pin to the given value.
       *
       * @param pin The pin number of the GPIO pin that the bus is connected to.
       */
      void setClkPin (int pin);
      /**
       * Set the pin number for the hold pin.
       *
       * @param pin The pin number of the GPIO pin that the bus is connected to.
       */
      void setHoldPin (int pin);
    private:
      std::vector<int> m_pin;
  };

  /**
   * @class LedSettings
   * @brief SPI 2840X Led settings
   */
  class LedSettings {

    public:
      /**
       * The constructor
       * takes a single parameter, which is the initial value of the
       * LedSettings object
       */
      LedSettings ();
      /**
       * The constructor takes an
       * optional parameter, which is the order of the LED. If the order is not
       * specified, it is set to UnsetValue8
       *
       * @param order The order of the LED in the LED frame.
       */
      LedSettings (const uint8_t order);
      /**
       * This function returns the order of the LED in the frame
       *
       * @return The order of the LED.
       */
      uint8_t order() const;
      /**
       * The function returns the frame mask of the LED settings
       *
       * @return The frame mask of the LED.
       */
      uint16_t frame() const;
      /**
       * Set the order of the LEDs.
       *
       * @param order The order of the LED.
       */
      void setOrder (const uint8_t order);
      /**
       * This function returns true if the value of the member variable m_order is equal
       * to the value UnsetValue8
       *
       * @return The value of the member variable m_order.
       */
      bool isNull() const;
      /**
       * The function returns true if the two objects are equal
       *
       * @param other the other LedSettings object to compare to
       *
       * @return The return value is a boolean value.  It is true if the two objects are
       * equal, and false if they are not equal.
       */
      bool operator== (const LedSettings &other) const;
      /**
       * The function returns true if the two objects are not equal
       *
       * @param other the other LedSettings object to compare to
       *
       * @return The return value is a boolean value.  It is true if the two objects are
       * not equal, and false if they are equal.
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
