//
// Unit Test for the class SpaIot::ControlPanel
//
// How to run:
// pio test -f test_10_controlpanel -v
// -v for verbose, if not specified only summary is printed (no TEST_MESSAGE, no TEST_PRINTF)
// We use a DIY board connected to a SSP spa.
#include <spaiot_test.h>
#include <config/hwconfig.h>
#include <controlpanel.h>

using namespace SpaIot;

const std::map<int, ButtonSettings>  & buttonSettings = DefaultConfig.buttons();
const std::map <int, LedSettings> & ledSettings =  DefaultConfig.leds();

//#define DISABLE_DESIRED_TEMP

ControlPanel panel;

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

void setUp (void) {

}

// void tearDown(void) {
// // clean stuff up here
// }

void test_constructor () {

  TEST_ASSERT_FALSE (panel.isOpened());
}


void test_getters () {

  frameCounter = panel.frameCounter();
  TEST_ASSERT_EQUAL (0, frameCounter);
  frameDropped =  panel.frameDropped();
  TEST_ASSERT_EQUAL (0, frameDropped);
  errorValue = panel.error();
  TEST_ASSERT_EQUAL (0, errorValue);
  rawStatus = panel.rawStatus();
  TEST_ASSERT_EQUAL (UnsetValue16, rawStatus);
  waterTemp = panel.waterTemp();
  TEST_ASSERT_EQUAL (UnsetValue16, waterTemp);
  desiredTemp = panel.desiredTemp();
  TEST_ASSERT_EQUAL (UnsetValue16, desiredTemp);
  sanitizerTime = panel.sanitizerTime();
  TEST_ASSERT_EQUAL (UnsetValue16, sanitizerTime);
  isSetupModeTriggered = panel.isSetupModeTriggered();
  TEST_ASSERT_FALSE (isSetupModeTriggered);

  for (const auto& elmt : panel.ledSettings()) {
    const int key = elmt.first;

    TEST_ASSERT_EQUAL (UnsetValue8, panel.isLedOn (key));
  }

  isPowerOn = panel.isPowerOn();
  TEST_ASSERT_EQUAL (UnsetValue8, isPowerOn);
  isFilterOn = panel.isFilterOn();
  TEST_ASSERT_EQUAL (UnsetValue8, isFilterOn);
  isBubbleOn = panel.isBubbleOn();
  TEST_ASSERT_EQUAL (UnsetValue8, isBubbleOn);
  isHeaterOn = panel.isHeaterOn();
  TEST_ASSERT_EQUAL (UnsetValue8, isHeaterOn);
  isHeatReached = panel.isHeatReached();
  TEST_ASSERT_EQUAL (UnsetValue8, isHeatReached);
  if (panel.hasLed (Jet)) {
    isJetOn = panel.isJetOn();
    TEST_ASSERT_EQUAL (UnsetValue8, isJetOn);
  }
  if (panel.hasLed (Sanitizer)) {
    isSanitizerOn = panel.isSanitizerOn();
    TEST_ASSERT_EQUAL (UnsetValue8, isSanitizerOn);
  }
}

void test_begin () {
  TEST_MESSAGE ("---> test_begin <---");
  panel.begin (DefaultConfig);
  TEST_ASSERT_TRUE (panel.isOpened ());

  TEST_ASSERT (buttonSettings == panel.buttonSettings());
  TEST_ASSERT (ledSettings == panel.ledSettings());

  for (const auto& elmt : panel.ledSettings()) {
    const int key = elmt.first;

    TEST_ASSERT_TRUE (panel.hasLed (key));
  }

  for (const auto& elmt : panel.buttonSettings()) {
    const int key = elmt.first;

    TEST_ASSERT_TRUE (panel.hasButton (key));
  }

  delay (100);
  TEST_ASSERT (panel.rawStatus() != UnsetValue16);
  for (const auto& elmt : SspLeds) {
    const int key = elmt.first;

    TEST_ASSERT_FALSE (panel.isLedOn (key));
  }
  isPowerOn = panel.isPowerOn();
  TEST_ASSERT_FALSE (isPowerOn);
  isFilterOn = panel.isFilterOn();
  TEST_ASSERT_FALSE (isFilterOn);
  isBubbleOn = panel.isBubbleOn();
  TEST_ASSERT_FALSE (isBubbleOn);
  isHeaterOn = panel.isHeaterOn();
  TEST_ASSERT_FALSE (isHeaterOn);
  isHeatReached = panel.isHeatReached();
  TEST_ASSERT_FALSE (isHeatReached);
  if (panel.hasLed (Jet)) {
    isJetOn = panel.isJetOn();
    TEST_ASSERT_FALSE (isJetOn);
  }
  if (panel.hasLed (Sanitizer)) {
    isSanitizerOn = panel.isSanitizerOn();
    TEST_ASSERT_FALSE (isSanitizerOn);
  }
}

