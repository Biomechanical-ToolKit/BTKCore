#ifndef C3DFileReaderTest_h
#define C3DFileReaderTest_h

#include <btkAcquisitionFileReader.h>

CXXTEST_SUITE(C3DFileReaderTest)
{
  // ================ Errors ================
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("test.c3d");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: test.c3d"));
  };
  
  // ================ www.c3d.org : Sample01 ================
  
  CXXTEST_TEST(Sample01_Eb015pi)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePath + "sample01/Eb015pi.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetMarkerFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetMarkerNumber(), 26);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetMarker(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetMarker(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_EQUALS(acq->GetAnalog(15)->GetLabel(), "CH16");
    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetDescription(), "FORCE Z-COMP 1");
    TS_ASSERT_DELTA(acq->GetMarker(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(0)->GetResidual()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(14)->GetResidual()(20), 1.91667, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(0,0), -26.6599, 0.001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(1,0), -25.8, 0.0001);
    TS_ASSERT_EQUALS(acq->GetMarker(14)->HasParent(), true);
    TS_ASSERT_EQUALS(acq->GetAnalog(14)->HasParent(), true);
    TS_ASSERT_EQUALS(acq->GetMetaData()->HasParent(), true);
    /*
    for (int i = 0 ; i < 10 ; ++i)
    {
      for (int j = 0 ; j < 3 ; ++j)
        std::cout << acq->GetMarker(0)->GetValues()(i,j) << " ";
      std::cout << "| "<< acq->GetMarker(0)->GetResidual()(i) << "\n";
    }
    std::cout << std::endl;
     */
  };
  
  CXXTEST_TEST(Sample01_Eb015si)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePath + "sample01/Eb015si.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetMarkerFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetMarkerNumber(), 26);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetMarker(0)->GetLabel(), "RFT1");
    TS_ASSERT_DELTA(acq->GetMarker(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(0)->GetResidual()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(14)->GetResidual()(20), 1.91667, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(0,0), -26.6599, 0.001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(1,0), -25.8, 0.0001);
  };
  
  CXXTEST_TEST(Sample01_Eb015vi)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePath + "sample01/Eb015vi.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetMarkerFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetMarkerNumber(), 26);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetMarker(0)->GetLabel(), "RFT1");
    TS_ASSERT_DELTA(acq->GetMarker(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(0)->GetResidual()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(14)->GetResidual()(20), 1.91667, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(0,0), -26.6599, 0.001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(1,0), -25.8, 0.0001);
  };
  
  CXXTEST_TEST(Sample01_Eb015pr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePath + "sample01/Eb015pr.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetMarkerFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetMarkerNumber(), 26);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetMarker(0)->GetLabel(), "RFT1");
    TS_ASSERT_DELTA(acq->GetMarker(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(0)->GetResidual()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(14)->GetResidual()(20), 1.91667, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(0,0), -26.6599, 0.001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(1,0), -25.8, 0.0001);
  };
  
  CXXTEST_TEST(Sample01_Eb015sr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePath + "sample01/Eb015sr.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetMarkerFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetMarkerNumber(), 26);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetMarker(0)->GetLabel(), "RFT1");
    TS_ASSERT_DELTA(acq->GetMarker(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(0)->GetResidual()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(14)->GetResidual()(20), 1.91667, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(0,0), -26.6599, 0.001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(1,0), -25.8, 0.0001);
  };
  
  CXXTEST_TEST(Sample01_Eb015vr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePath + "sample01/Eb015vr.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetMarkerFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetMarkerNumber(), 26);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetMarker(0)->GetLabel(), "RFT1");
    TS_ASSERT_DELTA(acq->GetMarker(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(0)->GetResidual()(0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetMarker(14)->GetResidual()(20), 1.91667, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(0), -26.6599, 0.001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(1), -25.8, 0.0001);
  };
  
};

CXXTEST_SUITE_REGISTRATION(C3DFileReaderTest)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample01_Eb015pi)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample01_Eb015si)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample01_Eb015vi)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample01_Eb015pr)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample01_Eb015sr)
CXXTEST_TEST_REGISTRATION(C3DFileReaderTest, Sample01_Eb015vr)
#endif
