#ifndef EMxFileReaderTest_h
#define EMxFileReaderTest_h

#include <btkAcquisitionFileReader.h>

CXXTEST_SUITE(EMxFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("test.emg");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: test.emg"));
  };
  
  CXXTEST_TEST(File1123xa01_G)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(EliteFilePathIN + "1123xa01/1123xa01.EMG");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 1000.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 4564);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 4564);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 1000.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 8);
    
    TS_ASSERT_EQUALS(acq->GetAnalog(0)->GetLabel(), "R_RF");
    TS_ASSERT_EQUALS(acq->GetAnalog(1)->GetLabel(), "R_BFCL");
    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetLabel(), "R_TA");
    TS_ASSERT_EQUALS(acq->GetAnalog(3)->GetLabel(), "R_GAL");
    TS_ASSERT_EQUALS(acq->GetAnalog(4)->GetLabel(), "L_RF");
    TS_ASSERT_EQUALS(acq->GetAnalog(5)->GetLabel(), "L_BFCL");
    TS_ASSERT_EQUALS(acq->GetAnalog(6)->GetLabel(), "L_TA");
    TS_ASSERT_EQUALS(acq->GetAnalog(7)->GetLabel(), "L_GAL");
    
    TS_ASSERT_EQUALS(acq->GetAnalog(0)->GetUnit(), "mV");
    TS_ASSERT_EQUALS(acq->GetAnalog(1)->GetUnit(), "mV");
    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetUnit(), "mV");
    TS_ASSERT_EQUALS(acq->GetAnalog(3)->GetUnit(), "mV");
    TS_ASSERT_EQUALS(acq->GetAnalog(4)->GetUnit(), "mV");
    TS_ASSERT_EQUALS(acq->GetAnalog(5)->GetUnit(), "mV");
    TS_ASSERT_EQUALS(acq->GetAnalog(6)->GetUnit(), "mV");
    TS_ASSERT_EQUALS(acq->GetAnalog(7)->GetUnit(), "mV");
    
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathIN + "others/1123x01EMG.c3d");
    reader2->Update();
    btk::Acquisition::Pointer acq2 = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), acq2->GetAnalogFrameNumber());
    TS_ASSERT_EQUALS(acq->GetPointUnit(), acq2->GetPointUnit());
    for (int i = 1 ; i < 50 ; i+=10)
    {
      TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(i), acq2->GetAnalog(0)->GetValues()(i), 0.00001);
      TS_ASSERT_DELTA(acq->GetAnalog(1)->GetValues()(i), acq2->GetAnalog(1)->GetValues()(i), 0.00001);
      TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(i), acq2->GetAnalog(2)->GetValues()(i), 0.00001);
      TS_ASSERT_DELTA(acq->GetAnalog(3)->GetValues()(i), acq2->GetAnalog(3)->GetValues()(i), 0.00001);
      TS_ASSERT_DELTA(acq->GetAnalog(4)->GetValues()(i), acq2->GetAnalog(4)->GetValues()(i), 0.00001);
      TS_ASSERT_DELTA(acq->GetAnalog(5)->GetValues()(i), acq2->GetAnalog(5)->GetValues()(i), 0.00001);
      TS_ASSERT_DELTA(acq->GetAnalog(6)->GetValues()(i), acq2->GetAnalog(6)->GetValues()(i), 0.00001);
      TS_ASSERT_DELTA(acq->GetAnalog(7)->GetValues()(i), acq2->GetAnalog(7)->GetValues()(i), 0.00001);
    }
  };
  
  CXXTEST_TEST(File1123xa01_F)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(EliteFilePathIN + "1123xa01/1123xa01.EMF");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 1000.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 4564);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 4564);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 1000.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 8);
    
    TS_ASSERT_EQUALS(acq->GetAnalog(0)->GetLabel(), "R_RF");
    TS_ASSERT_EQUALS(acq->GetAnalog(1)->GetLabel(), "R_BFCL");
    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetLabel(), "R_TA");
    TS_ASSERT_EQUALS(acq->GetAnalog(3)->GetLabel(), "R_GAL");
    TS_ASSERT_EQUALS(acq->GetAnalog(4)->GetLabel(), "L_RF");
    TS_ASSERT_EQUALS(acq->GetAnalog(5)->GetLabel(), "L_BFCL");
    TS_ASSERT_EQUALS(acq->GetAnalog(6)->GetLabel(), "L_TA");
    TS_ASSERT_EQUALS(acq->GetAnalog(7)->GetLabel(), "L_GAL");
    
    TS_ASSERT_EQUALS(acq->GetAnalog(0)->GetUnit(), "mV");
    TS_ASSERT_EQUALS(acq->GetAnalog(1)->GetUnit(), "mV");
    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetUnit(), "mV");
    TS_ASSERT_EQUALS(acq->GetAnalog(3)->GetUnit(), "mV");
    TS_ASSERT_EQUALS(acq->GetAnalog(4)->GetUnit(), "mV");
    TS_ASSERT_EQUALS(acq->GetAnalog(5)->GetUnit(), "mV");
    TS_ASSERT_EQUALS(acq->GetAnalog(6)->GetUnit(), "mV");
    TS_ASSERT_EQUALS(acq->GetAnalog(7)->GetUnit(), "mV");
  };
  
  CXXTEST_TEST(File1123xa01_R)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(EliteFilePathIN + "1123xa01/1123xa01.EMR");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 1000.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 4564);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 4564);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 1000.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 8);
    
    TS_ASSERT_EQUALS(acq->GetAnalog(0)->GetLabel(), "R_RF");
    TS_ASSERT_EQUALS(acq->GetAnalog(1)->GetLabel(), "R_BFCL");
    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetLabel(), "R_TA");
    TS_ASSERT_EQUALS(acq->GetAnalog(3)->GetLabel(), "R_GAL");
    TS_ASSERT_EQUALS(acq->GetAnalog(4)->GetLabel(), "L_RF");
    TS_ASSERT_EQUALS(acq->GetAnalog(5)->GetLabel(), "L_BFCL");
    TS_ASSERT_EQUALS(acq->GetAnalog(6)->GetLabel(), "L_TA");
    TS_ASSERT_EQUALS(acq->GetAnalog(7)->GetLabel(), "L_GAL");
    
    TS_ASSERT_EQUALS(acq->GetAnalog(0)->GetUnit(), "mV");
    TS_ASSERT_EQUALS(acq->GetAnalog(1)->GetUnit(), "mV");
    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetUnit(), "mV");
    TS_ASSERT_EQUALS(acq->GetAnalog(3)->GetUnit(), "mV");
    TS_ASSERT_EQUALS(acq->GetAnalog(4)->GetUnit(), "mV");
    TS_ASSERT_EQUALS(acq->GetAnalog(5)->GetUnit(), "mV");
    TS_ASSERT_EQUALS(acq->GetAnalog(6)->GetUnit(), "mV");
    TS_ASSERT_EQUALS(acq->GetAnalog(7)->GetUnit(), "mV");
  };
};

CXXTEST_SUITE_REGISTRATION(EMxFileReaderTest)
CXXTEST_TEST_REGISTRATION(EMxFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(EMxFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(EMxFileReaderTest, File1123xa01_G)
//CXXTEST_TEST_REGISTRATION(EMxFileReaderTest, File1123xa01_F)
//CXXTEST_TEST_REGISTRATION(EMxFileReaderTest, File1123xa01_R)
#endif
