/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#include "../controlpanel.h"
#include "framedecoder_p.h"
namespace SpaIot {

  class ControlPanel::Private : public FrameDecoder::Private {
    public:
      Private (ControlPanel *q);
      Private (ControlPanel *q, const HardwareSettings &hwsettings);
      uint8_t setKeyOn (int key, bool v);
      bool makeButtons();
      /**
         @brief Buttons list
         @return Constant reference on the [std::map](https://en.cppreference.com/w/cpp/container/map)  containing the buttons.
         The different key values are defined by SpaIot::Key
      */
      const std::map <int, Button> &buttons() const;

    public:
      std::map <int, Button> button;
      std::map <int, ButtonSettings> btnsettings;
      bool isopen;

    private:
      PIMPL_DECLARE_PUBLIC (ControlPanel);
  };

}
