/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

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
     @brief This class represents a button.

      It is a concrete class, and can be instantiated.
      This class is copyable and movable.
  */
  class Button {

    public:
      /**
         @brief The default constructor
      */
      Button ();

      /**
        @brief Destructor
      */
      virtual ~Button();

      /**
         @brief The constructor for the Button class with settings
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
      bool operator== (const Button &other) const;

      /**
         @brief Checks if the two objects are not equal
      */
      bool operator!= (const Button &other) const;

      /**
         @brief Returns the button identification key in the possible values of SpaIot::Key
      */
      int id() const;

      /**
         @brief Returns the button controller used by the button
      */
      const ButtonController &ctrl() const;

      /**
         @brief Returns the button controller used by the button
      */
      ButtonController &ctrl();

      /**
         @brief Returns the settings of the button
      */
      const ButtonSettings &settings() const;

      /**
         @brief Initializes the button, if its controller is not initialized, it is initialized.
      */
      void begin();

      /**
         @brief Presses the button, it is not released automatically
      */
      void press();

      /**
         @brief Releases the button
      */
      void release();

      /**
         @brief Presses and releases the button, \c HoldPressedMs milliseconds is the time the button is pressed
      */
      void push();

      /**
         @brief Checks if the button is opened
      */
      bool isOpen() const;

      /**
         @brief Checks if the button is pressed
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
