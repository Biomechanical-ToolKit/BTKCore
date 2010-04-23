#ifndef ANGFileReaderTest_h
#define ANGFileReaderTest_h

#include <btkAcquisitionFileReader.h>

CXXTEST_SUITE(ANGFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("test.ang");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: test.ang"));
  };
  
  CXXTEST_TEST(File1123xa01)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(EliteFilePathIN + "1123xa01/1123xa01.ANG");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 100.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 12);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 540);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 100.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(359,1), -8.62173, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(359,2), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(10)->GetValues().coeff(359,2), 13.39856, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(248,0), 1.48459, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(248,1), -4.36151, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(248,2), 48.60806, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(348,0), 3.01091, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(348,1), 3.93414, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(348,2), 19.30316, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues().coeff(260,0), 10.01915, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues().coeff(260,1), 14.19309, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues().coeff(260,2), 23.92921, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(318,0), 3.88671, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(318,1), 14.77117, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(318,2), 5.23453, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(207,1), -7.01393, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(10)->GetValues().coeff(207,2), 10.17704, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(135,0), 1.91810, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(135,1), 10.64852, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(135,2), 23.68338, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(333,0), -0.89883, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(333,1), 14.37692, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(333,2), -5.21001, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(445,0), 0.81079, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(445,1), 4.76353, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(445,2), 42.34875, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(394,0), 12.11894, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(394,1), 2.17873, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(394,2), 63.12521, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues().coeff(315,0), -1.25837, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues().coeff(315,1), 9.49885, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues().coeff(315,2), 28.33806, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(320,0), -0.45823, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(320,1), -5.18394, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(320,2), 6.23814, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(229,0), 12.74405, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(229,1), 5.51828, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(229,2), 65.28245, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(228,0), -1.85471, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(228,1), -4.04436, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(228,2), 7.50404, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(323,0), 5.08118, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(323,1), -9.89504, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(323,2), 9.27475, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(344,0), 3.54436, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(344,1), 1.28627, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(344,2), 4.43860, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(6)->GetValues().coeff(286,1), -16.39735, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(11)->GetValues().coeff(286,2), 1.89822, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(8)->GetValues().coeff(220,0), 2.46877, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(8)->GetValues().coeff(220,1), 3.31622, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(8)->GetValues().coeff(220,2), 6.96335, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(387,0), -2.24494, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(387,1), 2.22020, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(387,2), 43.82305, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(286,0), 2.24239, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(286,1), 4.03521, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(286,2), 13.45145, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(6)->GetValues().coeff(57,1), -11.85689, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(11)->GetValues().coeff(57,2), -16.68744, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(420,0), 8.57384, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(420,1), -19.39450, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(420,2), 16.32786, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(237,1), -14.43936, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(10)->GetValues().coeff(237,2), 10.31896, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(8)->GetValues().coeff(247,0), -3.10765, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(8)->GetValues().coeff(247,1), 0.30909, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(8)->GetValues().coeff(247,2), 5.85825, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(279,0), 0.02048, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(279,1), -1.24290, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(279,2), 45.71421, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(344,0), 3.44349, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(344,1), -1.77315, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(344,2), 47.57365, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(6)->GetValues().coeff(129,1), -15.23476, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(11)->GetValues().coeff(129,2), -0.52795, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(6)->GetValues().coeff(156,1), -16.40787, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(11)->GetValues().coeff(156,2), 9.14858, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(375,0), 0.77053, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(375,1), -0.82011, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(375,2), 4.43281, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(137,0), 2.28143, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(137,1), 11.62174, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(137,2), 22.88744, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(8)->GetValues().coeff(360,0), -1.32845, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(8)->GetValues().coeff(360,1), -1.08887, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(8)->GetValues().coeff(360,2), 5.23377, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(7)->GetValues().coeff(186,0), 2.30572, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(7)->GetValues().coeff(186,1), -2.29434, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(7)->GetValues().coeff(186,2), 47.98306, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(364,0), 1.15051, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(364,1), 0.49774, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(364,2), 5.13641, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(325,0), 4.77990, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(325,1), -1.42330, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(325,2), 23.07696, 1e-5);
    
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetResiduals().coeff(359), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetResiduals().coeff(207), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(6)->GetResiduals().coeff(286), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(6)->GetResiduals().coeff(57), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetResiduals().coeff(237), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(6)->GetResiduals().coeff(129), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(6)->GetResiduals().coeff(156), 0.0, 1e-5);
    
    TS_ASSERT_DELTA(acq->GetPoint(6)->GetResiduals().coeff(376), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetResiduals().coeff(96), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetResiduals().coeff(539), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetResiduals().coeff(17), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(6)->GetResiduals().coeff(476), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetResiduals().coeff(102), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(8)->GetResiduals().coeff(84), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetResiduals().coeff(103), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(8)->GetResiduals().coeff(489), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetResiduals().coeff(441), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetResiduals().coeff(87), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(6)->GetResiduals().coeff(375), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetResiduals().coeff(37), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetResiduals().coeff(49), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetResiduals().coeff(82), -1.0, 1e-5);
    
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RPelvisAngle");
    TS_ASSERT_EQUALS(acq->GetPoint(1)->GetLabel(), "RHipAngle");
    TS_ASSERT_EQUALS(acq->GetPoint(2)->GetLabel(), "LHipAngle");
    TS_ASSERT_EQUALS(acq->GetPoint(3)->GetLabel(), "RKneeAngle");
    TS_ASSERT_EQUALS(acq->GetPoint(4)->GetLabel(), "LKneeAngle");
    TS_ASSERT_EQUALS(acq->GetPoint(5)->GetLabel(), "RAnkleAngle");
    TS_ASSERT_EQUALS(acq->GetPoint(6)->GetLabel(), "LAnkleAngle");
    TS_ASSERT_EQUALS(acq->GetPoint(7)->GetLabel(), "RShoulderAngle");
    TS_ASSERT_EQUALS(acq->GetPoint(8)->GetLabel(), "LPelvisAngle");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetLabel(), "LShoulderAngle");
    TS_ASSERT_EQUALS(acq->GetPoint(10)->GetLabel(), "RFootProgressAngle");
    TS_ASSERT_EQUALS(acq->GetPoint(11)->GetLabel(), "LFootProgressAngle");
  };
};

CXXTEST_SUITE_REGISTRATION(ANGFileReaderTest)
CXXTEST_TEST_REGISTRATION(ANGFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(ANGFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(ANGFileReaderTest, File1123xa01)
#endif
