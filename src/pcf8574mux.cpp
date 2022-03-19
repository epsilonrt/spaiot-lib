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
#include <pcf8574mux.h>
#include "spaiotdebug.h"

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                             Pcf8574Mux
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  Pcf8574Mux::Pcf8574Mux (uint8_t slaveAddress, TwoWire & bus, uint8_t idleValue) :
    ButtonController(), m_pcf (slaveAddress, bus), m_idle (idleValue) {

  }

  //----------------------------------------------------------------------------
  Pcf8574Mux::Pcf8574Mux (const std::string & name, uint8_t slaveAddress,
                          TwoWire & bus, uint8_t idleValue) :
    ButtonController (name), m_pcf (slaveAddress, bus), m_idle (idleValue) {

  }

  //----------------------------------------------------------------------------
  void Pcf8574Mux::begin() {

    if ( (isOpened() == false) && (isNull() == false))  {
      SPAIOT_DBG ("Pcf8574Mux::begin(): opening");

      m_isopened = (m_pcf.write (m_idle) == 1);
    }
  }

  //----------------------------------------------------------------------------
  void Pcf8574Mux::end() {

    m_pcf.write (m_idle);
    ButtonController::end();
  }

  //----------------------------------------------------------------------------
  int Pcf8574Mux::select (int button) {

    if (isOpened()) {

      if (m_pcf.write (button) == 1) {

        m_selected = button;
      }
      else {

        SPAIOT_DBG ("Pcf8574Mux::select(%d): Failed !", button);
      }
    }
    return selected();
  }

  //----------------------------------------------------------------------------
  void Pcf8574Mux::deselect () {

    if (isOpened()) {

      if (m_pcf.write (m_idle) == 1) {

        m_selected = -1;
      }
      else {

        SPAIOT_DBG ("Pcf8574Mux::deselect(): Failed !");
      }
    }
  }

  //----------------------------------------------------------------------------
  bool Pcf8574Mux::isNull() const {

    return false;
  }

  //----------------------------------------------------------------------------
  bool Pcf8574Mux::operator== (const ButtonController &other) const {

    if (ButtonController::operator== (other)) {

      const Pcf8574Mux & o = static_cast<const Pcf8574Mux &> (other);
      return ( (m_pcf.address() == o.m_pcf.address()) &&
               (& m_pcf.bus() == & o.m_pcf.bus()) &&
               (m_idle == o.m_idle)
             );
    }
    return false;
  }

}
//------------------------------------------------------------------------------
