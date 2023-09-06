/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

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
