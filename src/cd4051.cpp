/*
 * SpaIot Library (c) by espilonrt - epsilonrt@gmail.com
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
#include <Arduino.h>
#include <cd4051.h>
#include "debug.h"

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                             Cd4051
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  Cd4051::Cd4051 () : Cd4051 (-1, -1, -1, -1)
  {}

  //----------------------------------------------------------------------------
  Cd4051::Cd4051 (int s0Pin, int s1Pin,
                  int s2Pin, int enPin) :
    ButtonController(), m_pin{s0Pin, s1Pin, s2Pin, enPin}
  {}

  //----------------------------------------------------------------------------
  Cd4051::Cd4051 (const std::initializer_list<int>& pins) :
    ButtonController(), m_pin (pins)
  {}

  //----------------------------------------------------------------------------
  int Cd4051::pin (int key) const {

    return m_pin.at (key);
  }

  //----------------------------------------------------------------------------
  void Cd4051::setPin (int key, int pin) {

    m_pin[key] = pin;
  }

  //----------------------------------------------------------------------------
  void Cd4051::begin() {

    if (isOpened() == false)  {
      DBG("Cd4051::begin(): opening");

      pinMode (m_pin.at (En), OUTPUT);
      digitalWrite (m_pin.at (En), HIGH);

      for (int i = 0; i < 3; i++) {

        pinMode (m_pin.at (i), OUTPUT);
        digitalWrite (m_pin.at (i), LOW);
      }
      m_isopened = true;
    }
  }

  //----------------------------------------------------------------------------
  int Cd4051::select (int button) {
    
    if (isOpened() && (button >= 0) && (button <= 7)) {

      digitalWrite (m_pin.at (S0), (button & 0x01) ? HIGH : LOW);
      digitalWrite (m_pin.at (S1), (button & 0x02) ? HIGH : LOW);
      digitalWrite (m_pin.at (S2), (button & 0x04) ? HIGH : LOW);
      digitalWrite (m_pin.at (En), LOW);
      m_selected = button;
    }
    return selected();
  }

  //----------------------------------------------------------------------------
  void Cd4051::deselect () {

    if (isOpened()) {

      digitalWrite (m_pin.at (En), HIGH);
      m_selected = -1;
    }
  }

  //----------------------------------------------------------------------------
  bool Cd4051::operator== (const ButtonController &other) const {

    if (ButtonController::operator== (other)) {

      return m_pin == static_cast<const Cd4051&> (other).m_pin;
    }
    return false;
  }

  //----------------------------------------------------------------------------
  bool Cd4051::isNull() const {
    bool ret = true;

    for (auto p : m_pin) {
      ret = ret && (p == -1);
    }
    return ret && (m_selected == -1);
  }

  //----------------------------------------------------------------------------
  //
  //                            Settings
  //
  //----------------------------------------------------------------------------
  Cd4051 Scip1Ctrl (4, 5, 15, 16);
  Cd4051 Scip2CtrlA (5, 4, 15, 16);
  Cd4051 Scip2CtrlB (5, 4, 15, 0);

  std::map<std::string, ButtonController &>
  ButtonController::Register = {
    {"Scip1Ctrl", Scip1Ctrl},
    {"Scip2CtrlA", Scip2CtrlA},
    {"Scip2CtrlB", Scip2CtrlB}
  };
}
//------------------------------------------------------------------------------
