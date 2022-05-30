//
// Unit Test for the class SpaIot::FrameDecoder
//
#include <spaiot_test.h>
#include <config/hwconfig.h>
#include <framedecoder.h>

// Uncomment to display the frame data 
//#define LOOP_ENABLED 1

using namespace SpaIot;

// My bus configuration
const BusSettings  & MyBus = DefaultConfig.bus() ;
// My Leds
const std::map <int, LedSettings> & MyLeds =  DefaultConfig.leds();

class TestFrameDecoder : public FrameDecoder {
  public:
    TestFrameDecoder (const BusSettings & bus,
                      const std::map <int, LedSettings> & leds) :
      FrameDecoder (bus, leds) {}
    TestFrameDecoder () : FrameDecoder () {}
};

TestFrameDecoder * decoder;
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

void test_constructor () {
  TestFrameDecoder bd;

  decoder = new TestFrameDecoder (MyBus, MyLeds);
  TEST_ASSERT_FALSE (decoder->isOpened());
}

void test_default_getters (TestFrameDecoder & bd) {

  frameCounter = bd.frameCounter();
  TEST_ASSERT_EQUAL (0, frameCounter);
  frameDropped =  bd.frameDropped();
  TEST_ASSERT_EQUAL (0, frameDropped);
  errorValue = bd.error();
  TEST_ASSERT_EQUAL (0, errorValue);
  rawStatus = bd.rawStatus();
  TEST_ASSERT_EQUAL (UnsetValue16, rawStatus);
  waterTemp = bd.waterTemp();
  TEST_ASSERT_EQUAL (UnsetValue16, waterTemp);
  desiredTemp = bd.desiredTemp();
  TEST_ASSERT_EQUAL (UnsetValue16, desiredTemp);
  sanitizerTime = bd.sanitizerTime();
  TEST_ASSERT_EQUAL (UnsetValue16, sanitizerTime);
  isSetupModeTriggered = bd.isSetupModeTriggered();
  TEST_ASSERT_FALSE (isSetupModeTriggered);
  isPowerOn = bd.isPowerOn();
  TEST_ASSERT_EQUAL (UnsetValue8, isPowerOn);
  isFilterOn = bd.isFilterOn();
  TEST_ASSERT_EQUAL (UnsetValue8, isFilterOn);
  isBubbleOn = bd.isBubbleOn();
  TEST_ASSERT_EQUAL (UnsetValue8, isBubbleOn);
  isHeaterOn = bd.isHeaterOn();
  TEST_ASSERT_EQUAL (UnsetValue8, isHeaterOn);
  isHeatReached = bd.isHeatReached();
  TEST_ASSERT_EQUAL (UnsetValue8, isHeatReached);
  if (bd.hasLed (Jet)) {
    isJetOn = bd.isJetOn();
    TEST_ASSERT_EQUAL (UnsetValue8, isJetOn);
  }
  if (bd.hasLed (Sanitizer)) {
    isSanitizerOn = bd.isSanitizerOn();
    TEST_ASSERT_EQUAL (UnsetValue8, isSanitizerOn);
  }
  isHeaterOn = bd.isHeaterOn();
  TEST_ASSERT_EQUAL (UnsetValue8, isHeaterOn);
}

void test_decoder_getters () {
  TestFrameDecoder & bd = *decoder;

  TEST_ASSERT (MyBus == bd.busSettings());
  TEST_ASSERT (MyLeds == bd.ledSettings());
  test_default_getters (bd);
  for (const auto & led : MyLeds) {
    int key = led.first;
    TEST_ASSERT_TRUE (bd.hasLed (key));
    TEST_ASSERT_EQUAL (UnsetValue8, bd.isLedOn (key));
  }
}

void test_default_constructor() {
  TestFrameDecoder bd;

  TEST_ASSERT_FALSE (bd.isOpened());
  TEST_ASSERT_TRUE (bd.busSettings().isNull());
  TEST_ASSERT_TRUE (bd.ledSettings().empty());
  test_default_getters (bd);
}

void test_begin_getters (TestFrameDecoder & bd) {

  TEST_ASSERT_TRUE (bd.isOpened ());
  delay (100);
  TEST_ASSERT (bd.rawStatus() != UnsetValue16);

  // check if all leds are off
  for (const auto & led : MyLeds) {

    TEST_ASSERT_EQUAL (false, bd.isLedOn (led.first));
  }

  isPowerOn = bd.isPowerOn();
  TEST_ASSERT_EQUAL (false, isPowerOn);
  isFilterOn = bd.isFilterOn();
  TEST_ASSERT_EQUAL (false, isFilterOn);
  isBubbleOn = bd.isBubbleOn();
  TEST_ASSERT_EQUAL (false, isBubbleOn);
  isHeaterOn = bd.isHeaterOn();
  TEST_ASSERT_EQUAL (false, isHeaterOn);
  isHeatReached = bd.isHeatReached();
  TEST_ASSERT_EQUAL (false, isHeatReached);
  if (bd.hasLed (Jet)) {
    isJetOn = bd.isJetOn();
    TEST_ASSERT_EQUAL (false, isJetOn);
  }
  if (bd.hasLed (Sanitizer)) {
    isSanitizerOn = bd.isSanitizerOn();
    TEST_ASSERT_EQUAL (false, isSanitizerOn);
  }
  isHeaterOn = bd.isHeaterOn();
  TEST_ASSERT_EQUAL (false, isHeaterOn);
}

