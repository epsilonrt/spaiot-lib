//
// Unit Test for the class SpaIot::Event
// How to run:
// pio test -f test_02_event -v
// -v for verbose, if not specified only summary is printed (no TEST_MESSAGE, no TEST_PRINTF)
//
#include <Arduino.h>
#include <unity.h>
#include <event.h>

using namespace SpaIot;

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_constructor_null (void) {
  Event s;

  TEST_ASSERT_TRUE (s.isEmpty());
  TEST_ASSERT_FALSE (s.isNull());
}

void test_constructor_notnull (void) {
  Event s1 (Event::Type::PowerOn, true);
  Event s2 (Event::Type::WaterTemp, 25);

  TEST_ASSERT_FALSE (s1.isEmpty());
  TEST_ASSERT_FALSE (s2.isEmpty());
}

void test_getters (void) {
  Event s1 (Event::Type::PowerOn, true);
  Event s2 (Event::Type::WaterTemp, 25);
  Event s3;
  bool b = s1;
  uint16_t u = s2;

  TEST_ASSERT_EQUAL (s1.value(), true);
  TEST_ASSERT_EQUAL (s1.type(), Event::Type::PowerOn);
  TEST_ASSERT_EQUAL (s2.value(), 25);
  TEST_ASSERT_EQUAL (s2.type(), Event::Type::WaterTemp);

  TEST_ASSERT_EQUAL (b, true);
  TEST_ASSERT_EQUAL (u, 25);
  b = s3;
  u = s3;
  TEST_ASSERT_EQUAL (b, false);
  TEST_ASSERT_EQUAL (u, UnsetValue16);
  TEST_ASSERT_FALSE(s3);
}

void test_setters (void) {
  Event s1;

  s1.setType (Event::Type::PowerOn);
  s1.setValue (true);
  TEST_ASSERT_EQUAL (s1.value(), true);
  TEST_ASSERT_EQUAL (s1.type(), Event::Type::PowerOn);
  s1.clear();
  TEST_ASSERT_TRUE (s1.isEmpty());
}

void test_comparison (void) {
  Event s1 (Event::Type::PowerOn, true);
  Event s2 (Event::Type::PowerOn, true);
  Event s3 (Event::Type::WaterTemp, 25);

  TEST_ASSERT (s1 == s2);
  TEST_ASSERT (s2 != s3);
}

void test_copy (void) {
  Event s1;
  Event s2 (Event::Type::PowerOn, true);
  Event s3 (s2);

  s1 = s2;
  TEST_ASSERT (s2 == s1);
  TEST_ASSERT (s3 == s2);
}

void test_move (void) {
  const Event s1 (Event::Type::PowerOn, true);

  // Test move constructor
  Event s2 = s1;
  Event s3 = std::move (s2);
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

    UNITY_END(); // stop unit testing
    done = true;
  }
}
