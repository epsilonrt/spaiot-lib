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
#include "testclient_p.h"
#include <unity.h>

using namespace SpaIot;

//----------------------------------------------------------------------------
// Private implementation

//----------------------------------------------------------------------------
void TestClient::Private::printEvent (const char *msg, Event e) {
  TEST_PRINTF ("%s: %s, value: %d", msg, EventTypeToString.at (e.type()).c_str(), e.value());
}

//----------------------------------------------------------------------------
// Protected constructor with private implementation
TestClient::TestClient (Private &dd) : SpaClient (dd) {}

//----------------------------------------------------------------------------
// API

//----------------------------------------------------------------------------
TestClient::TestClient () :
  TestClient (*new Private)
{}

//----------------------------------------------------------------------------
TestClient::TestClient (const std::set <Event::Type> &s) :
  TestClient (*new Private (s))
{}

//----------------------------------------------------------------------------
TestClient::TestClient (std::initializer_list <Event::Type> subscribedEvents) :
  TestClient (*new Private (std::set<Event::Type> (subscribedEvents)))
{}

//----------------------------------------------------------------------------
bool TestClient::handle() {
  Event e;
  unsigned eventCount = 0;

  TEST_MESSAGE ("TestClient::handle");
  while (pullFromSpa (e)) {

    Private::printEvent ("pull", e);
    pushToSpa (e);
    Private::printEvent ("push", e);
    eventCount++;
  }
  TEST_PRINTF ("TestClient::handle: %d events", eventCount);
  return eventCount > 0;
}

//----------------------------------------------------------------------------
// Map to convert Event::Type to string
const std::map <Event::Type, String> TestClient::Private::EventTypeToString = {
  {Event::Type::PowerOn, "PowerOn"},
  {Event::Type::FilterOn, "FilterOn"},
  {Event::Type::BubbleOn, "BubbleOn"},
  {Event::Type::JetOn, "JetOn"},
  {Event::Type::SanitizerOn, "SanitizerOn"},
  {Event::Type::HeaterOn, "HeaterOn"},
  {Event::Type::HeatReached, "PumHeatReachedp2"},
  {Event::Type::WaterTemp, "WaterTemp"},
  {Event::Type::DesiredTemp, "DesiredTemp"},
  {Event::Type::SanitizerTime, "SanitizerTime"},
  {Event::Type::ErrorCode, "ErrorCode"},
  {Event::Type::AmbientTemperature, "AmbientTemperature"},
  {Event::Type::NoEvent, "NoEvent"},
  {Event::Type::AnyEvent, "AnyEvent"}
};
