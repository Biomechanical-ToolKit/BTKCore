#ifndef KistlerDATFileIOTest_h
#define KistlerDATFileIOTest_h

#include <btkKistlerDATFileIO.h>

CXXTEST_SUITE(KistlerDATFileIOTest)
{
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::KistlerDATFileIO::Pointer pt = btk::KistlerDATFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileEmptyFile)
  {
    btk::KistlerDATFileIO::Pointer pt = btk::KistlerDATFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(KistlerDATFilePathIN + "Empty.dat"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk)
  {
    btk::KistlerDATFileIO::Pointer pt = btk::KistlerDATFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(KistlerDATFilePathIN + "BioWare17.dat"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(KistlerDATFileIOTest)
CXXTEST_TEST_REGISTRATION(KistlerDATFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(KistlerDATFileIOTest, CanReadFileEmptyFile)
CXXTEST_TEST_REGISTRATION(KistlerDATFileIOTest, CanReadFileOk)
#endif
