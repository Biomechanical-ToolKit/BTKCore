#ifndef RAWFileIOTest_h
#define RAWFileIOTest_h

#include <btkRAxFileIO.h>

CXXTEST_SUITE(RAWFileIOTest)
{
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::RAxFileIO::Pointer pt = btk::RAxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileFail)
  {
    btk::RAxFileIO::Pointer pt = btk::RAxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(EliteFilePathIN + "Fail.raw"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk)
  {
    btk::RAxFileIO::Pointer pt = btk::RAxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(EliteFilePathIN + "1123xa01/1123xa01.RAW"), true);
  };
  
  CXXTEST_TEST(CanWriteFileEmpty)
  {
    btk::RAxFileIO::Pointer pt = btk::RAxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile(""), false);
  };
  
  CXXTEST_TEST(CanWriteFileFail)
  {
    btk::RAxFileIO::Pointer pt = btk::RAxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.jpeg"), false);
  };
  
  CXXTEST_TEST(CanWriteFileOk)
  {
    btk::RAxFileIO::Pointer pt = btk::RAxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.raw"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(RAWFileIOTest)
CXXTEST_TEST_REGISTRATION(RAWFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(RAWFileIOTest, CanReadFileFail)
CXXTEST_TEST_REGISTRATION(RAWFileIOTest, CanReadFileOk)
CXXTEST_TEST_REGISTRATION(RAWFileIOTest, CanWriteFileEmpty)
CXXTEST_TEST_REGISTRATION(RAWFileIOTest, CanWriteFileFail)
CXXTEST_TEST_REGISTRATION(RAWFileIOTest, CanWriteFileOk)
#endif
