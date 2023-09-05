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
#include <private/client_p.h>
#include "testclient.h"

class TestClient::Private : public SpaIot::SpaClient::Private {
  public:
    Private (TestClient *q);
    Private (std::set<SpaIot::Event::Type> subscribedEvents, TestClient *q);

    void sendEventToSpa (SpaIot::Event event, unsigned long timeout = 1000);
    void booleanTest (SpaIot::Event::Type type, uint8_t currentTest, uint8_t nextTest, unsigned long timeout = 1000);

    void testPowerOn ();
    void testFilter ();
    void testBubble ();
    void testHeater ();
    void testPowerOff ();

    void testWaterTemp ();
    void testGetDesiredTemp ();
    void testSetDesiredTemp ();

    enum {
      TestInit = 0,
      TestPowerOn,
      TestWaterTemp,
      TestGetDesiredTemp,
      TestSetDesiredTemp,
      TestFilter,
      TestHeater,
      TestBubble,
      TestPowerOff,
      TestEnd,
      TestWaitingResponse
    };

    struct DataWaiting {
      SpaIot::Event event;
      unsigned long timeout;
      unsigned long sendTime;
      uint8_t currentTest;
      uint8_t testState;
      DataWaiting () : timeout (1000), sendTime (0), currentTest(TestInit), testState (0) {}
    };

    uint8_t nextTest;
    DataWaiting waiting;
    unsigned long startTestTime;
    unsigned long powerOnTime;

    static const std::map <uint8_t, String> TestToString;
    static const unsigned long InterTestDelayMs = 5000;

  private:
    PIMPL_DECLARE_PUBLIC (TestClient);
};