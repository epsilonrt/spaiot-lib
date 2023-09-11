//
// Unit Test for the class SpaIot::FrameDecoder
// How to run:
// pio test -f test_09_framedecoder -v
// -v for verbose, if not specified only summary is printed (no TEST_MESSAGE, no TEST_PRINTF)
#include <unity.h>
#include <framedecoder.h>
#include <spaiotdebug.h>
#include "MyBoardSettings.h"

// Set to 1 to enable the loop() function that will print all changes
#define LOOP_ENABLED 0
// Set to 1 to enable the frame counter (only for debug, LOOP_ENABLED must be 1 too)
#define FRAME_COUNTER_ENABLED 0

using namespace SpaIot;

// My bus configuration
const BusSettings   &MyBus = SpaBus;
// My Leds
const std::map <int, LedSettings> &MyLeds =  SspLeds;

uint32_t frameCounter;
uint32_t frameDropped;
uint16_t errorValue;
uint16_t rawStatus;
uint16_t waterTemp;
uint16_t desiredTemp;
uint16_t sanitizerTime;
bool isSetupModeTriggered;

uint8_t   isPowerOn;
uint8_t   isFilterOn;
uint8_t   isBubbleOn;
uint8_t   isHeaterOn;
uint8_t   isHeatReached;
uint8_t   isJetOn;
uint8_t   isSanitizerOn;

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

FrameDecoder fd (MyBus, MyLeds);

void test_constructor () {

  TEST_ASSERT_FALSE (fd.isOpen());
}

void test_default_getters (FrameDecoder &f) {

  frameCounter = f.frameCounter();
  TEST_ASSERT_EQUAL (0, frameCounter);
  frameDropped =  f.frameDropped();
  TEST_ASSERT_EQUAL (0, frameDropped);
  errorValue = f.error();
  TEST_ASSERT_EQUAL (0, errorValue);
  rawStatus = f.rawStatus();
  TEST_ASSERT_EQUAL (UnsetValue16, rawStatus);
  waterTemp = f.waterTemp();
  TEST_ASSERT_EQUAL (UnsetValue16, waterTemp);
  desiredTemp = f.desiredTemp();
  TEST_ASSERT_EQUAL (UnsetValue16, desiredTemp);
  sanitizerTime = f.sanitizerTime();
  TEST_ASSERT_EQUAL (UnsetValue16, sanitizerTime);
  isSetupModeTriggered = f.isSetupModeTriggered();
  TEST_ASSERT_FALSE (isSetupModeTriggered);
  isPowerOn = f.isPowerOn();
  TEST_ASSERT_EQUAL (UnsetValue8, isPowerOn);
  isFilterOn = f.isFilterOn();
  TEST_ASSERT_EQUAL (UnsetValue8, isFilterOn);
  isBubbleOn = f.isBubbleOn();
  TEST_ASSERT_EQUAL (UnsetValue8, isBubbleOn);
  isHeaterOn = f.isHeaterOn();
  TEST_ASSERT_EQUAL (UnsetValue8, isHeaterOn);
  isHeatReached = f.isHeatReached();
  TEST_ASSERT_EQUAL (UnsetValue8, isHeatReached);
  if (f.hasLed (Jet)) {
    isJetOn = f.isJetOn();
    TEST_ASSERT_EQUAL (UnsetValue8, isJetOn);
  }
  if (f.hasLed (Sanitizer)) {
    isSanitizerOn = f.isSanitizerOn();
    TEST_ASSERT_EQUAL (UnsetValue8, isSanitizerOn);
  }
  isHeaterOn = f.isHeaterOn();
  TEST_ASSERT_EQUAL (UnsetValue8, isHeaterOn);
}

void test_decoder_getters () {

  TEST_ASSERT (MyBus == fd.busSettings());
  TEST_ASSERT (MyLeds == fd.ledSettings());
  test_default_getters (fd);
  for (const auto &led : MyLeds) {
    int key = led.first;
    TEST_ASSERT_TRUE (fd.hasLed (key));
    TEST_ASSERT_EQUAL (UnsetValue8, fd.isLedOn (key));
  }
}

void test_default_constructor() {
  FrameDecoder f;

  TEST_ASSERT_FALSE (f.isOpen());
  TEST_ASSERT_TRUE (f.busSettings().isEmpty());
  TEST_ASSERT_TRUE (f.ledSettings().empty());
  test_default_getters (f);
}

