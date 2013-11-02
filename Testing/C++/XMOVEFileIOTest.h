#ifndef XMOVEFileIOTest_h
#define XMOVEFileIOTest_h

#include <btkXMOVEFileIO.h>

CXXTEST_SUITE(XMOVEFileIOTest)
{
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::XMOVEFileIO::Pointer pt = btk::XMOVEFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileFail)
  {
    btk::XMOVEFileIO::Pointer pt = btk::XMOVEFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(XMOVEFilePathIN + "Fail.XMOVE"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk1)
  {
    btk::XMOVEFileIO::Pointer pt = btk::XMOVEFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(XMOVEFilePathIN + "ADemo1_rewrite_XMove.xml"), true);
  };
  
  CXXTEST_TEST(CanReadFileOk2)
  {
    btk::XMOVEFileIO::Pointer pt = btk::XMOVEFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(XMOVEFilePathIN + "testReadWriteEmptyXMove.xmove"), true);
  };
  
  CXXTEST_TEST(CanReadFileOk3)
  {
    btk::XMOVEFileIO::Pointer pt = btk::XMOVEFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(XMOVEFilePathIN + "testReadWriteSimpleXMove.xml"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(XMOVEFileIOTest)
CXXTEST_TEST_REGISTRATION(XMOVEFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(XMOVEFileIOTest, CanReadFileFail)
CXXTEST_TEST_REGISTRATION(XMOVEFileIOTest, CanReadFileOk1)
CXXTEST_TEST_REGISTRATION(XMOVEFileIOTest, CanReadFileOk2)
CXXTEST_TEST_REGISTRATION(XMOVEFileIOTest, CanReadFileOk3)
#endif
