//
// Unit Test for the class SpaIot::SpaIot::Client
// How to run:
// pio test -f test_12_client -v
// -v for verbose, if not specified only summary is printed (no TEST_MESSAGE, no TEST_PRINTF)
//
#include <Arduino.h>
#include <vector>
#include <unity.h>
#include "testclient.h"

using namespace SpaIot;

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

const std::initializer_list <Event::Type> subscriptionList1 = {Event::Type::PowerOn, Event::Type::WaterTemp};
const std::set <Event::Type> subscriptionSet1 (subscriptionList1);
const std::initializer_list <Event::Type> subscriptionList2 = {Event::Type::PowerOn, Event::Type::WaterTemp, Event::Type::ErrorCode};
const std::set <Event::Type> subscriptionSet2 (subscriptionList2);

void test_constructor_null (void) {
  SpaIot::Client s; // Client is a class defined in the ESP32 core, so we need to disambiguate with the SpaIot namespace

  TEST_ASSERT_TRUE (s.isEmpty());
  TEST_ASSERT_FALSE (s.isNull());
  TEST_ASSERT_EQUAL_STRING (s.className().c_str(), "Client");
}

void test_constructor_notnull (void) {
  SpaIot::Client s1 (subscriptionList1);
  SpaIot::Client s2 (subscriptionSet1);

  TEST_ASSERT_FALSE (s1.isEmpty());
  TEST_ASSERT_FALSE (s2.isEmpty());
}

void test_getters (void) {
  SpaIot::Client s1 ;
  SpaIot::Client s2 (subscriptionList2);

  TEST_ASSERT_TRUE (s1.isEmpty());
  TEST_ASSERT_TRUE (s1.subscribedEvents().empty());
  TEST_ASSERT_TRUE (s1.isSubscribed (Event::Type::PowerOn)); // subscription is empty, so any event is subscribed

  TEST_ASSERT_FALSE (s2.isEmpty());
  TEST_ASSERT_TRUE (s2.isSubscribed (Event::Type::PowerOn));
  TEST_ASSERT_TRUE (s2.subscribedEvents() == subscriptionSet2);
  for (auto t : subscriptionSet2) {
    TEST_ASSERT_TRUE (s2.isSubscribed (Event::Type::PowerOn));
  }

  TEST_ASSERT_FALSE (s1.available());
}

void test_setters (void) {
  SpaIot::Client s1;

  s1.subscribe (Event::Type::PowerOn);
  TEST_ASSERT_TRUE (s1.isSubscribed (Event::Type::PowerOn));

  s1.subscribe (subscriptionSet2);
  TEST_ASSERT_TRUE (s1.subscribedEvents() == subscriptionSet2);

  s1.subscribe (subscriptionList2);
  TEST_ASSERT_TRUE (s1.subscribedEvents() == subscriptionSet2);

  s1.clear();
  TEST_ASSERT_TRUE (s1.isEmpty());
}

void test_comparison (void) {
  SpaIot::Client s1 (subscriptionList1);
  SpaIot::Client s2 (subscriptionList1);
  SpaIot::Client s3 (subscriptionList2);

  TEST_ASSERT (s1 == s2);
  TEST_ASSERT (s2 != s3);
}

void test_copy (void) {
  SpaIot::Client s1;
  SpaIot::Client s2 (subscriptionList1);
  SpaIot::Client s3 (s2);

  s1 = s2;
  TEST_ASSERT (s2 == s1);
  TEST_ASSERT (s3 == s2);
}

void test_move (void) {
  const SpaIot::Client s1 (subscriptionList1);

  // Test move constructor
  SpaIot::Client s2 = s1;
  SpaIot::Client s3 = std::move (s2);
  TEST_ASSERT (s3 == s1);
  TEST_ASSERT_TRUE (s2.isNull());

  // Test move assignment
  s2 = std::move (s3);
  TEST_ASSERT (s2 == s1);
  TEST_ASSERT_TRUE (s3.isNull());

  s3.clear(); // check if clear() reset the d_ptr instance
  s3 = s1;
  TEST_ASSERT (s3 == s1);
}

const std::vector <Event> sequence1 = {
  Event (Event::Type::PowerOn, true),
  Event (Event::Type::DesiredTemp, 25),
  Event (Event::Type::ErrorCode, 0)
};

const std::vector <Event> sequence2 = {
  Event (Event::Type::PowerOn, true),
  Event (Event::Type::WaterTemp, 25),
  Event (Event::Type::ErrorCode, 0),
  Event (Event::Type::PowerOn, false)
};


void test_write_read (void) {
  TestClient s1;
  TestClient s2 (subscriptionList2);
  //  subscriptionList2 = {Event::Type::PowerOn, Event::Type::WaterTemp, Event::Type::ErrorCode};

  TEST_ASSERT_EQUAL_STRING (s1.className().c_str(), "TestClient");
  
  Event ev;
  TEST_ASSERT_FALSE (s2.write (ev)); // type is NoEvent
  ev.setType (Event::Type::AnyEvent);
  TEST_ASSERT_FALSE (s2.write (ev));
  ev.setType (Event::Type::DesiredTemp); // this event is not subscribed
  TEST_ASSERT_FALSE (s2.write (ev));

  ev = s2.read(); // no events available
  TEST_ASSERT_EQUAL (ev.type(), Event::Type::NoEvent);

  ev.setType (Event::Type::PowerOn);
  ev.setValue (true);
  TEST_ASSERT_TRUE (s2.write (ev));
  s2.handle(); // all events write are returned by read
  ev = s2.read (Event::Type::WaterTemp);
  TEST_ASSERT_EQUAL (ev.type(), Event::Type::NoEvent);
  ev = s2.read (Event::Type::PowerOn);
  TEST_ASSERT_EQUAL (ev.type(), Event::Type::PowerOn);
  TEST_ASSERT_EQUAL (ev.value(), true);
  TEST_ASSERT_FALSE (s2.available());

  for (auto e : sequence2) {

    s1.write (e);
    s2.write (e);
  }

  s1.handle();
  s2.handle();

  TEST_ASSERT_TRUE (s1.available());
  TEST_ASSERT_TRUE (s2.available());

  for (auto e : sequence2) {
    Event e1, e2;

    e1 = s1.read ();
    TEST_ASSERT_TRUE (e1 == e);

    e2 = s2.read ();
    TEST_ASSERT_TRUE (e2 == e);
  }

  TEST_ASSERT_FALSE (s1.available());
  TEST_ASSERT_FALSE (s2.available());

  // void Client::write (const Event &event) {

  //   if (event.type() != Event::NoEvent && event.type() != Event::AnyEvent) {
  //     PIMPL_D (Client);

  //     if (d->subscribedEvents.empty() || d->subscribedEvents.count (event.type()) > 0) {

  //       d->inQ.push (event);
  //     }
  //   }
  // }

}


void setup() {

  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay (2000);

  UNITY_BEGIN();    // IMPORTANT LINE!
}

bool done = false;

void loop() {

  if (!done) {

    RUN_TEST (test_constructor_null);
    RUN_TEST (test_constructor_notnull);
    RUN_TEST (test_getters);
    RUN_TEST (test_setters);
    RUN_TEST (test_comparison);
    RUN_TEST (test_copy);
    RUN_TEST (test_move);
    RUN_TEST (test_write_read);

    UNITY_END(); // stop unit testing
    done = true;
  }
}
