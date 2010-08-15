#include "_TDDConfigure.h"

// BTK error messages are not displayed
#define TDD_SILENT_CERR

int main()
{
#if defined(TDD_SILENT_CERR)
  std::streambuf* standardErrorOutput = std::cerr.rdbuf(0);
#endif

  int err = CxxTest::ErrorPrinter().run();
  
#if defined(TDD_SILENT_CERR)
  std::cerr.rdbuf(standardErrorOutput);
#endif

  return err;
};

#include <cxxtest/Root.cpp>
