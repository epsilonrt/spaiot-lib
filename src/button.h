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

#include "buttonsettings.h"
#include "buttoncontroller.h"
#include "cd4051.h"

namespace SpaIot {

  /**
   * @class Button
   * @brief
   */
  class Button {

    public:
      Button ();
      Button (const ButtonSettings & settings);

      int id() const;
      const ButtonController & ctrl() const;
      ButtonController & ctrl();
      const ButtonSettings & settings() const;

      void begin();
      void press();
      void release();
      void push();
      bool isOpened() const;
      bool isNull() const;
      bool isPressed() const;
      bool operator== (const Button &other) const;
      bool operator!= (const Button &other) const;

    private:
      ButtonController * m_ctrl;
      const ButtonSettings * m_settings;
  };
}
