#ifndef TRBFileIOTest_h
#define TRBFileIOTest_h

#include <btkTRBFileIO.h>

CXXTEST_SUITE(TRBFileIOTest)
{
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::TRBFileIO::Pointer pt = btk::TRBFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileEmptyFile)
  {
    btk::TRBFileIO::Pointer pt = btk::TRBFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(TRBFilePathIN + "Empty.trb"), false);
  };
  
  CXXTEST_TEST(CanReadFileFail)
  {
    btk::TRBFileIO::Pointer pt = btk::TRBFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(TRBFilePathIN + "False.trb"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk)
  {
    btk::TRBFileIO::Pointer pt = btk::TRBFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(TRBFilePathIN + "gait.trb"), true);
  };
  
  CXXTEST_TEST(CanWriteFileEmpty)
  {
    btk::TRBFileIO::Pointer pt = btk::TRBFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile(""), false);
  };
  
  CXXTEST_TEST(CanWriteFileFail)
  {
    btk::TRBFileIO::Pointer pt = btk::TRBFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.jpeg"), false);
  };
  
  CXXTEST_TEST(CanWriteFileOk)
  {
    btk::TRBFileIO::Pointer pt = btk::TRBFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.trb"), true);
  };

};

CXXTEST_SUITE_REGISTRATION(TRBFileIOTest)
CXXTEST_TEST_REGISTRATION(TRBFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(TRBFileIOTest, CanReadFileEmptyFile)
CXXTEST_TEST_REGISTRATION(TRBFileIOTest, CanReadFileFail)
CXXTEST_TEST_REGISTRATION(TRBFileIOTest, CanReadFileOk)
// NO WRITE TESTS: ACQUISITION FILE IO SET AS READ ONLY
// CXXTEST_TEST_REGISTRATION(TRBFileIOTest, CanWriteFileEmpty)
// CXXTEST_TEST_REGISTRATION(TRBFileIOTest, CanWriteFileFail)
// CXXTEST_TEST_REGISTRATION(TRBFileIOTest, CanWriteFileOk)
#endif
