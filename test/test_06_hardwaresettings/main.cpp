//
// Unit Test for the class SpaIot::HardwareSettings
// How to run:
// pio test -f test_06_hardwaresettings -v
// -v for verbose, if not specified only summary is printed (no TEST_MESSAGE, no TEST_PRINTF)
//
#include <Arduino.h>
#include <unity.h>
#include <hardwaresettings.h>
#include <type_name.h>
#include <config/hwconfig.h>

using namespace SpaIot;

// ScipBus
const BusSettings &TestBus = DefaultConfig.bus();

// SspLeds
const std::map<int, LedSettings> TestSspLeds = {
  { Power,          LedSettings (0) },
  { Heater,         LedSettings (7) },
  { HeatReached,    LedSettings (9) },
  { Bubble,         LedSettings (10) },
  { Filter,         LedSettings (12) }
};
// SjbLeds
const std::map<int, LedSettings> TestSjbLeds = {
  { Power,          LedSettings (0) },
  { Bubble,         LedSettings (1) },
  { Heater,         LedSettings (7) },
  { HeatReached,    LedSettings (9) },
  { Jet,            LedSettings (10) },
  { Filter,         LedSettings (12) },
  { Sanitizer,      LedSettings (13) }
};
// Scip2SspButtons
const std::map<int, ButtonSettings> TestSspButtons = {
  { Filter,   ButtonSettings ("Scip2CtrlA", 1) },
  { Bubble,   ButtonSettings ("Scip2CtrlA", 3) },
  { TempDown, ButtonSettings ("Scip2CtrlA", 7) },

  { Power,    ButtonSettings ("Scip2CtrlB", 2) },
  { TempUp,   ButtonSettings ("Scip2CtrlB", 4) },
  { TempUnit, ButtonSettings ("Scip2CtrlB", 5) },
  { Heater,   ButtonSettings ("Scip2CtrlB", 7) }
};
// Scip2SjbButtons
const std::map<int, ButtonSettings> TestSjbButtons = {
  { Sanitizer,  ButtonSettings ("Scip2CtrlA", 0) },
  { Jet,        ButtonSettings ("Scip2CtrlA", 1) }, // this will have to be checked
  { Bubble,     ButtonSettings ("Scip2CtrlA", 3) }, // this will have to be checked
  { Filter,     ButtonSettings ("Scip2CtrlA", 7) },

  { TempDown,   ButtonSettings ("Scip2CtrlB", 1) },
  { Power,      ButtonSettings ("Scip2CtrlB", 2) },
  { TempUp,     ButtonSettings ("Scip2CtrlB", 4) },
  { TempUnit,   ButtonSettings ("Scip2CtrlB", 5) }, // this will have to be checked
  { Heater,     ButtonSettings ("Scip2CtrlB", 7) },
};

const HardwareSettings TestSSP (TestBus, TestSspLeds, TestSspButtons);
const HardwareSettings TestSJB (TestBus, TestSjbLeds, TestSjbButtons);

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_constructor_null (void) {
  HardwareSettings s;

  TEST_ASSERT_TRUE (s.isNull());
}

void test_constructor_notnull (void) {
  HardwareSettings s (TestBus, TestSspLeds, TestSspButtons);

  TEST_ASSERT_FALSE (s.isNull());
}

void test_getters (void) {
  HardwareSettings s (TestBus, TestSspLeds, TestSspButtons);

  TEST_ASSERT (TestBus == s.bus());
  TEST_ASSERT (TestSspLeds == s.leds());
  TEST_ASSERT (TestSspButtons == s.buttons());
}

void test_comparison (void) {
  HardwareSettings s1 (TestBus, TestSspLeds, TestSspButtons);
  HardwareSettings s2 (TestBus, TestSspLeds, TestSspButtons);
  HardwareSettings s3 (TestBus, TestSjbLeds, TestSjbButtons);

  TEST_ASSERT (s1 == s2);
  TEST_ASSERT (s2 != s3);
}

void test_assignation (void) {
  HardwareSettings s1;
  HardwareSettings s2 (TestBus, TestSspLeds, TestSspButtons);

  s1 = s2;
  TEST_ASSERT (s2 == s1);
}

void test_global (void) {

  TEST_ASSERT_TRUE (HardwareSettings::addToRegister ("TEST", TestSSP));
  TEST_ASSERT_FALSE (HardwareSettings::addToRegister ("TEST", TestSSP));
  TEST_ASSERT_FALSE (HardwareSettings::addToRegister ("SCIP2SSP", TestSSP));
  TEST_ASSERT_FALSE (HardwareSettings::addToRegister ("SCIP2SJB", TestSJB));
  TEST_ASSERT (HardwareSettings::getFromRegister ("TEST") == TestSSP);

  const HardwareSettings &s (HardwareSettings::getFromRegister ("SCIP2SJB"));

  TEST_ASSERT (s == TestSJB);
  TEST_ASSERT (s.bus() == TestSJB.bus());
  TEST_ASSERT (s.leds() == TestSJB.leds());
  TEST_ASSERT (s.buttons() == TestSJB.buttons());
  TEST_ASSERT (HardwareSettings::getFromRegister ("SCIP2SSP") == TestSSP);
}

void setup() {

  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay (2000);

  UNITY_BEGIN();    // IMPORTANT LINE!
  RUN_TEST (test_constructor_null);
  RUN_TEST (test_constructor_notnull);
  RUN_TEST (test_getters);
  RUN_TEST (test_comparison);
  RUN_TEST (test_assignation);
  RUN_TEST (test_global);

  UNITY_END(); // stop unit testing
}


void loop() {

}
