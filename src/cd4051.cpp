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
#include <cd4051.h>

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                             Cd4051
  //
  //----------------------------------------------------------------------------
  Cd4051 Scip1Ctrl (4, 5, 15, 16);
  Cd4051 Scip2CtrlA (5, 4, 15, 16);
  Cd4051 Scip2CtrlB (5, 4, 15, 0);

  std::map<std::string, ButtonController &>
  ButtonController::Register = {
    {"Scip1Ctrl", Scip1Ctrl},
    {"Scip2CtrlA", Scip2CtrlA},
    {"Scip2CtrlB", Scip2CtrlB}
  };
}
//------------------------------------------------------------------------------
