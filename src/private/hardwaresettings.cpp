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
#include "hardwaresettings_p.h"
#include "../type_name.h"


namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                            HardwareSettings
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // Private implementation

  //----------------------------------------------------------------------------
  HardwareSettings::Private::Private () :
    bus (nullptr), leds (nullptr), buttons (nullptr)
  {}

  //----------------------------------------------------------------------------
  HardwareSettings::Private::Private (const BusSettings &bus,
                                      const std::map <int, LedSettings> &leds,
                                      const std::map <int, ButtonSettings> &buttons) :
    bus (& bus), leds (& leds), buttons (&buttons)
  {}

  //----------------------------------------------------------------------------
  // Protected constructor with private implementation
  HardwareSettings::HardwareSettings (Private &dd) : d_ptr (&dd) {}

  //----------------------------------------------------------------------------
  // API

  //----------------------------------------------------------------------------
  // Default constructor
  // Call the protected constructor with private implementation
  HardwareSettings::HardwareSettings() :
    HardwareSettings (*new Private)
  {}

  //----------------------------------------------------------------------------
  // Destructor
  // No default implementation, unique_ptr will call the destructor of the
  // private implementation
  HardwareSettings::~HardwareSettings()
  {}

  //----------------------------------------------------------------------------
  HardwareSettings::HardwareSettings (const BusSettings &bus,
                                      const std::map <int, LedSettings> &leds,
                                      const std::map <int, ButtonSettings> &buttons) :
    HardwareSettings (*new Private (bus, leds, buttons))
  {}

  // ---------------------------------------------------------------------------
  // Copy constructor
  HardwareSettings::HardwareSettings (const HardwareSettings &other) :
    HardwareSettings (*new Private (*other.d_ptr)) {}

  // ---------------------------------------------------------------------------
  // Assignment operator
  HardwareSettings &HardwareSettings::operator= (const HardwareSettings &other) {

    if (this != &other) {

      *d_ptr = *other.d_ptr;
      return *this;
    }
    return *this;
  }

  // ---------------------------------------------------------------------------
  // Move constructor
  HardwareSettings::HardwareSettings (HardwareSettings &&other) = default;
  // HardwareSettings::HardwareSettings (HardwareSettings &&other) {

  //   d_ptr = std::move (other.d_ptr);
  // }

  // ---------------------------------------------------------------------------
  // Move assignment operator
  HardwareSettings &HardwareSettings::operator= (HardwareSettings &&other) = default;
  // HardwareSettings &HardwareSettings::operator= (HardwareSettings &&other) {

  //   if (this != &other) {

  //     d_ptr = std::move (other.d_ptr);
  //   }
  //   return *this;
  // }

  //----------------------------------------------------------------------------
  bool HardwareSettings::isNull() const {

    if (d_ptr != nullptr) {
      PIMPL_D (const HardwareSettings);

      return d->bus == nullptr && d->buttons == nullptr &&  d->leds == nullptr;
    }
    return true;
  }

  //----------------------------------------------------------------------------
  void HardwareSettings::clear() {

    if (d_ptr == nullptr) {

      d_ptr.reset (new Private);
    }
    else {

      d_ptr->bus = nullptr;
      d_ptr->buttons = nullptr;
      d_ptr->leds = nullptr;
    }
  }

  //----------------------------------------------------------------------------
  void HardwareSettings::print (Print &out) const {

    out.println (type_name (bus()));
    out.printf ("\t{%d,%d,%d}\n", bus().dataPin(), bus().clkPin(), bus().holdPin());

    out.println (type_name (leds().cbegin()->second));
    for (const auto &elmt : leds()) {
      int key = elmt.first;
      const LedSettings &led = elmt.second;

      out.printf ("\t{%d,%d}\n", key, led.order());
    }

    out.println (type_name (buttons().cbegin()->second));
    for (const auto &elmt : buttons()) {
      int key = elmt.first;
      const ButtonSettings &but = elmt.second;
      out.printf ("\t{%d,%s,%d}\n", key, but.controllerName().c_str(), but.id());
    }
    out.printf ("--------------------------------------------------------------------------------\n\n");
  }

  //----------------------------------------------------------------------------
  const BusSettings &HardwareSettings::bus() const {
    PIMPL_D (const HardwareSettings);

    return *d->bus;
  }

  //----------------------------------------------------------------------------
  const std::map <int, LedSettings> &HardwareSettings::leds() const {
    PIMPL_D (const HardwareSettings);

    return *d->leds;
  }

  //----------------------------------------------------------------------------
  const std::map <int, ButtonSettings> &HardwareSettings::buttons() const {
    PIMPL_D (const HardwareSettings);

    return *d->buttons;
  }

  //----------------------------------------------------------------------------
  bool HardwareSettings::operator== (const HardwareSettings &other) const {

    return bus() == other.bus() &&
           leds() == other.leds() &&
           buttons() == other.buttons();
  }

  //----------------------------------------------------------------------------
  bool HardwareSettings::operator!= (const HardwareSettings &other) const {

    return ! (*this == other);
  }

  //----------------------------------------------------------------------------
  // static
  bool HardwareSettings::addToRegister (const String &name, const HardwareSettings &settings) {

    if (Private::Register.count (name) == 0) {

      Private::Register.emplace (name, & settings);
      return true;
    }
    return false;
  }

  //----------------------------------------------------------------------------
  // static
  const HardwareSettings &HardwareSettings::getFromRegister (const String &name) {

    return * Private::Register.at (name);
  }

  //----------------------------------------------------------------------------
  // static
  bool HardwareSettings::registerContains (const String &name) {

    return Private::Register.count (name) == 1;
  }

  //----------------------------------------------------------------------------
  //
  //                            Settings
  //
  //----------------------------------------------------------------------------
  const HardwareSettings Scip2Ssp (ScipBus, SspLeds, Scip2SspButtons);
  const HardwareSettings Scip2Sjb (ScipBus, SjbLeds, Scip2SjbButtons);

  /*
     WARNING
     If you add a control panel setting here, you must update the 'model'
     drop-down list (<select id="model">) in the home.htm file
  */
  std::map<String, const HardwareSettings *> HardwareSettings::Private::Register {
    {"SCIP2SSP", &Scip2Ssp},
    {"SCIP2SJB", &Scip2Sjb}
  };
}
//------------------------------------------------------------------------------
