/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#include "../pcf8574mux.h"
#include "twowireslave.h"
#include "buttoncontroller_p.h"

namespace SpaIot {

  class Pcf8574Mux::Private : public ButtonController::Private {
    public:
      Private (uint8_t slaveAddress = 0x20, TwoWire &bus = Wire, uint8_t idleValue = 0xFF);
      Private (const String &name, uint8_t slaveAddress = 0x20, TwoWire &bus = Wire, uint8_t idleValue = 0xFF);
      TwoWireSlave pcf;
      uint8_t idle;
      static const int Id = 1;
  };

}
