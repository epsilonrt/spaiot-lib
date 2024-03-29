/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#include "multiplexer_p.h"
#include "../spaiotdebug.h"

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

      d_ptr.reset (new Private);
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

    if ( (isOpen() == false) && (isNull() == false))  {
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

    if (isOpen()) {
      PIMPL_D (Multiplexer);

      for (unsigned int i = 0; i < d->spin.size(); i++) {

        pinMode (d->spin.at (i), INPUT_PULLUP);
      }
      ButtonController::end();
    }
  }

  //----------------------------------------------------------------------------
  int Multiplexer::select (int button) {

    if (isOpen() && (button >= 0) && (button <= size())) {
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

    if (isOpen()) {
      PIMPL_D (Multiplexer);

      digitalWrite (d->inh, HIGH);
      d->selected = -1;
    }
  }

  //----------------------------------------------------------------------------
  int Multiplexer::selectPin (int order) const {
    PIMPL_D (const Multiplexer);

    return d->spin.at (order);
  }

  //----------------------------------------------------------------------------
  void Multiplexer::setSelectPin (int order, int pin) {
    PIMPL_D (Multiplexer);

    d->spin[order] = pin;
  }

  //----------------------------------------------------------------------------
  int Multiplexer::inhPin() const {
    PIMPL_D (const Multiplexer);

    return d->inh;
  }

  //----------------------------------------------------------------------------
  void Multiplexer::setInhPin (int pin) {
    PIMPL_D (Multiplexer);

    d->inh = pin;
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
  Multiplexer Scip2MuxA ("Scip2MuxA", {4, 5, 15}, 16);
  Multiplexer Scip2MuxB ("Scip2MuxB", {4, 5, 15}, 0);
  Multiplexer SpaIot8266MuxA ("SpaIot8266MuxA", {5, 4, 15}, 16); // A->GPIO5, B->GPIO4, C->GPIO15, INH->GPIO16
  Multiplexer SpaIot8266MuxB ("SpaIot8266MuxB", {5, 4, 15}, 0);  // A->GPIO5, B->GPIO4, C->GPIO15, INH->GPIO0
  Multiplexer SpaIot32MuxA ("SpaIot32MuxA", {27, 16, 17}, 33); // A->GPIO27, B->GPIO16, C->GPIO17, INH->GPIO33
  Multiplexer SpaIot32MuxB ("SpaIot32MuxB", {27, 16, 17}, 26); // A->GPIO27, B->GPIO16, C->GPIO17, INH->GPIO26
  Multiplexer SpaIotS3MuxA ("SpaIotS3MuxA", {4, 5, 6}, 7);  // A->GPIO4, B->GPIO5, C->GPIO6, INH->GPIO7
  Multiplexer SpaIotS3MuxB ("SpaIotS3MuxB", {4, 5, 6}, 15); // A->GPIO4, B->GPIO5, C->GPIO6, INH->GPIO15

  std::map<String, ButtonController &>
  ButtonController::Private::Register = {
    {"Scip2MuxA", Scip2MuxA},
    {"Scip2MuxB", Scip2MuxB},
    {"SpaIot8266MuxA", SpaIot8266MuxA},
    {"SpaIot8266MuxB", SpaIot8266MuxB},
    {"SpaIot32MuxA", SpaIot32MuxA},
    {"SpaIot32MuxB", SpaIot32MuxB},
    {"SpaIotS3MuxA", SpaIotS3MuxA},
    {"SpaIotS3MuxB", SpaIotS3MuxB}
  };

}
