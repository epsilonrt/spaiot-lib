//
// Unit Test for the type_name
// How to run:
// pio test -f test_05_type_name -v
// -v for verbose, if not specified only summary is printed (no TEST_MESSAGE, no TEST_PRINTF)
//
#include <Arduino.h>
#include <unity.h>
#include <type_name.h>

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

// Abstract class with ==operator
class A {

  public:
    virtual ~A() {}
    virtual void doit() = 0;
    virtual bool operator== (const A &other) const {

      return type_name (*this) == type_name (other);
    }
    virtual bool operator!= (const A &other) const {

      return ! (*this == other);
    }
};

// Derived class
class B : public A {

  public:
    B (int i = 0) : m_int (i) {}
    void doit() {}
    virtual bool operator== (const A &other) const {

      if (A::operator== (other)) {

        return m_int == static_cast<const B &> (other).m_int;
      }
      return false;
    }
    void set (int i) {
      m_int = i;
    }
  private:
    int m_int;
};

void test_type_name (void) {
  B b;
  A &a = b;
  B *pb = &b;
  A *pa = &a;

  TEST_PRINTF ("type_name(b) = %s --- type_name(a) = %s", type_name (b).c_str(), type_name (a).c_str());
  TEST_ASSERT (type_name (b) == "B");
  TEST_ASSERT (type_name (*pb) == "B");
  TEST_ASSERT (type_name (a) == "A");
  TEST_ASSERT (type_name (*pa) == "A");
}

void test_equal (void) {
  B b1;
  B b2;
  A &a1 = b1;
  A &a2 = b2;
  A *pa1 = &a1;
  B *pb1 = &b1;
  TEST_ASSERT (b1 == b2);
  TEST_ASSERT (a1 == b2);
  TEST_ASSERT (b1 == a2);
  TEST_ASSERT (a1 == a2);
  TEST_ASSERT (*pa1 == a1);
  TEST_ASSERT (*pb1 == b1);
  TEST_ASSERT (*pb1 == *pa1);
}

void test_notequal (void) {
  B b1 (1);
  B b2 (2);
  A &a1 = b1;
  A &a2 = b2;
  TEST_ASSERT (b1 != b2);
  TEST_ASSERT (a1 != b2);
  TEST_ASSERT (b1 != a2);
  TEST_ASSERT (a1 != a2);
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

    RUN_TEST (test_type_name);
    RUN_TEST (test_equal);
    RUN_TEST (test_notequal);

    UNITY_END(); // stop unit testing
    done = true;
  }
}
