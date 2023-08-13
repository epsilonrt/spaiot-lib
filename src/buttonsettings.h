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
#include <map>
#include "buttoncontroller.h"

namespace SpaIot {

  class ButtonController;

  /**
     @class ButtonSettings
     @brief This class represents the settings of a button
      It is a concrete class, and can be instantiated.
      This class is copyable and movable.
  */
  class ButtonSettings {
    public:

      /**
         @brief
      */
      ButtonSettings();
      
      /**
         @brief Destructor
      */
      virtual ~ButtonSettings();

      /**
         @brief
         @param controllerName
         @param buttonId
      */
      ButtonSettings (const String &controllerName, int buttonId);

      /**
         @brief
         @param controller
         @param buttonId
      */
      ButtonSettings (ButtonController &controller, int buttonId);

      /**
        @brief Copy Constructor
      */
      ButtonSettings (const ButtonSettings &other);

      /**
         @brief Move Constructor
         after the move, @b other is null, other.clear() **must be called** if it is to be used again
      */
      ButtonSettings (ButtonSettings &&other);

      /**
         @brief Sets the ButtonSettings object to be equal to @b other.
      */
      ButtonSettings &operator= (const ButtonSettings &other);

      /**
         @brief Move assignment
          after the move, @b other is null, other.clear() **must be called** if it is to be used again
      */
      ButtonSettings &operator= (ButtonSettings &&other);

      /**
       * @brief Clears the ButtonSettings object
       * After the call to this function, the ButtonSettings object is the same as one that has been default constructed 
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
         @brief
         @param other
      */
      bool operator== (const ButtonSettings &other) const;
      
      /**
         @brief
         @param other
      */
      bool operator!= (const ButtonSettings &other) const;

      /**
         @brief
         @return
      */
      const String &controllerName() const;
      /**
         @brief
         @return
      */
      int id() const;
      /**
         @brief
         @param id
      */
      void setId (int id);
      /**
         @brief
         @return
      */
      const ButtonController &ctrl() const;
      /**
         @brief
         @return
      */
      ButtonController &ctrl();

    protected:
      class Private;
      ButtonSettings (Private &dd);
      std::unique_ptr<Private> d_ptr;
    private:
      PIMPL_DECLARE_PRIVATE (ButtonSettings)
  };

  /**
     @brief
  */
  extern const std::map<int, ButtonSettings> Scip2SspButtons;

  /**
     @brief
  */
  extern const std::map<int, ButtonSettings> Scip2SjbButtons;
}
