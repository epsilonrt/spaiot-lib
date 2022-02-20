/**
 * SpaIot Library (c) by espilonrt - epsilonrt@gmail.com
 * This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>
 *
 * SpaIot library is licensed under a
 * Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
 *
 * You should have received a copy of the license along with this
 * work. If not, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.
 *
 * SpaIot library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY;
 */
#pragma once

#include "hardwaresettings.h"
#include "framedecoder.h"
#include "button.h"

namespace SpaIot {

  /**
   * @class ControlPanel
   * @brief
   *
   * It is a singleton class that can only be instantiated through getInstance()
   */
  class ControlPanel : public FrameDecoder {
    public:
      static ControlPanel * getInstance (const HardwareSettings & hwsettings);
      static ControlPanel * getInstance (const std::string & hwSettingsName);

      void begin();
      bool isOpened() const;
      bool hasButton (int key) const;
      bool pushButton (int key);
      const std::map <int, ButtonSettings> buttonSettings() const;

      uint8_t setPower (bool v = true);
      uint8_t setFilter (bool v = true);
      uint8_t setHeater (bool v = true);
      uint8_t setBubble (bool v = true);
      uint8_t setJet (bool v = true);

      bool setDesiredTemp (uint16_t temp);
      bool setSanitizerTime (uint16_t time);

      uint16_t waitForDesiredTemp (unsigned long MaxWaitingTimeMs = 5000);
      
    protected:
      ControlPanel (const HardwareSettings & hwsettings);
      uint8_t setKeyOn (int key, bool v);

    private:
      const std::map <int, ButtonSettings> & m_buttonSettings;
      bool m_isopened;
      std::map <int, Button> m_button;
      static ControlPanel * m_instance;
  };

}