void test_decoder_begin () {
  TestFrameDecoder & bd = *decoder;

  bd.begin();
  test_begin_getters (bd);
}

void test_default_begin () {
  TestFrameDecoder bd;

  bd.begin (MyBus, MyLeds);
  test_begin_getters (bd);
}

void setup() {
  SPAIOT_DBGLED_INIT();
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay (2000);
  TEST_PRINTF ("<IMPORTANT> The spa MUST be OFF before running this test !\n");
  UNITY_BEGIN();    // IMPORTANT LINE!
  RUN_TEST (test_constructor);
  RUN_TEST (test_decoder_getters);
  RUN_TEST (test_default_constructor);
  RUN_TEST (test_default_begin);
  RUN_TEST (test_decoder_begin);
#ifndef LOOP_ENABLED
  UNITY_END(); // stop unit testing
#endif
}

void loop() {
#ifdef LOOP_ENABLED
  uint32_t ul;
  uint16_t w;
  bool b;

  TEST_PRINTF ("> ");
  w = decoder->rawStatus();
  if (rawStatus != w) {
    TEST_PRINTF ("raw:%04X ", w);
    rawStatus = w;

    if (isPowerOn != decoder->isPowerOn()) {

      isPowerOn = decoder->isPowerOn();
      TEST_PRINTF ("Power:%d ", isPowerOn);
    }
    if (isFilterOn != decoder->isFilterOn()) {

      isFilterOn = decoder->isFilterOn();
      TEST_PRINTF ("Filter:%d ", isFilterOn);
    }
    if (isBubbleOn != decoder->isBubbleOn()) {

      isBubbleOn = decoder->isBubbleOn();
      TEST_PRINTF ("Bubble:%d ", isBubbleOn);
    }
    if (isHeaterOn != decoder->isHeaterOn()) {

      isHeaterOn = decoder->isHeaterOn();
      TEST_PRINTF ("Heater:%d ", isHeaterOn);
    }
    if (isHeatReached != decoder->isHeatReached()) {

      isHeatReached = decoder->isHeatReached();
      TEST_PRINTF ("HeatReached:%d ", isHeatReached);
    }
    if (decoder->hasLed (Jet)) {
      if (isJetOn != decoder->isJetOn()) {

        isJetOn = decoder->isJetOn();
        TEST_PRINTF ("Jet:%d ", isJetOn);
      }
    }
    if (decoder->hasLed (Sanitizer)) {
      if (isSanitizerOn != decoder->isSanitizerOn()) {

        isSanitizerOn = decoder->isSanitizerOn();
        TEST_PRINTF ("Sanitizer:%d ", isSanitizerOn);
      }
    }
    if (isHeaterOn != decoder->isHeaterOn()) {

      isHeaterOn = decoder->isHeaterOn();
      TEST_PRINTF ("Heating:%d ", isHeaterOn);
    }
  }

  w = decoder->waterTemp();
  if (waterTemp != w) {
    TEST_PRINTF ("water:%u ", w);
    waterTemp = w;
  }

  w = decoder->desiredTemp();
  if (desiredTemp != w) {
    TEST_PRINTF ("desired:%u ", w);
    desiredTemp  = w;
  }

  w = decoder->sanitizerTime();
  if (sanitizerTime != w) {
    TEST_PRINTF ("set:%u ", w);
    sanitizerTime = w;
  }

  b = decoder->isSetupModeTriggered();
  if (isSetupModeTriggered != b) {
    TEST_PRINTF ("setup:%u ", b);
    isSetupModeTriggered = b;
  }

  ul = decoder->frameCounter();
  if (ul != frameCounter) {
    TEST_PRINTF ("frame:%u ", ul);
    frameCounter = ul;
  }

  ul =  decoder->frameDropped();
  if (frameDropped != ul) {
    TEST_PRINTF ("dropped:%u ", ul);
    frameDropped = ul;
  }

  w = decoder->error();
  if (errorValue != w) {
    TEST_PRINTF ("error:%u", w);
    errorValue = w;
  }

  TEST_PRINTF ("\n");
#endif
  delay (1000);
}
