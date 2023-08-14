//
// Unit Test for the class SpaIot::Pcf8574Mux
// How to run:
// pio test -f test_07_pcf8574mux -v
// -v for verbose, if not specified only summary is printed (no TEST_MESSAGE, no TEST_PRINTF)
// 
// The buttons controller is a PCF8574 connected to 2 multiplexers 4051 as follows:
// - The selection pins {A, B, C} of the 2 multiplexers are connected 
//   respectively to {P0, P1, P2} on the PCF8574
// - The INH pin of [A7..A0] multiplexer is connected to P3 on the PCF8574
// - The INH pin of [B7..B0] multiplexer is connected to P4 on the PCF8574
// This gives us the following values for the buttons of a SSP spa:
// |    KEY   | BIT | P4 | P3 | P2 | P1 | P0 | DEC |
// |:--------:|:---:|:--:|:--:|:--:|:--:|:--:|:---:|
// |   Power  |  B2 |  0 |  1 |  0 |  1 |  0 |  10 |
// |  Filter  |  A1 |  1 |  0 |  0 |  0 |  1 |  17 |
// |  Bubble  |  A3 |  1 |  0 |  0 |  1 |  1 |  19 |
// |  Heater  |  B7 |  0 |  1 |  1 |  1 |  1 |  15 |
// |  TempUp  |  B4 |  0 |  1 |  1 |  0 |  0 |  12 |
// | TempDown |  A7 |  1 |  0 |  1 |  1 |  1 |  23 |
// | TempUnit |  B5 |  0 |  1 |  1 |  0 |  1 |  13 |
//
#include <spaiot_test.h>
#include <config/hwconfig.h>
#include <pcf8574mux.h>

using namespace SpaIot;

const uint8_t slaveAddr = Mux.address(); // PCF8574: 0x20 - PCF8574A: 0x38
const std::map<int, ButtonSettings> & TestButtons = DefaultConfig.buttons();

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }
void test_constructor_null (void) {
  Pcf8574Mux ctrl;

  TEST_ASSERT_FALSE (ctrl.isNull());
  TEST_ASSERT_TRUE (ctrl.isEmpty());
}

void test_constructor_notnull (void) {
  Pcf8574Mux ctrl (0x38);

  TEST_ASSERT_FALSE (ctrl.isNull());
  TEST_ASSERT_FALSE (ctrl.isEmpty());
  ctrl.clear();
  TEST_ASSERT_TRUE (ctrl.isEmpty());
}

void test_comparison (void) {
  Pcf8574Mux ctrl1 (0x20);
  Pcf8574Mux ctrl2 (0x20);
  Pcf8574Mux ctrl3 (0x38);

  TEST_ASSERT (ctrl1 == ctrl2);
  TEST_ASSERT (ctrl1 != ctrl3);
}

void test_copy (void) {
  Pcf8574Mux ctrl1 (0x20);
  Pcf8574Mux ctrl2 = ctrl1;
  Pcf8574Mux ctrl3 (0x38);

  TEST_ASSERT (ctrl1 == ctrl2);
  TEST_ASSERT (ctrl1 != ctrl3);
  ctrl3 = ctrl1;
  TEST_ASSERT (ctrl1 == ctrl3);
}

void test_move (void) {
  const Pcf8574Mux s1;

  // Test move constructor
  Pcf8574Mux s2 = s1;
  Pcf8574Mux s3 = std::move (s2);
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

void test_begin () {
  Pcf8574Mux ctrl (slaveAddr);

  TEST_ASSERT_FALSE (ctrl.isOpened ());
  ctrl.begin();
  TEST_ASSERT_TRUE (ctrl.isOpened ());
  TEST_ASSERT_FALSE (ctrl.isSelected ());
}

void pushButton (Pcf8574Mux &ctrl, int key) {

  TEST_ASSERT_EQUAL (TestButtons.at (key).id(), ctrl.select (TestButtons.at (key).id()));
  delay (HoldPressedMs);
  ctrl.deselect ();
}

void test_select () {
  Pcf8574Mux ctrl (slaveAddr);

  ctrl.begin();
  TEST_ASSERT_TRUE (ctrl.isOpened ());
  TEST_ASSERT_FALSE (ctrl.isSelected ());

  pushButton (ctrl, Power);
  delay (2000);

  for (auto button : TestButtons) {
    int key = button.first;

    if (key != Power) {

      pushButton (ctrl, key);
      delay (2000);
    }
  }
  pushButton (ctrl, TempUnit);
  pushButton (ctrl, Power);
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
    RUN_TEST (test_constructor_notnull);
    RUN_TEST (test_comparison);
    RUN_TEST (test_copy);
    RUN_TEST (test_begin);
    RUN_TEST (test_select);

    UNITY_END(); // stop unit testing
    done = true;
  }
}
