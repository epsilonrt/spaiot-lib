/**
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
#include <buttonsettings.h>

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                            ButtonSettings
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  ButtonSettings::ButtonSettings () :
    m_id (-1)
  {}

  //----------------------------------------------------------------------------
  ButtonSettings::ButtonSettings (const std::string & controllerName, int buttonId) :
    m_id (buttonId), m_controllerName (controllerName)
  {}

  //----------------------------------------------------------------------------
  const std::string & ButtonSettings::controllerName() const {

    return m_controllerName;
  }

  //----------------------------------------------------------------------------
  void ButtonSettings::setControllerName (const std::string & name) {

    m_controllerName = name;
  }

  //----------------------------------------------------------------------------
  int ButtonSettings::id() const {

    return m_id;
  }

  //----------------------------------------------------------------------------
  void ButtonSettings::setId (int id) {

    m_id = id;
  }

  //----------------------------------------------------------------------------
  bool ButtonSettings::operator== (const ButtonSettings &other) const {

    return m_id == other.m_id &&
           m_controllerName == other.m_controllerName;
  }

  //----------------------------------------------------------------------------
  bool ButtonSettings::operator!= (const ButtonSettings &other) const {

    return ! (*this == other);
  }

  //----------------------------------------------------------------------------
  bool ButtonSettings::isNull() const {

    return m_controllerName.empty() || m_id == -1;
  }

  /*
   * POWER        = P
   * FILTER       = F
   * HEATER       = H
   * HEATER_RED   = HR
   * HEATER_GREEN = HG
   * BUBBLE       = B
   * JET          = J
   * SANITIZER    = S
   * DOWN         = D
   * UP           = U
   * FC           = C
   *
   *         |-------------------------------------------------------------------------------|
   *         | B7 | B6 | B5 | B4 | B3 | B2 | B1 | B0 | A7 | A6 | A5 | A4 | A3 | A2 | A1 | A0 |
   * BIT     | 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
   *
   * BUT_SSP |  H |  0 |  C |  U |  0 |  P |  X | BZ |  D |  0 |  0 |  X |  B |  0 |  F |  X |
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
   *         | B7 | B6 | B5 | B4 | B3 | B2 | B1 | B0 | A7 | A6 | A5 | A4 | A3 | A2 | A1 | A0 |
   * BIT     | 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
   *
   * BUT_SJB |  H |  0 | *C |  U |  0 |  P |  D | BZ |  F |  0 |  0 |  X | *B |  0 | *J |  S |
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
