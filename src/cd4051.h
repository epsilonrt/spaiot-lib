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

#include "multiplexer.h"

namespace SpaIot {

  /**
   * @class Cd4051
   * @brief
   */
  class Cd4051 : public Multiplexer<8> {
    public:
      Cd4051 () :
        Multiplexer<8> ()
      {}

      Cd4051 (const std::initializer_list<int>& spins, int inhPin) :
        Multiplexer<8> (spins, inhPin) 
      {}
      
      Cd4051 (const String & name, const std::initializer_list<int>& spins, int inhPin) :
        Multiplexer<8> (name, spins, inhPin) 
      {}

      Cd4051 (int A, int B, int C, int INH) :
        Multiplexer<8> ({A, B, C}, INH)
      {}
      
      Cd4051 (const String & name, int A, int B, int C, int INH) :
        Multiplexer<8> (name, {A, B, C}, INH)
      {}
  };

  extern Cd4051 Scip1Ctrl;
  extern Cd4051 Scip2CtrlA;
  extern Cd4051 Scip2CtrlB;
}
