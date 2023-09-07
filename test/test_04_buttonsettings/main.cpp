//
// Unit Test for the class SpaIot::ButtonSettings
// How to run:
// pio test -f test_04_buttonsettings -v
// -v for verbose, if not specified only summary is printed (no TEST_MESSAGE, no TEST_PRINTF)
//
#include <Arduino.h>
#include <unity.h>
#include <buttonsettings.h>

using namespace SpaIot;

const std::map<int, ButtonSettings> TestButtons = {
  { Filter,   ButtonSettings ("Scip2MuxA", 1) },
  { Bubble,   ButtonSettings ("Scip2MuxA", 3) },
  { TempDown, ButtonSettings ("Scip2MuxA", 7) },

  { Power,    ButtonSettings ("Scip2MuxB", 2) },
  { TempUp,   ButtonSettings ("Scip2MuxB", 4) },
  { TempUnit, ButtonSettings ("Scip2MuxB", 5) },
  { Heater,   ButtonSettings ("Scip2MuxB", 7) }
};

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_constructor_null (void) {
  ButtonSettings s;

  TEST_ASSERT_TRUE (s.isEmpty());
  TEST_ASSERT_FALSE (s.isNull());
}

void test_constructor_notnull (void) {
  ButtonSettings s (TestButtons.at (Filter).controllerName(), TestButtons.at (Filter).id());

  TEST_ASSERT_FALSE (s.isEmpty());
}

void test_getters (void) {
  ButtonSettings s (TestButtons.at (Filter).controllerName(), TestButtons.at (Filter).id());

  TEST_ASSERT (TestButtons.at (Filter).controllerName() == s.controllerName());
  TEST_ASSERT_EQUAL (TestButtons.at (Filter).id(), s.id());
}

void test_setters (void) {
  ButtonSettings s;

  s.setId (TestButtons.at (Filter).id());
  TEST_ASSERT_EQUAL (TestButtons.at (Filter).id(), s.id());
  s.clear();
  TEST_ASSERT_TRUE (s.isEmpty());
}

void test_comparison (void) {
  ButtonSettings s1 (TestButtons.at (Filter).controllerName(), TestButtons.at (Filter).id());
  ButtonSettings s2 (TestButtons.at (Filter).controllerName(), TestButtons.at (Filter).id());
  ButtonSettings s3 (TestButtons.at (Power).controllerName(), TestButtons.at (Power).id());

  TEST_ASSERT (s1 == s2);
  TEST_ASSERT (s2 != s3);
}

void test_copy (void) {
  const ButtonSettings s1 (TestButtons.at (Filter).controllerName(), TestButtons.at (Filter).id());
  ButtonSettings s2;
  ButtonSettings s3 (s1);

  s2 = s1;
  TEST_ASSERT (s2 == s1);
  TEST_ASSERT (s3 == s1);
}

void test_move (void) {
  const   ButtonSettings s1 (TestButtons.at (Filter).controllerName(), TestButtons.at (Filter).id());

  // Test move constructor
  ButtonSettings s2 = s1;
  ButtonSettings s3 = std::move (s2);
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
  ButtonSettings s (TestButtons.at (Filter).controllerName(), TestButtons.at (Filter).id());

  TEST_ASSERT (Scip2SspButtons.at (Filter) == TestButtons.at (Filter));
  TEST_ASSERT (Scip2SspButtons == TestButtons);
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
