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
   * @class ButtonController
   * @brief
   */
  class ButtonController {
    public:
      ButtonController();
      virtual ~ButtonController() {}
      virtual void begin() = 0;
      virtual int select (int button) = 0;
      virtual void deselect () = 0;
      virtual int selected() const;
      virtual bool isOpened() const;
      virtual bool isNull() const;
      virtual bool isSelected() const;
      virtual bool operator== (const ButtonController &other) const;
      virtual bool operator!= (const ButtonController &other) const;
      
      static bool addToRegister (const std::string & name, ButtonController & controller);
      static ButtonController & getFromRegister (const std::string & name);

    protected:
      bool m_isopened;
      int m_selected;
      
    private:
      static std::map<std::string, ButtonController &> Register;
  };
}
