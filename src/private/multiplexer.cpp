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
#include "multiplexer_p.h"
#include <spaiotdebug.h>

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                         Multiplexer
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // Private implementation

  //----------------------------------------------------------------------------
  Multiplexer::Private::Private () :
    ButtonController::Private (Id), inh (-1)
  {}

  //----------------------------------------------------------------------------
  Multiplexer::Private::Private (const std::initializer_list<int> &spins, int inhPin) :
    ButtonController::Private (Id), spin (spins), inh (inhPin)
  {}

  //----------------------------------------------------------------------------
  Multiplexer::Private::Private (const String &name, const std::initializer_list<int> &spins, int inhPin) :
    ButtonController::Private (Id, name), spin (spins), inh (inhPin)
  {}

  //----------------------------------------------------------------------------
  // Protected constructor with private implementation
  Multiplexer::Multiplexer (Private &dd) : ButtonController (dd)
  {}

  //----------------------------------------------------------------------------
  // API

  //----------------------------------------------------------------------------
  // Default constructor
  // Call the protected constructor with private implementation
  Multiplexer::Multiplexer() :
    Multiplexer (*new Private)
  {}

  //----------------------------------------------------------------------------
  Multiplexer::Multiplexer (const std::initializer_list<int> &spins, int inhPin) :
    Multiplexer (*new Private (spins, inhPin))
  {}

  //----------------------------------------------------------------------------
  Multiplexer::Multiplexer (const String &name, const std::initializer_list<int> &spins, int inhPin) :
    Multiplexer (*new Private (name, spins, inhPin))
  {}

  // ---------------------------------------------------------------------------
  // Copy constructor
  Multiplexer::Multiplexer (const Multiplexer &other) :
    Multiplexer (*new Private (*other.d_func())) {}

  // ---------------------------------------------------------------------------
  // Assignment operator
  Multiplexer &Multiplexer::operator= (const Multiplexer &other) {

    if (this != &other) {

      *d_func() = *other.d_func();
      return *this;
    }
    return *this;
  }

  // ---------------------------------------------------------------------------
  // Move constructor
  Multiplexer::Multiplexer (Multiplexer &&other) = default;

  // ---------------------------------------------------------------------------
  // Move assignment operator
  Multiplexer &Multiplexer::operator= (Multiplexer &&other) = default;


  //----------------------------------------------------------------------------
  void Multiplexer::clear() {

    if (isNull()) {

      reset (new Private);
    }
    else {
      PIMPL_D (Multiplexer);

      end();
      d->spin.clear();
      d->inh = -1;
      ButtonController::clear();
    }
  }

  //----------------------------------------------------------------------------
  bool Multiplexer::isEmpty() const {

    if (! isNull()) {
      PIMPL_D (const Multiplexer);

      return d->spin.size() == 0 && d->inh == -1 && ButtonController::isEmpty();
    }
    return false;
  }

  //----------------------------------------------------------------------------
  bool Multiplexer::operator== (const ButtonController &other) const {

    if (ButtonController::operator== (other)) {
      PIMPL_D (const Multiplexer);

      const Multiplexer &o = static_cast<const Multiplexer &> (other);
      return (d->spin == o.d_func()->spin) && (d->inh == o.d_func()->inh);
    }
    return false;
  }

  //----------------------------------------------------------------------------
  void Multiplexer::begin() {

    if ( (isOpened() == false) && (isNull() == false))  {
      PIMPL_D (Multiplexer);

      SPAIOT_DBG ("Multiplexer::begin(): opening");
      pinMode (d->inh, OUTPUT);
      digitalWrite (d->inh, HIGH);

      for (unsigned int i = 0; i < d->spin.size(); i++) {

        pinMode (d->spin.at (i), OUTPUT);
        digitalWrite (d->spin.at (i), LOW);
      }
      ButtonController::begin();
    }
  }

  //----------------------------------------------------------------------------
  void Multiplexer::end() {
    
    if (isOpened()) {
      PIMPL_D (Multiplexer);

      for (unsigned int i = 0; i < d->spin.size(); i++) {

        pinMode (d->spin.at (i), INPUT_PULLUP);
      }
      ButtonController::end();
    }
  }

  //----------------------------------------------------------------------------
  int Multiplexer::select (int button) {

    if (isOpened() && (button >= 0) && (button <= size())) {
      PIMPL_D (Multiplexer);

      for (unsigned int i = 0; i < d->spin.size(); i++) {

        digitalWrite (d->spin.at (i), (button & (1 << i)) ? HIGH : LOW);

      }
      digitalWrite (d->inh, LOW);
      d->selected = button;;
    }
    return selected();
  }

  //----------------------------------------------------------------------------
  void Multiplexer::deselect () {

    if (isOpened()) {
      PIMPL_D (Multiplexer);

      digitalWrite (d->inh, HIGH);
      d->selected = -1;
    }
  }

  //----------------------------------------------------------------------------
  int Multiplexer::selectPin (int key) const {
    PIMPL_D (const Multiplexer);

    return d->spin.at (key);
  }

  //----------------------------------------------------------------------------
  void Multiplexer::setSelectPin (int key, int pin) {
    PIMPL_D (Multiplexer);

    d->spin[key] = pin;
  }

  //----------------------------------------------------------------------------
  int Multiplexer::size() const {
    PIMPL_D (const Multiplexer);

    return d->spin.size() > 0 ? 1 << d->spin.size() : 0;
  }

  //----------------------------------------------------------------------------
  //
  //                             Multiplexer
  //
  //----------------------------------------------------------------------------
  Multiplexer Scip1Ctrl ("Scip1Ctrl",  {4, 5, 15}, 16);
  Multiplexer Scip2CtrlA ("Scip2CtrlA", {4, 5, 15}, 16);
  Multiplexer Scip2CtrlB ("Scip2CtrlB", {4, 5, 15}, 0);

  std::map<String, ButtonController &>
  ButtonController::Private::Register = {
    {"Scip1Ctrl", Scip1Ctrl},
    {"Scip2CtrlA", Scip2CtrlA},
    {"Scip2CtrlB", Scip2CtrlB}
  };

}
