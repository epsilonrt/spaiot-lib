//
// Unit Test for the class SpaIot::Cd4051
//
#include <Arduino.h>
#include <unity.h>
#include <cd4051.h>

using namespace SpaIot;

/*
  Cd4051 Scip2CtrlA (5, 4, 15, 16);
  Cd4051 Scip2CtrlB (5, 4, 15, 0);
  const std::map<int, ButtonSettings> Scip2SspButtons = {
    { Filter,   ButtonSettings ("Scip2CtrlA", 1) },
    { Bubble,   ButtonSettings ("Scip2CtrlA", 3) },
    { TempDown, ButtonSettings ("Scip2CtrlA", 7) },

    { Power,    ButtonSettings ("Scip2CtrlB", 2) },
    { TempUp,   ButtonSettings ("Scip2CtrlB", 4) },
    { TempUnit, ButtonSettings ("Scip2CtrlB", 5) },
    { Heater,   ButtonSettings ("Scip2CtrlB", 7) }
  };
 */

std::initializer_list<int> CtrlAPin {5, 4, 15, 16};
std::initializer_list<int> CtrlABut {1, 3, 7};
std::initializer_list<int> CtrlBPin {5, 4, 15, 0};
std::initializer_list<int> CtrlBBut {2, 4, 5, 7};

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_constructor_null (void) {
  Cd4051 ctrl;

  TEST_ASSERT_TRUE (ctrl.isNull());
}

void test_constructor_notnull (void) {
  Cd4051 ctrl (CtrlAPin);

  TEST_ASSERT_FALSE (ctrl.isNull());
}

void test_getters (void) {
  Cd4051 ctrl (CtrlAPin);
  int key = Cd4051::S0;

  for (auto pin : CtrlAPin) {

    TEST_ASSERT_EQUAL (pin, ctrl.pin (key++));
  }
}

void test_setters (void) {
  Cd4051 ctrl (CtrlAPin);
  int key = Cd4051::S0;

  for (auto pin : CtrlBPin) {

    ctrl.setPin (key, pin);
    TEST_ASSERT_EQUAL (pin, ctrl.pin (key++));
  }
}

void test_comparison (void) {
  Cd4051 ctrl1 (CtrlAPin);
  Cd4051 ctrl2 (CtrlAPin);
  Cd4051 ctrl3 (CtrlBPin);

  TEST_ASSERT (ctrl1 == ctrl2);
  TEST_ASSERT (ctrl1 != ctrl3);
}

void test_assignation (void) {
  Cd4051 ctrl1;
  Cd4051 ctrl2 (CtrlAPin);

  ctrl1 = ctrl2;
  TEST_ASSERT (ctrl1 == ctrl2);
}

void test_global (void) {
  Cd4051 ctrl1 (CtrlAPin);
  Cd4051 ctrl2 (CtrlBPin);

  TEST_ASSERT (ctrl1 == Scip2CtrlA);
  TEST_ASSERT (ctrl2 == Scip2CtrlB);

  TEST_ASSERT_TRUE (ButtonController::addToRegister ("TEST", ctrl1));
  TEST_ASSERT_FALSE (ButtonController::addToRegister ("TEST", ctrl1));
  TEST_ASSERT_FALSE (ButtonController::addToRegister ("Scip2CtrlA", ctrl2));
  TEST_ASSERT_FALSE (ButtonController::addToRegister ("Scip2CtrlB", ctrl2));

  TEST_ASSERT (ctrl1 == ButtonController::getFromRegister ("TEST"));
  TEST_ASSERT (ctrl1 == ButtonController::getFromRegister ("Scip2CtrlA"));
  TEST_ASSERT (ctrl2 == ButtonController::getFromRegister ("Scip2CtrlB"));
}

void test_begin (std::initializer_list<int> pins) {
  Cd4051 ctrl (pins);

  TEST_ASSERT_FALSE (ctrl.isOpened ());
  ctrl.begin();
  TEST_ASSERT_TRUE (ctrl.isOpened ());
  TEST_ASSERT_FALSE (ctrl.isSelected ());
}

void test_begin_ctrla (void) {

  test_begin (CtrlAPin);
}

void test_begin_ctrlb (void) {

  test_begin (CtrlBPin);
}

void test_select (std::initializer_list<int> pins, std::initializer_list<int> buttons) {
  Cd4051 ctrl (pins);
  std::vector<int> pin = pins;

  ctrl.begin();
  for (auto button : buttons) {

    TEST_ASSERT_FALSE (ctrl.isSelected ());
    TEST_ASSERT_EQUAL (HIGH, digitalRead (pin[Cd4051::En]));
    TEST_ASSERT_EQUAL (button, ctrl.select (button));
    TEST_ASSERT_EQUAL (LOW, digitalRead (pin[Cd4051::En]));
    TEST_ASSERT_EQUAL ( (button & 0x01) ? HIGH : LOW, digitalRead (pin[Cd4051::S0]));
    TEST_ASSERT_EQUAL ( (button & 0x02) ? HIGH : LOW, digitalRead (pin[Cd4051::S1]));
    TEST_ASSERT_EQUAL ( (button & 0x04) ? HIGH : LOW, digitalRead (pin[Cd4051::S2]));
    ctrl.deselect ();
  }
}

void test_select_ctrla (void) {

  test_select (CtrlAPin, CtrlABut);
}

void test_select_ctrlb (void) {

  test_select (CtrlBPin, CtrlBBut);
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
    RUN_TEST (test_begin_ctrla);
    RUN_TEST (test_begin_ctrlb);
    RUN_TEST (test_select_ctrla);
    RUN_TEST (test_select_ctrlb);

    UNITY_END(); // stop unit testing
    done = true;
  }
}