void test_power_on () {
  TEST_MESSAGE ("---> test_power_on <---");

  if (panel.isPowerOn()) {

    TEST_ASSERT_FALSE (panel.setPower (false));
  }

  TEST_MESSAGE ("Set power on !");
  TEST_ASSERT_TRUE (panel.setPower ());
}

void test_filter () {

  TEST_MESSAGE ("---> test_filter <---");

  if (panel.isFilterOn()) {

    TEST_ASSERT_FALSE (panel.setFilter (false));
    delay (1000);
  }

  TEST_MESSAGE ("Set filter on !");
  TEST_ASSERT_TRUE (panel.setFilter ());
  delay (500);

  TEST_MESSAGE ("Set filter off !");
  TEST_ASSERT_FALSE (panel.setFilter (false));
}

void test_bubble () {

  TEST_MESSAGE ("---> test_bubble <---");
  if (panel.isBubbleOn()) {

    TEST_ASSERT_FALSE (panel.setBubble (false));
    delay (1000);
  }

  TEST_MESSAGE ("Set bubble on !");
  TEST_ASSERT_TRUE (panel.setBubble ());
  delay (500);

  TEST_MESSAGE ("Set bubble off !");
  TEST_ASSERT_FALSE (panel.setBubble (false));
}

void test_heater () {
  unsigned long t;

  TEST_MESSAGE ("---> test_heater <---");
  if (panel.isHeaterOn()) {

    TEST_ASSERT_FALSE (panel.setHeater (false));
    delay (1000);
  }

  TEST_MESSAGE ("Set heater on !");
  t = millis();
  TEST_ASSERT_TRUE (panel.setHeater ());
  TEST_PRINTF ("execution time: %d", millis() - t);
  TEST_ASSERT_TRUE (panel.isFilterOn());
  delay (500);

  TEST_MESSAGE ("Set heater off !");
  t = millis();
  TEST_ASSERT_FALSE (panel.setHeater (false));
  TEST_PRINTF ("execution time: %d", millis() - t);

  TEST_MESSAGE ("Set filter off !");
  TEST_ASSERT_FALSE (panel.setFilter (false));
}

void test_watertemp () {
  TEST_MESSAGE ("---> test_watertemp <---");

  unsigned long t = millis();
  waterTemp = panel.waitForWaterTemp();
  if (waterTemp != UnsetValue16) {

    t = millis() - t;
    TEST_PRINTF ("waterTemp: %d'C (response time %u ms)", waterTemp, t);
  }
  TEST_ASSERT (waterTemp != UnsetValue16);
}

void test_get_desiredtemp () {
  TEST_MESSAGE ("---> test_get_desiredtemp <---");

#ifndef DISABLE_DESIRED_TEMP
  unsigned long t = millis();
  desiredTemp = panel.waitForDesiredTemp();
  if (desiredTemp != UnsetValue16) {

    t = millis() - t;
    TEST_PRINTF ("desiredTemp: %d'C (response time %u ms)", desiredTemp, t);
  }
  panel.waitUntilDisplayBlink();
#else
  TEST_IGNORE_MESSAGE ("DISABLE_DESIRED_TEMP defined, nothing was done");
#endif
}

void test_set_desiredtemp () {
  TEST_MESSAGE ("---> test_set_desiredtemp <---");

#ifndef DISABLE_DESIRED_TEMP
  TEST_ASSERT_FALSE (panel.setDesiredTemp (10));
  TEST_ASSERT_FALSE (panel.setDesiredTemp (50));

  TEST_ASSERT_TRUE (panel.setDesiredTemp (38));
  TEST_ASSERT_EQUAL (panel.desiredTemp(), 38);
  panel.waitUntilDisplayBlink();

  TEST_ASSERT_TRUE (panel.setDesiredTemp (22));
  TEST_ASSERT_EQUAL (panel.desiredTemp(), 22);
  panel.waitUntilDisplayBlink();
#else
  TEST_IGNORE_MESSAGE ("DISABLE_DESIRED_TEMP defined, nothing was done");
#endif
}

void test_power_off () {
  TEST_MESSAGE ("---> test_power_off <---");

  TEST_MESSAGE ("Set heater off !");
  TEST_ASSERT_FALSE (panel.setPower (false));
}

void setup() {
  SPAIOT_DBGLED_INIT();
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
    RUN_TEST (test_getters);

    RUN_TEST (test_begin);
    delay (5000);

    RUN_TEST (test_power_on);
    delay (5000);

    RUN_TEST (test_watertemp);
    delay (5000);

    RUN_TEST (test_get_desiredtemp);
    RUN_TEST (test_set_desiredtemp);
    delay (5000);

    RUN_TEST (test_filter);
    delay (5000);

    RUN_TEST (test_bubble);
    delay (5000);

    RUN_TEST (test_heater);
    delay (5000);

    RUN_TEST (test_power_off);

    UNITY_END(); // stop unit testing
    done = true;
  }
}
