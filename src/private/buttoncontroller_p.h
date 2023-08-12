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
#pragma once

#include <buttoncontroller.h>

namespace SpaIot {

  class ButtonController::Private {
    public:
      Private (int typeId);
      Private (int typeId, const String &name);
      bool isSameType (const ButtonController &other) const;

    public:
      int typeId;
      bool isopened;
      int selected;
      mutable String name;
      static std::map<String, ButtonController &> Register;
  };

}
