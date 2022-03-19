/*
 * SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
 * This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>
 *
 * SpaIot library is licensed under a
 * Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
 *
 * You should have received a copy of the license along with this
 * work. If not, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.
 *
 * SpaIot library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY;
 */
#pragma once

#include <vector>
#include <string>
#include <initializer_list>
#include <type_traits>
#include <Arduino.h>
#include "spaiotdebug.h"

#include "buttoncontroller.h"

namespace SpaIot {

  /**
   * @class Multiplexer
   * @brief Analog multiplexer template
   */
  template <int W>
  class Multiplexer : public ButtonController {
    public:

      /**
       * The constructor of Multiplexer takes a list of integers and an integer as input.
       * The list of integers represents the selection pins that are connected to the multiplexer. The
       * integer represents the pin that is connected to the inhibit pin
       *
       * @param spins a list of integers that represent the pins that are connected to
       * the multiplexer, in ascending order of weight
       * @param inhPin The pin number of the inhibit pin ((active in high level)
       */
      Multiplexer (const std::initializer_list<int>& spins, int inhPin) :
        ButtonController(), m_spin (spins), m_inh (inhPin) {

        SPAIOT_ASSERT ( (1 << spins.size()) >= W, "The size of spins:%d does not allow you to select the number of channels:%d", spins.size(), W);
      }

      Multiplexer (const std::string & name, const std::initializer_list<int>& spins, int inhPin) :
        ButtonController (name), m_spin (spins), m_inh (inhPin) {

        SPAIOT_ASSERT ( (1 << spins.size()) >= W, "The size of spins:%d does not allow you to select the number of channels:%d", spins.size(), W);
      }

      /**
       * Default constructor
       * The Multiplexer class is a subclass of ButtonController
       */
      Multiplexer () :
        ButtonController(), m_inh (-1) {
      }

      /**
       * Return the number of multiplexer channels.
       *
       * @return the number of multiplexer channels
       */
      int size() const {

        return W;
      }

      /**
       * It opens the multiplexer.
       *
       * Selection pins are low logic state, the inihibition pin in the high logic state.
       */
      virtual void begin() {

        if ( (isOpened() == false) && (isNull() == false))  {
          SPAIOT_DBG ("Multiplexer::begin(): opening");

          pinMode (m_inh, OUTPUT);
          digitalWrite (m_inh, HIGH);

          for (unsigned int i = 0; i < m_spin.size(); i++) {

            pinMode (m_spin.at (i), OUTPUT);
            digitalWrite (m_spin.at (i), LOW);
          }
          m_isopened = true;
        }
      }

      /**
       * This function is called when the program ends.
       *
       * It sets the pins to be inputs with pullups
       */
      virtual void end() {

        for (unsigned int i = 0; i < m_spin.size(); i++) {

          pinMode (m_spin.at (i), INPUT_PULLUP);
        }
        ButtonController::end();
      }

      /**
       * If the button is pressed, the corresponding pin is set to high, and the other
       * pins are set to low
       *
       * @param button The button number to be selected.
       * @return ButtonController::selected()
       */
      virtual int select (int button) {

        if (isOpened() && (button >= 0) && (button <= W)) {

          for (unsigned int i = 0; i < m_spin.size(); i++) {

            digitalWrite (m_spin.at (i), (button & (1 << i)) ? HIGH : LOW);

          }
          digitalWrite (m_inh, LOW);
          m_selected = button;
        }
        return selected();
      }

      /**
       * It deselects the current selected item.
       */
      virtual void deselect () {

        if (isOpened()) {

          digitalWrite (m_inh, HIGH);
          m_selected = -1;
        }
      }

      /**
       * This function returns true if default construting
       *
       * @return A boolean value.
       */
      virtual bool isNull() const {

        return m_spin.size() == 0;
      }

      /**
       * The Multiplexer class has an operator== function that takes one argument,
       * a Multiplexer object.
       *
       * @param other the other object being compared to this one.
       * @return The result of the comparison of the two objects.
       */
      virtual bool operator== (const ButtonController &other) const {

        if (ButtonController::operator== (other)) {

          const Multiplexer<W>& o = static_cast<const Multiplexer<W>&> (other);
          return ( (m_spin == o.m_spin) && (m_inh == o.m_inh));
        }
        return false;
      }

      /**
       * Given a key, return the corresponding value
       *
       * @param key The key of the parameter.
       * @return The pin number for the given key.
       */
      int selectPin (int key) const {

        return m_spin.at (key);
      }

      /**
       * It sets the pin number for the select pin of the given key.
       *
       * @param key the key to be used for the pin
       * @param pin the pin number
       */
      void setSelectPin (int key, int pin) {

        m_spin[key] = pin;
      }

    protected:
      std::vector<int> m_spin;
      int m_inh;
  };

}
