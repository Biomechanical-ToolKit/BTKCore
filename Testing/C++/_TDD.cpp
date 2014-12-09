#include "_TDDConfigure.h"

// #include <btkLogger.h>

// BTK error messages are not displayed
// #define TDD_SILENT_CERR

int main()
{
// #if defined(TDD_SILENT_CERR)
//   btk::Logger::SetVerboseMode(btk::Logger::Quiet);
// #endif
  return CxxTest::ErrorPrinter().run();
};

#include <cxxtest/Root.cpp>
