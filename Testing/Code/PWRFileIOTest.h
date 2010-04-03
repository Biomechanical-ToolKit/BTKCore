#ifndef PWRFileIOTest_h
#define PWRFileIOTest_h

#include <btkPWRFileIO.h>

CXXTEST_SUITE(PWRFileIOTest)
{
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::PWRFileIO::Pointer pt = btk::PWRFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileFail)
  {
    btk::PWRFileIO::Pointer pt = btk::PWRFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(EliteFilePathIN + "Fail.pwr"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk)
  {
    btk::PWRFileIO::Pointer pt = btk::PWRFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(EliteFilePathIN + "1123xa01/1123xa01.PWR"), true);
  };
  
  CXXTEST_TEST(CanWriteFileEmpty)
  {
    btk::PWRFileIO::Pointer pt = btk::PWRFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile(""), false);
  };
  
  CXXTEST_TEST(CanWriteFileFail)
  {
    btk::PWRFileIO::Pointer pt = btk::PWRFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.jpeg"), false);
  };
  
  CXXTEST_TEST(CanWriteFileOk)
  {
    btk::PWRFileIO::Pointer pt = btk::PWRFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.pwr"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(PWRFileIOTest)
CXXTEST_TEST_REGISTRATION(PWRFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(PWRFileIOTest, CanReadFileFail)
CXXTEST_TEST_REGISTRATION(PWRFileIOTest, CanReadFileOk)
CXXTEST_TEST_REGISTRATION(PWRFileIOTest, CanWriteFileEmpty)
CXXTEST_TEST_REGISTRATION(PWRFileIOTest, CanWriteFileFail)
CXXTEST_TEST_REGISTRATION(PWRFileIOTest, CanWriteFileOk)
#endif
