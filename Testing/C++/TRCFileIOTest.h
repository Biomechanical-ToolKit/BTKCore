#ifndef TRCFileIOTest_h
#define TRCFileIOTest_h

#include <btkTRCFileIO.h>

CXXTEST_SUITE(TRCFileIOTest)
{
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::TRCFileIO::Pointer pt = btk::TRCFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileEmptyFile)
  {
    btk::TRCFileIO::Pointer pt = btk::TRCFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(TRCFilePathIN + "Empty.trc"), false);
  };
  
  CXXTEST_TEST(CanReadFileShouldFail)
  {
    btk::TRCFileIO::Pointer pt = btk::TRCFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(TRCFilePathIN + "Truncated.trc"), true);
  };
  
  CXXTEST_TEST(CanReadFileOk)
  {
    btk::TRCFileIO::Pointer pt = btk::TRCFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(TRCFilePathIN + "MOTEK/T.trc"), true);
  };
  
  CXXTEST_TEST(CanWriteFileEmpty)
  {
    btk::TRCFileIO::Pointer pt = btk::TRCFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile(""), false);
  };
  
  CXXTEST_TEST(CanWriteFileFail)
  {
    btk::TRCFileIO::Pointer pt = btk::TRCFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.jpeg"), false);
  };
  
  CXXTEST_TEST(CanWriteFileOk)
  {
    btk::TRCFileIO::Pointer pt = btk::TRCFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.trc"), true);
  };

  CXXTEST_TEST(CanWriteFileOk2)
  {
    btk::TRCFileIO::Pointer pt = btk::TRCFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile(TRCFilePathOUT + "MOTEK_T_rewrited.trc"), true);
  };

};

CXXTEST_SUITE_REGISTRATION(TRCFileIOTest)
CXXTEST_TEST_REGISTRATION(TRCFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(TRCFileIOTest, CanReadFileEmptyFile)
CXXTEST_TEST_REGISTRATION(TRCFileIOTest, CanReadFileShouldFail)
CXXTEST_TEST_REGISTRATION(TRCFileIOTest, CanReadFileOk)
CXXTEST_TEST_REGISTRATION(TRCFileIOTest, CanWriteFileEmpty)
CXXTEST_TEST_REGISTRATION(TRCFileIOTest, CanWriteFileFail)
CXXTEST_TEST_REGISTRATION(TRCFileIOTest, CanWriteFileOk)
CXXTEST_TEST_REGISTRATION(TRCFileIOTest, CanWriteFileOk2)
#endif
