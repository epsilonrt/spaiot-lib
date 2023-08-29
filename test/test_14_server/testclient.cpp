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
TestClient::Private::Private (TestClient *q) :
  SpaIot::Client::Private ("TestClient", q),
  nextTest (TestInit),
  startTestTime (0),
  powerOnTime (0) {}

//----------------------------------------------------------------------------
TestClient::Private::Private (std::set<SpaIot::Event::Type> subscribedEvents, TestClient *q) :
  Private (q) {

  subscribedEvents = subscribedEvents;
}

//----------------------------------------------------------------------------
void TestClient::Private::sendEventToSpa (Event event, unsigned long timeout) {

  TEST_PRINTF ("%s:%d: Send event %s, time: %d",
               __PRETTY_FUNCTION__, __LINE__, event.toString().c_str(), millis());
  pushToSpa (event);
  if (event.type() == Event::Type::SetDesiredTemp) {
    event.setType (Event::Type::DesiredTemp);
  }
  waiting.sendTime = millis();
  waiting.event = event;
  waiting.timeout = timeout;
  nextTest = TestWaitingResponse;
}


//----------------------------------------------------------------------------
void TestClient::Private::testPowerOn () {
  unsigned long now = millis();

  switch (waiting.testState) {
    case 0:
      waiting.currentTest = TestPowerOn;
      startTestTime = now;
      if (spaValue (Event::Type::PowerOn) == true) {

        sendEventToSpa (Event (Event::Type::PowerOn, false));
        waiting.testState = 1;
        break;
      }
    case 1:
      delay (100);
      sendEventToSpa (Event (Event::Type::PowerOn, true));
      waiting.testState = 2;
      break;
    case 2:
      powerOnTime = now;
      waiting.testState = 3;
      break;
    case 3:
      if (now - startTestTime >= InterTestDelayMs) {

        TEST_PRINTF ("PASS: %s test", TestToString.at (waiting.currentTest).c_str());
        waiting.testState = 0;
        nextTest = TestBubble;
      }
      break;
  }

}

//----------------------------------------------------------------------------
void TestClient::Private::booleanTest (Event::Type type, uint8_t current, uint8_t next, unsigned long timeout) {
  unsigned long now = millis();

  switch (waiting.testState) {
    case 0:
      waiting.currentTest = current;
      startTestTime = now;
      if (spaValue (type) == true) {

        sendEventToSpa (Event (type, false), timeout);
        waiting.testState = 1;
        break;
      }
    case 1:
      delay (100);
      sendEventToSpa (Event (type, true), timeout);
      waiting.testState = 2;
      break;
    case 2:
      delay (100);
      sendEventToSpa (Event (type, false), timeout);
      waiting.testState = 3;
      break;
    case 3:
      if (now - startTestTime >= InterTestDelayMs) {

        TEST_PRINTF ("PASS: %s test", TestToString.at (current).c_str());
        waiting.testState = 0;
        nextTest = next;
      }
      break;
  }

}

//----------------------------------------------------------------------------
void TestClient::Private::testBubble () {

  booleanTest (Event::Type::BubbleOn, TestBubble, TestHeater);
}

//----------------------------------------------------------------------------
void TestClient::Private::testHeater () {

  booleanTest (Event::Type::HeaterOn, TestHeater, TestFilter, 5000);
}

//----------------------------------------------------------------------------
void TestClient::Private::testFilter () {

  booleanTest (Event::Type::FilterOn, TestFilter, TestWaterTemp);
}

//----------------------------------------------------------------------------
void TestClient::Private::testPowerOff () {

  unsigned long now = millis();

  switch (waiting.testState) {
    case 0:
      waiting.currentTest = TestPowerOff;
      startTestTime = now;
      sendEventToSpa (Event (Event::Type::PowerOn, false));
      waiting.testState = 1;
      break;
    case 1:
      if (now - startTestTime >= InterTestDelayMs) {

        TEST_PRINTF ("PASS: %s test", TestToString.at (waiting.currentTest).c_str());
        waiting.testState = 0;
        nextTest = TestEnd;
      }
      break;
  }
}

