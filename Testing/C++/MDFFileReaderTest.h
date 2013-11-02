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
  
  CXXTEST_TEST(Gait_bilateral_1997_Kistlerx1_MDF_vs_C3D_float)
  {
    Gait_bilateral_1997_Kistlerx1_MDF_vs_C3D(C3DFilePathIN + "others/ADemo1_rewrite_c3d_pc_float.c3d");
  };
  
  CXXTEST_TEST(Gait_bilateral_1997_Kistlerx1_MDF_vs_C3D_integer)
  {
    Gait_bilateral_1997_Kistlerx1_MDF_vs_C3D(C3DFilePathIN + "others/ADemo1_rewrite_c3d_pc_integer.c3d");
  };
  
  void Gait_bilateral_1997_Kistlerx1_MDF_vs_C3D(const std::string& filename)
  {
    btk::AcquisitionFileReader::Pointer readerMDF = btk::AcquisitionFileReader::New();
    readerMDF->SetFilename(MDFFilePathIN + "gait-bilateral-1997-Kistlerx1.mdf");
    readerMDF->Update();
    btk::Acquisition::Pointer acqMDF = readerMDF->GetOutput();
     
    btk::AcquisitionFileReader::Pointer readerC3D = btk::AcquisitionFileReader::New();
    readerC3D->SetFilename(filename);
    readerC3D->Update();
    btk::Acquisition::Pointer acqC3D = readerC3D->GetOutput();
    
    TS_ASSERT_EQUALS(acqMDF->GetFirstFrame(), acqC3D->GetFirstFrame());
    TS_ASSERT_EQUALS(acqMDF->GetPointFrequency(), acqC3D->GetPointFrequency());
    TS_ASSERT_EQUALS(acqMDF->GetPointNumber(), acqC3D->GetPointNumber());
    TS_ASSERT_EQUALS(acqMDF->GetPointFrameNumber(), acqC3D->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acqMDF->GetAnalogFrequency(), acqC3D->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acqMDF->GetAnalogNumber(), acqC3D->GetAnalogNumber());
    
    // EMGs
    for (int i = 0 ; i < 8 ; ++i)
      for (int j = 0 ; j < acqC3D->GetAnalogFrameNumber() ; j+=10)
        TS_ASSERT_DELTA(acqMDF->GetAnalog(i)->GetValues()(j), acqC3D->GetAnalog(i)->GetValues()(j), 1e-5);
    // FPs
    for (int i = 8 ; i < 16 ; ++i)
      for (int j = 0 ; j < acqC3D->GetAnalogFrameNumber() ; j+=10)
        TS_ASSERT_DELTA(acqMDF->GetAnalog(i)->GetValues()(j), acqC3D->GetAnalog(i)->GetValues()(j), 1e-5);
  };
};

CXXTEST_SUITE_REGISTRATION(MDFFileReaderTest)
CXXTEST_TEST_REGISTRATION(MDFFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(MDFFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(MDFFileReaderTest, Gait_bilateral_1997_Kistlerx1)
CXXTEST_TEST_REGISTRATION(MDFFileReaderTest, Gait_bilateral_1997_Kistlerx1_MDF_vs_C3D_float)
CXXTEST_TEST_REGISTRATION(MDFFileReaderTest, Gait_bilateral_1997_Kistlerx1_MDF_vs_C3D_integer)
#endif
