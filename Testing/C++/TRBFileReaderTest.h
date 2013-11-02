#ifndef TRBFileReaderTest_h
#define TRBFileReaderTest_h

#include <btkAcquisitionFileReader.h>
#include <btkTRBFileIO.h>

CXXTEST_SUITE(TRBFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("test.trb");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: test.trb"));
  };

  CXXTEST_TEST(FalseFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(TRBFilePathIN + "False.trb");
    reader->SetAcquisitionIO(btk::TRBFileIO::New());
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::TRBFileIOException &e, e.what(), std::string("Invalid header key."));
  };
  
  CXXTEST_TEST(Gait)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(TRBFilePathIN + "gait.trb");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 100.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 33);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 487);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 100.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPointUnit(), "mm");

    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "R.Shoulder");
    TS_ASSERT_EQUALS(acq->GetPoint(1)->GetLabel(), "R.Offset");
    TS_ASSERT_EQUALS(acq->GetPoint(2)->GetLabel(), "R.Elbow");
    TS_ASSERT_EQUALS(acq->GetPoint(3)->GetLabel(), "R.Wrist");
    TS_ASSERT_EQUALS(acq->GetPoint(4)->GetLabel(), "L.Shoulder");
    TS_ASSERT_EQUALS(acq->GetPoint(5)->GetLabel(), "L.Elbow");
    TS_ASSERT_EQUALS(acq->GetPoint(6)->GetLabel(), "L.Wrist");
    TS_ASSERT_EQUALS(acq->GetPoint(7)->GetLabel(), "R.ASIS");
    TS_ASSERT_EQUALS(acq->GetPoint(8)->GetLabel(), "L.ASIS");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetLabel(), "V.Sacral");
    TS_ASSERT_EQUALS(acq->GetPoint(10)->GetLabel(), "R.Thigh");
    TS_ASSERT_EQUALS(acq->GetPoint(11)->GetLabel(), "R.Knee");
    TS_ASSERT_EQUALS(acq->GetPoint(12)->GetLabel(), "R.Shank");
    TS_ASSERT_EQUALS(acq->GetPoint(13)->GetLabel(), "R.Ankle");
    TS_ASSERT_EQUALS(acq->GetPoint(14)->GetLabel(), "R.Heel");
    TS_ASSERT_EQUALS(acq->GetPoint(15)->GetLabel(), "R.Toe");
    TS_ASSERT_EQUALS(acq->GetPoint(16)->GetLabel(), "L.Thigh");
    TS_ASSERT_EQUALS(acq->GetPoint(17)->GetLabel(), "L.Knee");
    TS_ASSERT_EQUALS(acq->GetPoint(18)->GetLabel(), "L.Shank");
    TS_ASSERT_EQUALS(acq->GetPoint(19)->GetLabel(), "L.Ankle");
    TS_ASSERT_EQUALS(acq->GetPoint(20)->GetLabel(), "L.Heel");
    TS_ASSERT_EQUALS(acq->GetPoint(21)->GetLabel(), "L.Toe");
    TS_ASSERT_EQUALS(acq->GetPoint(22)->GetLabel(), "R.Knee.Medial");
    TS_ASSERT_EQUALS(acq->GetPoint(23)->GetLabel(), "R.Ankle.Medial");
    TS_ASSERT_EQUALS(acq->GetPoint(24)->GetLabel(), "L.Knee.Medial");
    TS_ASSERT_EQUALS(acq->GetPoint(25)->GetLabel(), "L.Ankle.Medial");
    TS_ASSERT_EQUALS(acq->GetPoint(26)->GetLabel(), "R.Foot.Medial");
    TS_ASSERT_EQUALS(acq->GetPoint(27)->GetLabel(), "R.Foot.Lateral");
    TS_ASSERT_EQUALS(acq->GetPoint(28)->GetLabel(), "L.Foot.Medial");
    TS_ASSERT_EQUALS(acq->GetPoint(29)->GetLabel(), "L.Foot.Lateral");
    TS_ASSERT_EQUALS(acq->GetPoint(30)->GetLabel(), "C7");
    TS_ASSERT_EQUALS(acq->GetPoint(31)->GetLabel(), "IJ");
    TS_ASSERT_EQUALS(acq->GetPoint(32)->GetLabel(), "PX");
    
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(133,0), 2229.475342, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(10)->GetValues()(142,1), 125.505516, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(15)->GetResiduals()(142), 0.231882, 1e-5);
  };
  
  CXXTEST_TEST(Anto_TRB_vs_TRC)
  {
    // TRB
    btk::AcquisitionFileReader::Pointer trbReader = btk::AcquisitionFileReader::New();
    trbReader->SetFilename(TRBFilePathIN + "Anto.trb");
    btk::Acquisition::Pointer acq1 = trbReader->GetOutput();
    acq1->Update();
    // TRC
    btk::AcquisitionFileReader::Pointer trcReader = btk::AcquisitionFileReader::New();
    trcReader->SetFilename(TRCFilePathIN + "Anto.trc");
    btk::Acquisition::Pointer acq2 = trcReader->GetOutput();
    acq2->Update();
    TS_ASSERT_EQUALS(acq1->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq1->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq1->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq1->GetAnalogNumber(), acq2->GetAnalogNumber());
    TS_ASSERT_EQUALS(acq1->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq1->GetAnalogFrameNumber(), acq2->GetAnalogFrameNumber());
    TS_ASSERT_EQUALS(acq1->GetEventNumber(), acq2->GetEventNumber());
    
    for (int i = 1 ; i < acq1->GetPointFrameNumber() ; ++i)
    {
      for (int j = 0 ; j < acq1->GetPointNumber() ; ++j)
      {
        TS_ASSERT_DELTA(acq1->GetPoint(j)->GetValues()(i,0), acq2->GetPoint(j)->GetValues()(i,0), 1e-5);
        TS_ASSERT_DELTA(acq1->GetPoint(j)->GetValues()(i,1), acq2->GetPoint(j)->GetValues()(i,1), 1e-5);
        TS_ASSERT_DELTA(acq1->GetPoint(j)->GetValues()(i,2), acq2->GetPoint(j)->GetValues()(i,2), 1e-5);
      }
    }
  }
};

CXXTEST_SUITE_REGISTRATION(TRBFileReaderTest)
CXXTEST_TEST_REGISTRATION(TRBFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(TRBFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(TRBFileReaderTest, FalseFile)
CXXTEST_TEST_REGISTRATION(TRBFileReaderTest, Gait)
CXXTEST_TEST_REGISTRATION(TRBFileReaderTest, Anto_TRB_vs_TRC)
#endif
