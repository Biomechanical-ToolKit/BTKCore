#ifndef BSFFileIOTest_h
#define BSFFileIOTest_h

#include <btkBSFFileIO.h>

CXXTEST_SUITE(BSFFileIOTest)
{
  CXXTEST_TEST(AvailableOperations)
  {
    TS_ASSERT_EQUALS(btk::BSFFileIO::HasReadOperation(), true);
    TS_ASSERT_EQUALS(btk::BSFFileIO::HasWriteOperation(), false);
  };
  
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::BSFFileIO::Pointer pt = btk::BSFFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileEmptyFile)
  {
    btk::BSFFileIO::Pointer pt = btk::BSFFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(BSFFilePathIN + "Empty.bsf"), false);
  };
  
  CXXTEST_TEST(CanReadFileFail)
  {
    btk::BSFFileIO::Pointer pt = btk::BSFFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(BSFFilePathIN + "others/Fail.bsf"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk)
  {
    btk::BSFFileIO::Pointer pt = btk::BSFFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(BSFFilePathIN + "Trial01868.bsf"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(BSFFileIOTest)
CXXTEST_TEST_REGISTRATION(BSFFileIOTest, AvailableOperations)
CXXTEST_TEST_REGISTRATION(BSFFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(BSFFileIOTest, CanReadFileEmptyFile)
CXXTEST_TEST_REGISTRATION(BSFFileIOTest, CanReadFileFail)
CXXTEST_TEST_REGISTRATION(BSFFileIOTest, CanReadFileOk)
#endif
