#ifndef XLSOrthoTrakFileIOTest_h
#define XLSOrthoTrakFileIOTest_h

#include <btkXLSOrthoTrakFileIO.h>

CXXTEST_SUITE(XLSOrthoTrakFileIOTest)
{
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::XLSOrthoTrakFileIO::Pointer pt = btk::XLSOrthoTrakFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileEmptyFile)
  {
    btk::XLSOrthoTrakFileIO::Pointer pt = btk::XLSOrthoTrakFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(XLSOrthoTrakFilePathIN + "Empty.xls"), false);
  };
  
  CXXTEST_TEST(CanReadFileFail)
  {
    btk::XLSOrthoTrakFileIO::Pointer pt = btk::XLSOrthoTrakFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(XLSOrthoTrakFilePathIN + "others/Fail.xls"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk)
  {
    btk::XLSOrthoTrakFileIO::Pointer pt = btk::XLSOrthoTrakFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(XLSOrthoTrakFilePathIN + "Gait.xls"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(XLSOrthoTrakFileIOTest)
CXXTEST_TEST_REGISTRATION(XLSOrthoTrakFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(XLSOrthoTrakFileIOTest, CanReadFileEmptyFile)
CXXTEST_TEST_REGISTRATION(XLSOrthoTrakFileIOTest, CanReadFileFail)
CXXTEST_TEST_REGISTRATION(XLSOrthoTrakFileIOTest, CanReadFileOk)
#endif
