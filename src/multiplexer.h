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

#include <initializer_list>
#include <buttoncontroller.h>

namespace SpaIot {

  /**
     @class Multiplexer
     @brief Analog multiplexer for buttons

      This class implements a multiplexer for buttons. It is a concrete class, and can be instantiated.
      It is a ButtonController, and can be used as such.
      This class is copyable and movable.
  */
  class Multiplexer : public ButtonController {
    public:
      /**
         @brief Constructor

         The constructor of Multiplexer takes a list of integers and an integer as input.
         The list of integers represents the selection pins that are connected to the multiplexer. The
         integer represents the pin that is connected to the inhibit pin

         @param spins a list of integers that represent the pins that are connected to
         the multiplexer, in ascending order of weight
         @param inhPin The pin number of the inhibit pin ((active in high level)
      */
      Multiplexer (const std::initializer_list<int> &spins, int inhPin);

      /**
         @brief Constructor with name

         The constructor of Multiplexer takes a list of integers and an integer as input.
         The list of integers represents the selection pins that are connected to the multiplexer. The
         integer represents the pin that is connected to the inhibit pin

         @param name The name of the multiplexer
         @param spins a list of integers that represent the pins that are connected to
         the multiplexer, in ascending order of weight
         @param inhPin The pin number of the inhibit pin ((active in high level)
      */
      Multiplexer (const String &name, const std::initializer_list<int> &spins, int inhPin);

      /**
         @brief Default constructor

         isEmpty() will return true after this constructor, used for copy or move
      */
      Multiplexer ();

      /**
        @copydoc ButtonController::ButtonController (const ButtonController &other)
      */
      Multiplexer (const Multiplexer &other);

      /**
         @copydoc ButtonController::operator= (const ButtonController &other)
      */
      Multiplexer &operator= (const Multiplexer &other);

      /**
         @copydoc ButtonController::ButtonController (ButtonController &&other)
      */
      Multiplexer (Multiplexer &&other);

      /**
         @copydoc ButtonController::operator= (const ButtonController &&other)
      */
      Multiplexer &operator= (Multiplexer &&other);

      /**
         @brief Given a key, return the corresponding value

         @param key The key of the parameter.
         @return The pin number for the given key.
      */
      int selectPin (int key) const;

      /**
         @brief It sets the pin number for the select pin of the given key.

         @param key the key to be used for the pin
         @param pin the pin number
      */
      void setSelectPin (int key, int pin);

      /**
        @brief Returns the number of multiplexer channels that are available

        That is the number of buttons that can be connected to the multiplexer
      */
      int size() const;

      // re-implemented methods from ButtonController
      virtual void clear();
      virtual bool isEmpty() const;
      virtual bool operator== (const ButtonController &other) const;
      virtual void begin();
      virtual void end();
      virtual int select (int button);
      virtual void deselect ();

    protected:
      class Private;
      Multiplexer (Private &dd);
    private:
      PIMPL_DECLARE_PRIVATE (Multiplexer)
  };

  extern Multiplexer Scip1Ctrl;
  extern Multiplexer Scip2CtrlA;
  extern Multiplexer Scip2CtrlB;
}
