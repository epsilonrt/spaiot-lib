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

#include "global.h"

namespace SpaIot {

  /**
     @brief Spa Events

     This enum lists the events that can be sent or receive by the spa
  */
  class Event {
    public:
      /**
         @enum Type
         @brief Type of event
      */
      enum Type {
        PowerOn = Key::Power,             //!< Power On, boolean, true when the spa is powered on, false when the spa is powered off
        FilterOn = Key::Filter,           //!< Filter On, boolean, true when the filter is on, false when the filter is off
        BubbleOn = Key::Bubble,           //!< Bubble On, boolean, true when the bubble is on, false when the bubble is off
        JetOn = Key::Jet,                 //!< Jet On, boolean, true when the jet is on, false when the jet is off
        SanitizerOn = Key::Sanitizer,     //!< Sanitizer On, boolean, true when the sanitizer is on, false when the sanitizer is off
        HeaterOn = Key::Heater,           //!< Heater On, boolean, true when the heater is on, false when the heater is off
        HeatReached = Key::HeatReached,   //!< Heat Reached, boolean, true when the water temperature is reached (led green), false when the water temperature is not reached (led red or Heater Off)
        WaterTemp = Key::TempUp + 1,      //!< Water temperature in °C
        DesiredTemp,                      //!< Desired water temperature in °C
        SetDesiredTemp,                   //!< Set desired water temperature in °C
        SanitizerTime,                    //!< Remaining sanitation time in hours
        ErrorCode,                        //!< Error code displayed by the control panel, 0 if no error
        AmbientTemperature,               //!< Ambient temperature in °C (if air temperature sensor is available)
        NoEvent = -1,                     //!< No event
        AnyEvent = -2                     //!< Any event
      };

      /**
         @brief Construct a new Event object

         @param type  The event type
         @param value  The event value
      */
      Event (Type type = Type::NoEvent, uint16_t value = 0);

      /**
         @brief Destructor
      */
      virtual ~Event();

      /**
        @brief Copy Constructor
      */
      Event (const Event &other);

      /**
         @brief Move Constructor
         after the move, @b other is null, other.clear() **must be called** if it is to be used again
      */
      Event (Event &&other);

      /**
         @brief Sets the Event object to be equal to @b other.
      */
      Event &operator= (const Event &other);

      /**
         @brief Move assignment
          after the move, @b other is null, other.clear() **must be called** if it is to be used again
      */
      Event &operator= (Event &&other);

      /**
         @brief Clears the Event object
         After the call to this function, the Event object is the same as one that has been default constructed
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
      bool operator== (const Event &other) const;

      /**
         @brief Checks if the two objects are not equal
      */
      bool operator!= (const Event &other) const;

      /**
       * @brief  Conversion to bool
       * 
       * Convert the Event object value to a boolean value.
       * @return true if the event value is true and type is not NoEvent, false otherwise
       */
      // explicit operator bool() const;
      operator bool() const;

      /**
       * @brief Conversion to uint16_t
       * 
       * @return the event value or UnsetValue16 if type is NoEvent, false otherwise
       */
      // explicit operator bool() const;
      operator uint16_t() const;

      /**
         @brief Set the Data object

         @param value
      */
      void setValue (uint16_t value);

      /**
         @brief Set the Type object

         @param type  The event type
      */
      void setType (Type type);

      /**
         @brief Get the Type object

         @return The event type
      */
      Type type() const;

      /**
         @brief Get the Value object

         @return The event value
      */
      uint16_t value() const;

      /**
         @brief Convert the event to a string

         @return The event as a string
      */
      String toString() const;

      /**
         @brief Convert the event type to a string

         @param type  The event type
         @return The event type as a string
      */
      static String typeToString (Type type);

    protected:
      class Private;
      Event (Private &dd);
      std::unique_ptr<Private> d_ptr;
    private:
      PIMPL_DECLARE_PRIVATE (Event)
  };
}