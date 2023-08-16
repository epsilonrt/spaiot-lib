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

#include <queue>
#include "../client.h"

namespace SpaIot {

  class Client::Private {
    public:
      Private ();
      Private (std::set<Event::Type> subscribedEvents);
      std::queue<Event> outQ; // out queue, events to be sent to the spa
      std::queue<Event> inQ;  // in queue, events received from the spa
      std::set<Event::Type> subscribedEvents; // events subscribed by the client
  };
}
