//
// Unit Test for the class SpaIot::BusSettings
// How to run:
// pio test -f test_02_bussettings -v
// -v for verbose, if not specified only summary is printed (no TEST_MESSAGE, no TEST_PRINTF)
//
#include <Arduino.h>
#include <unity.h>
#include <bussettings.h>
#include "MyBoardSettings.h"

using namespace SpaIot;

const int dataPin = SpaBus.dataPin();;
const int clkPin = SpaBus.clkPin();;
const int holdPin = SpaBus.holdPin();;

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_constructor_null (void) {
  BusSettings s;

  TEST_ASSERT_TRUE (s.isEmpty());
  TEST_ASSERT_FALSE (s.isNull());
}

void test_constructor_notnull (void) {
  BusSettings s1 (dataPin, clkPin, holdPin);

  TEST_PRINTF ("dataPin: %d, clkPin: %d, holdPin: %d\n", dataPin, clkPin, holdPin);
  TEST_ASSERT_FALSE (s1.isEmpty());
}

void test_getters (void) {
  BusSettings s (dataPin, clkPin, holdPin);

  TEST_ASSERT_EQUAL (dataPin, s.dataPin());
  TEST_ASSERT_EQUAL (clkPin, s.clkPin());
  TEST_ASSERT_EQUAL (holdPin, s.holdPin());
}

void test_setters (void) {
  BusSettings s;

  s.setDataPin (dataPin);
  s.setClkPin (clkPin);
  s.setHoldPin (holdPin);
  TEST_ASSERT_EQUAL (dataPin, s.dataPin());
  TEST_ASSERT_EQUAL (clkPin, s.clkPin());
  TEST_ASSERT_EQUAL (holdPin, s.holdPin());
  s.clear();
  TEST_ASSERT_TRUE (s.isEmpty());
}

void test_comparison (void) {
  BusSettings s1 (dataPin, clkPin, holdPin);
  BusSettings s2 (dataPin, clkPin, holdPin);
  BusSettings s3 (dataPin + 1, clkPin + 1, holdPin + 1);

  TEST_ASSERT (s1 == s2);
  TEST_ASSERT (s2 != s3);
}

void test_copy (void) {
  BusSettings s1;
  BusSettings s2 (dataPin, clkPin, holdPin);
  BusSettings s3 (s2);

  s1 = s2;
  TEST_ASSERT (s2 == s1);
  TEST_ASSERT (s3 == s2);
}

void test_move (void) {
  const BusSettings s1 (dataPin, clkPin, holdPin);

  // Test move constructor
  BusSettings s2 = s1;
  BusSettings s3 = std::move (s2);
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
