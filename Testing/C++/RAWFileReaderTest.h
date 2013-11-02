#ifndef RAWFileReaderTest_h
#define RAWFileReaderTest_h

#include <btkAcquisitionFileReader.h>

CXXTEST_SUITE(RAWFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("test.raw");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: test.raw"));
  };
  
  CXXTEST_TEST(File1123xa01)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(EliteFilePathIN + "1123xa01/1123xa01.RAW");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 100.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 20);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 540);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 100.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);

    TS_ASSERT_DELTA(acq->GetPoint(13)->GetValues().coeff(149,0), -385.70615, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(13)->GetValues().coeff(149,1), 338.20898, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(13)->GetValues().coeff(149,2), 222.04495, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(353,0), 1959.34851, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(353,1), 508.34012, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(353,2), 566.64362, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(316,0), 1635.93323, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(316,1), 357.17288, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(316,2), 440.82053, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(405,0), 2515.46265, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(405,1), 323.87112, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(405,2), 406.84567, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(13)->GetValues().coeff(273,0), 1187.13660, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(13)->GetValues().coeff(273,1), 353.16656, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(13)->GetValues().coeff(273,2), 223.44200, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(295,0), 1248.63220, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(295,1), 602.31555, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(295,2), 443.51645, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(453,0), 2733.84204, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(453,1), 307.90460, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(453,2), 375.83929, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(439,0), 2754.92139, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(439,1), 340.59167, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(439,2), 311.79965, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(332,0), 1669.32544, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(332,1), 988.52081, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(332,2), 209.06201, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(16)->GetValues().coeff(189,0), -60.90079, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(16)->GetValues().coeff(189,1), 139.96115, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(16)->GetValues().coeff(189,2), 244.33583, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(10)->GetValues().coeff(316,0), 1568.09045, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(10)->GetValues().coeff(316,1), 686.38354, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(10)->GetValues().coeff(316,2), 251.35460, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(15)->GetValues().coeff(408,0), 2321.67554, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(15)->GetValues().coeff(408,1), 131.93297, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(15)->GetValues().coeff(408,2), 131.60762, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(11)->GetValues().coeff(205,0), 225.91554, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(11)->GetValues().coeff(205,1), 589.97125, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(11)->GetValues().coeff(205,2), 189.19629, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(15)->GetValues().coeff(286,0), 1356.19678, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(15)->GetValues().coeff(286,1), 145.85417, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(15)->GetValues().coeff(286,2), 127.99424, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(504,0), 3009.21240, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(504,1), 776.25140, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(504,2), 137.65021, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(307,0), 1399.41785, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(307,1), 987.89197, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(307,2), 190.33136, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(15)->GetValues().coeff(87,0), -1282.99902, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(15)->GetValues().coeff(87,1), 136.25478, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(15)->GetValues().coeff(87,2), 108.94767, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(10)->GetValues().coeff(168,0), -141.90837, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(10)->GetValues().coeff(168,1), 657.51764, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(10)->GetValues().coeff(168,2), 220.82463, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(12)->GetValues().coeff(89,0), -1078.77649, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(12)->GetValues().coeff(89,1), 462.92294, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(12)->GetValues().coeff(89,2), 89.69810, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(13)->GetValues().coeff(142,0), -407.76535, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(13)->GetValues().coeff(142,1), 341.52997, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(13)->GetValues().coeff(142,2), 220.77994, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetValues().coeff(372,0), 2181.17065, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetValues().coeff(372,1), 305.07443, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetValues().coeff(372,2), 232.78011, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues().coeff(243,0), 734.33850, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues().coeff(243,1), 673.63373, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues().coeff(243,2), 390.69043, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(18)->GetValues().coeff(123,0), -744.32880, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(18)->GetValues().coeff(123,1), 1022.00867, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(18)->GetValues().coeff(123,2), 322.83267, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(16)->GetValues().coeff(82,0), -1329.35474, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(16)->GetValues().coeff(82,1), 54.96968, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(16)->GetValues().coeff(82,2), 234.59486, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(19)->GetValues().coeff(290,0), 1132.23718, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(19)->GetValues().coeff(290,1), 678.57300, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(19)->GetValues().coeff(290,2), 315.77557, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(15)->GetValues().coeff(2,0), -2177.69312, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(15)->GetValues().coeff(2,1), 132.39363, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(15)->GetValues().coeff(2,2), 116.22683, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(16)->GetValues().coeff(140,0), -437.25488, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(16)->GetValues().coeff(140,1), 49.86324, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(16)->GetValues().coeff(140,2), 242.16388, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(18)->GetValues().coeff(232,0), 503.02524, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(18)->GetValues().coeff(232,1), 1024.24841, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(18)->GetValues().coeff(232,2), 286.68088, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(78,0), -1165.62549, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(78,1), 668.81195, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(78,2), 379.26620, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(11)->GetValues().coeff(469,0), 2952.58716, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(11)->GetValues().coeff(469,1), 682.78882, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(11)->GetValues().coeff(469,2), 186.73724, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(296,0), 1261.42896, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(296,1), 601.23077, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(296,2), 441.87872, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(10)->GetValues().coeff(189,0), 105.55612, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(10)->GetValues().coeff(189,1), 677.94263, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(10)->GetValues().coeff(189,2), 228.86499, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues().coeff(216,0), 427.71317, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues().coeff(216,1), 660.23297, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues().coeff(216,2), 390.26993, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(129,0), -580.64294, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(129,1), 665.23450, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(129,2), 399.84921, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(99,0), -886.32263, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(99,1), 292.73615, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues().coeff(99,2), 420.51883, 1e-5);
    
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals().coeff(0), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetResiduals().coeff(0), 0.0, 1e-5);
    
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "r should");
    TS_ASSERT_EQUALS(acq->GetPoint(1)->GetLabel(), "r asis");
    TS_ASSERT_EQUALS(acq->GetPoint(2)->GetLabel(), "r thigh");
    TS_ASSERT_EQUALS(acq->GetPoint(3)->GetLabel(), "r bar 1");
    TS_ASSERT_EQUALS(acq->GetPoint(4)->GetLabel(), "r knee 1");
    TS_ASSERT_EQUALS(acq->GetPoint(5)->GetLabel(), "r knee 2");
    TS_ASSERT_EQUALS(acq->GetPoint(6)->GetLabel(), "r bar 2");
    TS_ASSERT_EQUALS(acq->GetPoint(7)->GetLabel(), "r mall");
    TS_ASSERT_EQUALS(acq->GetPoint(8)->GetLabel(), "r met");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetLabel(), "l should");
    TS_ASSERT_EQUALS(acq->GetPoint(10)->GetLabel(), "l asis");
    TS_ASSERT_EQUALS(acq->GetPoint(11)->GetLabel(), "l thigh");
    TS_ASSERT_EQUALS(acq->GetPoint(12)->GetLabel(), "l bar 1");
    TS_ASSERT_EQUALS(acq->GetPoint(13)->GetLabel(), "l knee 1");
    TS_ASSERT_EQUALS(acq->GetPoint(14)->GetLabel(), "l knee 2");
    TS_ASSERT_EQUALS(acq->GetPoint(15)->GetLabel(), "l bar 2");
    TS_ASSERT_EQUALS(acq->GetPoint(16)->GetLabel(), "l mall");
    TS_ASSERT_EQUALS(acq->GetPoint(17)->GetLabel(), "l met");
    TS_ASSERT_EQUALS(acq->GetPoint(18)->GetLabel(), "c7");
    TS_ASSERT_EQUALS(acq->GetPoint(19)->GetLabel(), "sacrum");
  };
};

CXXTEST_SUITE_REGISTRATION(RAWFileReaderTest)
CXXTEST_TEST_REGISTRATION(RAWFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(RAWFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(RAWFileReaderTest, File1123xa01)
#endif
