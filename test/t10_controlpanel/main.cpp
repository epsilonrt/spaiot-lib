//
// Unit Test for the class SpaIot::ControlPanel
//
// We use a DIY board connected to a SSP spa, the configuration  is as follows:
//
//  const HardwareSettings Scip2Ssp (ScipBus, SspLeds, Scip2SspButtons);
//
//  const BusSettings ScipBus (12, 14, 13);
//  const std::map<int, LedSettings> SspLeds = {
//    { Power,          LedSettings (0) },
//    { Heater,         LedSettings (7) },
//    { HeatReached,    LedSettings (9) },
//    { Bubble,         LedSettings (10) },
//    { Filter,         LedSettings (12) }//  };
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
#include <controlpanel.h>
#include <debug.h>

using namespace SpaIot;

const HardwareSettings & hwSettings                   = Scip2Ssp;
const BusSettings & busSettings                       = ScipBus;
const std::map<int, LedSettings> & ledSettings        = SspLeds;
const std::map<int, ButtonSettings> & buttonSettings  = Scip2SspButtons;

ControlPanel * panel;
uint32_t frameCounter;
uint32_t frameDropped;
uint16_t errorValue;
uint16_t rawStatus;
uint16_t sanitizerTime;
bool isSetupModeTriggered;
uint8_t   isPowerOn;
uint8_t   isFilterOn;
uint8_t   isBubbleOn;
uint8_t   isHeaterOn;
uint8_t   isHeatReached;
uint8_t   isJetOn;
uint8_t   isSanitizerOn;
uint16_t waterTemp = UnsetValue16;
uint16_t desiredTemp = UnsetValue16;

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_constructor () {

  panel = ControlPanel::getInstance (hwSettings);
  TEST_ASSERT_FALSE (panel->isOpened());
}


void test_getters () {

  TEST_ASSERT (busSettings == panel->busSettings());

  TEST_ASSERT (buttonSettings == panel->buttonSettings());
  for (const auto& [ key, cfg ] : panel->buttonSettings()) {

    TEST_ASSERT_TRUE (panel->hasButton (key));
  }

  TEST_ASSERT (ledSettings == panel->ledSettings());
  for (const auto& [ key, led ] : panel->ledSettings()) {

    TEST_ASSERT_TRUE (panel->hasLed (key));
  }

  frameCounter = panel->frameCounter();
  TEST_ASSERT_EQUAL (0, frameCounter);
  frameDropped =  panel->frameDropped();
  TEST_ASSERT_EQUAL (0, frameDropped);
  errorValue = panel->error();
  TEST_ASSERT_EQUAL (0, errorValue);
  rawStatus = panel->rawStatus();
  TEST_ASSERT_EQUAL (UnsetValue16, rawStatus);
  waterTemp = panel->waterTemp();
  TEST_ASSERT_EQUAL (UnsetValue16, waterTemp);
  desiredTemp = panel->desiredTemp();
  TEST_ASSERT_EQUAL (UnsetValue16, desiredTemp);
  sanitizerTime = panel->sanitizerTime();
  TEST_ASSERT_EQUAL (UnsetValue16, sanitizerTime);
  isSetupModeTriggered = panel->isSetupModeTriggered();
  TEST_ASSERT_FALSE (isSetupModeTriggered);

  for (const auto& [ key, led ] : panel->ledSettings()) {

    TEST_ASSERT_EQUAL (UnsetValue8, panel->isLedOn (key));
  }

  isPowerOn = panel->isPowerOn();
  TEST_ASSERT_EQUAL (UnsetValue8, isPowerOn);
  isFilterOn = panel->isFilterOn();
  TEST_ASSERT_EQUAL (UnsetValue8, isFilterOn);
  isBubbleOn = panel->isBubbleOn();
  TEST_ASSERT_EQUAL (UnsetValue8, isBubbleOn);
  isHeaterOn = panel->isHeaterOn();
  TEST_ASSERT_EQUAL (UnsetValue8, isHeaterOn);
  isHeatReached = panel->isHeatReached();
  TEST_ASSERT_EQUAL (UnsetValue8, isHeatReached);
  if (panel->hasLed (Jet)) {
    isJetOn = panel->isJetOn();
    TEST_ASSERT_EQUAL (UnsetValue8, isJetOn);
  }
  if (panel->hasLed (Sanitizer)) {
    isSanitizerOn = panel->isSanitizerOn();
    TEST_ASSERT_EQUAL (UnsetValue8, isSanitizerOn);
  }
}

