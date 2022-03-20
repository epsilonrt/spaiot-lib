//
// Test de la led de test ESP (bleue)
//

#include <Arduino.h>
#include <unity.h>

#define LED_PIN LED_BUILTIN
#ifndef DEBUG_LED
#define DEBUG_LED LED_BUILTIN
#endif
#ifndef DEBUG_LED_ONSTATE
  #define LED_OFF HIGH
  #define LED_ON LOW
#else
  #if DEBUG_LED_ONSTATE == HIGH
    #define LED_OFF LOW
    #define LED_ON HIGH
  #else
    #define LED_OFF HIGH
    #define LED_ON LOW
  #endif
#endif

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

// Vérifie que le numéro de la led par défaut est LED_BUILTIN
void test_led_builtin_pin_number (void) {
  TEST_ASSERT_EQUAL (LED_BUILTIN, DEBUG_LED);
}

// Allume la led, et vérifie que la broche est bien à l'état correspondant
void test_led_state_high (void) {
  digitalWrite (DEBUG_LED, LED_ON);
  TEST_ASSERT_EQUAL (LED_ON, digitalRead (DEBUG_LED));
}

// Eteint la led et vérifie que la broche est bien à l'état correspondant
void test_led_state_low (void) {
  digitalWrite (DEBUG_LED, LED_OFF);
  TEST_ASSERT_EQUAL (LED_OFF, digitalRead (DEBUG_LED));
}

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay (2000);

  UNITY_BEGIN();    // IMPORTANT LINE!
  RUN_TEST (test_led_builtin_pin_number);
  
  // broche de la led en sortie
  pinMode (DEBUG_LED, OUTPUT);
}

uint8_t i = 0;
uint8_t max_blinks = 5;

void loop() {

  // Allume puis éteint la led max_blinks fois
  if (i < max_blinks) {
    RUN_TEST (test_led_state_high);
    delay (500);
    RUN_TEST (test_led_state_low);
    delay (500);
    i++;
  }
  else if (i == max_blinks) {
    UNITY_END(); // stop unit testing
  }
}
