/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#include <map>
#include <initializer_list>
#include "global.h"

namespace SpaIot  {

  /**
     @class LedSettings
     @brief SPI 2840X Led settings

      This class represents the settings of a SPI 2840X Led.
      It is a concrete class, and can be instantiated.
      This class is copyable and movable.
  */
  class LedSettings {

    public:
      /**
         The constructor
         takes a single parameter, which is the initial value of the
         LedSettings object
      */
      LedSettings ();

      /**
        @brief Destructor
      */
      virtual ~LedSettings();

      /**
         The constructor takes an
         optional parameter, which is the order of the LED. If the order is not
         specified, it is set to UnsetValue8

         @param order The order of the LED in the LED frame.
      */
      LedSettings (const uint8_t order);

      /**
        @brief Copy Constructor
      */
      LedSettings (const LedSettings &other);

      /**
         @brief Move Constructor
         after the move, @b other is null, other.clear() **must be called** if it is to be used again
      */
      LedSettings (LedSettings &&other);

      /**
         @brief Sets the LedSettings object to be equal to @b other.
      */
      LedSettings &operator= (const LedSettings &other);

      /**
         @brief Move assignment
          after the move, @b other is null, other.clear() **must be called** if it is to be used again
      */
      LedSettings &operator= (LedSettings &&other);

      /**
       * @brief Clears the LedSettings object
       * After the call to this function, the LedSettings object is the same as one that has been default constructed 
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

         @param other the other LedSettings object to compare to

         @return The return value is a boolean value.  It is true if the two objects are
         equal, and false if they are not equal.
      */
      bool operator== (const LedSettings &other) const;

      /**
         The function returns true if the two objects are not equal

         @param other the other LedSettings object to compare to

         @return The return value is a boolean value.  It is true if the two objects are
         not equal, and false if they are equal.
      */
      bool operator!= (const LedSettings &other) const;

      /**
         This function returns the order of the LED in the frame

         @return The order of the LED.
      */
      uint8_t order() const;

      /**
         The function returns the frame mask of the LED settings

         @return The frame mask of the LED.
      */
      uint16_t frame() const;

      /**
         Set the order of the LEDs.

         @param order The order of the LED.
      */
      void setOrder (const uint8_t order);

    protected:
      class Private;
      LedSettings (Private &dd);
      std::unique_ptr<Private> d_ptr;
    private:
      PIMPL_DECLARE_PRIVATE (LedSettings)
  };

  /** @addtogroup HardwareSettings
     @{
     @name Leds
     @{
  */

  /**
     @brief Leds settings for the Ssp SPA

     See below for the description of the settings:

     @code
      |    Button   | Bit |
      |:-----------:|:---:|
      |    Power    |  A0 |
      |    Heater   |  A7 |
      | HeatReached |  B1 |
      |    Bubble   |  B2 |
      |    Filter   |  B4 |
     @endcode 
  */
  extern const std::map<int, LedSettings> SspLeds;

  /**
     @brief Leds settings for the Sjb SPA

     See below for the description of the settings:

     @code
      |    Button   | Bit |
      |:-----------:|:---:|
      |    Power    |  A0 |
      |    Bubble   |  A1 |
      |    Heater   |  A7 |
      | HeatReached |  B1 |
      |     Jet     |  B2 |
      |    Filter   |  B4 |
      |  Sanitizer  |  B5 |
     @endcode 
  */
  extern const std::map<int, LedSettings> SjbLeds;

  /** @} */
  /** @} */
}
