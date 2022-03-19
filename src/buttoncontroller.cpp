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
#include <buttoncontroller.h>
#include <type_name.h>
#include <sstream>

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                         ButtonController
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  ButtonController::ButtonController() :  m_isopened (false), m_selected (-1)
  {}

  //----------------------------------------------------------------------------
  ButtonController::ButtonController (const std::string & name) :
    m_isopened (false), m_selected (-1), m_name (name)
  {}

  //----------------------------------------------------------------------------
  ButtonController::~ButtonController() {

    end();
  }

  //----------------------------------------------------------------------------
  bool ButtonController::isOpened() const {

    return m_isopened;
  }

  //----------------------------------------------------------------------------
  bool ButtonController::isSelected() const {

    return selected() != -1;
  }

  //----------------------------------------------------------------------------
  int ButtonController::selected() const {

    return m_selected;
  }

  //----------------------------------------------------------------------------
  bool ButtonController::operator== (const ButtonController &other) const {

    return type_name (*this) == type_name (other);
  }

  //----------------------------------------------------------------------------
  bool ButtonController::operator!= (const ButtonController &other) const {

    return ! (*this == other);
  }

  //----------------------------------------------------------------------------
  // static
  bool ButtonController::addToRegister (const std::string & name, ButtonController & controller) {

    if (Register.count (name) == 0) {

      Register.emplace (name, controller);
      controller.m_name = name;
      return true;
    }
    return false;
  }

  //----------------------------------------------------------------------------
  // static
  ButtonController & ButtonController::getFromRegister (const std::string & name) {

    return Register.at (name);
  }

  //----------------------------------------------------------------------------
  // static
  bool ButtonController::registerContains (const std::string & name) {

    return Register.count (name) == 1;
  }

  //----------------------------------------------------------------------------
  bool ButtonController::isNull() const {

    return true;
  }

  //----------------------------------------------------------------------------
  void ButtonController::end() {

    m_isopened = false;
  }

  //----------------------------------------------------------------------------
  const std::string & ButtonController::name() const {

    if (m_name.empty()) {
      std::stringstream ss;
      
      ss << "ButCtrl" << reinterpret_cast<uintptr_t> (this);
      m_name =  ss.str();
    }
    return m_name;
  }

}
//------------------------------------------------------------------------------
