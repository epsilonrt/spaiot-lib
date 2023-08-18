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

#include <map>
#include "../event.h"
#include "../server.h"
#include "controlpanel_p.h"

namespace SpaIot {

  class Server::Private : public ControlPanel::Private {
    public:
      Private (Server *q);
      Private (Server *q, const HardwareSettings &hwsettings);

      void sendEvent (const Event &event);
      bool sendState (const Event &event);
      bool sendValue (const Event &event);

      bool setPreviousState (Event::Type type, uint8_t value);
      bool setPreviousValue (Event::Type type, uint16_t value);

      uint8_t previousState (Event::Type type);
      uint16_t previousValue (Event::Type type);

      bool setPreviousState (const Event &event);
      uint8_t previousState (const Event &event);
      bool setPreviousValue (const Event &event);
      uint16_t previousValue (const Event &event);

    public:
      std::map<String, SpaIot::Client *> clients;
      const ServerSettings *settings;
      unsigned long previousPublishTime;
      std::map<Event::Type, uint8_t> previousStates;
      std::map<Event::Type, uint16_t> previousValues;

      static const std::map<Event::Type, uint8_t>  PreviousStatesDefault;
      static const std::map<Event::Type, uint16_t> PreviousValuesDefault;
    private:
      PIMPL_DECLARE_PUBLIC (Server);
  };
}
