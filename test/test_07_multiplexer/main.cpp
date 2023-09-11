//
// Unit Test for the class SpaIot::Multiplexer
// How to run:
// pio test -f test_07_multiplexer -v
// -v for verbose, if not specified only summary is printed (no TEST_MESSAGE, no TEST_PRINTF)
//
#include <unity.h>
#include <vector>
#include <multiplexer.h>
#include <buttonsettings.h>
#include "MyBoardSettings.h"

using namespace SpaIot;

const int PinA = SpaMuxA.selectPin (Multiplexer::A);
const int PinB = SpaMuxA.selectPin (Multiplexer::B);
const int PinC = SpaMuxA.selectPin (Multiplexer::C);
std::initializer_list<int> CtrlSelectPin {PinA, PinB, PinC};
const int CtrlAInhPin = SpaMuxA.inhPin ();
const int CtrlBInhPin = SpaMuxB.inhPin ();

const std::map<int, ButtonSettings> &TestButtons = SpaButtons;
std::initializer_list<int> CtrlABut {1, 3, 7};
std::initializer_list<int> CtrlBBut {2, 4, 5, 7};

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_constructor_null (void) {
  Multiplexer ctrl;

  TEST_ASSERT_FALSE (ctrl.isNull());
  TEST_ASSERT_TRUE (ctrl.isEmpty());
  TEST_ASSERT_EQUAL (0, ctrl.size ());
}

void test_constructor_notnull (void) {
  Multiplexer ctrl (CtrlSelectPin, CtrlAInhPin);

  TEST_ASSERT_EQUAL (8, ctrl.size ());
  TEST_ASSERT_FALSE (ctrl.isNull());
  TEST_ASSERT_FALSE (ctrl.isEmpty());
}

void test_getters (void) {
  Multiplexer ctrl (CtrlSelectPin, CtrlAInhPin);
  int key = 0;

  TEST_ASSERT_EQUAL (ctrl.size(), 8);
  for (auto pin : CtrlSelectPin) {

    TEST_ASSERT_EQUAL (pin, ctrl.selectPin (key++));
  }
}

void test_setters (void) {
  Multiplexer ctrl (CtrlSelectPin, CtrlAInhPin);
  int key = 0;

  for (auto pin : CtrlSelectPin) {

    ctrl.setSelectPin (key, pin);
    TEST_ASSERT_EQUAL (pin, ctrl.selectPin (key++));
  }
  ctrl.clear();
  TEST_ASSERT_TRUE (ctrl.isEmpty());
}

void test_comparison (void) {
  Multiplexer ctrl1 (CtrlSelectPin, CtrlAInhPin);
  Multiplexer ctrl2 (CtrlSelectPin, CtrlAInhPin);
  Multiplexer ctrl3 (CtrlSelectPin, CtrlBInhPin);

  TEST_ASSERT (ctrl1 == ctrl2);
  TEST_ASSERT (ctrl1 != ctrl3);
}

void test_copy (void) {
  Multiplexer ctrl1 (CtrlSelectPin, CtrlAInhPin);
  Multiplexer ctrl2 = ctrl1;
  Multiplexer ctrl3 (CtrlSelectPin, CtrlBInhPin);

  TEST_ASSERT (ctrl1 == ctrl2);
  TEST_ASSERT (ctrl1 != ctrl3);
  ctrl3 = ctrl1;
  TEST_ASSERT (ctrl1 == ctrl3);
}

void test_move (void) {
  const Multiplexer s1 (CtrlSelectPin, CtrlAInhPin);

  // Test move constructor
  Multiplexer s2 = s1;
  Multiplexer s3 = std::move (s2);
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
  Multiplexer ctrl1 (CtrlSelectPin, CtrlAInhPin);
  Multiplexer ctrl2 (CtrlSelectPin, CtrlBInhPin);

  TEST_ASSERT_TRUE (ButtonController::addToRegister ("CtrlA", ctrl1));
  TEST_ASSERT_TRUE (ButtonController::addToRegister ("CtrlB", ctrl2));
  TEST_ASSERT_FALSE (ButtonController::addToRegister ("CtrlA", ctrl1));

  TEST_ASSERT (ctrl1 == ButtonController::getFromRegister ("CtrlA"));
  TEST_ASSERT (ctrl2 == ButtonController::getFromRegister ("CtrlB"));
}

void test_begin (std::initializer_list<int> spins, int inh) {
  Multiplexer ctrl (spins, inh);

  TEST_ASSERT_FALSE (ctrl.isOpen ());
  ctrl.begin();
  TEST_ASSERT_TRUE (ctrl.isOpen ());
  TEST_ASSERT_FALSE (ctrl.isSelected ());
}

void test_begin_ctrla (void) {

  test_begin (CtrlSelectPin, CtrlAInhPin);
}

void test_begin_ctrlb (void) {

  test_begin (CtrlSelectPin, CtrlBInhPin);
}


void test_select (std::initializer_list<int> spins, int inh, std::initializer_list<int> buttons) {
  Multiplexer ctrl (spins, inh);
  std::vector<int> pin = spins;

  ctrl.begin();
  for (auto button : buttons) {

    TEST_ASSERT_FALSE (ctrl.isSelected ());
    TEST_ASSERT_EQUAL (HIGH, digitalRead (inh));
    TEST_ASSERT_EQUAL (button, ctrl.select (button));
    TEST_ASSERT_EQUAL (LOW, digitalRead (inh));

    for (unsigned int i = 0; i < pin.size(); i++) {

      TEST_ASSERT_EQUAL ( (button & (1 << i)) ? HIGH : LOW, digitalRead (pin[i]));
    }
    ctrl.deselect ();
  }
}
void test_select_ctrla (void) {

  test_select (CtrlSelectPin, CtrlAInhPin, CtrlABut);
}

void test_select_ctrlb (void) {

  test_select (CtrlSelectPin, CtrlBInhPin, CtrlBBut);
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
    RUN_TEST (test_begin_ctrla);
    RUN_TEST (test_begin_ctrlb);
    RUN_TEST (test_select_ctrla);
    RUN_TEST (test_select_ctrlb);

    UNITY_END(); // stop unit testing
    done = true;
  }
}
