//
// Unit Test for the class SpaIot::Button
//
// We use a DIY board connected to a SSP spa, the push buttons configuration
// is as follows:

#include <Arduino.h>
#include <unity.h>
#include <button.h>
#include <spaiotdebug.h>

using namespace SpaIot;

std::map <int, Button> button;
std::initializer_list<int> sequence {Power,     Filter,   Bubble,   TempUnit,
      TempUnit,  Heater,   TempUp,   TempUp,
      TempUp,    TempDown, TempDown, Bubble,
      Heater,    Filter,   Power};

#if defined(ESP8266)
// My button controllers
Cd4051 TestCtrlA (5, 4, 15, 16); // A->GPIO5, B->GPIO4, C->GPIO15, INH->GPIO16
Cd4051 TestCtrlB (5, 4, 15, 0);  // A->GPIO5, B->GPIO4, C->GPIO15, INH->GPIO0

#elif defined(ESP32)
// My button controllers
Cd4051 TestCtrlA (27, 16, 17, 25); // A->GPIO27, B->GPIO16, C->GPIO17, INH->GPIO25
Cd4051 TestCtrlB (27, 16, 17, 26); // A->GPIO27, B->GPIO16, C->GPIO17, INH->GPIO26

#else
#error unsupported platform
#endif

// My buttons configuration (SSP)
const std::map<int, ButtonSettings> TestButtons = {
  { Filter,   ButtonSettings ("TestCtrlA", 1) },  // Filter   -> A1
  { Bubble,   ButtonSettings ("TestCtrlA", 3) },  // Bubble   -> A3
  { TempDown, ButtonSettings ("TestCtrlA", 7) },  // TempDown -> A7

  { Power,    ButtonSettings ("TestCtrlB", 2) },  // Power    -> B2
  { TempUp,   ButtonSettings ("TestCtrlB", 4) },  // TempUp   -> B4
  { TempUnit, ButtonSettings ("TestCtrlB", 5) },  // TempUnit -> B5
  { Heater,   ButtonSettings ("TestCtrlB", 7) }   // Heater   -> B7
};

void setUp (void) {

  // The button controllers must be registered before getInstance() call
  ButtonController::addToRegister ("TestCtrlA", TestCtrlA);
  ButtonController::addToRegister ("TestCtrlB", TestCtrlB);
}

// void tearDown(void) {
// // clean stuff up here
// }

void test_constructor_null () {
  Button but;

  TEST_ASSERT_TRUE (but.isNull());
}

void test_constructor_and_getters () {

  for (const auto& elmt : TestButtons) {

    const int key = elmt.first;
    const ButtonSettings & cfg = elmt.second;

    button.emplace (key, Button (cfg));
    Button & but = button.at (key);

    TEST_ASSERT_FALSE (but.isNull());

    // check configuration
    TEST_ASSERT (cfg == but.settings());
    TEST_ASSERT_EQUAL (cfg.id(), but.id());
    ButtonController & ctrl = (cfg.controllerName() == "TestCtrlA") ?
                              TestCtrlA : TestCtrlB;
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

  for (auto& elmt : button) {

    Button & but = elmt.second;

    but.begin();
    TEST_ASSERT_TRUE (but.isOpened ());
    TEST_ASSERT_FALSE (but.isPressed ());
  }
}

void press_release (int key) {

  button[key].press();
  TEST_ASSERT_TRUE (button[key].isPressed ());
  delay (HoldPressedMs);
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
