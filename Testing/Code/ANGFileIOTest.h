#ifndef ANGFileIOTest_h
#define ANGFileIOTest_h

#include <btkANGFileIO.h>

CXXTEST_SUITE(ANGFileIOTest)
{
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::ANGFileIO::Pointer pt = btk::ANGFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileFail)
  {
    btk::ANGFileIO::Pointer pt = btk::ANGFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(EliteFilePathIN + "Fail.ang"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk)
  {
    btk::ANGFileIO::Pointer pt = btk::ANGFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(EliteFilePathIN + "1123xa01/1123xa01.ANG"), true);
  };
  
  CXXTEST_TEST(CanWriteFileEmpty)
  {
    btk::ANGFileIO::Pointer pt = btk::ANGFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile(""), false);
  };
  
  CXXTEST_TEST(CanWriteFileFail)
  {
    btk::ANGFileIO::Pointer pt = btk::ANGFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.jpeg"), false);
  };
  
  CXXTEST_TEST(CanWriteFileOk)
  {
    btk::ANGFileIO::Pointer pt = btk::ANGFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.ang"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(ANGFileIOTest)
CXXTEST_TEST_REGISTRATION(ANGFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(ANGFileIOTest, CanReadFileFail)
CXXTEST_TEST_REGISTRATION(ANGFileIOTest, CanReadFileOk)
CXXTEST_TEST_REGISTRATION(ANGFileIOTest, CanWriteFileEmpty)
CXXTEST_TEST_REGISTRATION(ANGFileIOTest, CanWriteFileFail)
CXXTEST_TEST_REGISTRATION(ANGFileIOTest, CanWriteFileOk)
#endif
