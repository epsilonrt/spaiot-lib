/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SpaIot library is licensed under a
   Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.

   You should have received a copy of the license along with this
   work. If not, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.

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
         This function returns true if the value of the member variable m_order is equal
         to the value UnsetValue8 or if the object was moved from.

         @return The value of the member variable m_order.
      */
      bool isNull() const;

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

  /**
     @brief Leds settings for the Ssp SPA
  */
  extern const std::map<int, LedSettings> SspLeds;

  /**
     @brief Leds settings for the Sjb SPA
  */
  extern const std::map<int, LedSettings> SjbLeds;
}
