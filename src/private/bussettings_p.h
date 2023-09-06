/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#include <vector>
#include "../bussettings.h"

namespace SpaIot {

  class BusSettings::Private {
    public:
      Private ();
      Private (const int dataPin, const int clkPin, const int holdPin);
      Private (const std::initializer_list<int> &pins);
      std::vector<int> pin;
  };
}
