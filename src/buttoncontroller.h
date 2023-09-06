/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#include <map>
#include "global.h"

namespace SpaIot {

  /**
     @class ButtonController
     @brief This class is the base class of all button controllers

     It is an abstract class, and cannot be instantiated. It provides a common interface for all button controllers.
     This class is copyable and movable.

     select() and deselect() are pure virtual functions that must be implemented by derived classes
  */
  class ButtonController {
    public:
      /**
        @brief Copy Constructor
      */
      ButtonController (const ButtonController &other);

      /**
        @brief Move Constructor
        after the move, @b other is null, other.clear() **must be called** if it is to be used again
      */
      ButtonController (ButtonController &&other);

      /**
         @brief Assignment operator
      */
      virtual ButtonController &operator= (const ButtonController &other);

      /**
         @brief Move assignment
          after the move, @b other is null, other.clear() **must be called** if it is to be used again
      */
      virtual ButtonController &operator= (ButtonController &&other);

      /**
         @brief Destructor
         By default, this calls the virtual function end()
      */
      virtual ~ButtonController();

      /**
         @brief Sets all attributes to their default values (same as the default constructor)
      */
      virtual void clear();

      /**
         @brief Returns true if the object is null (i.e. it has not been initialized, d_ptr is null)
      */
      bool isNull() const;

      /**
         @brief Checks if all attributes are set to their default values (same as the default constructor)
      */
      virtual bool isEmpty() const;

      /**
         @brief Returns true if the two objects are equal
      */
      virtual bool operator== (const ButtonController &other) const;

      /**
         @brief Returns true if the two objects are not equal
      */
      virtual bool operator!= (const ButtonController &other) const;

      /**
         @brief Configures all ressources needed to use the object
      */
      virtual void begin();

      /**
         @brief Releases all ressources used by the object
      */
      virtual void end();

      /**
         @brief Selects a button

         @param button the button to be selected
         @return the value return by selected()
      */
      virtual int select (int button) = 0;

      /**
         @brief Deselects the button
         After the call to this function, isSelected() must return false, and selected() must return -1
      */
      virtual void deselect () = 0;

      /**
         @brief Returns the button number that is selected, or -1 if no button is selected
      */
      virtual int selected() const;

      /**
         @brief Returns true if begin() has been successfully called
      */
      virtual bool isOpen() const;

      /**
         @brief Returns true if a button is selected
      */
      virtual bool isSelected() const;

      /**
         @brief Returns the name of the ButtonController object if it has been set, otherwise it returns an empty string
      */
      virtual const String &name() const;

      /**
         @brief Adds a ButtonController object to the register
         @param name The name of the ButtonController object that will be used to retrieve it from the register
         @param controller The ButtonController object
      */
      static bool addToRegister (const String &name, ButtonController &controller);

      /**
         @brief Returns a ButtonController object from the register

         The user must call registerContains() before calling this function to check if the register contains a ButtonController object with the given name,
         otherwise an exception will be thrown std::out_of_range (and if exceptions are not enabled, the program will terminate)

         @param name  The name of the ButtonController object that will be used to retrieve it from the register
         @return The ButtonController object
      */
      static ButtonController &getFromRegister (const String &name);

      /**
         @brief Returns true if the register contains a ButtonController object with the given name
         @param name The name of the ButtonController object that will be used to retrieve it from the register
         @return true if the register contains a ButtonController object with the given name
      */
      static bool registerContains (const String &name);

    protected:
      class Private;
      ButtonController (Private &dd);
      std::unique_ptr<Private> d_ptr;
    private:
      PIMPL_DECLARE_PRIVATE (ButtonController)
  };
}
