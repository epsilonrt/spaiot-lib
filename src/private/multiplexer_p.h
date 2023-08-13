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

#include <vector>
#include "../multiplexer.h"
#include "buttoncontroller_p.h"

namespace SpaIot {

  class Multiplexer::Private : public ButtonController::Private {
    public:
      Private();
      Private (const std::initializer_list<int> &spins, int inhPin);
      Private (const String &name, const std::initializer_list<int> &spins, int inhPin);

      std::vector<int> spin;
      int inh;
      static const int Id = 0;
  };

}
