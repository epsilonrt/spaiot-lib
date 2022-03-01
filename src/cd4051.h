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
#include "buttoncontroller.h"

namespace SpaIot {

  /**
   * @class Cd4051
   * @brief
   */
  class Cd4051 : public ButtonController {
    public:
      enum { S0 = 0, S1, S2, En };

      /**
       * @brief 
       */
      Cd4051 ();
      Cd4051 (const std::initializer_list<int>& pins);
      Cd4051 (int s0Pin, int s1Pin, int s2Pin, int enPin);

      int pin (int key) const;
      void setPin (int key, int pin);

      void begin();
      int select (int button);
      void deselect ();
      bool isNull() const;
      virtual bool operator== (const ButtonController &other) const;

    private:
      std::vector<int> m_pin;
  };
  
  extern Cd4051 Scip1Ctrl;
  extern Cd4051 Scip2CtrlA;
  extern Cd4051 Scip2CtrlB;
}
