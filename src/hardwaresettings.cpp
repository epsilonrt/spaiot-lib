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
#include <hardwaresettings.h>
#include <type_name.h>


namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                            HardwareSettings
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  HardwareSettings::HardwareSettings () :
    m_bus (nullptr), m_leds (nullptr), m_buttons (nullptr) {

  }

  //----------------------------------------------------------------------------
  HardwareSettings::HardwareSettings (const BusSettings & bus,
                                      const std::map <int, LedSettings> & leds,
                                      const std::map <int, ButtonSettings> & buttons) :
    m_bus (& bus), m_leds (& leds), m_buttons (&buttons) {

  }

  //----------------------------------------------------------------------------
  bool HardwareSettings::isNull() const {

    return m_bus == nullptr && m_buttons == nullptr &&  m_leds == nullptr;
  }

#ifndef NODEBUG_SPAIOT
  //----------------------------------------------------------------------------
  void HardwareSettings::print (Print &out) const {

    out.println (type_name (bus()));
    out.printf ("\t{%d,%d,%d}\n", bus().dataPin(), bus().clkPin(), bus().holdPin());

    out.println (type_name (leds().cbegin()->second));
    for (const auto & elmt : leds()) {
      int key = elmt.first;
      const LedSettings & led = elmt.second;
      
      out.printf ("\t{%d,%d}\n", key, led.order());
    }

    out.println (type_name (buttons().cbegin()->second));
    for (const auto & elmt : buttons()) {
      int key = elmt.first;
      const ButtonSettings & but = elmt.second;
      out.printf ("\t{%d,%s,%d}\n", key, but.controllerName().c_str(), but.id());
    }
    out.printf ("--------------------------------------------------------------------------------\n\n");
  }
#endif

  //----------------------------------------------------------------------------
  const BusSettings & HardwareSettings::bus() const {

    return *m_bus;
  }

  //----------------------------------------------------------------------------
  const std::map <int, LedSettings> & HardwareSettings::leds() const {

    return *m_leds;
  }

  //----------------------------------------------------------------------------
  const std::map <int, ButtonSettings> & HardwareSettings::buttons() const {

    return *m_buttons;
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
  bool HardwareSettings::addToRegister (const std::string & name, const HardwareSettings & settings) {

    if (Register.count (name) == 0) {

      Register.emplace (name, & settings);
      return true;
    }
    return false;
  }

  //----------------------------------------------------------------------------
  // static
  const HardwareSettings & HardwareSettings::getFromRegister (const std::string & name) {

    return * Register.at (name);
  }

  //----------------------------------------------------------------------------
  // static
  bool HardwareSettings::registerContains (const std::string & name) {
    
    return Register.count (name) == 1;
  }

  //----------------------------------------------------------------------------
  //
  //                            Settings
  //
  //----------------------------------------------------------------------------
  const HardwareSettings Scip2Ssp (ScipBus, SspLeds, Scip2SspButtons);
  const HardwareSettings Scip2Sjb (ScipBus, SjbLeds, Scip2SjbButtons);

  /*
   * WARNING
   * If you add a control panel setting here, you must update the 'model'
   * drop-down list (<select id="model">) in the home.htm file
   */
  std::map<std::string, const HardwareSettings *> HardwareSettings::Register {
    {"SCIP2SSP", &Scip2Ssp},
    {"SCIP2SJB", &Scip2Sjb}
  };
}
//------------------------------------------------------------------------------
