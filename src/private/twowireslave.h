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

#include <Arduino.h>
#include <Wire.h>
namespace SpaIot {

  /**
   * @class TwoWireSlave
   *
   * Allows write-read access to an I²C slave circuit
   */
  class TwoWireSlave {
    public:

      /**
       * @brief Constructor
       * @param slaveAddress 7-bit I²C slave m_address, right-aligned
       * @param bus reference on the I²C bus to use
       */
      TwoWireSlave (uint8_t slaveAddress, TwoWire & bus = Wire);

      /**
       * @brief Write a byte in the integrated circuit
       *
       * The frame is made up of a start condition, the slave m_address with an
       * RW bit low, the \c value byte and a stop condition.
       * @param value byte to write
       * @return number of bytes written, -1 if error
       */
      int write (uint8_t value);

      /**
       * @brief Reading a byte in the integrated circuit
       *
       * The frame is made up of a start condition, the slave m_address with an
       * RW bit high, the byte read and a stop condition.
       * @return byte read as unsigned, -1 if error
       */
      int read();

      /**
       * @brief Write several bytes in the integrated circuit
       *
       * The frame is made up of a start condition, the slave m_address with an
       * RW bit low, the bytes to write and a stop condition.
       * @param values pointer to the bytes to write
       * @param len number of bytes to write
       * @return number of bytes written, -1 if error
       */
      int write (const uint8_t *values, uint16_t len);

      /**
       * @brief Reading several bytes in the integrated circuit
       *
       * The frame is made up of a start condition, the slave m_address with an
       * RW bit high, the bytes read and a stop condition.
       * @param values pointer where to store bytes, must be able to store at least n bytes
       * @param len number  of bytes to read
       * @return number of bytes read, -1 if error
       */
      int read (uint8_t *values, uint16_t len);

      /**
       * @brief Write a byte in an integrated circuit register
       *
       * The frame is made up of a start condition, the slave m_address with an
       * RW bit low, the \c dataAddress, the \c value byte and a stop condition.
       * @param dataAddress register m_address or control byte
       * @param value byte to write
       * @return number of bytes written, -1 if error
       */
      int write (uint8_t dataAddress, uint8_t value);

      /**
       * @brief Reading a byte in an integrated circuit register
       *
       * The frame is made up of a start condition, the slave m_address with an
       * RW bit high, the \c dataAddress, the byte read and a stop condition.
       * @param dataAddress register m_address or control byte
       * @return byte read
       */
      int read (uint8_t dataAddress);

      /**
       * @brief Write bytes in integrated circuit registers
       *
       * The frame is made up of a start condition, the slave m_address with an
       * RW bit low, the \c dataAddress, the bytes to write and a stop condition.
       * @param dataAddress register m_address or control byte
       * @param values pointer to the bytes to write
       * @param len number of bytes to write
       * @return number of bytes written, -1 if error
       */
      int write (uint8_t dataAddress, const uint8_t *values, uint16_t len);

      /**
       * @brief Reading several bytes in the integrated circuit
       *
       * The frame is made up of a start condition, the slave m_address with an
       * RW bit high, the \c dataAddress, the bytes read and a stop condition.
       * @param dataAddress register m_address or control byte
       * @param values pointer where to store bytes, must be able to store at least n bytes
       * @param len number of bytes to read
       * @return number of bytes read, -1 if error
       */
      int read (uint8_t dataAddress, uint8_t *values, uint16_t len);

      /**
       * @brief Reading of the slave m_address of the integrated circuit
       * @return 7-bit I²C slave m_address, right-aligned
       */
      inline uint8_t address() const {

        return m_address;
      }

      /**
       * @brief Change of the slave m_address of the integrated circuit
       * @param slaveAddress 7-bit I²C slave m_address, right-aligned
       */
      inline void setAddress (uint8_t slaveAddress) {

        m_address = slaveAddress;
      }

      /**
       * @brief Returns the I²C bus used to access the integrated circuit
       * @return reference on the I²C bus used
       */
      inline TwoWire & bus() const {

        return *m_bus;
      }

    private:
      TwoWire * m_bus;
      uint8_t m_address;
  };
}
