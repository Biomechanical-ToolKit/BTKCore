#ifndef RAHFileIOTest_h
#define RAHFileIOTest_h

#include <btkRAxFileIO.h>

CXXTEST_SUITE(RAHFileIOTest)
{
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::RAxFileIO::Pointer pt = btk::RAxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileFail)
  {
    btk::RAxFileIO::Pointer pt = btk::RAxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(EliteFilePathIN + "Fail.rah"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk)
  {
    btk::RAxFileIO::Pointer pt = btk::RAxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(EliteFilePathIN + "1123xa01/1123xa01.RAH"), true);
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
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.rah"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(RAHFileIOTest)
CXXTEST_TEST_REGISTRATION(RAHFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(RAHFileIOTest, CanReadFileFail)
CXXTEST_TEST_REGISTRATION(RAHFileIOTest, CanReadFileOk)
// NO WRITE TESTS: ACQUISITION FILE IO SET AS READ ONLY
// CXXTEST_TEST_REGISTRATION(RAHFileIOTest, CanWriteFileEmpty)
// CXXTEST_TEST_REGISTRATION(RAHFileIOTest, CanWriteFileFail)
// CXXTEST_TEST_REGISTRATION(RAHFileIOTest, CanWriteFileOk)
#endif
