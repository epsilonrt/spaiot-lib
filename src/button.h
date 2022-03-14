/*
 * SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
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
#include "pcf8574mux.h"

namespace SpaIot {

  /**
   * @class Button
   * @brief
   */
  class Button {

    public:
      /**
       * @brief 
       */
      Button ();
      /**
       * @brief 
       */
      Button (const ButtonSettings & settings);

      /**
       * @brief 
       * @return 
       */
      int id() const;
      /**
       * @brief 
       * @return 
       */
      const ButtonController & ctrl() const;
      /**
       * @brief 
       * @return 
       */
      ButtonController & ctrl();
      /**
       * @brief 
       * @return 
       */
      const ButtonSettings & settings() const;

      /**
       * @brief 
       */
      void begin();
      /**
       * @brief 
       */
      void press();
      /**
       * @brief 
       */
      void release();
      /**
       * @brief 
       */
      void push();
      /**
       * @brief 
       * @return 
       */
      bool isOpened() const;
      /**
       * @brief 
       * @return 
       */
      bool isNull() const;
      /**
       * @brief 
       * @return 
       */
      bool isPressed() const;
      /**
       * @brief 
       * @param other
       */
      bool operator== (const Button &other) const;
      /**
       * @brief 
       * @param other
       */
      bool operator!= (const Button &other) const;

    private:
      ButtonController * m_ctrl;
      const ButtonSettings * m_settings;
  };
}
