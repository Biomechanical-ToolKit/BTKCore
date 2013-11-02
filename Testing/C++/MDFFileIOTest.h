#ifndef MDFFileIOTest_h
#define MDFFileIOTest_h

#include <btkMDFFileIO.h>

CXXTEST_SUITE(MDFFileIOTest)
{
  CXXTEST_TEST(AvailableOperations)
  {
    TS_ASSERT_EQUALS(btk::MDFFileIO::HasReadOperation(), true);
    TS_ASSERT_EQUALS(btk::MDFFileIO::HasWriteOperation(), false);
  };
  
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::MDFFileIO::Pointer pt = btk::MDFFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileFail)
  {
    btk::MDFFileIO::Pointer pt = btk::MDFFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(MDFFilePathIN + "Fail.MDF"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk)
  {
    btk::MDFFileIO::Pointer pt = btk::MDFFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(MDFFilePathIN + "gait-bilateral-1997-Kistlerx1.mdf"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(MDFFileIOTest)
CXXTEST_TEST_REGISTRATION(MDFFileIOTest, AvailableOperations)
CXXTEST_TEST_REGISTRATION(MDFFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(MDFFileIOTest, CanReadFileFail)
CXXTEST_TEST_REGISTRATION(MDFFileIOTest, CanReadFileOk)
#endif
