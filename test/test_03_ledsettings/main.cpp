//
// Unit Test for the class SpaIot::LedSettings
// How to run:
// pio test -f test_03_ledsettings -v
// -v for verbose, if not specified only summary is printed (no TEST_MESSAGE, no TEST_PRINTF)
//
#include <Arduino.h>
#include <unity.h>
#include <ledsettings.h>

using namespace SpaIot;

const uint8_t ledId = 12;

const std::map<int, LedSettings> MyLeds = {
  { Power,          LedSettings (0) },
  { Heater,         LedSettings (7) },
  { HeatReached,    LedSettings (9) },
  { Bubble,         LedSettings (10) },
  { Filter,         LedSettings (12) }
};

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_constructor_null (void) {
  LedSettings led;

  TEST_ASSERT_TRUE (led.isNull());
}

void test_constructor_notnull (void) {
  LedSettings led (ledId);

  TEST_ASSERT_FALSE (led.isNull());
}

void test_getters (void) {
  const uint16_t frame = 1 << ledId;
  LedSettings led (ledId);

  TEST_ASSERT_EQUAL (ledId, led.order());
  TEST_ASSERT_EQUAL (frame, led.frame());
}

void test_setters (void) {
  const uint16_t frame = 1 << ledId;
  LedSettings led;

  led.setOrder (ledId);
  TEST_ASSERT_EQUAL (ledId, led.order());
  TEST_ASSERT_EQUAL (frame, led.frame());

  led.setOrder (ledId + 16);
  TEST_ASSERT_EQUAL (ledId, led.order());
  TEST_ASSERT_EQUAL (frame, led.frame());
  led.clear();
  TEST_ASSERT_TRUE (led.isNull());
}

void test_comparison (void) {
  LedSettings led1 (ledId);
  LedSettings led2 (ledId);
  LedSettings led3 (ledId + 1);

  TEST_ASSERT (led1 == led2);
  TEST_ASSERT (led1 != led3);
}

void test_copy (void) {
  LedSettings led1;
  LedSettings led2 (ledId);
  LedSettings led3 (led2);

  led1 = led2;
  TEST_ASSERT (led2 == led1);
  TEST_ASSERT (led3 == led2);
}

void test_move (void) {
  const   LedSettings led1 (ledId);

  // Test move constructor
  LedSettings led2 = led1;
  LedSettings led3 = std::move (led2);
  TEST_ASSERT (led3 == led1);
  TEST_ASSERT_TRUE (led2.isNull());

  // Test move assignment
  led2 = std::move (led3);
  TEST_ASSERT (led2 == led1);
  TEST_ASSERT_TRUE (led3.isNull());

  led3.clear(); // check if clear() reset the d_ptr instance
  led3 = led1;
  TEST_ASSERT (led3 == led1);
}

void test_global (void) {
  LedSettings led (ledId);

  TEST_ASSERT (MyLeds == SspLeds);
  TEST_ASSERT (MyLeds.at (Filter) == led); // [] operator not const, we use at()
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