//----------------------------------------------------------------------------
void TestClient::Private::testWaterTemp () {
  unsigned long now = millis();

  if (spaValue (Event::Type::WaterTemp) != UnsetValue16) {

    TEST_PRINTF ("WaterTemp: %d'C, reading time %d ms", spaValue (Event::Type::WaterTemp), now - powerOnTime);
    TEST_PRINTF ("PASS: %s test", TestToString.at (TestWaterTemp).c_str());
    nextTest = TestGetDesiredTemp;
  }
  else {

    if (now - powerOnTime >= 30000) {

      TEST_FAIL_MESSAGE (TestToString.at (TestWaterTemp).c_str());
      nextTest = TestPowerOff;
    }
  }
}

//----------------------------------------------------------------------------
void TestClient::Private::testGetDesiredTemp () {
  unsigned long now = millis();

  switch (waiting.testState) {
    case 0:
      waiting.currentTest = TestGetDesiredTemp;
      startTestTime = now;
      sendEventToSpa (Event (Event::Type::DesiredTemp, 0), 5000);
      waiting.testState = 1;
      break;
    case 1:
      TEST_PRINTF ("desiredTemp: %d'C, reading time %d ms", spaValue (Event::Type::DesiredTemp), now - startTestTime);
      TEST_ASSERT (spaValue (Event::Type::DesiredTemp) >= DesiredTempMin);
      TEST_ASSERT (spaValue (Event::Type::DesiredTemp) <= DesiredTempMax);
      waiting.testState = 2;
      break;
    case 2:
      if (now - startTestTime >= InterTestDelayMs) {

        TEST_PRINTF ("PASS: %s test", TestToString.at (waiting.currentTest).c_str());
        waiting.testState = 0;
        nextTest = TestSetDesiredTemp;
      }
      break;
  }
}

//----------------------------------------------------------------------------
void TestClient::Private::testSetDesiredTemp () {
  unsigned long now = millis();

  switch (waiting.testState) {
    case 0:
      waiting.currentTest = TestSetDesiredTemp;
      startTestTime = now;
      if (spaValue (Event::Type::DesiredTemp) == (DesiredTempMin + 5)) {

        sendEventToSpa (Event (Event::Type::SetDesiredTemp, DesiredTempMax - 5), 30000);
      }
      else {

        sendEventToSpa (Event (Event::Type::SetDesiredTemp, DesiredTempMin + 5), 30000);
      }
      waiting.testState = 1;
      break;
    case 1:
      TEST_PRINTF ("desiredTemp: %d'C, setting time %d ms", spaValue (Event::Type::DesiredTemp), now - startTestTime);
      waiting.testState = 2;
      break;
    case 2:
      startTestTime = now;
      if (spaValue (Event::Type::DesiredTemp) == (DesiredTempMin + 5)) {

        sendEventToSpa (Event (Event::Type::SetDesiredTemp, DesiredTempMax - 5), 30000);
      }
      else {

        sendEventToSpa (Event (Event::Type::SetDesiredTemp, DesiredTempMin + 5), 30000);
      }
      waiting.testState = 3;
      break;
    case 3:
      TEST_PRINTF ("desiredTemp: %d'C, setting time %d ms", spaValue (Event::Type::DesiredTemp), now - startTestTime);
      waiting.testState = 4;
      break;
    case 4:
      if (now - startTestTime >= InterTestDelayMs) {

        TEST_PRINTF ("PASS: %s test", TestToString.at (waiting.currentTest).c_str());
        waiting.testState = 0;
        nextTest = TestPowerOff;
      }
      break;
  }

}

//----------------------------------------------------------------------------
// Protected constructor with private implementation
TestClient::TestClient (Private &dd) : Client (dd) {}

