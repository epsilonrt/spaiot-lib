//
// Unit Test for the class SpaIot::ButtonSettings
//
#include <Arduino.h>
#include <unity.h>
#include <buttonsettings.h>

using namespace SpaIot;

const std::map<int, ButtonSettings> TestButtons = {
  { Filter,   ButtonSettings ("Scip2CtrlA", 1) },
  { Bubble,   ButtonSettings ("Scip2CtrlA", 3) },
  { TempDown, ButtonSettings ("Scip2CtrlA", 7) },

  { Power,    ButtonSettings ("Scip2CtrlB", 2) },
  { TempUp,   ButtonSettings ("Scip2CtrlB", 4) },
  { TempUnit, ButtonSettings ("Scip2CtrlB", 5) },
  { Heater,   ButtonSettings ("Scip2CtrlB", 7) }
};

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_constructor_null (void) {
  ButtonSettings s;

  TEST_ASSERT_TRUE (s.isNull());
}

void test_constructor_notnull (void) {
  ButtonSettings s (TestButtons.at (Filter).controllerName(), TestButtons.at (Filter).id());

  TEST_ASSERT_FALSE (s.isNull());
}

void test_getters (void) {
  ButtonSettings s (TestButtons.at (Filter).controllerName(), TestButtons.at (Filter).id());

  TEST_ASSERT (TestButtons.at (Filter).controllerName() == s.controllerName());
  TEST_ASSERT_EQUAL (TestButtons.at (Filter).id(), s.id());
}

void test_setters (void) {
  ButtonSettings s;

  s.setControllerName (TestButtons.at (Filter).controllerName());
  s.setId (TestButtons.at (Filter).id());
  TEST_ASSERT (TestButtons.at (Filter).controllerName() == s.controllerName());
  TEST_ASSERT_EQUAL (TestButtons.at (Filter).id(), s.id());
}

void test_comparison (void) {
  ButtonSettings s1 (TestButtons.at (Filter).controllerName(), TestButtons.at (Filter).id());
  ButtonSettings s2 (TestButtons.at (Filter).controllerName(), TestButtons.at (Filter).id());
  ButtonSettings s3 (TestButtons.at (Power).controllerName(), TestButtons.at (Power).id());

  TEST_ASSERT (s1 == s2);
  TEST_ASSERT (s2 != s3);
}

void test_assignation (void) {
  ButtonSettings s1;
  ButtonSettings s2 (TestButtons.at (Filter).controllerName(), TestButtons.at (Filter).id());

  s1 = s2;
  TEST_ASSERT (s2 == s1);
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
    RUN_TEST (test_assignation);
    RUN_TEST (test_global);

    UNITY_END(); // stop unit testing
    done = true;
  }
}
