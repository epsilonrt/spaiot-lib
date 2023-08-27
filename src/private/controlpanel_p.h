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
      bool isopen;
      std::map <int, Button> button;
      std::map <int, ButtonSettings> btnsettings;
    private:
      PIMPL_DECLARE_PUBLIC (ControlPanel);
  };

}
