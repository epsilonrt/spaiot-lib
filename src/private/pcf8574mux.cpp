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
#include "pcf8574mux_p.h"
#include "../spaiotdebug.h"

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                             Pcf8574Mux
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // Private implementation

  //----------------------------------------------------------------------------
  Pcf8574Mux::Private::Private (uint8_t slaveAddress, TwoWire &bus, uint8_t idleValue) :
    ButtonController::Private (Id), pcf (slaveAddress, bus), idle (idleValue)
  {}

  //----------------------------------------------------------------------------
  Pcf8574Mux::Private::Private (const String &name, uint8_t slaveAddress,
                                TwoWire &bus, uint8_t idleValue) :
    ButtonController::Private (Id, name), pcf (slaveAddress, bus), idle (idleValue)
  {}

  //----------------------------------------------------------------------------
  // Protected constructor with private implementation
  Pcf8574Mux::Pcf8574Mux (Private &dd) : ButtonController (dd)
  {}

  //----------------------------------------------------------------------------
  // API

  //----------------------------------------------------------------------------
  Pcf8574Mux::Pcf8574Mux (uint8_t slaveAddress, TwoWire &bus, uint8_t idleValue) :
    Pcf8574Mux (*new Private (slaveAddress, bus, idleValue))
  {}

  //----------------------------------------------------------------------------
  Pcf8574Mux::Pcf8574Mux (const String &name, uint8_t slaveAddress,
                          TwoWire &bus, uint8_t idleValue) :
    Pcf8574Mux (*new Private (name, slaveAddress, bus, idleValue))
  {}

  // ---------------------------------------------------------------------------
  // Copy constructor
  Pcf8574Mux::Pcf8574Mux (const Pcf8574Mux &other) :
    Pcf8574Mux (*new Private (*other.d_func())) {}

  // ---------------------------------------------------------------------------
  // Assignment operator
  Pcf8574Mux &Pcf8574Mux::operator= (const Pcf8574Mux &other) {

    if (this != &other) {

      *d_func() = *other.d_func();
      return *this;
    }
    return *this;
  }

  // ---------------------------------------------------------------------------
  // Move constructor
  Pcf8574Mux::Pcf8574Mux (Pcf8574Mux &&other) = default;

  // ---------------------------------------------------------------------------
  // Move assignment operator
  Pcf8574Mux &Pcf8574Mux::operator= (Pcf8574Mux &&other) = default;

  //----------------------------------------------------------------------------
  void Pcf8574Mux::clear() {

    if (isNull()) {

      d_ptr.reset (new Private);
    }
    else {
      PIMPL_D (Pcf8574Mux);

      end();
      d->pcf.setAddress (0x20);
      d->idle = 0xFF;
      ButtonController::clear();
    }
  }

  //----------------------------------------------------------------------------
  bool Pcf8574Mux::isEmpty() const {

    if (! isNull()) {
      PIMPL_D (const Pcf8574Mux);

      return d->pcf.address() == 0x20 && & d->pcf.bus() == & Wire && d->idle == 0xFF && ButtonController::isEmpty();
    }
    return false;
  }

  //----------------------------------------------------------------------------
  bool Pcf8574Mux::operator== (const ButtonController &other) const {

    if (ButtonController::operator== (other)) {
      PIMPL_D (const Pcf8574Mux);

      const Pcf8574Mux &o = static_cast<const Pcf8574Mux &> (other);
      return ( (d->pcf.address() == o.d_func()->pcf.address()) &&
               (& d->pcf.bus() == & o.d_func()->pcf.bus()) &&
               (d->idle == o.d_func()->idle)
             );
    }
    return false;
  }

  //----------------------------------------------------------------------------
  void Pcf8574Mux::begin() {

    if ( (isOpened() == false) && (isNull() == false))  {
      PIMPL_D (Pcf8574Mux);

      SPAIOT_DBGP (PSTR ("Pcf8574Mux::begin(): opening"));
      if (d->pcf.write (d->idle) == 1) {

        ButtonController::begin();
      }
      else {

        SPAIOT_DBGP (PSTR ("Pcf8574Mux::begin(): Failed !"));
      }
    }
  }

  //----------------------------------------------------------------------------
  void Pcf8574Mux::end() {

    if (isOpened()) {
      PIMPL_D (Pcf8574Mux);

      d->pcf.write (d->idle);
      ButtonController::end();
    }
  }

  //----------------------------------------------------------------------------
  int Pcf8574Mux::select (int button) {

    if (isOpened()) {
      PIMPL_D (Pcf8574Mux);

      if (d->pcf.write (button) == 1) {

        d->selected = button;
      }
      else {

        SPAIOT_DBGP (PSTR ("Pcf8574Mux::select(%d): Failed !"), button);
      }
    }
    return selected();
  }

  //----------------------------------------------------------------------------
  void Pcf8574Mux::deselect () {

    if (isOpened()) {
      PIMPL_D (Pcf8574Mux);

      if (d->pcf.write (d->idle) == 1) {

        d->selected = -1;
      }
      else {

        SPAIOT_DBGP (PSTR ("Pcf8574Mux::deselect(): Failed !"));
      }
    }
  }

  //----------------------------------------------------------------------------
  uint8_t Pcf8574Mux::address() const {
    PIMPL_D (const Pcf8574Mux);

    return d->pcf.address();
  }

}
//------------------------------------------------------------------------------
