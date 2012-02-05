#ifndef TDFFileReaderTest_h
#define TDFFileReaderTest_h

#include <btkAcquisitionFileReader.h>
#include <btkTDFFileIO.h>

CXXTEST_SUITE(TDFFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("test.tdf");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: test.tdf"));
  };

  CXXTEST_TEST(FalseFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(TDFFilePathIN + "false.tdf");
    reader->SetAcquisitionIO(btk::TDFFileIO::New());
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::TDFFileIOException &e, e.what(), std::string("Invalid header key."));
  };
  
  CXXTEST_TEST(gait9)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(TDFFilePathIN + "gait9.tdf");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 250.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 30);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 1686);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 1000.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 18);
  };
};

CXXTEST_SUITE_REGISTRATION(TDFFileReaderTest)
CXXTEST_TEST_REGISTRATION(TDFFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(TDFFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(TDFFileReaderTest, FalseFile)
CXXTEST_TEST_REGISTRATION(TDFFileReaderTest, gait9)
#endif
