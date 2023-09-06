/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#include <queue>
#include <map>
#include "../spaclient.h"

namespace SpaIot {

  class SpaClient::Private {
    public:
      Private (const char *className, SpaClient *q = nullptr);
      Private (const char *className, const std::set<Event::Type> &subscribedEvents, SpaClient *q = nullptr);
      virtual ~Private() {}

      void pushToSpa (const Event &event);
      bool pullFromSpa (Event &event);
      uint16_t spaValue (Event::Type type) const;
      virtual bool pollSpaEvents ();

      std::queue<Event> outQ; // out queue, events to be sent to the spa
      std::queue<Event> inQ;  // in queue, events received from the spa
      std::set<Event::Type> subscribedEvents; // events subscribed by the client
      const String className;
      SpaClient *const q_ptr;
      bool isopen;
      std::map <SpaIot::Event::Type, uint16_t> spaValues;
  };
}
