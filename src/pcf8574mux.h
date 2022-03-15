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

#include "buttoncontroller.h"
#include "twowireslave.h"

namespace SpaIot {

  /**
   * @class Pcf8574Mux
   * @brief Multiplexer controlled by a PCF8574
   *
   *  Inheriting from the ButtonController class.
   */
  class Pcf8574Mux : public ButtonController {
    public:

      /**
       * It creates a new Pcf8574Mux object.
       *
       * @param slaveAddress The address of the PCF8574 chip.
       * @param bus The TwoWire object that is used to communicate with the PCF8574.
       * @param idleValue The value that the pins will be set to when the device is not
       * in use.
       */
      Pcf8574Mux (uint8_t slaveAddress = 0x20, TwoWire & bus = Wire, uint8_t idleValue = 0xFF);

      /**
       * It opens the PCF8574 chip.
       */
      virtual void begin();
      
      virtual void end();

      /**
       * It writes the value of the button to the PCF8574 chip.
       *
       * @param button the button to be selected.
       *
       * @return the value return by selected()
       */
      virtual int select (int button);

      /**
       * It deselects the mux.
       */
      virtual void deselect ();

      /**
       * This function returns true if the object is a null pointer
       *
       * @return The return value is a boolean value.  The return value is true if the
       *         device is null, and false if the device is not null.
       */
      virtual bool isNull() const;

      /**
       * This function is called when the ButtonController is compared to another
       * ButtonController
       *
       * @param other The ButtonController to compare against.
       *
       * @return true if equal
       */
      virtual bool operator== (const ButtonController &other) const;

    protected:
      TwoWireSlave m_pcf;
      uint8_t m_idle;
  };

}
