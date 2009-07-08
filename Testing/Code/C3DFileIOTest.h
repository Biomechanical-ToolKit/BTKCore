#ifndef C3DFileIOTest_h
#define C3DFileIOTest_h

#include <btkC3DFileIO.h>

CXXTEST_SUITE(C3DFileIOTest)
{
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::C3DFileIO::Pointer pt = btk::C3DFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileEmptyFile)
  {
    btk::C3DFileIO::Pointer pt = btk::C3DFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(C3DFilePathIN + "others/Empty.c3d"), false);
  };
  
  CXXTEST_TEST(CanReadFileFail)
  {
    btk::C3DFileIO::Pointer pt = btk::C3DFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(C3DFilePathIN + "others/Fail.c3d"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk)
  {
    btk::C3DFileIO::Pointer pt = btk::C3DFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(C3DFilePathIN + "sample01/Eb015pi.C3D"), true);
  };
  
  CXXTEST_TEST(CanWriteFileEmpty)
  {
    btk::C3DFileIO::Pointer pt = btk::C3DFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile(""), false);
  };
  
  CXXTEST_TEST(CanWriteFileFail)
  {
    btk::C3DFileIO::Pointer pt = btk::C3DFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.jpeg"), false);
  };
  
  CXXTEST_TEST(CanWriteFileOk)
  {
    btk::C3DFileIO::Pointer pt = btk::C3DFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.c3d"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(C3DFileIOTest)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, CanReadFileEmptyFile)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, CanReadFileFail)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, CanReadFileOk)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, CanWriteFileEmpty)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, CanWriteFileFail)
CXXTEST_TEST_REGISTRATION(C3DFileIOTest, CanWriteFileOk)
#endif
