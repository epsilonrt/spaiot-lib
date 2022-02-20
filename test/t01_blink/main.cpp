//
// Test de la led du module ESP (bleue)
//

#include <Arduino.h>
#include <unity.h>

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

// Vérifie que le numéro de la led par défaut est 2
void test_led_builtin_pin_number (void) {
  TEST_ASSERT_EQUAL (2, LED_BUILTIN);
}

// Allume la led, active à l'état bas et vérifie que la broche est bien à l'état bas
void test_led_state_high (void) {
  digitalWrite (LED_BUILTIN, LOW);
  TEST_ASSERT_EQUAL (LOW, digitalRead (LED_BUILTIN));
}

// Eteint la led en mettant la broche à l'état haut et vérifie que la broche est bien à l'état haut
void test_led_state_low (void) {
  digitalWrite (LED_BUILTIN, HIGH);
  TEST_ASSERT_EQUAL (HIGH, digitalRead (LED_BUILTIN));
}

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay (2000);

  UNITY_BEGIN();    // IMPORTANT LINE!
  RUN_TEST (test_led_builtin_pin_number);
  
  // broche de la led en sortie
  pinMode (LED_BUILTIN, OUTPUT);
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