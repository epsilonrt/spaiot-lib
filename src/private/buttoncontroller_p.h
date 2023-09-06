/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#include "../buttoncontroller.h"

namespace SpaIot {

  class ButtonController::Private {
    public:
      Private (int typeId);
      Private (int typeId, const String &name);
      bool isSameType (const ButtonController &other) const;

    public:
      int typeId;
      bool isopen;
      int selected;
      mutable String name;
      static std::map<String, ButtonController &> Register;
  };

}
