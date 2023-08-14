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
#include <sstream>
#include "buttoncontroller_p.h"

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                         ButtonController
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // Private implementation

  //----------------------------------------------------------------------------
  ButtonController::Private::Private (int typeId) :
    typeId (typeId), isopened (false), selected (-1)
  {}

  //----------------------------------------------------------------------------
  ButtonController::Private::Private (int typeId, const String &name) :
    typeId (typeId), isopened (false), selected (-1), name (name)
  {}

  //----------------------------------------------------------------------------
  bool ButtonController::Private::isSameType (const ButtonController &other) const {

    return typeId == other.d_ptr->typeId;
  }

  //----------------------------------------------------------------------------
  // Protected constructor with private implementation
  ButtonController::ButtonController (Private &dd) : d_ptr (&dd) {}

  //----------------------------------------------------------------------------
  // API

  // ---------------------------------------------------------------------------
  // Copy constructor
  ButtonController::ButtonController (const ButtonController &other) :
    ButtonController (*new Private (*other.d_ptr)) {}

  // ---------------------------------------------------------------------------
  // Assignment operator
  ButtonController &ButtonController::operator= (const ButtonController &other) {

    if (this != &other) {

      *d_ptr = *other.d_ptr;
    }
    return *this;
  }

  // ---------------------------------------------------------------------------
  // Move constructor
  ButtonController::ButtonController (ButtonController &&other) = default;

  // ---------------------------------------------------------------------------
  // Move assignment operator
  ButtonController &ButtonController::operator= (ButtonController &&other) = default;

  //----------------------------------------------------------------------------
  // Destructor
  // No default implementation, unique_ptr will call the destructor of the
  // private implementation
  ButtonController::~ButtonController() {

    end();
  }

  //----------------------------------------------------------------------------
  bool ButtonController::isNull() const {

    return d_ptr == nullptr;
  }

  //----------------------------------------------------------------------------
  bool ButtonController::isEmpty() const {

    if (! isNull()) {
      PIMPL_D (const ButtonController);

      return d->selected == -1 && d->isopened == false && d->name.length() == 0;
    }
    return false;
  }

  //----------------------------------------------------------------------------
  void ButtonController::clear() {

    if (! isNull()) {
      PIMPL_D (const ButtonController);

      d_ptr->isopened = false;
      d_ptr->selected = -1;
      d_ptr->name.clear();
    }
  }

  //----------------------------------------------------------------------------
  bool ButtonController::operator== (const ButtonController &other) const {
    PIMPL_D (const ButtonController);

    if (d->isSameType (other)) {

      return d->selected == other.d_ptr->selected && d->isopened == other.d_ptr->isopened && d->name == other.d_ptr->name;
    }
    return false;
  }

  //----------------------------------------------------------------------------
  bool ButtonController::operator!= (const ButtonController &other) const {

    return ! (*this == other);
  }

  //----------------------------------------------------------------------------
  void ButtonController::begin() {
    PIMPL_D (ButtonController);

    d->isopened = true;
  }

  //----------------------------------------------------------------------------
  bool ButtonController::isOpened() const {
    PIMPL_D (const ButtonController);

    return d->isopened;
  }

  //----------------------------------------------------------------------------
  bool ButtonController::isSelected() const {
    PIMPL_D (const ButtonController);

    return selected() != -1;
  }

  //----------------------------------------------------------------------------
  int ButtonController::selected() const {
    PIMPL_D (const ButtonController);

    return d->selected;
  }

  //----------------------------------------------------------------------------
  // static
  bool ButtonController::addToRegister (const String &name, ButtonController &controller) {

    if (Private::Register.count (name) == 0) {

      Private::Register.emplace (name, controller);
      controller.d_ptr->name = name;
      return true;
    }
    return false;
  }

  //----------------------------------------------------------------------------
  // static
  ButtonController &ButtonController::getFromRegister (const String &name) {

    return Private::Register.at (name);
  }

  //----------------------------------------------------------------------------
  // static
  bool ButtonController::registerContains (const String &name) {

    return Private::Register.count (name) == 1;
  }

  //----------------------------------------------------------------------------
  void ButtonController::end() {
    PIMPL_D (ButtonController);

    d->isopened = false;
  }

  //----------------------------------------------------------------------------
  const String &ButtonController::name() const {
    PIMPL_D (const ButtonController);

    if (d->name.length() == 0) {
      std::stringstream ss;

      ss << "ButCtrl" << reinterpret_cast<uintptr_t> (this);
      d->name =  ss.str().c_str();
    }
    return d->name;
  }

}
//------------------------------------------------------------------------------
