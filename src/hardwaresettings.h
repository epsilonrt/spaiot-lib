/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

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
     @brief This class represents the hardware settings of a SpaIot device

      This class is copyable and movable.
  */
  class HardwareSettings {
    public:
      /**
         @brief The default constructor
      */
      HardwareSettings();

      /**
         @brief Destructor
      */
      virtual ~HardwareSettings();

      /**
         @brief The constructor for the HardwareSettings class takes three parameters
         @param bus  the bus settings
         @param leds the led settings
         @param buttons the button settings
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
         @brief Returns true if the object is null (i.e. it has not been initialized, d_ptr is null)
      */
      bool isNull() const;

      /**
         @brief Checks if all attributes are set to their default values (same as the default constructor)
      */
      bool isEmpty() const;

      /**
         @brief Checks if the two objects are equal
      */
      bool operator== (const HardwareSettings &other) const;

      /**
         @brief Checks if the two objects are not equal
      */
      bool operator!= (const HardwareSettings &other) const;

      /**
         @brief Returns the bus settings
      */
      const BusSettings   &bus() const;

      /**
         @brief Returns the led settings as a map
      */
      const std::map <int, LedSettings> &leds() const;

      /**
         @brief Returns the button settings as a map
      */
      const std::map <int, ButtonSettings> &buttons() const;

      /**
         @brief Prints the object to the Print interface
         @param out the Print interface, for example Serial
      */
      void print (Print &out) const;

      /**
         @brief Adds a HardwareSettings object to the register
         @param name the registered name designating the HardwareSettings object in the register
         @param settings the HardwareSettings object to be added to the register
         @return true if the object has been added to the register, false if the name is already in the register
      */
      static bool addToRegister (const String &name, const HardwareSettings &settings);

      /**
         @brief Returns the HardwareSettings object from the register
         @param name the registered name designating the HardwareSettings object in the register

         You must check if the register contains the name with registerContains() before calling this function
         @return the HardwareSettings object from the register
      */
      static const HardwareSettings &getFromRegister (const String &name);

      /**
         @brief Returns true if the register contains the name
         @param name the registered name designating the HardwareSettings object in the register
         @return true if the register contains the name
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
