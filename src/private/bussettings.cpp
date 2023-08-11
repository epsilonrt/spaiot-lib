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
#include "bussettings_p.h"
#include <cassert>

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                                  BusSettings
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // Private implementation

  //----------------------------------------------------------------------------
  BusSettings::Private::Private () : pin (3, -1)
  {}

  //----------------------------------------------------------------------------
  BusSettings::Private::Private (const int dataPin, const int clkPin, const int holdPin) :
    pin  { dataPin, clkPin, holdPin }
  {}


  //----------------------------------------------------------------------------
  BusSettings::Private::Private (const std::initializer_list<int> &pins) :
    pin (pins) {

    assert (pins.size() >= 3);
  }

  //----------------------------------------------------------------------------
  // Protected constructor with private implementation
  BusSettings::BusSettings (Private &dd) : d_ptr (&dd) {}

  //----------------------------------------------------------------------------
  // API

  //----------------------------------------------------------------------------
  // Default constructor
  // Call the protected constructor with private implementation
  BusSettings::BusSettings() :
    BusSettings (*new Private) {}

  //----------------------------------------------------------------------------
  // Destructor
  // No default implementation, unique_ptr will call the destructor of the
  // private implementation
  BusSettings::~BusSettings() {}

  //----------------------------------------------------------------------------
  BusSettings::BusSettings (const int dataPin, const int clkPin, const int holdPin) :
    BusSettings (*new Private (dataPin, clkPin, holdPin)) {}

  //----------------------------------------------------------------------------
  BusSettings::BusSettings (const std::initializer_list<int> &pins) :
    BusSettings (*new Private (pins)) {}

  // ---------------------------------------------------------------------------
  // Copy constructor
  BusSettings::BusSettings (const BusSettings &other) :
    BusSettings (*new Private (*other.d_ptr)) {}

  // ---------------------------------------------------------------------------
  // Assignment operator
  BusSettings &BusSettings::operator= (const BusSettings &other) {

    if (this != &other) {

      *d_ptr = *other.d_ptr;
      return *this;
    }
    return *this;
  }

  // ---------------------------------------------------------------------------
  // Move constructor
  BusSettings::BusSettings (BusSettings &&other) = default;
  // BusSettings::BusSettings (BusSettings &&other) {

  //   d_ptr = std::move (other.d_ptr);
  // }

  // ---------------------------------------------------------------------------
  // Move assignment operator
  BusSettings &BusSettings::operator= (BusSettings &&other) = default;
  // BusSettings &BusSettings::operator= (BusSettings &&other) {

  //   if (this != &other) {

  //     d_ptr = std::move (other.d_ptr);
  //   }
  //   return *this;
  // }

  //----------------------------------------------------------------------------
  void BusSettings::clear() {

    if (d_ptr == nullptr) {

      d_ptr.reset (new Private);
    }
    else {

      d_ptr->pin = { -1, -1, -1 };
    }
  }

  //----------------------------------------------------------------------------
  void BusSettings::setDataPin (int pin) {
    PIMPL_D (BusSettings);

    d->pin[0] = pin;
  }

  //----------------------------------------------------------------------------
  void BusSettings::setClkPin (int pin) {
    PIMPL_D (BusSettings);

    d->pin[1] = pin;
  }

  //----------------------------------------------------------------------------
  void BusSettings::setHoldPin (int pin) {
    PIMPL_D (BusSettings);

    d->pin[2] = pin;
  }

  //----------------------------------------------------------------------------
  bool BusSettings::isNull() const {
    
    if (d_ptr != nullptr) {
      PIMPL_D (const BusSettings);

      return d->pin[0] == -1 || d->pin[1] == -1 || d->pin[2] == -1;
    }
    return true;
  }

  //----------------------------------------------------------------------------
  int BusSettings::dataPin() const {
    PIMPL_D (const BusSettings);

    return d->pin[0];
  }

  //----------------------------------------------------------------------------
  int BusSettings::clkPin() const {
    PIMPL_D (const BusSettings);

    return d->pin[1];
  }

  //----------------------------------------------------------------------------
  int BusSettings::holdPin() const {
    PIMPL_D (const BusSettings);

    return d->pin[2];
  }

  //----------------------------------------------------------------------------
  bool BusSettings::operator== (const BusSettings &other) const {
    PIMPL_D (const BusSettings);

    return d->pin == other.d_func()->pin;
  }

  //----------------------------------------------------------------------------
  bool BusSettings::operator!= (const BusSettings &other) const {
    PIMPL_D (const BusSettings);

    return ! (*this == other);
  }

  //----------------------------------------------------------------------------
  //
  //                            Settings
  //
  //----------------------------------------------------------------------------
  const BusSettings ScipBus (DataPin, ClkPin, HoldPin);
}
//------------------------------------------------------------------------------
