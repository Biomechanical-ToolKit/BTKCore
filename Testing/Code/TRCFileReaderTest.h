#ifndef TRCFileReaderTest_h
#define TRCFileReaderTest_h

#include <btkAcquisitionFileReader.h>

CXXTEST_SUITE(TRCFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("test.trc");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: test.trc"));
  };

  CXXTEST_TEST(TruncatedFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(TRCFilePathIN + "Truncated.trc");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::TRCFileIOException &e, e.what(), std::string("Unexcepted end of file."));
  };

  
  CXXTEST_TEST(MOTEK_T)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(TRCFilePathIN + "MOTEK/T.trc");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 30);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 41);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 89);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 30);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPointUnit(), "mm");

    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues()(3,0) , -171.03949, 0.000001);
    TS_ASSERT_EQUALS(acq->GetPoint(1)->GetLabel().compare("LBHD"), 0);
  };

  CXXTEST_TEST(MOTEK_ballet)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(TRCFilePathIN + "MOTEK/ballet_reduced.trc");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 30);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 41);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 100);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 30);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);

    TS_ASSERT_DELTA(acq->GetPoint(40)->GetValues()(82,2), -112.24601, 0.000001);

  };

  CXXTEST_TEST(KneeWithOcclusion)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(TRCFilePathIN + "Knee.trc");
    //double start = reader->GetTimestamp();
    reader->Update();
    //std::cout << std::endl << (reader->GetTimestamp() - start) / 1000 << std::endl;
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 71);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);

    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues()(83,0), 301.65476, 0.000001);
    TS_ASSERT_DELTA(acq->GetPoint(70)->GetResiduals()(82), -1.0, 0.000001);
  };
};

CXXTEST_SUITE_REGISTRATION(TRCFileReaderTest)
CXXTEST_TEST_REGISTRATION(TRCFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(TRCFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(TRCFileReaderTest, TruncatedFile)
CXXTEST_TEST_REGISTRATION(TRCFileReaderTest, MOTEK_T)
CXXTEST_TEST_REGISTRATION(TRCFileReaderTest, MOTEK_ballet)
CXXTEST_TEST_REGISTRATION(TRCFileReaderTest, KneeWithOcclusion)
#endif
