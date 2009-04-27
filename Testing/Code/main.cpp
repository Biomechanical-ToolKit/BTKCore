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

int main()
{
  return CxxTest::ErrorPrinter().run();
};

#define C3DFilePath std::string(TDD_C3DFilePath)

#include "C3DFileIOTest.h"
#include "C3DFileReaderTest.h"
#include "AcquisitionTest.h"
#include "MarkerTest.h"
#include "MarkerCollectionTest.h"
#include "MeasureFrameExtractorTest.h"
#include "MetaDataEntryTest.h"
#include "MetaDataEntryValueTest.h"
#include "PipelineTest.h"
#ifdef HAVE_BTKVTKADAPTER
  #include "VTKMarkerSourceTest.h"
#endif

//#include <nvwa/debug_new.h> // memory leak detector
#include <cxxtest/Root.cpp>
