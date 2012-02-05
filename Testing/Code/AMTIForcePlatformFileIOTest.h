#ifndef AMTIForcePlatformFileIOTest_h
#define AMTIForcePlatformFileIOTest_h

#include <btkAMTIForcePlatformFileIO.h>

CXXTEST_SUITE(AMTIForcePlatformFileIOTest)
{
  CXXTEST_TEST(CanReadFileEmpty)
  {
    btk::AMTIForcePlatformFileIO::Pointer pt = btk::AMTIForcePlatformFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(""), false);
  };
  
  CXXTEST_TEST(CanReadFileEmptyFile)
  {
    btk::AMTIForcePlatformFileIO::Pointer pt = btk::AMTIForcePlatformFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(AMTIForcePlatformFilePathIN + "empty.asc"), false);
  };
  
  CXXTEST_TEST(CanReadFileOk)
  {
    btk::AMTIForcePlatformFileIO::Pointer pt = btk::AMTIForcePlatformFileIO::New();
    TS_ASSERT_EQUALS(pt->CanReadFile(AMTIForcePlatformFilePathIN + "Trial1-FxFyFzMxMyMz.asc"), true);
  };
  
  CXXTEST_TEST(CanWriteFileEmpty)
  {
    btk::AMTIForcePlatformFileIO::Pointer pt = btk::AMTIForcePlatformFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile(""), false);
  };
  
  CXXTEST_TEST(CanWriteFileFail)
  {
    btk::AMTIForcePlatformFileIO::Pointer pt = btk::AMTIForcePlatformFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.jpeg"), false);
  };
  
  CXXTEST_TEST(CanWriteFileOk)
  {
    btk::AMTIForcePlatformFileIO::Pointer pt = btk::AMTIForcePlatformFileIO::New();
    TS_ASSERT_EQUALS(pt->CanWriteFile("test.asc"), true);
  };
};

CXXTEST_SUITE_REGISTRATION(AMTIForcePlatformFileIOTest)
CXXTEST_TEST_REGISTRATION(AMTIForcePlatformFileIOTest, CanReadFileEmpty)
CXXTEST_TEST_REGISTRATION(AMTIForcePlatformFileIOTest, CanReadFileEmptyFile)
CXXTEST_TEST_REGISTRATION(AMTIForcePlatformFileIOTest, CanReadFileOk)
CXXTEST_TEST_REGISTRATION(AMTIForcePlatformFileIOTest, CanWriteFileEmpty)
CXXTEST_TEST_REGISTRATION(AMTIForcePlatformFileIOTest, CanWriteFileFail)
CXXTEST_TEST_REGISTRATION(AMTIForcePlatformFileIOTest, CanWriteFileOk)
#endif
