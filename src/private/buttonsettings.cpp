/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

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

  // ---------------------------------------------------------------------------
  // Move assignment operator
  ButtonSettings &ButtonSettings::operator= (ButtonSettings &&other) = default;

  //----------------------------------------------------------------------------
  bool ButtonSettings::isNull() const {

    return d_ptr == nullptr;
  }

  //----------------------------------------------------------------------------
  bool ButtonSettings::isEmpty() const {

    if (!isNull()) {
      PIMPL_D (const ButtonSettings);

      return ! ( (d->ctrl == nullptr) ?  ButtonController::registerContains (d->ctrlName) : true);
    }
    return false;
  }

  //----------------------------------------------------------------------------
  void ButtonSettings::clear() {

    if (isNull()) {
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
    { Filter,   ButtonSettings ("Scip2MuxA", 1) },
    { Bubble,   ButtonSettings ("Scip2MuxA", 3) },
    { TempDown, ButtonSettings ("Scip2MuxA", 7) },

    { Power,    ButtonSettings ("Scip2MuxB", 2) },
    { TempUp,   ButtonSettings ("Scip2MuxB", 4) },
    { TempUnit, ButtonSettings ("Scip2MuxB", 5) },
    { Heater,   ButtonSettings ("Scip2MuxB", 7) }
  };
  const std::map<int, ButtonSettings> SpaIot8266SspButtons = {
    { Filter,   ButtonSettings ("SpaIot8266MuxA", 1) },
    { Bubble,   ButtonSettings ("SpaIot8266MuxA", 3) },
    { TempDown, ButtonSettings ("SpaIot8266MuxA", 7) },

    { Power,    ButtonSettings ("SpaIot8266MuxB", 2) },
    { TempUp,   ButtonSettings ("SpaIot8266MuxB", 4) },
    { TempUnit, ButtonSettings ("SpaIot8266MuxB", 5) },
    { Heater,   ButtonSettings ("SpaIot8266MuxB", 7) }
  };
  const std::map<int, ButtonSettings> SpaIot32SspButtons = {
    { Filter,   ButtonSettings ("SpaIot32MuxA", 1) },
    { Bubble,   ButtonSettings ("SpaIot32MuxA", 3) },
    { TempDown, ButtonSettings ("SpaIot32MuxA", 7) },

    { Power,    ButtonSettings ("SpaIot32MuxB", 2) },
    { TempUp,   ButtonSettings ("SpaIot32MuxB", 4) },
    { TempUnit, ButtonSettings ("SpaIot32MuxB", 5) },
    { Heater,   ButtonSettings ("SpaIot32MuxB", 7) }
  };
  const std::map<int, ButtonSettings> SpaIotS3SspButtons = {
    { Filter,   ButtonSettings ("SpaIotS3MuxA", 1) },
    { Bubble,   ButtonSettings ("SpaIotS3MuxA", 3) },
    { TempDown, ButtonSettings ("SpaIotS3MuxA", 7) },

    { Power,    ButtonSettings ("SpaIotS3MuxB", 2) },
    { TempUp,   ButtonSettings ("SpaIotS3MuxB", 4) },
    { TempUnit, ButtonSettings ("SpaIotS3MuxB", 5) },
    { Heater,   ButtonSettings ("SpaIotS3MuxB", 7) }
  };
  const std::map<int, ButtonSettings> SpaIot8574SspButtons = {
    { Filter,   ButtonSettings ("SpaIotPcf8574Mux", 16 + 1) },
    { Bubble,   ButtonSettings ("SpaIotPcf8574Mux", 16 + 3) },
    { TempDown, ButtonSettings ("SpaIotPcf8574Mux", 16 + 7) },

    { Power,    ButtonSettings ("SpaIotPcf8574Mux", 8 + 2) },
    { TempUp,   ButtonSettings ("SpaIotPcf8574Mux", 8 + 4) },
    { TempUnit, ButtonSettings ("SpaIotPcf8574Mux", 8 + 5) },
    { Heater,   ButtonSettings ("SpaIotPcf8574Mux", 8 + 7) }
  };

  /*         |-------------------------------------------------------------------------------|
             | B7 | B6 | B5 | B4 | B3 | B2 | B1 | B0 | A7 | A6 | A5 | A4 | A3 | A2 | A1 | A0 |
     BIT     | 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |

     BUT_SJB |  H |  0 | *C |  U |  0 |  P |  D | BZ |  F |  0 |  0 |  X | *B |  0 | *J |  S |
   *        * hypothesis that must be verified !
    ---------------------------------------------------------------------------*/
  const std::map<int, ButtonSettings> Scip2SjbButtons = {
    { Sanitizer,  ButtonSettings ("Scip2MuxA", 0) },
    { Jet,        ButtonSettings ("Scip2MuxA", 1) }, // this will have to be checked
    { Bubble,     ButtonSettings ("Scip2MuxA", 3) }, // this will have to be checked
    { Filter,     ButtonSettings ("Scip2MuxA", 7) },

    { TempDown,   ButtonSettings ("Scip2MuxB", 1) },
    { Power,      ButtonSettings ("Scip2MuxB", 2) },
    { TempUp,     ButtonSettings ("Scip2MuxB", 4) },
    { TempUnit,   ButtonSettings ("Scip2MuxB", 5) }, // this will have to be checked
    { Heater,     ButtonSettings ("Scip2MuxB", 7) },
  };
  const std::map<int, ButtonSettings> SpaIot8266SjbButtons = {
    { Sanitizer,  ButtonSettings ("SpaIot8266MuxA", 0) },
    { Jet,        ButtonSettings ("SpaIot8266MuxA", 1) }, // this will have to be checked
    { Bubble,     ButtonSettings ("SpaIot8266MuxA", 3) }, // this will have to be checked
    { Filter,     ButtonSettings ("SpaIot8266MuxA", 7) },

    { TempDown,   ButtonSettings ("SpaIot8266MuxB", 1) },
    { Power,      ButtonSettings ("SpaIot8266MuxB", 2) },
    { TempUp,     ButtonSettings ("SpaIot8266MuxB", 4) },
    { TempUnit,   ButtonSettings ("SpaIot8266MuxB", 5) }, // this will have to be checked
    { Heater,     ButtonSettings ("SpaIot8266MuxB", 7) },
  };
  const std::map<int, ButtonSettings> SpaIot32SjbButtons = {
    { Sanitizer,  ButtonSettings ("SpaIot32MuxA", 0) },
    { Jet,        ButtonSettings ("SpaIot32MuxA", 1) }, // this will have to be checked
    { Bubble,     ButtonSettings ("SpaIot32MuxA", 3) }, // this will have to be checked
    { Filter,     ButtonSettings ("SpaIot32MuxA", 7) },

    { TempDown,   ButtonSettings ("SpaIot32MuxB", 1) },
    { Power,      ButtonSettings ("SpaIot32MuxB", 2) },
    { TempUp,     ButtonSettings ("SpaIot32MuxB", 4) },
    { TempUnit,   ButtonSettings ("SpaIot32MuxB", 5) }, // this will have to be checked
    { Heater,     ButtonSettings ("SpaIot32MuxB", 7) },
  };
  const std::map<int, ButtonSettings> SpaIotS3SjbButtons = {
    { Sanitizer,  ButtonSettings ("SpaIotS3MuxA", 0) },
    { Jet,        ButtonSettings ("SpaIotS3MuxA", 1) }, // this will have to be checked
    { Bubble,     ButtonSettings ("SpaIotS3MuxA", 3) }, // this will have to be checked
    { Filter,     ButtonSettings ("SpaIotS3MuxA", 7) },

    { TempDown,   ButtonSettings ("SpaIotS3MuxB", 1) },
    { Power,      ButtonSettings ("SpaIotS3MuxB", 2) },
    { TempUp,     ButtonSettings ("SpaIotS3MuxB", 4) },
    { TempUnit,   ButtonSettings ("SpaIotS3MuxB", 5) }, // this will have to be checked
    { Heater,     ButtonSettings ("SpaIotS3MuxB", 7) },
  };
  const std::map<int, ButtonSettings> SpaIot8574SjbButtons = {
    { Sanitizer,  ButtonSettings ("SpaIotPcf8574Mux", 16 + 0) },
    { Jet,        ButtonSettings ("SpaIotPcf8574Mux", 16 + 1) }, // this will have to be checked
    { Bubble,     ButtonSettings ("SpaIotPcf8574Mux", 16 + 3) }, // this will have to be checked
    { Filter,     ButtonSettings ("SpaIotPcf8574Mux", 16 + 7) },

    { TempDown,   ButtonSettings ("SpaIotPcf8574Mux", 8 + 1) },
    { Power,      ButtonSettings ("SpaIotPcf8574Mux", 8 + 2) },
    { TempUp,     ButtonSettings ("SpaIotPcf8574Mux", 8 + 4) },
    { TempUnit,   ButtonSettings ("SpaIotPcf8574Mux", 8 + 5) }, // this will have to be checked
    { Heater,     ButtonSettings ("SpaIotPcf8574Mux", 8 + 7) },
  };
}
//------------------------------------------------------------------------------
