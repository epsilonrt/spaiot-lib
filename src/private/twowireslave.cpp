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
#include "twowireslave.h"
#include <spaiotdebug.h>

namespace SpaIot {

// -----------------------------------------------------------------------------
//
//                            TwoWireSlave class
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
  TwoWireSlave::TwoWireSlave (uint8_t slaveAddress, TwoWire & bus) :
    m_bus (bus), m_address (slaveAddress)  {
  }

// -----------------------------------------------------------------------------
  int TwoWireSlave::read (uint8_t *values, uint16_t len) {
    int ret;

    ret = m_bus.requestFrom (static_cast<int> (m_address), static_cast<int> (len));
    if (ret > 0) {

      while (m_bus.available()) {

        *values++ = m_bus.read();
      }
    }
    return ret;
  }

// -----------------------------------------------------------------------------
  int TwoWireSlave::read (uint8_t dataAddress, uint8_t *values, uint16_t len) {
    int ret = 0;

    m_bus.beginTransmission (m_address);
    ret = m_bus.write (dataAddress);

    if (ret == 1) {

      if (m_bus.endTransmission (false) == 0) {

        ret = this->read (values, len);
      }
      else {

        ret = -1;
      }
    }
    return ret;
  }

// -----------------------------------------------------------------------------
  int TwoWireSlave::read (uint8_t dataAddress) {
    int ret;
    uint8_t data = 0xFF;

    ret = this->read (dataAddress, &data, 1);
    if (ret == 1) {
      ret = static_cast<unsigned> (data);
    }
    return ret;
  }

// -----------------------------------------------------------------------------
  int TwoWireSlave::read () {
    int ret;
    uint8_t data = 0xFF;

    ret = this->read (&data, 1);
    if (ret == 1) {
      ret = static_cast<unsigned> (data);
    }
    return ret;
  }

// -----------------------------------------------------------------------------
  int TwoWireSlave::write (const uint8_t *values, uint16_t len) {
    int ret;

    m_bus.beginTransmission (m_address);
    ret = m_bus.write (values, len);
    if (ret > 0) {

      if (m_bus.endTransmission() != 0) {

        ret = -1;
      }
    }
    return ret;
  }

// -----------------------------------------------------------------------------
  int TwoWireSlave::write (uint8_t dataAddress, const uint8_t *values, uint16_t len) {
    int ret;

    m_bus.beginTransmission (m_address);

    ret = m_bus.write (dataAddress);
    if (ret == 1) {

      ret = m_bus.write (values, len);
      if (ret > 0) {

        if (m_bus.endTransmission() != 0) {

          ret = -1;
        }
      }
    }

    return ret;
  }

// -----------------------------------------------------------------------------
  int TwoWireSlave::write (uint8_t dataAddress, uint8_t value) {

    return this->write (dataAddress, &value, 1);
  }

// -----------------------------------------------------------------------------
  int TwoWireSlave::write (uint8_t value) {

    SPAIOT_DBG ("TwoWireSlave::write(%d)", value);
    return this->write (&value, 1);
  }
}