void test_begin () {

  panel->begin();
  TEST_ASSERT_TRUE (panel->isOpened ());
  delay (100);
  TEST_ASSERT (panel->rawStatus() != UnsetValue16);
  for (const auto& [ key, led ] : SspLeds) {

    TEST_ASSERT_EQUAL (false, panel->isLedOn (key));
  }
  isPowerOn = panel->isPowerOn();
  TEST_ASSERT_EQUAL (false, isPowerOn);
  isFilterOn = panel->isFilterOn();
  TEST_ASSERT_EQUAL (false, isFilterOn);
  isBubbleOn = panel->isBubbleOn();
  TEST_ASSERT_EQUAL (false, isBubbleOn);
  isHeaterOn = panel->isHeaterOn();
  TEST_ASSERT_EQUAL (false, isHeaterOn);
  isHeatReached = panel->isHeatReached();
  TEST_ASSERT_EQUAL (false, isHeatReached);
  if (panel->hasLed (Jet)) {
    isJetOn = panel->isJetOn();
    TEST_ASSERT_EQUAL (false, isJetOn);
  }
  if (panel->hasLed (Sanitizer)) {
    isSanitizerOn = panel->isSanitizerOn();
    TEST_ASSERT_EQUAL (false, isSanitizerOn);
  }
}

void test_power_on () {

  if (panel->isPowerOn()) {

    TEST_ASSERT_FALSE (panel->setPower (false));
  }
  TEST_ASSERT_TRUE (panel->setPower ());
}

void test_filter () {

  if (panel->isFilterOn()) {

    TEST_ASSERT_FALSE (panel->setFilter (false));
    delay (100);
  }
  TEST_ASSERT_TRUE (panel->setFilter ());
  delay (100);
  TEST_ASSERT_FALSE (panel->setFilter (false));
  delay (5000);
}

void test_bubble () {
  if (panel->isBubbleOn()) {

    TEST_ASSERT_FALSE (panel->setBubble (false));
    delay (100);
  }
  TEST_ASSERT_TRUE (panel->setBubble ());
  delay (150);
  TEST_ASSERT_FALSE (panel->setBubble (false));
  delay (5000);
}

void test_heater () {
  if (panel->isHeaterOn()) {

    TEST_ASSERT_FALSE (panel->setHeater (false));
  }
  TEST_MESSAGE ("Set heater on !");
  TEST_ASSERT_TRUE (panel->setHeater ());
  TEST_ASSERT_TRUE (panel->isFilterOn());
  TEST_MESSAGE ("Set heater off !");
  TEST_ASSERT_FALSE (panel->setHeater (false));
  panel->setFilter (false);
  delay (5000);
}

void test_watertemp () {

  unsigned long t = millis();
  waterTemp = panel->waitForWaterTemp();
  if (waterTemp != UnsetValue16) {

    t = millis() - t;
    Serial.printf ("waterTemp: %d'C (response time %lu ms)\n", waterTemp, t);
  }
  TEST_ASSERT (waterTemp != UnsetValue16);
}

void test_get_desiredtemp () {

  unsigned long t = millis();
  desiredTemp = panel->waitForDesiredTemp();
  if (desiredTemp != UnsetValue16) {

    t = millis() - t;
    Serial.printf ("desiredTemp: %d'C (response time %lu ms)\n", desiredTemp, t);
  }
  panel->waitUntilDisplayBlink();
}

void test_set_desiredtemp () {

  TEST_ASSERT_FALSE (panel->setDesiredTemp (10));
  TEST_ASSERT_FALSE (panel->setDesiredTemp (50));
  TEST_ASSERT_TRUE (panel->setDesiredTemp (38));
  panel->waitUntilDisplayBlink();
  TEST_ASSERT_TRUE (panel->setDesiredTemp (22));
  panel->waitUntilDisplayBlink();
}

void test_power_off () {

  TEST_ASSERT_FALSE (panel->setPower (false));
}

void setup() {
  DLED_INIT();
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay (2000);

  UNITY_BEGIN();    // IMPORTANT LINE!
}

bool done = false;

void loop() {

  if (!done) {

    RUN_TEST (test_constructor);
    RUN_TEST (test_getters);
    RUN_TEST (test_begin);
    RUN_TEST (test_power_on);

    RUN_TEST (test_watertemp);

    RUN_TEST (test_get_desiredtemp);
    RUN_TEST (test_set_desiredtemp);

    RUN_TEST (test_filter);
    RUN_TEST (test_bubble);
    RUN_TEST (test_heater);

    RUN_TEST (test_power_off);

    UNITY_END(); // stop unit testing
    done = true;
  }
}
