/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SpaIot library is licensed under a
   Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.

   You should have received a copy of the license along with this
   work. If not, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#include "ledsettings_p.h"
#include <cassert>

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                                LedSettings
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // Private implementation
  LedSettings::Private::Private () : order (UnsetValue8)
  {}

  //----------------------------------------------------------------------------
  // Protected constructor with private implementation
  LedSettings::LedSettings (Private &dd) : d_ptr (&dd)
  {}

  //----------------------------------------------------------------------------
  // API

  //----------------------------------------------------------------------------
  LedSettings::LedSettings () : LedSettings (*new Private)
  {}

  //----------------------------------------------------------------------------
  // Destructor
  // No default implementation, unique_ptr will call the destructor of the
  // private implementation
  LedSettings::~LedSettings() {}

  //----------------------------------------------------------------------------
  LedSettings::LedSettings (const uint8_t order) : LedSettings ()  {
    PIMPL_D (LedSettings);

    d->order = (order != UnsetValue8 ? order % 16 : UnsetValue8);
  }

  // ---------------------------------------------------------------------------
  // Copy constructor
  LedSettings::LedSettings (const LedSettings &other) :
    LedSettings (*new Private (*other.d_ptr)) {}

  // ---------------------------------------------------------------------------
  // Assignment operator
  LedSettings &LedSettings::operator= (const LedSettings &other) {

    if (this != &other) {

      *d_ptr = *other.d_ptr;
      return *this;
    }
    return *this;
  }

  // ---------------------------------------------------------------------------
  // Move constructor
  LedSettings::LedSettings (LedSettings &&other) = default;
  // LedSettings::LedSettings (LedSettings &&other) {

  //   d_ptr = std::move (other.d_ptr);
  // }

  // ---------------------------------------------------------------------------
  // Move assignment operator
  LedSettings &LedSettings::operator= (LedSettings &&other) = default;
  // LedSettings &LedSettings::operator= (LedSettings &&other) {

  //   if (this != &other) {

  //     d_ptr = std::move (other.d_ptr);
  //   }
  //   return *this;
  // }

  //----------------------------------------------------------------------------
  void LedSettings::clear() {

    if (d_ptr == nullptr) {

      d_ptr.reset (new Private);
    }
    else {

      d_ptr->order = UnsetValue8;
    }
  }

  //----------------------------------------------------------------------------
  uint16_t LedSettings::frame() const {
    PIMPL_D (const LedSettings);

    return isNull() ? 0 : 1 << d->order;
  }

  //----------------------------------------------------------------------------
  uint8_t LedSettings::order() const {
    PIMPL_D (const LedSettings);

    return d->order;
  }

  //----------------------------------------------------------------------------
  void LedSettings::setOrder (const uint8_t order) {
    PIMPL_D (LedSettings);

    d->order = order % 16;
  }

  //----------------------------------------------------------------------------
  bool LedSettings::isNull() const {

    if (d_ptr != nullptr) {
      PIMPL_D (const LedSettings);

      return d->order == UnsetValue8;
    }
    return true;
  }

  //----------------------------------------------------------------------------
  bool LedSettings::operator== (const LedSettings &other) const {
    PIMPL_D (const LedSettings);

    return d->order == other.d_func()->order;
  }

  //----------------------------------------------------------------------------
  bool LedSettings::operator!= (const LedSettings &other) const {
    PIMPL_D (const LedSettings);

    return ! (*this == other);
  }

  //----------------------------------------------------------------------------
  //
  //                            Settings
  //
  //----------------------------------------------------------------------------

  /*
     POWER        = P
     FILTER       = F
     HEATER       = H
     HEATER_RED   = HR
     HEATER_GREEN = HG
     BUBBLE       = B
     JET          = J
     SANITIZER    = S
     DOWN         = D
     UP           = U
     FC           = C
             |-------------------------------------------------------------------------------|
             | B7 | B6 | B5 | B4 | B3 | B2 | B1 | B0 | A7 | A6 | A5 | A4 | A3 | A2 | A1 | A0 |
     BIT     | 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |

     LED_SSP |    |  1 |    |  F |    |  B | HG |    | HR |    |    |    |    |    |    |  P |
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
             | B7 | B6 | B5 | B4 | B3 | B2 | B1 | B0 | A7 | A6 | A5 | A4 | A3 | A2 | A1 | A0 |
     BIT     | 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |

     LED_SJB |    |  1 |  S |  F |    |  J | HG |    | HR |    |    |    |    |    |  B |  P |
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