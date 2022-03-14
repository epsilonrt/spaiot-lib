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

#include "buttoncontroller.h"
#include "twowireslave.h"

namespace SpaIot {

  /**
   * @class Pcf8574Mux
   * @brief 
   */
  class Pcf8574Mux : public ButtonController {
    public:

      /**
       * @brief 
       * @param slaveAddress
       * @param bus
       * @param idleValue
       */
      Pcf8574Mux (uint8_t slaveAddress = 0x20, TwoWire & bus = Wire, uint8_t idleValue = 0xFF);

      /**
       * @brief
       */
      virtual void begin();

      /**
       * @brief
       * @param button
       * @return
       */
      virtual int select (int button);

      /**
       * @brief
       */
      virtual void deselect ();

      /**
       * @brief
       * @return
       */
      virtual bool isNull() const;

      /**
       * @brief
       * @param other
       * @return
       */
      virtual bool operator== (const ButtonController &other) const;

    protected:
      TwoWireSlave m_pcf;
      uint8_t m_idle;
  };

}
