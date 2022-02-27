/*
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
#include <Arduino.h>
#include <button.h>
#include "debug.h"

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                                Button
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  Button::Button () :
    m_ctrl (nullptr), m_settings (nullptr)
  {}

  //----------------------------------------------------------------------------
  Button::Button (const ButtonSettings & settings) :
    m_ctrl (& ButtonController::getFromRegister (settings.controllerName())),
    m_settings (&settings)
  {}

  //----------------------------------------------------------------------------
  const ButtonController & Button::ctrl() const {

    return *m_ctrl;
  }

  //----------------------------------------------------------------------------
  ButtonController & Button::ctrl() {

    return *m_ctrl;
  }

  //----------------------------------------------------------------------------
  const ButtonSettings & Button::settings() const {

    return *m_settings;
  }

  //----------------------------------------------------------------------------
  int Button::id() const {

    return m_settings->id();
  }

  //----------------------------------------------------------------------------
  bool Button::isOpened() const {

    if (isNull() == false) {

      return m_ctrl->isOpened();
    }
    return false;
  }

  //----------------------------------------------------------------------------
  void Button::begin() {

    if (isNull() == false) {
      if (m_ctrl->isOpened() == false) {

        DBG ("Button::begin: button(%d) opening %s controller", id(),
             m_settings->controllerName().c_str());
        m_ctrl->begin();
      }
    }
  }

  //----------------------------------------------------------------------------
  void Button::press() {
    
    if (isOpened()) {
      
      m_ctrl->select (id());
    }
  }

  //----------------------------------------------------------------------------
  bool Button::isPressed() const {

    if (isOpened()) {
      
      return m_ctrl->isSelected();
    }
    return false;
  }

  //----------------------------------------------------------------------------
  void Button::release() {

    if (isOpened()) {
      
      m_ctrl->deselect ();
    }
  }

  //----------------------------------------------------------------------------
  void Button::push() {

    press();
    delay (HoldPressedMs);
    release();
  }

  //----------------------------------------------------------------------------
  bool Button::isNull() const {

    return m_ctrl == nullptr || m_settings == nullptr;
  }

  //----------------------------------------------------------------------------
  bool Button::operator== (const Button &other) const {

    if (isNull() == false) {

      return *m_settings == *other.m_settings &&
             *m_ctrl == *other.m_ctrl;
    }
    return false;
  }

  //----------------------------------------------------------------------------
  bool Button::operator!= (const Button &other) const {

    return ! (*this == other);
  }

}
//------------------------------------------------------------------------------
