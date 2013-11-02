#ifndef CALForcePlateFileIOTest_h
#define CALForcePlateFileIOTest_h

#include <btkCALForcePlateFileIO.h>

CXXTEST_SUITE(CALForcePlateFileIOTest)
{
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::CALForcePlateFileIO::Pointer pt = btk::CALForcePlateFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileEmptyFile)
  {
    btk::CALForcePlateFileIO::Pointer pt = btk::CALForcePlateFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(CALForcePlateFilePathIN + "Empty.cal"), false);
  };
  
  CXXTEST_TEST(CanReadFileFail)
  {
    btk::CALForcePlateFileIO::Pointer pt = btk::CALForcePlateFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(CALForcePlateFilePathIN + "others/Fail.cal"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk)
  {
    btk::CALForcePlateFileIO::Pointer pt = btk::CALForcePlateFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(CALForcePlateFilePathIN + "Forcepla.cal"), true);
  };
  
  CXXTEST_TEST(CanWriteFileEmpty)
  {
    btk::CALForcePlateFileIO::Pointer pt = btk::CALForcePlateFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile(""), false);
  };
  
  CXXTEST_TEST(CanWriteFileFail)
  {
    btk::CALForcePlateFileIO::Pointer pt = btk::CALForcePlateFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.jpeg"), false);
  };
  
  CXXTEST_TEST(CanWriteFileOk)
  {
    btk::CALForcePlateFileIO::Pointer pt = btk::CALForcePlateFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.cal"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(CALForcePlateFileIOTest)
CXXTEST_TEST_REGISTRATION(CALForcePlateFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(CALForcePlateFileIOTest, CanReadFileEmptyFile)
CXXTEST_TEST_REGISTRATION(CALForcePlateFileIOTest, CanReadFileFail)
CXXTEST_TEST_REGISTRATION(CALForcePlateFileIOTest, CanReadFileOk)
CXXTEST_TEST_REGISTRATION(CALForcePlateFileIOTest, CanWriteFileEmpty)
CXXTEST_TEST_REGISTRATION(CALForcePlateFileIOTest, CanWriteFileFail)
CXXTEST_TEST_REGISTRATION(CALForcePlateFileIOTest, CanWriteFileOk)
#endif
