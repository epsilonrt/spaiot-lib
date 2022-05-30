//
// Unit Test for the class SpaIot::Pcf8574Mux
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

void test_constructor (void) {
  Pcf8574Mux ctrl;

  TEST_ASSERT_FALSE (ctrl.isNull());
}

void test_comparison (void) {
  Pcf8574Mux ctrl1 (0x20);
  Pcf8574Mux ctrl2 (0x20);
  Pcf8574Mux ctrl3 (0x38);

  TEST_ASSERT (ctrl1 == ctrl2);
  TEST_ASSERT (ctrl1 != ctrl3);
}

void test_assignation (void) {
  Pcf8574Mux ctrl1 (0x20);
  Pcf8574Mux ctrl2 (0x38);

  //ctrl1 = ctrl2;
  //TEST_ASSERT (ctrl1 == ctrl2);
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

    RUN_TEST (test_constructor);
    RUN_TEST (test_comparison);
    //RUN_TEST (test_assignation);
    RUN_TEST (test_begin);
    RUN_TEST (test_select);

    UNITY_END(); // stop unit testing
    done = true;
  }
}
