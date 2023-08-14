//
// Unit Test for the class SpaIot::Button
// How to run:
// pio test -f test_08_button -v
// -v for verbose, if not specified only summary is printed (no TEST_MESSAGE, no TEST_PRINTF)
//
#include <spaiot_test.h>
#include <config/hwconfig.h>
#include <button.h>

using namespace SpaIot;

std::map <int, Button> button;
std::initializer_list<int> sequence {Power,     Filter,   Bubble,   TempUnit,
      TempUnit,  Heater,   TempUp,   TempUp,
      TempUp,    TempDown, TempDown, Bubble,
      Heater,    Filter,   Power};


// My buttons configuration (SSP)
const std::map<int, ButtonSettings> & TestButtons = DefaultConfig.buttons();

void setUp (void) {

}

// void tearDown(void) {
// // clean stuff up here
// }

void test_constructor_null () {
  Button but;

  TEST_ASSERT_TRUE (but.isEmpty());
  TEST_ASSERT_FALSE (but.isNull());
}

void test_constructor_notnull (void) {
  Button but (TestButtons.at (Power));

  TEST_ASSERT_FALSE (but.isEmpty());
}

void test_constructor_and_getters () {

  for (const auto& elmt : TestButtons) {

    const int key = elmt.first;
    const ButtonSettings & cfg = elmt.second;

    button.emplace (key, Button (cfg));
    Button & but = button.at (key);

    TEST_ASSERT_FALSE (but.isEmpty());

    // check configuration
    TEST_ASSERT (cfg == but.settings());
    TEST_ASSERT_EQUAL (cfg.id(), but.id());
    TEST_ASSERT (cfg.ctrl() == but.ctrl());
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

void test_copy (void) {
  Button but1 (TestButtons.at (Power));
  Button but2 = but1;
  Button but3 (TestButtons.at (Filter));

  TEST_ASSERT (but1 == but2);
  TEST_ASSERT (but1 != but3);
  but3 = but1;
  TEST_ASSERT (but1 == but3);
}

void test_move (void) {
  const Button but1;

  // Test move constructor
  Button but2 = but1;
  Button but3 = std::move (but2);
  TEST_ASSERT (but3 == but1);
  TEST_ASSERT_TRUE (but2.isNull());

  // Test move assignment
  but2 = std::move (but3);
  TEST_ASSERT (but2 == but1);
  TEST_ASSERT_TRUE (but3.isNull());

  but3.clear(); // check if clear() reset the d_ptr instance
  but3 = but1;
  TEST_ASSERT (but3 == but1);
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
  Wire.begin();

  UNITY_BEGIN();    // IMPORTANT LINE!
}

bool done = false;

void loop() {

  if (!done) {

    RUN_TEST (test_constructor_null);
    RUN_TEST (test_constructor_and_getters);
    RUN_TEST (test_comparison);
    RUN_TEST (test_copy);
    RUN_TEST (test_move);
    RUN_TEST (test_begin);
    RUN_TEST (test_press_release);
    RUN_TEST (test_push);

    UNITY_END(); // stop unit testing
    done = true;
  }
}
