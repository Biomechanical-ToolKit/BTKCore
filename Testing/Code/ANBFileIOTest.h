#ifndef ANBFileIOTest_h
#define ANBFileIOTest_h

#include <btkANBFileIO.h>

CXXTEST_SUITE(ANBFileIOTest)
{
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::ANBFileIO::Pointer pt = btk::ANBFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileEmptyFile)
  {
    btk::ANBFileIO::Pointer pt = btk::ANBFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(ANBFilePathIN + "Empty.anb"), false);
  };
  
  CXXTEST_TEST(CanReadFileFail)
  {
    btk::ANBFileIO::Pointer pt = btk::ANBFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(ANBFilePathIN + "others/Fail.anb"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk)
  {
    btk::ANBFileIO::Pointer pt = btk::ANBFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(ANBFilePathIN + "Gait.anb"), true);
  };
  
  CXXTEST_TEST(CanWriteFileEmpty)
  {
    btk::ANBFileIO::Pointer pt = btk::ANBFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile(""), false);
  };
  
  CXXTEST_TEST(CanWriteFileFail)
  {
    btk::ANBFileIO::Pointer pt = btk::ANBFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.jpeg"), false);
  };
  
  CXXTEST_TEST(CanWriteFileOk)
  {
    btk::ANBFileIO::Pointer pt = btk::ANBFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.anb"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(ANBFileIOTest)
CXXTEST_TEST_REGISTRATION(ANBFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(ANBFileIOTest, CanReadFileEmptyFile)
CXXTEST_TEST_REGISTRATION(ANBFileIOTest, CanReadFileFail)
CXXTEST_TEST_REGISTRATION(ANBFileIOTest, CanReadFileOk)
CXXTEST_TEST_REGISTRATION(ANBFileIOTest, CanWriteFileEmpty)
CXXTEST_TEST_REGISTRATION(ANBFileIOTest, CanWriteFileFail)
CXXTEST_TEST_REGISTRATION(ANBFileIOTest, CanWriteFileOk)
#endif
