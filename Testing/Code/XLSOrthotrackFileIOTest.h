#ifndef XLSOrthotrackFileIOTest_h
#define XLSOrthotrackFileIOTest_h

#include <btkXLSOrthotrackFileIO.h>

CXXTEST_SUITE(XLSOrthotrackFileIOTest)
{
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::XLSOrthotrackFileIO::Pointer pt = btk::XLSOrthotrackFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileEmptyFile)
  {
    btk::XLSOrthotrackFileIO::Pointer pt = btk::XLSOrthotrackFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(XLSOrthotrackFilePathIN + "Empty.xls"), false);
  };
  
  CXXTEST_TEST(CanReadFileFail)
  {
    btk::XLSOrthotrackFileIO::Pointer pt = btk::XLSOrthotrackFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(XLSOrthotrackFilePathIN + "others/Fail.xls"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk)
  {
    btk::XLSOrthotrackFileIO::Pointer pt = btk::XLSOrthotrackFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(XLSOrthotrackFilePathIN + "Gait.xls"), true);
  };
  
  CXXTEST_TEST(CanWriteFileEmpty)
  {
    btk::XLSOrthotrackFileIO::Pointer pt = btk::XLSOrthotrackFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile(""), false);
  };
  
  CXXTEST_TEST(CanWriteFileFail)
  {
    btk::XLSOrthotrackFileIO::Pointer pt = btk::XLSOrthotrackFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.jpeg"), false);
  };
  
  CXXTEST_TEST(CanWriteFileOk)
  {
    btk::XLSOrthotrackFileIO::Pointer pt = btk::XLSOrthotrackFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.xls"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(XLSOrthotrackFileIOTest)
CXXTEST_TEST_REGISTRATION(XLSOrthotrackFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(XLSOrthotrackFileIOTest, CanReadFileEmptyFile)
CXXTEST_TEST_REGISTRATION(XLSOrthotrackFileIOTest, CanReadFileFail)
CXXTEST_TEST_REGISTRATION(XLSOrthotrackFileIOTest, CanReadFileOk)
CXXTEST_TEST_REGISTRATION(XLSOrthotrackFileIOTest, CanWriteFileEmpty)
CXXTEST_TEST_REGISTRATION(XLSOrthotrackFileIOTest, CanWriteFileFail)
CXXTEST_TEST_REGISTRATION(XLSOrthotrackFileIOTest, CanWriteFileOk)
#endif
