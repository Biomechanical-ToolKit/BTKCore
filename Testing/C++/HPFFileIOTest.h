#ifndef HPFFileIOTest_h
#define HPFFileIOTest_h

#include <btkHPFFileIO.h>

CXXTEST_SUITE(HPFFileIOTest)
{
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::HPFFileIO::Pointer pt = btk::HPFFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileFail)
  {
    btk::HPFFileIO::Pointer pt = btk::HPFFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(HPFFilePathIN + "Fail.hpf"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk1)
  {
    btk::HPFFileIO::Pointer pt = btk::HPFFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(HPFFilePathIN + "Run_number_34_VTT_Rep_1.6.hpf"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(HPFFileIOTest)
CXXTEST_TEST_REGISTRATION(HPFFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(HPFFileIOTest, CanReadFileFail)
CXXTEST_TEST_REGISTRATION(HPFFileIOTest, CanReadFileOk1)
#endif
