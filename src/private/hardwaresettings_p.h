/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#include "../hardwaresettings.h"

namespace SpaIot {

  class HardwareSettings::Private {
    public:
      Private ();
      Private (const BusSettings &bus,
               const std::map <int, LedSettings> &leds,
               const std::map <int, ButtonSettings> &buttons);
      const BusSettings *bus;
      const std::map <int, LedSettings> *leds;
      const std::map <int, ButtonSettings> *buttons;
      static std::map<String, const HardwareSettings *> Register;
  };

}
