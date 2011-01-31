#ifndef EMFFileIOTest_h
#define EMFFileIOTest_h

#include <btkEMFFileIO.h>

CXXTEST_SUITE(EMFFileIOTest)
{
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
  
  CXXTEST_TEST(CanWriteFileEmpty)
  {
    btk::EMFFileIO::Pointer pt = btk::EMFFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile(""), false);
  };
  
  CXXTEST_TEST(CanWriteFileFail)
  {
    btk::EMFFileIO::Pointer pt = btk::EMFFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.jpeg"), false);
  };
  
  CXXTEST_TEST(CanWriteFileOk)
  {
    btk::EMFFileIO::Pointer pt = btk::EMFFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.EMF"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(EMFFileIOTest)
CXXTEST_TEST_REGISTRATION(EMFFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(EMFFileIOTest, CanReadFileEmptyFile)
CXXTEST_TEST_REGISTRATION(EMFFileIOTest, CanReadFileOk)
CXXTEST_TEST_REGISTRATION(EMFFileIOTest, CanWriteFileEmpty)
CXXTEST_TEST_REGISTRATION(EMFFileIOTest, CanWriteFileFail)
CXXTEST_TEST_REGISTRATION(EMFFileIOTest, CanWriteFileOk)
#endif