//----------------------------------------------------------------------------
// API

//----------------------------------------------------------------------------
TestClient::TestClient () :
  TestClient (*new Private (this))
{}

//----------------------------------------------------------------------------
TestClient::TestClient (const std::set <Event::Type> &s) :
  TestClient (*new Private (s, this))
{}

//----------------------------------------------------------------------------
TestClient::TestClient (std::initializer_list <Event::Type> subscribedEvents) :
  TestClient (*new Private (std::set<Event::Type> (subscribedEvents), this))
{}

//----------------------------------------------------------------------------
bool TestClient::handle() {
  PIMPL_D (TestClient);

  unsigned long now = millis();

  d->pollSpaEvents();

  switch (d->nextTest) {
    case Private::TestInit:
      d->nextTest = Private::TestPowerOn;
      break;
    case Private::TestPowerOn:
      d->testPowerOn();
      break;
    case Private::TestWaterTemp:
      d->testWaterTemp();
      break;
    case Private::TestFilter:
      d->testFilter();
      break;
    case Private::TestHeater:
      d->testHeater();
      break;
    case Private::TestBubble:
      d->testBubble();
      break;
    case Private::TestGetDesiredTemp:
      d->testGetDesiredTemp();
      break;
    case Private::TestSetDesiredTemp:
      d->testSetDesiredTemp();
      break;
    case Private::TestPowerOff:
      d->testPowerOff();
      break;
    case Private::TestEnd:
      return false;
      break;
    case Private::TestWaitingResponse:
      if (d->spaValue (d->waiting.event.type()) == d->waiting.event.value() ||
          (d->waiting.event.type() == Event::Type::DesiredTemp && d->spaValue (d->waiting.event.type()) != UnsetValue16)) {

        switch (d->waiting.currentTest) {
          case Private::TestPowerOn:
            d->testPowerOn();
            break;
          case Private::TestWaterTemp:
            d->testWaterTemp();
            break;
          case Private::TestFilter:
            d->testFilter();
            break;
          case Private::TestHeater:
            d->testHeater();
            break;
          case Private::TestBubble:
            d->testBubble();
            break;
          case Private::TestGetDesiredTemp:
            d->testGetDesiredTemp();
            return true;
            break;
          case Private::TestSetDesiredTemp:
            d->testSetDesiredTemp();
            break;
          case Private::TestPowerOff:
            d->testPowerOff();
            break;
          default:
            TEST_FAIL_MESSAGE ("Invalid next test");
            break;
        }
      }
      else if ( (now - d->waiting.sendTime) >= d->waiting.timeout) {

        TEST_PRINTF ("%s:%d: Timeout waiting for event %s, now: %d, sendTime: %d, timeout: %d",
                     __PRETTY_FUNCTION__, __LINE__, d->waiting.event.toString().c_str(), now, d->waiting.sendTime, d->waiting.timeout);

        TEST_FAIL_MESSAGE ("Timeout waiting for event");
      }
      break;
  }
  return true;
}

//----------------------------------------------------------------------------
bool TestClient::begin() {
  PIMPL_D (TestClient);

  d->nextTest = Private::TestInit;
  return Client::begin();
}

//----------------------------------------------------------------------------
// Map to convert test number to string
const std::map <uint8_t, String> TestClient::Private::TestToString = {
  {TestInit, "Init"},
  {TestPowerOn, "PowerOn"},
  {TestWaterTemp, "WaterTemp"},
  {TestGetDesiredTemp, "GetDesiredTemp"},
  {TestSetDesiredTemp, "SetDesiredTemp"},
  {TestFilter, "Filter"},
  {TestHeater, "Heater"},
  {TestBubble, "Bubble"},
  {TestPowerOff, "PowerOff"},
  {TestEnd, "End"},
  {TestWaitingResponse, "WaitingResponse"}
};