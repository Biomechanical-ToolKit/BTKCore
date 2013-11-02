#ifndef EMFFileIOTest_h
#define EMFFileIOTest_h

#include <btkEMFFileIO.h>

CXXTEST_SUITE(EMFFileIOTest)
{
  CXXTEST_TEST(AvailableOperations)
  {
    TS_ASSERT_EQUALS(btk::EMFFileIO::HasReadOperation(), true);
    TS_ASSERT_EQUALS(btk::EMFFileIO::HasWriteOperation(), false);
  };

  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::EMFFileIO::Pointer pt = btk::EMFFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileEmptyFile)
  {
    btk::EMFFileIO::Pointer pt = btk::EMFFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(EMFFilePathIN + "empty.emf"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk)
  {
    btk::EMFFileIO::Pointer pt = btk::EMFFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(EMFFilePathIN + "test.emf"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(EMFFileIOTest)
CXXTEST_TEST_REGISTRATION(EMFFileIOTest, AvailableOperations)
CXXTEST_TEST_REGISTRATION(EMFFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(EMFFileIOTest, CanReadFileEmptyFile)
CXXTEST_TEST_REGISTRATION(EMFFileIOTest, CanReadFileOk)
#endif
