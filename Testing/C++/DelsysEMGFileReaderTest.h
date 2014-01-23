#ifndef DelsysEMGFileReaderTest_h
#define DelsysEMGFileReaderTest_h

#include <btkAcquisitionFileReader.h>
#include <btkDelsysEMGFileIO.h>
#include <btkConvert.h>

CXXTEST_SUITE(DelsysEMGFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("foo.emg");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: foo.emg"));
  };

  CXXTEST_TEST(FileFormatV3)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(DelsysEMGFilePathIN + "Set1[Rep2]_v3.emg");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    btk::DelsysEMGFileIO::Pointer io = static_pointer_cast<btk::DelsysEMGFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetFileVersion(),3);
    
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(),8);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(),60000);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(),2000);
    for (int i = 0 ; i < 8; ++i)
    {
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetLabel(), "EMG Ch" + btk::ToString(i+1));
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetGain(), btk::Analog::PlusMinus1);
    }
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues().coeff(0), 1.6327e-5, 1e-9);
    TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues().coeff(29999), 3.4180e-5, 1e-9);
    TS_ASSERT_DELTA(acq->GetAnalog(5)->GetValues().coeff(12000), 3.4180e-5, 1e-9)
    TS_ASSERT_DELTA(acq->GetAnalog(7)->GetValues().coeff(59999), 4.3945e-5, 1e-9);
  };
};

CXXTEST_SUITE_REGISTRATION(DelsysEMGFileReaderTest)
CXXTEST_TEST_REGISTRATION(DelsysEMGFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(DelsysEMGFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(DelsysEMGFileReaderTest, FileFormatV3)
#endif
