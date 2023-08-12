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

#include <pcf8574mux.h>
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
