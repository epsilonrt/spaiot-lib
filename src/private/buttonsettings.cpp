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
#include "buttonsettings_p.h"
#include "../spaiotdebug.h"

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                            ButtonSettings
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // Private implementation

  //----------------------------------------------------------------------------
  ButtonSettings::Private::Private () : id (-1), ctrl (nullptr)
  {}

  //----------------------------------------------------------------------------
  ButtonSettings::Private::Private (const String &controllerName, int buttonId) :
    id (buttonId), ctrlName (controllerName), ctrl (nullptr)
  {}

  //----------------------------------------------------------------------------
  ButtonSettings::Private::Private (ButtonController &controller, int buttonId) :
    id (buttonId), ctrlName (controller.name()), ctrl (&controller)
  {}

  //----------------------------------------------------------------------------
  // Protected constructor with private implementation
  ButtonSettings::ButtonSettings (Private &dd) : d_ptr (&dd) {}

  //----------------------------------------------------------------------------
  // API

  //----------------------------------------------------------------------------
  // Default constructor
  // Call the protected constructor with private implementation
  ButtonSettings::ButtonSettings() :
    ButtonSettings (*new Private) {}

  //----------------------------------------------------------------------------
  // Destructor
  // No default implementation, unique_ptr will call the destructor of the
  // private implementation
  ButtonSettings::~ButtonSettings() {}

  //----------------------------------------------------------------------------
  ButtonSettings::ButtonSettings (const String &controllerName, int buttonId) :
    ButtonSettings (*new Private (controllerName, buttonId)) {}

  //----------------------------------------------------------------------------
  ButtonSettings::ButtonSettings (ButtonController &controller, int buttonId) :
    ButtonSettings (*new Private (controller, buttonId)) {}

  // ---------------------------------------------------------------------------
  // Copy constructor
  ButtonSettings::ButtonSettings (const ButtonSettings &other) :
    ButtonSettings (*new Private (*other.d_ptr)) {}

  // ---------------------------------------------------------------------------
  // Assignment operator
  ButtonSettings &ButtonSettings::operator= (const ButtonSettings &other) {

    if (this != &other) {

      *d_ptr = *other.d_ptr;
      return *this;
    }
    return *this;
  }

  // ---------------------------------------------------------------------------
  // Move constructor
  ButtonSettings::ButtonSettings (ButtonSettings &&other) = default;
  // ButtonSettings::ButtonSettings (ButtonSettings &&other) {

  //   d_ptr = std::move (other.d_ptr);
  // }

  // ---------------------------------------------------------------------------
  // Move assignment operator
  ButtonSettings &ButtonSettings::operator= (ButtonSettings &&other) = default;
  // ButtonSettings &ButtonSettings::operator= (ButtonSettings &&other) {

  //   if (this != &other) {

  //     d_ptr = std::move (other.d_ptr);
  //   }
  //   return *this;
  // }

  //----------------------------------------------------------------------------
  bool ButtonSettings::isNull() const {
    if (d_ptr != nullptr) {
      PIMPL_D (const ButtonSettings);

      return ! ( (d->ctrl == nullptr) ?  ButtonController::registerContains (d->ctrlName) : true);
    }
    return true;
  }

  //----------------------------------------------------------------------------
  void ButtonSettings::clear() {

    if (d_ptr == nullptr) {

      d_ptr.reset (new Private);
    }
    else {
      PIMPL_D (ButtonSettings);

      d->id = -1;
      d->ctrlName.clear();
      d->ctrl = nullptr;
    }
  }

  //----------------------------------------------------------------------------
  bool ButtonSettings::operator== (const ButtonSettings &other) const {
    PIMPL_D (const ButtonSettings);

    return d->id == other.d_func()->id &&
           d->ctrlName == other.d_func()->ctrlName &&
           d->ctrl == other.d_func()->ctrl;
  }

  //----------------------------------------------------------------------------
  bool ButtonSettings::operator!= (const ButtonSettings &other) const {

    return ! (*this == other);
  }

  //----------------------------------------------------------------------------
  const ButtonController &ButtonSettings::ctrl() const {
    PIMPL_D (const ButtonSettings);

    if (d->ctrl == nullptr)  {

      if (ButtonController::registerContains (d->ctrlName)) {

        d->ctrl = & ButtonController::getFromRegister (d->ctrlName);
      }
      else {

        SPAIOT_DBG ("%s:%d: <Critical Error> Unable to find the '%s' "
                    "ButtonController in the register, check his name !",
                    __PRETTY_FUNCTION__, __LINE__,
                    d->ctrlName.c_str());
      }
    }
    return *d->ctrl;
  }

  //----------------------------------------------------------------------------
  ButtonController &ButtonSettings::ctrl() {
    // https://riptutorial.com/cplusplus/example/16974/avoiding-duplication-of-code-in-const-and-non-const-getter-methods-
    return const_cast<ButtonController &> (const_cast<const ButtonSettings *> (this)->ctrl());
  }

  //----------------------------------------------------------------------------
  const String &ButtonSettings::controllerName() const {
    PIMPL_D (const ButtonSettings);

    return d->ctrlName;
  }

  //----------------------------------------------------------------------------
  int ButtonSettings::id() const {
    PIMPL_D (const ButtonSettings);

    return d->id;
  }

  //----------------------------------------------------------------------------
  void ButtonSettings::setId (int id) {
    PIMPL_D (ButtonSettings);

    d->id = id;
  }

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

     BUT_SSP |  H |  0 |  C |  U |  0 |  P |  X | BZ |  D |  0 |  0 |  X |  B |  0 |  F |  X |
    ---------------------------------------------------------------------------*/
  const std::map<int, ButtonSettings> Scip2SspButtons = {
    { Filter,   ButtonSettings ("Scip2CtrlA", 1) },
    { Bubble,   ButtonSettings ("Scip2CtrlA", 3) },
    { TempDown, ButtonSettings ("Scip2CtrlA", 7) },

    { Power,    ButtonSettings ("Scip2CtrlB", 2) },
    { TempUp,   ButtonSettings ("Scip2CtrlB", 4) },
    { TempUnit, ButtonSettings ("Scip2CtrlB", 5) },
    { Heater,   ButtonSettings ("Scip2CtrlB", 7) }
  };

  /*         |-------------------------------------------------------------------------------|
             | B7 | B6 | B5 | B4 | B3 | B2 | B1 | B0 | A7 | A6 | A5 | A4 | A3 | A2 | A1 | A0 |
     BIT     | 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |

     BUT_SJB |  H |  0 | *C |  U |  0 |  P |  D | BZ |  F |  0 |  0 |  X | *B |  0 | *J |  S |
   *        * hypothesis that must be verified !
    ---------------------------------------------------------------------------*/
  const std::map<int, ButtonSettings> Scip2SjbButtons = {
    { Sanitizer,  ButtonSettings ("Scip2CtrlA", 0) },
    { Jet,        ButtonSettings ("Scip2CtrlA", 1) }, // this will have to be checked
    { Bubble,     ButtonSettings ("Scip2CtrlA", 3) }, // this will have to be checked
    { Filter,     ButtonSettings ("Scip2CtrlA", 7) },

    { TempDown,   ButtonSettings ("Scip2CtrlB", 1) },
    { Power,      ButtonSettings ("Scip2CtrlB", 2) },
    { TempUp,     ButtonSettings ("Scip2CtrlB", 4) },
    { TempUnit,   ButtonSettings ("Scip2CtrlB", 5) }, // this will have to be checked
    { Heater,     ButtonSettings ("Scip2CtrlB", 7) },
  };
}
//------------------------------------------------------------------------------
