//
// Unit Test for the class SpaIot::BusSettings
//
#include <Arduino.h>
#include <unity.h>
#include <bussettings.h>

using namespace SpaIot;

const int DataPin = 12;
const int ClkPin  = 14;
const int HoldPin = 13;

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_constructor_null (void) {
  BusSettings s;

  TEST_ASSERT_TRUE (s.isNull());
}

void test_constructor_notnull (void) {
  BusSettings s1 (DataPin, ClkPin, HoldPin);
  BusSettings s2 {DataPin, ClkPin, HoldPin};

  TEST_ASSERT_FALSE (s1.isNull());
  TEST_ASSERT_FALSE (s2.isNull());
}

void test_getters (void) {
  BusSettings s (DataPin, ClkPin, HoldPin);

  TEST_ASSERT_EQUAL (DataPin, s.dataPin());
  TEST_ASSERT_EQUAL (ClkPin, s.clkPin());
  TEST_ASSERT_EQUAL (HoldPin, s.holdPin());
}

void test_setters (void) {
  BusSettings s;

  s.setDataPin (DataPin);
  s.setClkPin (ClkPin);
  s.setHoldPin (HoldPin);
  TEST_ASSERT_EQUAL (DataPin, s.dataPin());
  TEST_ASSERT_EQUAL (ClkPin, s.clkPin());
  TEST_ASSERT_EQUAL (HoldPin, s.holdPin());
}

void test_comparison (void) {
  BusSettings s1 (DataPin, ClkPin, HoldPin);
  BusSettings s2 (DataPin, ClkPin, HoldPin);
  BusSettings s3 (DataPin + 1, ClkPin + 1, HoldPin + 1);

  TEST_ASSERT (s1 == s2);
  TEST_ASSERT (s2 != s3);
}

void test_assignation (void) {
  BusSettings s1;
  BusSettings s2 (DataPin, ClkPin, HoldPin);

  s1 = s2;
  TEST_ASSERT (s2 == s1);
}

void test_global (void) {
  BusSettings s (DataPin, ClkPin, HoldPin);

  TEST_ASSERT (DiyScipBus == s);
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
    RUN_TEST (test_assignation);
    RUN_TEST (test_global);

    UNITY_END(); // stop unit testing
    done = true;
  }
}
