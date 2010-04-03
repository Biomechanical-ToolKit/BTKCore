#ifndef GRxFileIOTest_h
#define GRxFileIOTest_h

#include <btkGRxFileIO.h>

CXXTEST_SUITE(GRxFileIOTest)
{
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::GRxFileIO::Pointer pt = btk::GRxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileFail)
  {
    btk::GRxFileIO::Pointer pt = btk::GRxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(EliteFilePathIN + "Fail.gr1"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk1)
  {
    btk::GRxFileIO::Pointer pt = btk::GRxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(EliteFilePathIN + "1123xa01/1123xa01.GR1"), true);
  };
  
  CXXTEST_TEST(CanReadFileOk2)
  {
    btk::GRxFileIO::Pointer pt = btk::GRxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(EliteFilePathIN + "1123xa01/1123xa01.GR2"), true);
  };
  
  CXXTEST_TEST(CanWriteFileEmpty)
  {
    btk::GRxFileIO::Pointer pt = btk::GRxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile(""), false);
  };
  
  CXXTEST_TEST(CanWriteFileFail1)
  {
    btk::GRxFileIO::Pointer pt = btk::GRxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.jpeg"), false);
  };
  
  CXXTEST_TEST(CanWriteFileFail2)
  {
    btk::GRxFileIO::Pointer pt = btk::GRxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.gra"), false);
  };
  
  CXXTEST_TEST(CanWriteFileFail3)
  {
    btk::GRxFileIO::Pointer pt = btk::GRxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.grd"), false);
  };
  
  CXXTEST_TEST(CanWriteFileOk1)
  {
    btk::GRxFileIO::Pointer pt = btk::GRxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.gr1"), true);
  };
  
  CXXTEST_TEST(CanWriteFileOk2)
  {
    btk::GRxFileIO::Pointer pt = btk::GRxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.gr2"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(GRxFileIOTest)
CXXTEST_TEST_REGISTRATION(GRxFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(GRxFileIOTest, CanReadFileFail)
CXXTEST_TEST_REGISTRATION(GRxFileIOTest, CanReadFileOk1)
CXXTEST_TEST_REGISTRATION(GRxFileIOTest, CanReadFileOk2)
CXXTEST_TEST_REGISTRATION(GRxFileIOTest, CanWriteFileEmpty)
CXXTEST_TEST_REGISTRATION(GRxFileIOTest, CanWriteFileFail1)
CXXTEST_TEST_REGISTRATION(GRxFileIOTest, CanWriteFileFail2)
CXXTEST_TEST_REGISTRATION(GRxFileIOTest, CanWriteFileFail3)
CXXTEST_TEST_REGISTRATION(GRxFileIOTest, CanWriteFileOk1)
CXXTEST_TEST_REGISTRATION(GRxFileIOTest, CanWriteFileOk2)
#endif
