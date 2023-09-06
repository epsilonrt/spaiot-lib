/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#include "../serversettings.h"

namespace SpaIot {

  class ServerSettings::Private {
    public:
      Private ();
      String spaModel;
      unsigned long publishInterval;
  };

}
