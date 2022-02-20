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

#include <map>
#include <string>
#include "global.h"

namespace SpaIot {

  /**
   * @class ButtonSettings
   * @brief
   */
  class ButtonSettings {
    public:
      ButtonSettings();
      ButtonSettings (const std::string & controllerName, int buttonId);
      const std::string & controllerName() const;
      void setControllerName (const std::string & name);
      int id() const;
      void setId (int id);
      bool isNull() const;
      bool operator== (const ButtonSettings &other) const;
      bool operator!= (const ButtonSettings &other) const;

    private:
      int m_id;
      std::string  m_controllerName;
  };

  extern const std::map<int, ButtonSettings> DiyScip2SspButtons;
  extern const std::map<int, ButtonSettings> DiyScip2SjbButtons;
}
