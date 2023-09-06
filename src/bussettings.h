/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#include <initializer_list>
#include "global.h"

namespace SpaIot  {

  /**
     @class BusSettings
     @brief SPI 2840X Bus settings

      This class represents the settings of a SPI 2840X Bus.
      It is a concrete class, and can be instantiated.
      This class is copyable and movable.
  */
  class BusSettings {
    public:
      /**
         The default constructor
         creates a BusSettings object and initializes the pin vector to
         {-1, -1, -1}
      */
      BusSettings();

      /**
         @brief Destructor
      */
      virtual ~BusSettings();

      /**
         The constructor for the BusSettings class takes three parameters, and stores
         them in the private member variables

         @param dataPin The pin number of the data pin of the MCP23017 chip.
         @param clkPin The pin number of the clock pin.
         @param holdPin The pin that is used to hold the data line high.
      */
      BusSettings (const int dataPin, const int clkPin, const int holdPin);

      /**
         The constructor takes a list of pins
         
         The list of pins must be exactly 3 pins, and the pins must be in the order { dataPin, clkPin, holdPin }
         If the list is not exactly 3 pins, the comportment is undefined.
         @param pins A list of 3 pins to use for the bus.
      */
      BusSettings (const std::initializer_list<int> &pins);

      /**
        @brief Copy Constructor
      */
      BusSettings (const BusSettings &other);

      /**
         @brief Move Constructor
         after the move, @b other is null, other.clear() **must be called** if it is to be used again
      */
      BusSettings (BusSettings &&other);

      /**
         @brief Sets the BusSettings object to be equal to @b other.
      */
      BusSettings &operator= (const BusSettings &other);

      /**
         @brief Move assignment
          after the move, @b other is null, other.clear() **must be called** if it is to be used again
      */
      BusSettings &operator= (BusSettings &&other);

      /**
       * @brief Clears the BusSettings object
       * After the call to this function, the BusSettings object is the same as one that has been default constructed 
       */
      void clear();

      /**
         @brief Returns true if the object is null (i.e. it has not been initialized, d_ptr is null)
      */
      bool isNull() const;

      /**
         @brief Checks if all attributes are set to their default values (same as the default constructor)
      */
      bool isEmpty() const;

      /**
         The function returns true if the two objects are equal

         @param other the other BusSettings object to compare to

         @return The return value is a boolean value.  It is true if the two objects are
         equal, and false if they are not equal.
      */
      bool operator== (const BusSettings &other) const;

      /**
         The function returns true if the two objects are not equal

         @param other the other BusSettings object to compare to

         @return The return value is a boolean value.  It is true if the two objects are
         not equal, and false if they are equal.
      */
      bool operator!= (const BusSettings &other) const;

      /**
         This function returns the data pin number

         @return The pin number of the GPIO pin that the bus is connected to.
      */
      int dataPin() const;

      /**
         This function returns the clock pin number

         @return The pin number of the GPIO pin that the bus is connected to.
      */
      int clkPin() const;

      /**
         This function returns the pin number of the hold pin

         @return The pin number of the GPIO pin that the bus is connected to.
      */
      int holdPin() const;

      /**
         Set the data pin to the given value.

         @param pin The pin number of the GPIO pin that the bus is connected to.
      */
      void setDataPin (int pin);

      /**
         Set the clock pin to the given value.

         @param pin The pin number of the GPIO pin that the bus is connected to.
      */
      void setClkPin (int pin);

      /**
         Set the pin number for the hold pin.

         @param pin The pin number of the GPIO pin that the bus is connected to.
      */
      void setHoldPin (int pin);

    protected:
      class Private;
      BusSettings (Private &dd);
      std::unique_ptr<Private> d_ptr;
    private:
      PIMPL_DECLARE_PRIVATE (BusSettings)
  };

  /**
     @brief BusSettings for the ScipBus
  */
  extern const BusSettings ScipBus;
}
