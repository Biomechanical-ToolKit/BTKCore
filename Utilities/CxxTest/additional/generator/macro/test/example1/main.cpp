// This is only an example to use the CxxTest generator based on macros

#define _CXXTEST_HAVE_STD // Standard library used
#define _CXXTEST_HAVE_EH // Exceptions managed by CxxTest

#include <cxxtest/TestGenerator.h>

// ------------------------------------------------------------------------- //
//                                UNIT TESTS
// ------------------------------------------------------------------------- //
// The tests can be defined here or in other files. It is not necessary to
// include any header with the tests here as all the methods representing the 
// unit tests are defined as static methods. You only need to compile the file(s)
// with the units tests at the same time than your 'main.cpp' file to include
// them.

// You can give any name for the suite
CXXTEST_SUITE(DummyTest)
{
  // You can give any name for the suite
  CXXTEST_TEST(equal)
  {
    TS_ASSERT_EQUALS(a, 0);
  };
  
  CXXTEST_TEST(differ)
  {
    int b = 3;
    TS_ASSERT_DIFFERS(a, b);
  };
  
  // OPTIONAL
  // You can add test fixtures (setup, teardown) if necessary
  // These method must be added in the declaration of the suite, but can be
  // added before or after the tests.
  CXXTEST_SUITE_SETUP()
  {
    a = 0;
  };
  
  CXXTEST_SUITE_TEARDOWN()
  {
    a += 1;
  };
  
private:
  int a;
};

// To activate the suite, you to register it
CXXTEST_SUITE_REGISTRATION(DummyTest);
// Same for the test. You only need to comment the next line to unactive the test
CXXTEST_TEST_REGISTRATION(DummyTest, equal);
CXXTEST_TEST_REGISTRATION(DummyTest, differ);

// ------------------------------------------------------------------------- //
//                                    MAIN
// ------------------------------------------------------------------------- //

// The main function needs only to run the selected printer.
// By default, only the error printer is embedded with this generator.
// But you can modify the file cxxtestgen to add another one and run it here
int main()
{
  return CxxTest::ErrorPrinter().run();
};

#include <cxxtest/Root.cpp> // DO NOT FORGET THIS LINE AT THE END