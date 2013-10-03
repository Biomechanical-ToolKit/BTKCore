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
};

CXXTEST_SUITE_REGISTRATION(GRxFileIOTest)
CXXTEST_TEST_REGISTRATION(GRxFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(GRxFileIOTest, CanReadFileFail)
CXXTEST_TEST_REGISTRATION(GRxFileIOTest, CanReadFileOk1)
CXXTEST_TEST_REGISTRATION(GRxFileIOTest, CanReadFileOk2)
#endif
