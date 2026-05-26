/*
   SpaIot Library (c) by epsilonrt - epsilonrt@epsilonrt.fr
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#include "../button.h"

namespace SpaIot {

  class Button::Private {
    public:
      Private ();
      Private (const ButtonSettings & settings);
      ButtonSettings settings;
  };

}
