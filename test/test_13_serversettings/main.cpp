//
// Unit Test for the class SpaIot::ServerSettings
// How to run:
// pio test -f test_13_serversettings -v
// -v for verbose, if not specified only summary is printed (no TEST_MESSAGE, no TEST_PRINTF)
//
#include <Arduino.h>
#include <unity.h>
#include <serversettings.h>

using namespace SpaIot;


// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_constructor_null (void) {
  ServerSettings s;

  TEST_ASSERT_TRUE (s.isEmpty());
  TEST_ASSERT_FALSE (s.isNull());
}

void test_constructor_notnull (void) {
  ServerSettings s ("TestSpa", 10000);

  TEST_ASSERT_FALSE (s.isEmpty());
}

void test_getters (void) {
  ServerSettings s ("TestSpa", 10000);

  TEST_ASSERT_EQUAL_STRING ("TestSpa", s.spaModel().c_str());
  TEST_ASSERT_EQUAL (10000, s.publishInterval());
}

void test_setters (void) {
  ServerSettings s;

  s.setSpaModel ("TestSpa");
  TEST_ASSERT_EQUAL_STRING ("TestSpa", s.spaModel().c_str());
  s.setPublishInterval (10000);
  TEST_ASSERT_EQUAL (10000, s.publishInterval());

  s.clear();
  TEST_ASSERT_TRUE (s.isEmpty());
}

void test_comparison (void) {
  ServerSettings s1 ("TestSpa", 10000);
  ServerSettings s2 ("TestSpa", 10000);
  ServerSettings s3 ("TestSpa", 20000);
  ServerSettings s4 ("TestSpa2", 10000);

  TEST_ASSERT (s1 == s2);
  TEST_ASSERT (s1 != s3);
  TEST_ASSERT (s1 != s4);
}

void test_copy (void) {
  ServerSettings s1;
  ServerSettings s2 ("TestSpa", 10000);
  ServerSettings s3 (s2);

  s1 = s2;
  TEST_ASSERT (s2 == s1);
  TEST_ASSERT (s3 == s2);
}

void test_move (void) {
  const    ServerSettings s1 ("TestSpa", 10000);

  // Test move constructor
  ServerSettings s2 = s1;
  ServerSettings s3 = std::move (s2);
  TEST_ASSERT (s3 == s1);
  TEST_ASSERT_TRUE (s2.isNull());

  // Test move assignment
  s2 = std::move (s3);
  TEST_ASSERT (s2 == s1);
  TEST_ASSERT_TRUE (s3.isNull());

  s3.clear(); // check if clear() reset the d_ptr instance
  s3 = s1;
  TEST_ASSERT (s3 == s1);
}

void setup() {

  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay (2000);

  UNITY_BEGIN();    // IMPORTANT LINE!
}

bool done = false;

void loop() {

  if (!done) {

    RUN_TEST (test_constructor_null);
    RUN_TEST (test_constructor_notnull);
    RUN_TEST (test_getters);
    RUN_TEST (test_setters);
    RUN_TEST (test_comparison);
    RUN_TEST (test_copy);
    RUN_TEST (test_move);

    UNITY_END(); // stop unit testing
    done = true;
  }
}
