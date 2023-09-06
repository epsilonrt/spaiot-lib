/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#include "button_p.h"
#include "../spaiotdebug.h"

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                                Button
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // Private implementation

  //----------------------------------------------------------------------------
  Button::Private::Private ()
  {}

  //----------------------------------------------------------------------------
  Button::Private::Private (const ButtonSettings &settings) :
    settings (settings)
  {}

  //----------------------------------------------------------------------------
  // Protected constructor with private implementation
  Button::Button (Private &dd) : d_ptr (&dd) {}

  //----------------------------------------------------------------------------
  // API

  //----------------------------------------------------------------------------
  // Default constructor
  // Call the protected constructor with private implementation
  Button::Button() :
    Button (*new Private) {}

  //----------------------------------------------------------------------------
  // Destructor
  // No default implementation, unique_ptr will call the destructor of the
  // private implementation
  Button::~Button() {}

  //----------------------------------------------------------------------------
  Button::Button (const ButtonSettings &settings) :
    Button (*new Private (settings)) {}

  // ---------------------------------------------------------------------------
  // Copy constructor
  Button::Button (const Button &other) :
    Button (*new Private (*other.d_ptr)) {}

  // ---------------------------------------------------------------------------
  // Assignment operator
  Button &Button::operator= (const Button &other) {

    if (this != &other) {

      *d_ptr = *other.d_ptr;
      return *this;
    }
    return *this;
  }

  // ---------------------------------------------------------------------------
  // Move constructor
  Button::Button (Button &&other) = default;

  // ---------------------------------------------------------------------------
  // Move assignment operator
  Button &Button::operator= (Button &&other) = default;

  //----------------------------------------------------------------------------
  bool Button::isNull() const {

    return d_ptr == nullptr;
  }

  //----------------------------------------------------------------------------
  bool Button::isEmpty() const {

    if (!isNull()) {
      PIMPL_D (const Button);

      return d->settings.isEmpty();
    }
    return false;
  }

  //----------------------------------------------------------------------------
  void Button::clear() {

    if (isNull()) {

      d_ptr.reset (new Private);
    }
    else {
      PIMPL_D (Button);

      d->settings.clear();
    }
  }

  //----------------------------------------------------------------------------
  bool Button::operator== (const Button &other) const {
    PIMPL_D (const Button);

    return d->settings == other.d_func()->settings;
  }

  //----------------------------------------------------------------------------
  bool Button::operator!= (const Button &other) const {

    return ! (*this == other);
  }

  //----------------------------------------------------------------------------
  bool Button::isOpen() const {

    return ctrl().isOpen();
  }

  //----------------------------------------------------------------------------
  const ButtonController &Button::ctrl() const {
    PIMPL_D (const Button);

    return d->settings.ctrl();
  }

  //----------------------------------------------------------------------------
  ButtonController &Button::ctrl() {
    PIMPL_D (Button);

    return d->settings.ctrl();
  }

  //----------------------------------------------------------------------------
  const ButtonSettings &Button::settings() const {
    PIMPL_D (const Button);

    return d->settings;
  }

  //----------------------------------------------------------------------------
  int Button::id() const {
    PIMPL_D (const Button);

    return d->settings.id();
  }

  //----------------------------------------------------------------------------
  void Button::begin() {
    PIMPL_D (const Button);

    if (isNull() == false) {

      if (ctrl().isOpen() == false) {

        SPAIOT_DBG ("Button::begin: button(%d) opening %s controller", id(),
                    d->settings.ctrl().name().c_str());
        ctrl().begin();
      }
    }
    else {
      SPAIOT_DBG ("%s:%d: <Critical Error> Unable to open the '%s' "
                  "ButtonController in the register, check his name !",
                  __PRETTY_FUNCTION__, __LINE__,
                  d->settings.ctrl().name().c_str());
    }
  }

  //----------------------------------------------------------------------------
  void Button::press() {

    if (isOpen()) {

      ctrl().select (id());
    }
  }

  //----------------------------------------------------------------------------
  bool Button::isPressed() const {

    if (isOpen()) {

      return ctrl().isSelected();
    }
    return false;
  }

  //----------------------------------------------------------------------------
  void Button::release() {

    if (isOpen()) {

      ctrl().deselect ();
    }
  }

  //----------------------------------------------------------------------------
  void Button::push() {

    press();
    delay (HoldPressedMs);
    release();
  }

}
//------------------------------------------------------------------------------
