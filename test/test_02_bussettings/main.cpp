//
// Unit Test for the class SpaIot::BusSettings
// How to run:
// pio test -f test_02_bussettings -v
// -v for verbose, if not specified only summary is printed (no TEST_MESSAGE, no TEST_PRINTF)
//
#include <Arduino.h>
#include <unity.h>
#include <bussettings.h>
#include <config/hwconfig.h>

using namespace SpaIot;

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
  BusSettings s1 (DataPin, ClkPin, HoldPin);
  BusSettings s2 {DataPin, ClkPin, HoldPin};

  TEST_ASSERT_FALSE (s1.isEmpty());
  TEST_ASSERT_FALSE (s2.isEmpty());
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
  s.clear();
  TEST_ASSERT_TRUE (s.isEmpty());
}

void test_comparison (void) {
  BusSettings s1 (DataPin, ClkPin, HoldPin);
  BusSettings s2 (DataPin, ClkPin, HoldPin);
  BusSettings s3 (DataPin + 1, ClkPin + 1, HoldPin + 1);

  TEST_ASSERT (s1 == s2);
  TEST_ASSERT (s2 != s3);
}

void test_copy (void) {
  BusSettings s1;
  BusSettings s2 (DataPin, ClkPin, HoldPin);
  BusSettings s3 (s2);

  s1 = s2;
  TEST_ASSERT (s2 == s1);
  TEST_ASSERT (s3 == s2);
}

void test_move (void) {
  const BusSettings s1 (DataPin, ClkPin, HoldPin);

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


void test_global (void) {
  BusSettings s (DataPin, ClkPin, HoldPin);

  TEST_ASSERT (DefaultConfig.bus() == s);
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
    RUN_TEST (test_global);

    UNITY_END(); // stop unit testing
    done = true;
  }
}
