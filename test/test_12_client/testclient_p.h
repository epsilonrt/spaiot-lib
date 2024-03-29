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
#include <private/spaclient_p.h>
#include "testclient.h"

class TestClient::Private : public SpaClient::Private {
  public:
    Private () : SpaClient::Private ("TestClient") {}
    Private (std::set<SpaIot::Event::Type> subscribedEvents) :
      SpaClient::Private ("TestClient", subscribedEvents) {}
    static void printEvent (const char *msg, SpaIot::Event e);
    static const std::map <SpaIot::Event::Type, String> EventTypeToString;
};