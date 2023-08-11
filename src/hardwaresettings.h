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

#include <Arduino.h>
#include "bussettings.h"
#include "ledsettings.h"
#include "buttonsettings.h"

namespace SpaIot {

  /**
     @class HardwareSettings
     @brief
  */
  class HardwareSettings {
    public:
      /**
         @brief
      */
      HardwareSettings();

      /**
         @brief Destructor
      */
      virtual ~HardwareSettings();

      /**
         @brief
         @param bus
         @param leds
         @param buttons
      */
      HardwareSettings (const BusSettings &bus,
                        const std::map <int, LedSettings> &leds,
                        const std::map <int, ButtonSettings> &buttons);

      /**
        @brief Copy Constructor
      */
      HardwareSettings (const HardwareSettings &other);

      /**
         @brief Move Constructor
         after the move, @b other is null, other.clear() **must be called** if it is to be used again
      */
      HardwareSettings (HardwareSettings &&other);

      /**
         @brief Sets the HardwareSettings object to be equal to @b other.
      */
      HardwareSettings &operator= (const HardwareSettings &other);

      /**
         @brief Move assignment
          after the move, @b other is null, other.clear() **must be called** if it is to be used again
      */
      HardwareSettings &operator= (HardwareSettings &&other);

      /**
         @brief Clears the HardwareSettings object
         After the call to this function, the HardwareSettings object is the same as one that has been default constructed
      */
      void clear();

      /**
        @brief
        @return
      */
      bool isNull() const;

      /**
         @brief
         @param other
      */
      bool operator== (const HardwareSettings &other) const;

      /**
         @brief
         @param other
      */
      bool operator!= (const HardwareSettings &other) const;

      /**
         @brief
         @return
      */
      const BusSettings   &bus() const;

      /**
         @brief
         @return
      */
      const std::map <int, LedSettings> &leds() const;

      /**
         @brief
         @return
      */
      const std::map <int, ButtonSettings> &buttons() const;

      /**
         @brief
         @param out
      */
      void print (Print &out) const;

      /**
         @brief
         @param name
         @param settings
         @return
      */
      static bool addToRegister (const String &name, const HardwareSettings &settings);

      /**
         @brief
         @param name
         @return
      */
      static const HardwareSettings &getFromRegister (const String &name);

      /**
         @brief
         @param name
         @return
      */
      static bool registerContains (const String &name);

    protected:
      class Private;
      HardwareSettings (Private &dd);
      std::unique_ptr<Private> d_ptr;
    private:
      PIMPL_DECLARE_PRIVATE (HardwareSettings)
  };

  /**
     @brief
  */
  extern const HardwareSettings Scip2Ssp;
  /**
     @brief
  */
  extern const HardwareSettings Scip2Sjb;
}
