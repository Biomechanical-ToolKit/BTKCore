#ifndef XMOVEFileReaderTest_h
#define XMOVEFileReaderTest_h

#include "_TDDIO_Open3DMotion_Utils.h"

#include <btkAcquisitionFileReader.h>
#include <btkConvert.h>

CXXTEST_SUITE(XMOVEFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("test.XMOVE");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: test.XMOVE"));
  };
  
  CXXTEST_TEST(ADemo1_rewrite_XMove)
  {
    btk_o3dm_ADemo1_test(XMOVEFilePathIN + "ADemo1_rewrite_XMove.xml");
  };
};

CXXTEST_SUITE_REGISTRATION(XMOVEFileReaderTest)
CXXTEST_TEST_REGISTRATION(XMOVEFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(XMOVEFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(XMOVEFileReaderTest, ADemo1_rewrite_XMove)
#endif
