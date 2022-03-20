//
// Unit Test for the class SpaIot::ControlPanel
//
// We use a DIY board connected to a SSP spa.

#include <Arduino.h>
#include <unity.h>
#include <controlpanel.h>
#include <spaiotdebug.h>

using namespace SpaIot;

//#define DISABLE_DESIRED_TEMP

// My bus configuration :
#if defined(ESP8266)
// SDATA  -> GPIO12
// SCLK   -> GPIO14
// nWR    -> GPIO13
const BusSettings busSettings (12, 14, 13);

// My button controllers
Cd4051 MuxA ("U3", 5, 4, 15, 16); // A->GPIO5, B->GPIO4, C->GPIO15, INH->GPIO16
Cd4051 MuxB ("U4", 5, 4, 15, 0);  // A->GPIO5, B->GPIO4, C->GPIO15, INH->GPIO0

#elif defined(ESP32)
// SDATA  -> GPIO23
// SCLK   -> GPIO18
// nWR    -> GPIO19
const BusSettings busSettings (23, 18, 19);

// My button controllers
Cd4051 MuxA ("U3", 27, 16, 17, 25); // A->GPIO27, B->GPIO16, C->GPIO17, INH->GPIO25
Cd4051 MuxB ("U4", 27, 16, 17, 26); // A->GPIO27, B->GPIO16, C->GPIO17, INH->GPIO26

#else
#error unsupported platform
#endif

const std::map<int, LedSettings> & ledSettings        = SspLeds;

// My buttons configuration (SSP)
const std::map<int, ButtonSettings> buttonSettings = {
  { Filter,   ButtonSettings (MuxA, 1) },  // Filter   -> A1
  { Bubble,   ButtonSettings (MuxA, 3) },  // Bubble   -> A3
  { TempDown, ButtonSettings (MuxA, 7) },  // TempDown -> A7

  { Power,    ButtonSettings (MuxB, 2) },  // Power    -> B2
  { TempUp,   ButtonSettings (MuxB, 4) },  // TempUp   -> B4
  { TempUnit, ButtonSettings (MuxB, 5) },  // TempUnit -> B5
  { Heater,   ButtonSettings (MuxB, 7) }   // Heater   -> B7
};

// My custom configuration
const HardwareSettings hwSettings (busSettings, ledSettings, buttonSettings);

ControlPanel & panel = ControlPanel::singleton (hwSettings);

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
  SPAIOT_DBG ("---> test_constructor <---");

  TEST_ASSERT_FALSE (panel.isOpened());
}


void test_getters () {
  SPAIOT_DBG ("---> test_getters <---");

  for (const auto& elmt : buttonSettings) {
    const int key = elmt.first;
    const ButtonSettings & settings = elmt.second;

    TEST_ASSERT_TRUE (panel.hasButton (key));

    const Button & button = panel.button (key);
    const Button & button2 = panel.buttons().at (key);

    TEST_ASSERT (button == button2);
    TEST_ASSERT (button.settings() == settings);
  }

  TEST_ASSERT (ledSettings == panel.ledSettings());
  for (const auto& elmt : panel.ledSettings()) {
    const int key = elmt.first;

    TEST_ASSERT_TRUE (panel.hasLed (key));
  }

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
  SPAIOT_DBG ("---> test_begin <---");

  panel.begin();
  TEST_ASSERT_TRUE (panel.isOpened ());
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
  SPAIOT_DBG ("---> test_power_on <---");

  if (panel.isPowerOn()) {

    TEST_ASSERT_FALSE (panel.setPower (false));
  }

  SPAIOT_DBG ("Set power on !");
  TEST_ASSERT_TRUE (panel.setPower ());
}

void test_filter () {

  SPAIOT_DBG ("---> test_filter <---");

  if (panel.isFilterOn()) {

    TEST_ASSERT_FALSE (panel.setFilter (false));
    delay (1000);
  }

  SPAIOT_DBG ("Set filter on !");
  TEST_ASSERT_TRUE (panel.setFilter ());
  delay (500);

  SPAIOT_DBG ("Set filter off !");
  TEST_ASSERT_FALSE (panel.setFilter (false));
}

void test_bubble () {

  SPAIOT_DBG ("---> test_bubble <---");
  if (panel.isBubbleOn()) {

    TEST_ASSERT_FALSE (panel.setBubble (false));
    delay (1000);
  }

  SPAIOT_DBG ("Set bubble on !");
  TEST_ASSERT_TRUE (panel.setBubble ());
  delay (500);

  SPAIOT_DBG ("Set bubble off !");
  TEST_ASSERT_FALSE (panel.setBubble (false));
}

void test_heater () {
  unsigned long t;

  SPAIOT_DBG ("---> test_heater <---");
  if (panel.isHeaterOn()) {

    TEST_ASSERT_FALSE (panel.setHeater (false));
    delay (1000);
  }

  SPAIOT_DBG ("Set heater on !");
  t = millis();
  TEST_ASSERT_TRUE (panel.setHeater ());
  SPAIOT_DBG ("execution time: %ld\n", millis() - t);
  TEST_ASSERT_TRUE (panel.isFilterOn());
  delay (500);

  SPAIOT_DBG ("Set heater off !");
  t = millis();
  TEST_ASSERT_FALSE (panel.setHeater (false));
  SPAIOT_DBG ("execution time: %ld\n", millis() - t);
  
  SPAIOT_DBG ("Set filter off !");
  TEST_ASSERT_FALSE (panel.setFilter (false));
}

void test_watertemp () {
  SPAIOT_DBG ("---> test_watertemp <---");

  unsigned long t = millis();
  waterTemp = panel.waitForWaterTemp();
  if (waterTemp != UnsetValue16) {

    t = millis() - t;
    SPAIOT_DBG ("waterTemp: %d'C (response time %lu ms)\n", waterTemp, t);
  }
  TEST_ASSERT (waterTemp != UnsetValue16);
}

void test_get_desiredtemp () {
  SPAIOT_DBG ("---> test_get_desiredtemp <---");

#ifndef DISABLE_DESIRED_TEMP
  unsigned long t = millis();
  desiredTemp = panel.waitForDesiredTemp();
  if (desiredTemp != UnsetValue16) {

    t = millis() - t;
    SPAIOT_DBG ("desiredTemp: %d'C (response time %lu ms)\n", desiredTemp, t);
  }
  panel.waitUntilDisplayBlink();
#else
  TEST_IGNORE_MESSAGE ("DISABLE_DESIRED_TEMP defined, nothing was done");
#endif
}

void test_set_desiredtemp () {
  SPAIOT_DBG ("---> test_set_desiredtemp <---");

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
  SPAIOT_DBG ("---> test_power_off <---");

  SPAIOT_DBG ("Set heater off !");
  TEST_ASSERT_FALSE (panel.setPower (false));
}

void setup() {
  SPAIOT_DBGLED_INIT();
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
