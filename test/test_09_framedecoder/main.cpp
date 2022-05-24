//
// Unit Test for the class SpaIot::FrameDecoder
//
// We use a DIY board connected to a SSP spa, the configuration  is as follows:
//
//  const BusSettings MyBus (12, 14, 13); // for ESP8266
//  const BusSettings MyBus (18, 16, 17); // for ESP32
//
//  const std::map<int, LedSettings> SspLeds = {
//    { Power,          LedSettings (0) },
//    { Heater,         LedSettings (7) },
//    { HeatReached,    LedSettings (9) },
//    { Bubble,         LedSettings (10) },
//    { Filter,         LedSettings (12) }
//  };
#include <Arduino.h>
#include <unity.h>
#include <framedecoder.h>
#include <spaiotdebug.h>

//#define LOOP_ENABLED 1


using namespace SpaIot;

// My bus configuration :
#if defined(ESP8266)
// SDATA  -> GPIO12
// SCLK   -> GPIO14
// nWR    -> GPIO13
const BusSettings MyBus (12, 14, 13);

#elif defined(ESP32)
// SDATA  -> GPIO23
// SCLK   -> GPIO18
// nWR    -> GPIO19
const BusSettings MyBus (23, 18, 19);

#else
#error unsupported platform
#endif



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

  decoder = new TestFrameDecoder (MyBus, SspLeds);
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
  TEST_ASSERT (SspLeds == bd.ledSettings());
  test_default_getters (bd);
  for (const auto & led : SspLeds) {
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
  for (const auto & led : SspLeds) {

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
  
  bd.begin(MyBus, SspLeds);
  test_begin_getters (bd);
}

void setup() {
  SPAIOT_DBGLED_INIT();
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay (2000);
  TEST_ASSERT_MESSAGE (true, "<IMPORTANT> The spa MUST be OFF.");
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

  Serial.print ("> ");
  w = decoder->rawStatus();
  if (rawStatus != w) {
    Serial.printf ("raw:%04X ", w);
    rawStatus = w;

    if (isPowerOn != decoder->isPowerOn()) {

      isPowerOn = decoder->isPowerOn();
      Serial.printf ("Power:%d ", isPowerOn);
    }
    if (isFilterOn != decoder->isFilterOn()) {

      isFilterOn = decoder->isFilterOn();
      Serial.printf ("Filter:%d ", isFilterOn);
    }
    if (isBubbleOn != decoder->isBubbleOn()) {

      isBubbleOn = decoder->isBubbleOn();
      Serial.printf ("Bubble:%d ", isBubbleOn);
    }
    if (isHeaterOn != decoder->isHeaterOn()) {

      isHeaterOn = decoder->isHeaterOn();
      Serial.printf ("Heater:%d ", isHeaterOn);
    }
    if (isHeatReached != decoder->isHeatReached()) {

      isHeatReached = decoder->isHeatReached();
      Serial.printf ("HeatReached:%d ", isHeatReached);
    }
    if (decoder->hasLed (Jet)) {
      if (isJetOn != decoder->isJetOn()) {

        isJetOn = decoder->isJetOn();
        Serial.printf ("Jet:%d ", isJetOn);
      }
    }
    if (decoder->hasLed (Sanitizer)) {
      if (isSanitizerOn != decoder->isSanitizerOn()) {

        isSanitizerOn = decoder->isSanitizerOn();
        Serial.printf ("Sanitizer:%d ", isSanitizerOn);
      }
    }
    if (isHeaterOn != decoder->isHeaterOn()) {

      isHeaterOn = decoder->isHeaterOn();
      Serial.printf ("Heating:%d ", isHeaterOn);
    }
  }

  w = decoder->waterTemp();
  if (waterTemp != w) {
    Serial.printf ("water:%u ", w);
    waterTemp = w;
  }

  w = decoder->desiredTemp();
  if (desiredTemp != w) {
    Serial.printf ("desired:%u ", w);
    desiredTemp  = w;
  }

  w = decoder->sanitizerTime();
  if (sanitizerTime != w) {
    Serial.printf ("set:%u ", w);
    sanitizerTime = w;
  }

  b = decoder->isSetupModeTriggered();
  if (isSetupModeTriggered != b) {
    Serial.printf ("setup:%u ", b);
    isSetupModeTriggered = b;
  }

  ul = decoder->frameCounter();
  if (ul != frameCounter) {
    Serial.printf ("frame:%u ", ul);
    frameCounter = ul;
  }

  ul =  decoder->frameDropped();
  if (frameDropped != ul) {
    Serial.printf ("dropped:%u ", ul);
    frameDropped = ul;
  }

  w = decoder->error();
  if (errorValue != w) {
    Serial.printf ("error:%u", w);
    errorValue = w;
  }

  Serial.println ("");
#endif
  delay (1000);
}
