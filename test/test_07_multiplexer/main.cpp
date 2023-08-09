//
// Unit Test for the class SpaIot::Cd4051
// How to run:
// pio test -f test_07_multiplexer -v
// -v for verbose, if not specified only summary is printed (no TEST_MESSAGE, no TEST_PRINTF)
//
#include <spaiot_test.h>
#include <cd4051.h>

using namespace SpaIot;

#if defined(ESP8266)
const std::initializer_list<int> CtrlSelectPin {5, 4, 15}; // A->GPIO5, B->GPIO4, C->GPIO15
const int CtrlAInhPin = 16; // INH->GPIO16
const int CtrlBInhPin = 0;  // INH->GPIO0

#elif defined(ARDUINO_LOLIN_S3)
const std::initializer_list<int> CtrlSelectPin {4, 5, 6}; // A->GPIO4, B->GPIO5, C->GPIO6
const int CtrlAInhPin = 7; // INH->GPIO7
const int CtrlBInhPin = 15; // INH->GPIO15

#elif defined(ESP32)
const std::initializer_list<int> CtrlSelectPin {27, 16, 17}; // A->GPIO27, B->GPIO16, C->GPIO17
const int CtrlAInhPin = 33; // INH->GPIO25
const int CtrlBInhPin = 26; // INH->GPIO26

#else
#error unsupported platform
#endif

std::initializer_list<int> CtrlABut {1, 3, 7};
std::initializer_list<int> CtrlBBut {2, 4, 5, 7};

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_constructor_null (void) {
  Cd4051 ctrl;

  TEST_ASSERT_EQUAL (8, ctrl.size ());
  TEST_ASSERT_TRUE (ctrl.isNull());
}

void test_constructor_notnull (void) {
  Cd4051 ctrl (CtrlSelectPin, CtrlAInhPin);

  TEST_ASSERT_FALSE (ctrl.isNull());
}

void test_getters (void) {
  Cd4051 ctrl (CtrlSelectPin, CtrlAInhPin);
  int key = 0;

  TEST_ASSERT_EQUAL (ctrl.size(), 8);
  for (auto pin : CtrlSelectPin) {

    TEST_ASSERT_EQUAL (pin, ctrl.selectPin (key++));
  }
}

void test_setters (void) {
  Cd4051 ctrl (CtrlSelectPin, CtrlAInhPin);
  int key = 0;

  for (auto pin : CtrlSelectPin) {

    ctrl.setSelectPin (key, pin);
    TEST_ASSERT_EQUAL (pin, ctrl.selectPin (key++));
  }
}

void test_comparison (void) {
  Cd4051 ctrl1 (CtrlSelectPin, CtrlAInhPin);
  Cd4051 ctrl2 (CtrlSelectPin, CtrlAInhPin);
  Cd4051 ctrl3 (CtrlSelectPin, CtrlBInhPin);

  TEST_ASSERT (ctrl1 == ctrl2);
  TEST_ASSERT (ctrl1 != ctrl3);
}

void test_assignation (void) {
  Cd4051 ctrl1;
  Cd4051 ctrl2 (CtrlSelectPin, CtrlAInhPin);

  ctrl1 = ctrl2;
  TEST_ASSERT (ctrl1 == ctrl2);
}

void test_global (void) {
  Cd4051 ctrl1 (CtrlSelectPin, CtrlAInhPin);
  Cd4051 ctrl2 (CtrlSelectPin, CtrlBInhPin);

  TEST_ASSERT_TRUE (ButtonController::addToRegister ("CtrlA", ctrl1));
  TEST_ASSERT_TRUE (ButtonController::addToRegister ("CtrlB", ctrl2));
  TEST_ASSERT_FALSE (ButtonController::addToRegister ("CtrlA", ctrl1));

  TEST_ASSERT (ctrl1 == ButtonController::getFromRegister ("CtrlA"));
  TEST_ASSERT (ctrl2 == ButtonController::getFromRegister ("CtrlB"));
}

void test_begin (std::initializer_list<int> spins, int inh) {
  Cd4051 ctrl (spins, inh);

  TEST_ASSERT_FALSE (ctrl.isOpened ());
  ctrl.begin();
  TEST_ASSERT_TRUE (ctrl.isOpened ());
  TEST_ASSERT_FALSE (ctrl.isSelected ());
}

void test_begin_ctrla (void) {

  test_begin (CtrlSelectPin, CtrlAInhPin);
}

void test_begin_ctrlb (void) {

  test_begin (CtrlSelectPin, CtrlBInhPin);
}

void test_select (std::initializer_list<int> spins, int inh, std::initializer_list<int> buttons) {
  Cd4051 ctrl (spins, inh);
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
