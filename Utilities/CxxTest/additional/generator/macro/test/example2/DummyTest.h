// Note: This is exactly the same suite and tests than in 'example1' but in 
// another file. The type of file to use (H, CPP, FOO, etc.) is not important 
// as its content will be included in another file which will be compiled.

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