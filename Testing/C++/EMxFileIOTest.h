#ifndef EMxFileIOTest_h
#define EMxFileIOTest_h

#include <btkEMxFileIO.h>

CXXTEST_SUITE(EMxFileIOTest)
{
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::EMxFileIO::Pointer pt = btk::EMxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileFail)
  {
    btk::EMxFileIO::Pointer pt = btk::EMxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(EliteFilePathIN + "Fail.gr1"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk1)
  {
    btk::EMxFileIO::Pointer pt = btk::EMxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(EliteFilePathIN + "1123xa01/1123xa01.EMF"), true);
  };
  
  CXXTEST_TEST(CanReadFileOk2)
  {
    btk::EMxFileIO::Pointer pt = btk::EMxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(EliteFilePathIN + "1123xa01/1123xa01.EMG"), true);
  };
  
  CXXTEST_TEST(CanReadFileOk3)
  {
    btk::EMxFileIO::Pointer pt = btk::EMxFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(EliteFilePathIN + "1123xa01/1123xa01.EMR"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(EMxFileIOTest)
CXXTEST_TEST_REGISTRATION(EMxFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(EMxFileIOTest, CanReadFileFail)
//CXXTEST_TEST_REGISTRATION(EMxFileIOTest, CanReadFileOk1)
CXXTEST_TEST_REGISTRATION(EMxFileIOTest, CanReadFileOk2)
//CXXTEST_TEST_REGISTRATION(EMxFileIOTest, CanReadFileOk3)
#endif
