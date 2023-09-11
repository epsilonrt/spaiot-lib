//
// Unit Test for the SpaServer class 
//
// How to run:
// pio test -f test_14_server -v
// -v for verbose, if not specified only summary is printed (no TEST_MESSAGE, no TEST_PRINTF)
// We use a DIY board connected to a SSP spa.
#include <unity.h>
#include <spaserver.h>
#include <spaiotdebug.h>
#include "testclient.h"
#include "MyBoardSettings.h"

using namespace SpaIot;

// We use the default configuration SpaConfig defined in MyBoardSettings.h

SpaServer server; // the server, SpaIot:: is needed because there is a SpaServer class in the Arduino framework
ServerSettings srvsettings ("TestSpa", 30000); // the server settings, TestSpa is the name of settings added to the register in setup()
TestClient client; // the client, process all tests in handle()

void setUp (void) {

}

// void tearDown(void) {
// // clean stuff up here
// }


void test_getters () {

  TEST_ASSERT_FALSE (server.isOpen());
  TEST_ASSERT_EQUAL (server.clientCount(), 0);
}

void test_setters () {
  SpaIot::SpaClient client2;

  TEST_ASSERT (client.className() == "TestClient");
  TEST_ASSERT (client2.className() == "SpaClient");

  TEST_ASSERT_TRUE (server.addClient (client));
  TEST_ASSERT_EQUAL (server.client("TestClient"), &client);

  TEST_ASSERT_TRUE (server.addClient (client2));
  TEST_ASSERT_EQUAL (server.client("SpaClient"), &client2);

  TEST_ASSERT_EQUAL (server.clientCount(), 2);

  TEST_ASSERT_FALSE (server.addClient (client2));
  TEST_ASSERT_TRUE (server.removeClient (client2));
  TEST_ASSERT_EQUAL (server.clientCount(), 1);

  TEST_ASSERT_TRUE (server.addClient (client2));
  TEST_ASSERT_TRUE (server.removeClient ("SpaClient"));
  TEST_ASSERT_EQUAL (server.clientCount(), 1);
}

void test_client_server () {

  TEST_ASSERT_EQUAL (server.client("TestClient"), &client);
  TEST_ASSERT_TRUE (client.begin ());
  TEST_ASSERT_TRUE (client.isOpen ());

  TEST_ASSERT_TRUE (server.begin (srvsettings));
  TEST_ASSERT_TRUE (server.isOpen ());

  TEST_ASSERT (*server.settings() == srvsettings);

  while (server.handle()) {
    // All tests are done in TestClient::handle(), called by server.handle()
  }

}

void setup() {
  SPAIOT_DBGLED_INIT();
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay (2000);
  Wire.begin();

  HardwareSettings::addToRegister (srvsettings.spaModel(), SpaConfig);
  UNITY_BEGIN();    // IMPORTANT LINE!

  RUN_TEST (test_getters);
  RUN_TEST (test_setters);

  RUN_TEST (test_client_server);
  delay (5000);

  UNITY_END(); // stop unit testing
}


void loop() {
  // nothing to be done here.
  // All tests are done in setup()
}
