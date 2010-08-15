#include "_TDDConfigure.h"

// BTK error messages are not displayed
#define TDD_SILENT_CERR

#include "btkBinaryFileStream.h"
#include <cxxtest/HtmlFilePrinter.h>

int main()
{
#if defined(TDD_SILENT_CERR)
  std::streambuf* standardErrorOutput = std::cerr.rdbuf(0);
#endif

  //int err = CxxTest::ErrorPrinter().run();
  
#if defined(TEST_MMFSTREAM)
  int err = CxxTest::HtmlFilePrinter("/Users/Alzathar/Code/BTK/BTK/trunk/build-mac-testing/Documentation/CxxTest_mmfstream/").run();
#else
  int err = CxxTest::HtmlFilePrinter("/Users/Alzathar/Code/BTK/BTK/trunk/build-mac-testing/Documentation/CxxTest/").run();
#endif
  

#if defined(TDD_SILENT_CERR)
  std::cerr.rdbuf(standardErrorOutput);
#endif

  return err;
};

#include <cxxtest/Root.cpp>
