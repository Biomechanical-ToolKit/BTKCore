#ifndef CLBFileIOTest_h
#define CLBFileIOTest_h

#include <btkCLBFileIO.h>

CXXTEST_SUITE(CLBFileIOTest)
{
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::CLBFileIO::Pointer pt = btk::CLBFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileEmptyFile)
  {
    btk::CLBFileIO::Pointer pt = btk::CLBFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(CLBFilePathIN + "Empty.clb"), false);
  };
  
  CXXTEST_TEST(CanReadFileFail)
  {
    btk::CLBFileIO::Pointer pt = btk::CLBFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(CLBFilePathIN + "others/Fail.clb"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk)
  {
    btk::CLBFileIO::Pointer pt = btk::CLBFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(CLBFilePathIN + "NoScale.clb"), true);
  };
  
  CXXTEST_TEST(CanReadFileOk2)
  {
    btk::CLBFileIO::Pointer pt = btk::CLBFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(CLBFilePathIN + "Truncated.clb"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(CLBFileIOTest)
CXXTEST_TEST_REGISTRATION(CLBFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(CLBFileIOTest, CanReadFileEmptyFile)
CXXTEST_TEST_REGISTRATION(CLBFileIOTest, CanReadFileFail)
CXXTEST_TEST_REGISTRATION(CLBFileIOTest, CanReadFileOk)
CXXTEST_TEST_REGISTRATION(CLBFileIOTest, CanReadFileOk2)
#endif
