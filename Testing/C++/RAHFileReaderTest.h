#ifndef RAHFileReaderTest_h
#define RAHFileReaderTest_h

#include <btkAcquisitionFileReader.h>

CXXTEST_SUITE(RAHFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("test.rah");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: test.rah"));
  };
  
  CXXTEST_TEST(TruncatedFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(EliteFilePathIN + "Truncated.RAH");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::Exception &e, e.what(), std::string("Unexpected end of file."));
  };

  CXXTEST_TEST(File1123xa01)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(EliteFilePathIN + "1123xa01/1123xa01.RAH");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 100.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 20);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 540);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 100.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);

    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(0,0), 0.0, 0.000001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(0,1), 0.0, 0.000001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(0,2), 0.0, 0.000001);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(0,0), -1852.34277, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(0,1), 499.53497, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(0,2), 509.78344, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(80,0), -1158.22705, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(80,1), 988.39789, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(80,2), 422.60355, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(10)->GetValues().coeff(234,0), 637.04022, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(10)->GetValues().coeff(234,1), 675.45465, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(10)->GetValues().coeff(234,2), 226.33702, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(18)->GetValues().coeff(439,0), 2604.32666, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(18)->GetValues().coeff(439,1), 1034.25464, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(18)->GetValues().coeff(439,2), 309.51300, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues().coeff(341,0), 1832.53162, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues().coeff(341,1), 680.60736, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues().coeff(341,2), 426.64050, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(10)->GetValues().coeff(150,0), -344.84213, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(10)->GetValues().coeff(150,1), 676.62488, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(10)->GetValues().coeff(150,2), 223.83476, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(432,0), 2629.85425, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(432,1), 620.29193, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(432,2), 419.01434, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(18)->GetValues().coeff(227,0), 448.21405, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(18)->GetValues().coeff(227,1), 1019.61627, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(18)->GetValues().coeff(227,2), 288.53629, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(19)->GetValues().coeff(427,0), 2495.63281, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(19)->GetValues().coeff(427,1), 688.92334, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(19)->GetValues().coeff(427,2), 300.66312, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(354,0), 1894.92065, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(354,1), 995.06921, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(354,2), 493.04913, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(18)->GetValues().coeff(458,0), 2747.07324, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(18)->GetValues().coeff(458,1), 1029.85999, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(18)->GetValues().coeff(458,2), 292.33994, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(15)->GetValues().coeff(366,0), 2121.45190, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(15)->GetValues().coeff(366,1), 143.69389, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(15)->GetValues().coeff(366,2), 146.04190, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(353,0), 1959.34851, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(353,1), 508.34012, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(353,2), 566.64362, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(381,0), 2150.47754, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(381,1), 991.99841, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(381,2), 455.05170, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(149,0), -377.55890, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(149,1), 1001.35638, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(149,2), 423.32904, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(6)->GetValues().coeff(375,0), 1971.25293, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(6)->GetValues().coeff(375,1), 160.61484, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(6)->GetValues().coeff(375,2), 545.25269, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(7)->GetValues().coeff(236,0), 602.27185, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(7)->GetValues().coeff(236,1), 96.85654, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(7)->GetValues().coeff(236,2), 383.63309, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(15)->GetValues().coeff(413,0), 2327.65698, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(15)->GetValues().coeff(413,1), 132.80907, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(15)->GetValues().coeff(413,2), 133.55246, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(100,0), -948.33716, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(100,1), 1016.77582, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(100,2), 202.41180, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(12)->GetValues().coeff(240,0), 670.72345, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(12)->GetValues().coeff(240,1), 469.66174, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(12)->GetValues().coeff(240,2), 92.99429, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(15)->GetValues().coeff(383,0), 2300.52002, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(15)->GetValues().coeff(383,1), 141.87332, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(15)->GetValues().coeff(383,2), 136.74149, 1e-5);
    
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals().coeff(0), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetResiduals().coeff(0), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals().coeff(513), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(16)->GetResiduals().coeff(52), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(7)->GetResiduals().coeff(401), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(18)->GetResiduals().coeff(68), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(19)->GetResiduals().coeff(517), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(19)->GetResiduals().coeff(85), -1.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetResiduals().coeff(516), -1.0, 1e-5);
    
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

CXXTEST_SUITE_REGISTRATION(RAHFileReaderTest)
CXXTEST_TEST_REGISTRATION(RAHFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(RAHFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(RAHFileReaderTest, TruncatedFile)
CXXTEST_TEST_REGISTRATION(RAHFileReaderTest, File1123xa01)
#endif
