/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#include "../event.h"
#include <map>

namespace SpaIot {

  class Event::Private {
    public:
      Private ();
      Private (Type type, uint16_t value);
      Type type;
      uint16_t value;
      static const std::map <Event::Type, String> TypeToString;
  };
}
