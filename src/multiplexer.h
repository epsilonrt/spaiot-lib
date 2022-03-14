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
       * @brief
       * @param spins The list of selection pins, in ascending order of weight
       * @param inhPin Inhibition pin (active in high level)
       */
      Multiplexer (const std::initializer_list<int>& spins, int inhPin) :
        ButtonController(), m_spin (spins), m_inh (inhPin) {
          
        SPAIOT_ASSERT ((1 << spins.size()) >= W, "The size of spins:%d does not allow you to select the number of channels:%d", spins.size(), W);
      }

      Multiplexer () :
        ButtonController(), m_inh (-1) {
      }

      /**
       * @brief
       * @return
       */
      int size() const {

        return W;
      }

      /**
       * @brief
       */
      virtual void begin() {

        if ( (isOpened() == false) && (isNull() == false))  {
          SPAIOT_DBG ("Multiplexer::begin(): opening");

          pinMode (m_inh, OUTPUT);
          digitalWrite (m_inh, HIGH);

          for (int i = 0; i < m_spin.size(); i++) {

            pinMode (m_spin.at (i), OUTPUT);
            digitalWrite (m_spin.at (i), LOW);
          }
          m_isopened = true;
        }
      }

      /**
       * @brief
       * @param button
       * @return
       */
      virtual int select (int button) {

        if (isOpened() && (button >= 0) && (button <= W)) {

          for (int i = 0; i < m_spin.size(); i++) {

            digitalWrite (m_spin.at (i), (button & (1 << i)) ? HIGH : LOW);

          }
          digitalWrite (m_inh, LOW);
          m_selected = button;
        }
        return selected();
      }

      /**
       * @brief
       */
      virtual void deselect () {

        if (isOpened()) {

          digitalWrite (m_inh, HIGH);
          m_selected = -1;
        }
      }

      /**
       * @brief
       * @return
       */
      virtual bool isNull() const {

        return m_spin.size() == 0;
      }

      /**
       * @brief
       * @param other
       * @return
       */
      virtual bool operator== (const ButtonController &other) const {

        if (ButtonController::operator== (other)) {

          const Multiplexer<W>& o = static_cast<const Multiplexer<W>&> (other);
          return ( (m_spin == o.m_spin) && (m_inh == o.m_inh));
        }
        return false;
      }

      /**
       * @brief
       * @param key
       * @return
       */
      int selectPin (int key) const {

        return m_spin.at (key);
      }

      /**
       * @brief
       * @param key
       * @param pin
       */
      void setSelectPin (int key, int pin) {

        m_spin[key] = pin;
      }

    protected:
      std::vector<int> m_spin;
      int m_inh;
  };

}
