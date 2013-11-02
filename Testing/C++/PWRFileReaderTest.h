#ifndef PWRFileReaderTest_h
#define PWRFileReaderTest_h

#include <btkAcquisitionFileReader.h>

CXXTEST_SUITE(PWRFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("test.pwr");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: test.pwr"));
  };
  
  CXXTEST_TEST(File1123xa01)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(EliteFilePathIN + "1123xa01/1123xa01.PWR");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 100.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 6);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 540);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 100.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(314,0), -0.11903, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(330,0), -0.13812, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(284,0), -0.09118, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(268,0), 0.09010, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(310,0), 0.07822, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(356,0), 0.98495, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(298,0), 1.11355, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(195,0), 1.65349, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues().coeff(264,0), -0.11671, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(152,0), -0.95714, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(375,0), -0.30800, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(289,0), -0.29107, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(305,0), 1.27361, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues().coeff(225,0), -1.75807, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(309,0), 4.05187, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(398,0), -2.57781, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues().coeff(159,0), -12.45553, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(252,0), 7.94718, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(301,0), 0.10317, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(187,0), -15.32014, 1e-5);
    
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetResiduals().coeff(13), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetResiduals().coeff(474), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetResiduals().coeff(0), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetResiduals().coeff(432), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetResiduals().coeff(398), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetResiduals().coeff(133), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetResiduals().coeff(45), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetResiduals().coeff(481), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetResiduals().coeff(415), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetResiduals().coeff(501), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals().coeff(9), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetResiduals().coeff(465), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetResiduals().coeff(456), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetResiduals().coeff(17), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetResiduals().coeff(66), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetResiduals().coeff(79), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetResiduals().coeff(23), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetResiduals().coeff(66), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetResiduals().coeff(460), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetResiduals().coeff(145), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals().coeff(511), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals().coeff(57), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetResiduals().coeff(89), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetResiduals().coeff(502), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetResiduals().coeff(464), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetResiduals().coeff(531), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetResiduals().coeff(424), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetResiduals().coeff(95), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals().coeff(72), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetResiduals().coeff(507), -1.0, 1e-5);
    
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RHipPower");
    TS_ASSERT_EQUALS(acq->GetPoint(1)->GetLabel(), "RKneePower");
    TS_ASSERT_EQUALS(acq->GetPoint(2)->GetLabel(), "RAnklePower");
    TS_ASSERT_EQUALS(acq->GetPoint(3)->GetLabel(), "LHipPower");
    TS_ASSERT_EQUALS(acq->GetPoint(4)->GetLabel(), "LKneePower");
    TS_ASSERT_EQUALS(acq->GetPoint(5)->GetLabel(), "LAnklePower");
  };
};

CXXTEST_SUITE_REGISTRATION(PWRFileReaderTest)
CXXTEST_TEST_REGISTRATION(PWRFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(PWRFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(PWRFileReaderTest, File1123xa01)
#endif
