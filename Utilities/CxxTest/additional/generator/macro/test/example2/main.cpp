// This is only an example to use the CxxTest generator based on macros
// In this example, only the CxxTest options are defined and the main() function implemented

#define _CXXTEST_HAVE_STD // Standard library used
#define _CXXTEST_HAVE_EH // Exceptions managed by CxxTest

#include <cxxtest/TestGenerator.h>

int main()
{
  return CxxTest::ErrorPrinter().run();
};

#include <cxxtest/Root.cpp>