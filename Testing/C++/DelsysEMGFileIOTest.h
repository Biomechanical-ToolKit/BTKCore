#ifndef DelsysEMGFileIOTest_h
#define DelsysEMGFileIOTest_h

#include <btkDelsysEMGFileIO.h>

CXXTEST_SUITE(DelsysEMGFileIOTest)
{
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::DelsysEMGFileIO::Pointer pt = btk::DelsysEMGFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileFail)
  {
    btk::DelsysEMGFileIO::Pointer pt = btk::DelsysEMGFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(DelsysEMGFilePathIN + "Fail.emg"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk)
  {
    btk::DelsysEMGFileIO::Pointer pt = btk::DelsysEMGFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(DelsysEMGFilePathIN + "Set1[Rep2]_v3.emg"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(DelsysEMGFileIOTest)
CXXTEST_TEST_REGISTRATION(DelsysEMGFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(DelsysEMGFileIOTest, CanReadFileFail)
CXXTEST_TEST_REGISTRATION(DelsysEMGFileIOTest, CanReadFileOk)
#endif
