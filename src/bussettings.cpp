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
#include <bussettings.h>
#include <cassert>

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                                  BusSettings
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  BusSettings::BusSettings (const int dataPin, const int clkPin, const int holdPin) :
    m_pin  { dataPin, clkPin, holdPin }
  {}

  //----------------------------------------------------------------------------
  BusSettings::BusSettings () :
    m_pin (3, -1)
  {}

  //----------------------------------------------------------------------------
  BusSettings::BusSettings (const std::initializer_list<int>& pins) :
    m_pin (pins) {

    assert (pins.size() >= 3);
  }

  //----------------------------------------------------------------------------
  void BusSettings::setDataPin (int pin) {

    m_pin[0] = pin;
  }

  //----------------------------------------------------------------------------
  void BusSettings::setClkPin (int pin) {

    m_pin[1] = pin;
  }

  //----------------------------------------------------------------------------
  void BusSettings::setHoldPin (int pin) {

    m_pin[2] = pin;
  }

  //----------------------------------------------------------------------------
  bool BusSettings::isNull() const {

    return m_pin[0] == -1 || m_pin[1] == -1 || m_pin[2] == -1;
  }

  //----------------------------------------------------------------------------
  int BusSettings::dataPin() const {

    return m_pin[0];
  }

  //----------------------------------------------------------------------------
  int BusSettings::clkPin() const {

    return m_pin[1];
  }

  //----------------------------------------------------------------------------
  int BusSettings::holdPin() const {

    return m_pin[2];
  }

  //----------------------------------------------------------------------------
  bool BusSettings::operator== (const BusSettings &other) const {

    return m_pin[0] == other.m_pin[0] &&
           m_pin[1] == other.m_pin[1] &&
           m_pin[2] == other.m_pin[2];
  }

  //----------------------------------------------------------------------------
  bool BusSettings::operator!= (const BusSettings &other) const {

    return ! (*this == other);
  }

  //----------------------------------------------------------------------------
  //
  //                                LedSettings
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  LedSettings::LedSettings () :
    LedSettings (UnsetValue8)
  {}

  //----------------------------------------------------------------------------
  LedSettings::LedSettings (const uint8_t order) :
    m_order (order != UnsetValue8 ? order % 16 : UnsetValue8)
  {}

  //----------------------------------------------------------------------------
  uint16_t LedSettings::frame() const {

    return isNull() ? 0 : 1 << m_order;
  }

  //----------------------------------------------------------------------------
  uint8_t LedSettings::order() const {

    return m_order;
  }

  //----------------------------------------------------------------------------
  void LedSettings::setOrder (const uint8_t order) {

    m_order = order % 16;
  }

  //----------------------------------------------------------------------------
  bool LedSettings::isNull() const {

    return m_order == UnsetValue8;
  }

  //----------------------------------------------------------------------------
  bool LedSettings::operator== (const LedSettings &other) const {

    return m_order == other.m_order;
  }

  //----------------------------------------------------------------------------
  bool LedSettings::operator!= (const LedSettings &other) const {

    return ! (*this == other);
  }

  //----------------------------------------------------------------------------
  //
  //                            Settings
  //
  //----------------------------------------------------------------------------
  const BusSettings DiyScipBus (12, 14, 13);

  /*
   * POWER        = P
   * FILTER       = F
   * HEATER       = H
   * HEATER_RED   = HR
   * HEATER_GREEN = HG
   * BUBBLE       = B
   * JET          = J
   * SANITIZER    = S
   * DOWN         = D
   * UP           = U
   * FC           = C
   *         |-------------------------------------------------------------------------------|
   *         | B7 | B6 | B5 | B4 | B3 | B2 | B1 | B0 | A7 | A6 | A5 | A4 | A3 | A2 | A1 | A0 |
   * BIT     | 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
   *
   * LED_SSP |    |  1 |    |  F |    |  B | HG |    | HR |    |    |    |    |    |    |  P |
   */
  //----------------------------------------------------------------------------
  const std::map<int, LedSettings> SspLeds = {
    { Power,          LedSettings (0) },
    { Heater,         LedSettings (7) },
    { HeatReached,    LedSettings (9) },
    { Bubble,         LedSettings (10) },
    { Filter,         LedSettings (12) }
  };

  /*         |-------------------------------------------------------------------------------|
   *         | B7 | B6 | B5 | B4 | B3 | B2 | B1 | B0 | A7 | A6 | A5 | A4 | A3 | A2 | A1 | A0 |
   * BIT     | 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
   *
   * LED_SJB |    |  1 |  S |  F |    |  J | HG |    | HR |    |    |    |    |    |  B |  P |
   */
  //----------------------------------------------------------------------------
  const std::map<int, LedSettings> SjbLeds = {
    { Power,          LedSettings (0) },
    { Bubble,         LedSettings (1) },
    { Heater,         LedSettings (7) },
    { HeatReached,    LedSettings (9) },
    { Jet,            LedSettings (10) },
    { Filter,         LedSettings (12) },
    { Sanitizer,      LedSettings (13) }
  };

}
//------------------------------------------------------------------------------
