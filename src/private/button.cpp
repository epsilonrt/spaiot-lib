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
#include <Arduino.h>
#include <button.h>
#include <spaiotdebug.h>

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                                Button
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  Button::Button ()
  {}

  //----------------------------------------------------------------------------
  Button::Button (const ButtonSettings & settings) :
    m_settings (settings)
  {}

  //----------------------------------------------------------------------------
  const ButtonController & Button::ctrl() const {

    return m_settings.ctrl();
  }

  //----------------------------------------------------------------------------
  ButtonController & Button::ctrl() {

    return m_settings.ctrl();
  }

  //----------------------------------------------------------------------------
  const ButtonSettings & Button::settings() const {

    return m_settings;
  }

  //----------------------------------------------------------------------------
  int Button::id() const {

    return m_settings.id();
  }

  //----------------------------------------------------------------------------
  bool Button::isNull() const {

    return m_settings.isNull();
  }

  //----------------------------------------------------------------------------
  bool Button::isOpened() const {

    return ctrl().isOpened();
  }

  //----------------------------------------------------------------------------
  bool Button::operator== (const Button &other) const {

    return m_settings == other.m_settings;
  }

  //----------------------------------------------------------------------------
  void Button::begin() {

    if (isNull() == false) {
      if (ctrl().isOpened() == false) {

        SPAIOT_DBG ("Button::begin: button(%d) opening %s controller", id(),
                    m_settings.ctrl().name().c_str());
        ctrl().begin();
      }
    }
    else {
      SPAIOT_DBG ("%s:%d: <Critical Error> Unable to open the '%s' "
                  "ButtonController in the register, check his name !",
                  __PRETTY_FUNCTION__, __LINE__,
                  m_settings.ctrl().name().c_str());
    }
  }

  //----------------------------------------------------------------------------
  void Button::press() {

    if (isOpened()) {

      ctrl().select (id());
    }
  }

  //----------------------------------------------------------------------------
  bool Button::isPressed() const {

    if (isOpened()) {

      return ctrl().isSelected();
    }
    return false;
  }

  //----------------------------------------------------------------------------
  void Button::release() {

    if (isOpened()) {

      ctrl().deselect ();
    }
  }

  //----------------------------------------------------------------------------
  void Button::push() {

    press();
    delay (HoldPressedMs);
    release();
  }

  //----------------------------------------------------------------------------
  bool Button::operator!= (const Button &other) const {

    return ! (*this == other);
  }

}
//------------------------------------------------------------------------------
