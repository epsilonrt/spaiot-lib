/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#include "../buttonsettings.h"

namespace SpaIot {

  class ButtonSettings::Private {
    public:
      Private ();
      Private (const String &controllerName, int buttonId);
      Private (ButtonController &controller, int buttonId);
      int id;
      String  ctrlName;
      mutable ButtonController * ctrl;
  };

}
