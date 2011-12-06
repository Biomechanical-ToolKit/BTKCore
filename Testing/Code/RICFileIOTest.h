#ifndef RICFileIOTest_h
#define RICFileIOTest_h

#include <btkRICFileIO.h>

CXXTEST_SUITE(RICFileIOTest)
{
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::RICFileIO::Pointer pt = btk::RICFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileFail)
  {
    btk::RICFileIO::Pointer pt = btk::RICFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(EliteFilePathIN + "Fail.ric"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk)
  {
    btk::RICFileIO::Pointer pt = btk::RICFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(EliteFilePathIN + "1123xa01/1123xa01.RIC"), true);
  };
  
  CXXTEST_TEST(CanWriteFileEmpty)
  {
    btk::RICFileIO::Pointer pt = btk::RICFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile(""), false);
  };
  
  CXXTEST_TEST(CanWriteFileFail)
  {
    btk::RICFileIO::Pointer pt = btk::RICFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.jpeg"), false);
  };
  
  CXXTEST_TEST(CanWriteFileOk)
  {
    btk::RICFileIO::Pointer pt = btk::RICFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("1123xa01/test.ric"), true);
  };
  
  CXXTEST_TEST(CanWriteFileOkBis)
  {
    btk::RICFileIO::Pointer pt = btk::RICFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("foo/bar.rif"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(RICFileIOTest)
CXXTEST_TEST_REGISTRATION(RICFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(RICFileIOTest, CanReadFileFail)
CXXTEST_TEST_REGISTRATION(RICFileIOTest, CanReadFileOk)
CXXTEST_TEST_REGISTRATION(RICFileIOTest, CanWriteFileEmpty)
CXXTEST_TEST_REGISTRATION(RICFileIOTest, CanWriteFileFail)
CXXTEST_TEST_REGISTRATION(RICFileIOTest, CanWriteFileOk)
CXXTEST_TEST_REGISTRATION(RICFileIOTest, CanWriteFileOkBis)
#endif
