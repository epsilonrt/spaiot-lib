//
// Unit Test for the class SpaIot::Button
//
// We use a DIY board connected to a SSP spa, the push buttons configuration
// is as follows:
//  const std::map<int, ButtonSettings> Scip2SspButtons = {
//    { Filter,   ButtonSettings ("Scip2CtrlA", 1) },
//    { Bubble,   ButtonSettings ("Scip2CtrlA", 3) },
//    { TempDown, ButtonSettings ("Scip2CtrlA", 7) },
//
//    { Power,    ButtonSettings ("Scip2CtrlB", 2) },
//    { TempUp,   ButtonSettings ("Scip2CtrlB", 4) },
//    { TempUnit, ButtonSettings ("Scip2CtrlB", 5) },
//    { Heater,   ButtonSettings ("Scip2CtrlB", 7) }
//  };
#include <Arduino.h>
#include <unity.h>
#include <button.h>
#include <debug.h>

using namespace SpaIot;

const unsigned long ButtonIntervalMs = 500;
std::map <int, Button> button;
std::initializer_list<int> sequence {Power,     Filter,   Bubble,   TempUnit,
      TempUnit,  Heater,   TempUp,   TempUp,
      TempUp,    TempDown, TempDown, Bubble,
      Heater,    Filter,   Power};

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_constructor_null () {
  Button but;

  TEST_ASSERT_TRUE (but.isNull());
}

void test_constructor_and_getters () {

  for (const auto& [ key, cfg ] : Scip2SspButtons) {

    button.emplace (key, Button (cfg));
    Button & but = button.at (key);

    TEST_ASSERT_FALSE (but.isNull());

    // check configuration
    TEST_ASSERT (cfg == but.settings());
    TEST_ASSERT_EQUAL (cfg.id(), but.id());
    ButtonController & ctrl = (cfg.controllerName() == "Scip2CtrlA") ?
                              Scip2CtrlA : Scip2CtrlB;
    TEST_ASSERT (ctrl == but.ctrl());
    TEST_ASSERT_FALSE (but.isOpened());
    TEST_ASSERT_FALSE (but.isPressed());
  }
}

void test_comparison () {
  Button & but1 (button.at (Power));
  Button & but2 (button.at (Power));
  Button & but3 (button.at (Filter));

  TEST_ASSERT (but1 == but2);
  TEST_ASSERT (but1 != but3);
}

void test_assignation () {
  Button but1;
  Button & but2 (button.at (Power));

  but1 = but2;
  TEST_ASSERT (but1 == but2);
}

void test_begin () {

  for (auto& [ key, but ] : button) {

    but.begin();
    TEST_ASSERT_TRUE (but.isOpened ());
    TEST_ASSERT_FALSE (but.isPressed ());
  }
}

void press_release (int key) {

  button[key].press();
  TEST_ASSERT_TRUE (button[key].isPressed ());
  delay (Button::HoldPressedMs);
  button[key].release();
  TEST_ASSERT_FALSE (button[key].isPressed ());
}

void test_press_release () {
  for (auto key : sequence) {

    press_release (key);
    delay (2000);
  }
}

void test_push () {
  for (auto key : sequence) {

    button[key].push();
    delay (ButtonIntervalMs);
  }
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
    RUN_TEST (test_constructor_and_getters);
    RUN_TEST (test_comparison);
    RUN_TEST (test_assignation);
    RUN_TEST (test_begin);
    RUN_TEST (test_press_release);
    RUN_TEST (test_push);

    UNITY_END(); // stop unit testing
    done = true;
  }
}
