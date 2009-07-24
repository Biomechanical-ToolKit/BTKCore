#ifndef ANCFileIOTest_h
#define ANCFileIOTest_h

#include <btkANCFileIO.h>

CXXTEST_SUITE(ANCFileIOTest)
{
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::ANCFileIO::Pointer pt = btk::ANCFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileEmptyFile)
  {
    btk::ANCFileIO::Pointer pt = btk::ANCFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(ANCFilePathIN + "Empty.anc"), false);
  };
  
  CXXTEST_TEST(CanReadFileFail)
  {
    btk::ANCFileIO::Pointer pt = btk::ANCFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(ANCFilePathIN + "others/Fail.anc"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk)
  {
    btk::ANCFileIO::Pointer pt = btk::ANCFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(ANCFilePathIN + "Gait.anc"), true);
  };
  
  CXXTEST_TEST(CanWriteFileEmpty)
  {
    btk::ANCFileIO::Pointer pt = btk::ANCFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile(""), false);
  };
  
  CXXTEST_TEST(CanWriteFileFail)
  {
    btk::ANCFileIO::Pointer pt = btk::ANCFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.jpeg"), false);
  };
  
  CXXTEST_TEST(CanWriteFileOk)
  {
    btk::ANCFileIO::Pointer pt = btk::ANCFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.anc"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(ANCFileIOTest)
CXXTEST_TEST_REGISTRATION(ANCFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(ANCFileIOTest, CanReadFileEmptyFile)
CXXTEST_TEST_REGISTRATION(ANCFileIOTest, CanReadFileFail)
CXXTEST_TEST_REGISTRATION(ANCFileIOTest, CanReadFileOk)
CXXTEST_TEST_REGISTRATION(ANCFileIOTest, CanWriteFileEmpty)
CXXTEST_TEST_REGISTRATION(ANCFileIOTest, CanWriteFileFail)
CXXTEST_TEST_REGISTRATION(ANCFileIOTest, CanWriteFileOk)
#endif
