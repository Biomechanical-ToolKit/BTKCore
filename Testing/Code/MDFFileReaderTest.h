#ifndef MDFFileReaderTest_h
#define MDFFileReaderTest_h

#include "_TDDIO_Open3DMotion_Utils.h"

#include <btkAcquisitionFileReader.h>
#include <btkConvert.h>

CXXTEST_SUITE(MDFFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("test.MDF");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: test.MDF"));
  };
  
  CXXTEST_TEST(Gait_bilateral_1997_Kistlerx1)
  {
    btk_o3dm_ADemo1_test(MDFFilePathIN + "gait-bilateral-1997-Kistlerx1.mdf");
  };
};

CXXTEST_SUITE_REGISTRATION(MDFFileReaderTest)
CXXTEST_TEST_REGISTRATION(MDFFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(MDFFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(MDFFileReaderTest, Gait_bilateral_1997_Kistlerx1)
#endif
