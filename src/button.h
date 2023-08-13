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

#include "buttonsettings.h"
#include "buttoncontroller.h"
#include "multiplexer.h"
#include "pcf8574mux.h"

namespace SpaIot {

  /**
     @class Button
     @brief
  */
  class Button {

    public:
      /**
         @brief
      */
      Button ();

      /**
        @brief Destructor
      */
      virtual ~Button();

      /**
         @brief
      */
      Button (const ButtonSettings &settings);

      /**
        @brief Copy Constructor
      */
      Button (const Button &other);

      /**
         @brief Move Constructor
         after the move, @b other is null, other.clear() **must be called** if it is to be used again
      */
      Button (Button &&other);

      /**
         @brief Sets the Button object to be equal to @b other.
      */
      Button &operator= (const Button &other);

      /**
         @brief Move assignment
          after the move, @b other is null, other.clear() **must be called** if it is to be used again
      */
      Button &operator= (Button &&other);

      /**
         @brief Clears the Button object
         After the call to this function, the Button object is the same as one that has been default constructed
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
      bool operator== (const Button &other) const;
      /**
         @brief
         @param other
      */
      bool operator!= (const Button &other) const;
      /**
         @brief
         @return
      */
      int id() const;
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
      /**
         @brief
         @return
      */
      const ButtonSettings &settings() const;
      /**
         @brief
      */
      void begin();
      /**
         @brief
      */
      void press();
      /**
         @brief
      */
      void release();
      /**
         @brief
      */
      void push();
      /**
         @brief
         @return
      */
      bool isOpened() const;
      /**
         @brief
         @return
      */
      bool isPressed() const;

    protected:
      class Private;
      Button (Private &dd);
      std::unique_ptr<Private> d_ptr;
    private:
      PIMPL_DECLARE_PRIVATE (Button)
  };
}
