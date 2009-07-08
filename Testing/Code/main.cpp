#include "tddDefinitions.h"

#ifndef CXXTEST_RUNNING
    #define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#define _CXXTEST_HAVE_EH

#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/ErrorPrinter.h>

// BTK error messages are not displayed
#define TDD_SILENT_CERR

int main()
{
#if defined(TDD_SILENT_CERR)
  std::streambuf* standardErrorOutput = std::cerr.rdbuf(0);
  int err = CxxTest::ErrorPrinter().run();
  std::cerr.rdbuf(standardErrorOutput);
  return err;
#else
  return CxxTest::ErrorPrinter().run();
#endif
};

#define C3DFilePathIN std::string(TDD_C3DFilePathIN)
#define C3DFilePathOUT std::string(TDD_C3DFilePathOUT)
#define TRCFilePathIN std::string(TDD_TRCFilePathIN)
#define TRCFilePathOUT std::string(TDD_TRCFilePathOUT)

#include "AcquisitionTest.h"
#include "C3DFileIOTest.h"
#include "C3DFileReaderTest.h"
#include "C3DFileWriterTest.h"
#include "ForcePlatformTypesTest.h"
#include "ForcePlatformsExtractorTest.h"
#include "PointTest.h"
#include "PointCollectionTest.h"
#include "MeasureFrameExtractorTest.h"
#include "MetaDataEntryTest.h"
#include "MetaDataEntryValueTest.h"
#include "PipelineTest.h"
#include "SpecializedPointsExtractorTest.h"
#include "TRCFileIOTest.h"
#include "TRCFileReaderTest.h"
#include "TRCFileWriterTest.h"
#ifdef HAVE_BTKVTKADAPTER
  #include "VTKPointSourceTest.h"
#endif

#include <cxxtest/Root.cpp>