void test_begin_getters (FrameDecoder &f) {

  TEST_ASSERT_TRUE (f.isOpen ());
  delay (100);
  TEST_ASSERT (f.rawStatus() != UnsetValue16);

  // check if all leds are off
  for (const auto &led : MyLeds) {

    TEST_ASSERT_EQUAL (false, f.isLedOn (led.first));
  }

  isPowerOn = f.isPowerOn();
  TEST_ASSERT_EQUAL (false, isPowerOn);
  isFilterOn = f.isFilterOn();
  TEST_ASSERT_EQUAL (false, isFilterOn);
  isBubbleOn = f.isBubbleOn();
  TEST_ASSERT_EQUAL (false, isBubbleOn);
  isHeaterOn = f.isHeaterOn();
  TEST_ASSERT_EQUAL (false, isHeaterOn);
  isHeatReached = f.isHeatReached();
  TEST_ASSERT_EQUAL (false, isHeatReached);
  if (f.hasLed (Jet)) {
    isJetOn = f.isJetOn();
    TEST_ASSERT_EQUAL (false, isJetOn);
  }
  if (f.hasLed (Sanitizer)) {
    isSanitizerOn = f.isSanitizerOn();
    TEST_ASSERT_EQUAL (false, isSanitizerOn);
  }
  isHeaterOn = f.isHeaterOn();
  TEST_ASSERT_EQUAL (false, isHeaterOn);
}

void test_decoder_begin () {
  FrameDecoder &f = fd;

  f.begin();
  test_begin_getters (f);
}

void test_default_begin () {
  FrameDecoder f;

  f.begin (MyBus, MyLeds);
  test_begin_getters (f);
}

void setup() {
  SPAIOT_DBGLED_INIT();
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay (2000);
  TEST_MESSAGE ("<IMPORTANT> The spa MUST be OFF before running this test !");
  UNITY_BEGIN();    // IMPORTANT LINE!
  RUN_TEST (test_constructor);
  RUN_TEST (test_decoder_getters);
  RUN_TEST (test_default_constructor);
  RUN_TEST (test_default_begin);
  RUN_TEST (test_decoder_begin);
  #if ! LOOP_ENABLED
  UNITY_END(); // stop unit testing
  #endif
}

void loop() {
  #if LOOP_ENABLED
  uint32_t ul;
  uint16_t w;
  bool b;

  w = fd.rawStatus();
  if (rawStatus != w) {
    TEST_PRINTF ("raw:%X ", w);
    rawStatus = w;

    if (isPowerOn != fd.isPowerOn()) {

      isPowerOn = fd.isPowerOn();
      TEST_PRINTF ("Power:%d ", isPowerOn);
    }
    if (isFilterOn != fd.isFilterOn()) {

      isFilterOn = fd.isFilterOn();
      TEST_PRINTF ("Filter:%d ", isFilterOn);
    }
    if (isBubbleOn != fd.isBubbleOn()) {

      isBubbleOn = fd.isBubbleOn();
      TEST_PRINTF ("Bubble:%d ", isBubbleOn);
    }
    if (isHeaterOn != fd.isHeaterOn()) {

      isHeaterOn = fd.isHeaterOn();
      TEST_PRINTF ("Heater:%d ", isHeaterOn);
    }
    if (isHeatReached != fd.isHeatReached()) {

      isHeatReached = fd.isHeatReached();
      TEST_PRINTF ("HeatReached:%d ", isHeatReached);
    }
    if (fd.hasLed (Jet)) {
      if (isJetOn != fd.isJetOn()) {

        isJetOn = fd.isJetOn();
        TEST_PRINTF ("Jet:%d ", isJetOn);
      }
    }
    if (fd.hasLed (Sanitizer)) {
      if (isSanitizerOn != fd.isSanitizerOn()) {

        isSanitizerOn = fd.isSanitizerOn();
        TEST_PRINTF ("Sanitizer:%d ", isSanitizerOn);
      }
    }
    if (isHeaterOn != fd.isHeaterOn()) {

      isHeaterOn = fd.isHeaterOn();
      TEST_PRINTF ("Heating:%d ", isHeaterOn);
    }
  }

  w = fd.waterTemp();
  if (waterTemp != w) {
    TEST_PRINTF ("water:%u ", w);
    waterTemp = w;
  }

  w = fd.desiredTemp();
  if (desiredTemp != w) {
    TEST_PRINTF ("desired:%u ", w);
    desiredTemp  = w;
  }

  w = fd.sanitizerTime();
  if (sanitizerTime != w) {
    TEST_PRINTF ("set:%u ", w);
    sanitizerTime = w;
  }

  b = fd.isSetupModeTriggered();
  if (isSetupModeTriggered != b) {
    TEST_PRINTF ("setup:%u ", b);
    isSetupModeTriggered = b;
  }

  #if FRAME_COUNTER_ENABLED
  ul = fd.frameCounter();
  if (ul != frameCounter) {
    TEST_PRINTF ("frame:%u ", ul);
    frameCounter = ul;
  }
  #endif

  ul =  fd.frameDropped();
  if (frameDropped != ul) {
    TEST_PRINTF ("dropped:%u ", ul);
    frameDropped = ul;
  }

  w = fd.error();
  if (errorValue != w) {
    TEST_PRINTF ("error:%u", w);
    errorValue = w;
  }

  #endif
  delay (1000);
}
