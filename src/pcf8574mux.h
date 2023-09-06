/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#include <Wire.h>
#include "buttoncontroller.h"

namespace SpaIot {

  /**
     @class Pcf8574Mux
     @brief Multiplexer controlled by a PCF8574

      This class implements a multiplexer for buttons that is controlled by a PCF8574. 
      It is a concrete class, and can be instantiated.
      This class is copyable and movable.
  */
  class Pcf8574Mux : public ButtonController {
    public:

      /**
         It creates a new Pcf8574Mux object.

         @param slaveAddress The address of the PCF8574 chip.
         @param bus The TwoWire object that is used to communicate with the PCF8574.
         @param idleValue The value that the pins will be set to when the device is not
         in use.
      */
      Pcf8574Mux (uint8_t slaveAddress = 0x20, TwoWire &bus = Wire, uint8_t idleValue = 0xFF);

      /**
         It creates a new Pcf8574Mux object.

         @param name controller name
         @param slaveAddress The address of the PCF8574 chip.
         @param bus The TwoWire object that is used to communicate with the PCF8574.
         @param idleValue The value that the pins will be set to when the device is not
         in use.
      */
      Pcf8574Mux (const String &name, uint8_t slaveAddress = 0x20, TwoWire &bus = Wire, uint8_t idleValue = 0xFF);

      /**
        @copydoc ButtonController::ButtonController (const ButtonController &other)
      */
      Pcf8574Mux (const Pcf8574Mux &other);

      /**
         @copydoc ButtonController::operator= (const ButtonController &other)
      */
      Pcf8574Mux &operator= (const Pcf8574Mux &other);

      /**
         @copydoc ButtonController::ButtonController (ButtonController &&other)
      */
      Pcf8574Mux (Pcf8574Mux &&other);

      /**
         @copydoc ButtonController::operator= (const ButtonController &&other)
      */
      Pcf8574Mux &operator= (Pcf8574Mux &&other);

      /**
         @brief Reading of the slave m_address of the integrated circuit
         @return 7-bit I²C slave m_address, right-aligned
      */
      uint8_t address() const;

      // re-implemented methods from ButtonController
      virtual void clear();
      virtual bool isEmpty() const;
      virtual bool operator== (const ButtonController &other) const;
      virtual void begin();
      virtual void end();
      virtual int select (int button);
      virtual void deselect ();

    protected:
      class Private;
      Pcf8574Mux (Private &dd);
    private:
      PIMPL_DECLARE_PRIVATE (Pcf8574Mux)
  };

}
