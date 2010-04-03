#ifndef MOMFileReaderTest_h
#define MOMFileReaderTest_h

#include <btkAcquisitionFileReader.h>

CXXTEST_SUITE(MOMFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("test.mom");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: test.mom"));
  };
  
  CXXTEST_TEST(File1123xa01)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(EliteFilePathIN + "1123xa01/1123xa01.MOM");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 100.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 6);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 540);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 100.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(281,0), 0.25327*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(281,1), 0.03494*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(281,2), -0.96760*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals().coeff(281), 0.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(390,0), 0.10561*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(390,1), -0.12087*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(390,2), -1.84825*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals().coeff(390), 0.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(356,0), 0.75189*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(356,1), 0.15205*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(356,2), -0.19614*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetResiduals().coeff(356), 0.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(432,0), 0.00000, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(432,1), 0.00000, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(432,2), 0.00000, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetResiduals().coeff(432), 0.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(233,0), -0.17147*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(233,1), 0.21526*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(233,2), -8.19451*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetResiduals().coeff(233), 0.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(93,0), 0.00000, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(93,1), 0.00000, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(93,2), 0.00000, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(32,0), 0.00000, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(32,1), 0.00000, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(32,2), 0.00000, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(284,0), 0.02494*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(284,1), 0.00545*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(284,2), -0.10915*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(354,0), 0.82112*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(354,1), 0.08304*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(354,2), -0.70758*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(157,0), 0.02302*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(157,1), 0.01164*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(157,2), -0.08758*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(8,0), -0.00000, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(8,1), -0.00000, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(8,2), 0.00000, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues().coeff(201,0), 2.94973*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues().coeff(201,1), -4.38634*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues().coeff(201,2), -1.99278*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(264,0), 0.01296*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(264,1), -0.00259*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(264,2), -0.10106*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(513,0), -0.00000, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(513,1), -0.00000, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(513,2), 0.00000, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(295,0), 0.01977*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(295,1), 0.03352*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(295,2), -0.07875*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(225,0), -6.29219*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(225,1), -0.04541*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(225,2), 11.31787*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(162,0), -0.04136*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(162,1), -0.03566*1000.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(162,2), -0.40256*1000.0, 1e-2);
    
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals().coeff(22), -1.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetResiduals().coeff(441), -1.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetResiduals().coeff(525), -1.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals().coeff(045), -1.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetResiduals().coeff(448), -1.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals().coeff(90), -1.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetResiduals().coeff(28), -1.0, 1e-2);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetResiduals().coeff(145), -1.0, 1e-2);
    
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RHipMoment");
    TS_ASSERT_EQUALS(acq->GetPoint(1)->GetLabel(), "RKneeMoment");
    TS_ASSERT_EQUALS(acq->GetPoint(2)->GetLabel(), "RAnkleMoment");
    TS_ASSERT_EQUALS(acq->GetPoint(3)->GetLabel(), "LHipMoment");
    TS_ASSERT_EQUALS(acq->GetPoint(4)->GetLabel(), "LKneeMoment");
    TS_ASSERT_EQUALS(acq->GetPoint(5)->GetLabel(), "LAnkleMoment");
  };
};

CXXTEST_SUITE_REGISTRATION(MOMFileReaderTest)
CXXTEST_TEST_REGISTRATION(MOMFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(MOMFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(MOMFileReaderTest, File1123xa01)
#endif
