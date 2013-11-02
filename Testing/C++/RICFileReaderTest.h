#ifndef RICFileReaderTest_h
#define RICFileReaderTest_h

#include <btkAcquisitionFileReader.h>

CXXTEST_SUITE(RICFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("test.ric");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: test.ric"));
  };
  
  CXXTEST_TEST(File1123xa01)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(EliteFilePathIN + "1123xa01/1123xa01.RIC");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 100.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 14);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 540);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 100.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("POINT")->GetChild("MARKERS_FILTERED")->GetInfo()->ToInt(0), 1);
    
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 6);
    TS_ASSERT_EQUALS(acq->GetEvent(0)->GetLabel(), "Foot Off");
    TS_ASSERT_EQUALS(acq->GetEvent(0)->GetContext(), "Left");
    TS_ASSERT_DELTA(acq->GetEvent(0)->GetTime(), 2.57, 1e-5);
    TS_ASSERT_EQUALS(acq->GetEvent(0)->GetFrame(), 257);
    TS_ASSERT_EQUALS(acq->GetEvent(1)->GetLabel(), "Foot Off");
    TS_ASSERT_EQUALS(acq->GetEvent(1)->GetContext(), "Right");
    TS_ASSERT_DELTA(acq->GetEvent(1)->GetTime(), 2.19, 1e-5);
    TS_ASSERT_EQUALS(acq->GetEvent(1)->GetFrame(), 219);
    TS_ASSERT_EQUALS(acq->GetEvent(2)->GetLabel(), "Foot Strike");
    TS_ASSERT_EQUALS(acq->GetEvent(2)->GetContext(), "Left");
    TS_ASSERT_DELTA(acq->GetEvent(2)->GetTime(), 2.13, 1e-5);
    TS_ASSERT_EQUALS(acq->GetEvent(2)->GetFrame(), 213);
    TS_ASSERT_EQUALS(acq->GetEvent(3)->GetLabel(), "Foot Strike");
    TS_ASSERT_EQUALS(acq->GetEvent(3)->GetContext(), "Left");
    TS_ASSERT_DELTA(acq->GetEvent(3)->GetTime(), 2.96, 1e-5);
    TS_ASSERT_EQUALS(acq->GetEvent(3)->GetFrame(), 296);
    TS_ASSERT_EQUALS(acq->GetEvent(4)->GetLabel(), "Foot Strike");
    TS_ASSERT_EQUALS(acq->GetEvent(4)->GetContext(), "Right");
    TS_ASSERT_DELTA(acq->GetEvent(4)->GetTime(), 1.73, 1e-5);
    TS_ASSERT_EQUALS(acq->GetEvent(4)->GetFrame(), 173);
    TS_ASSERT_EQUALS(acq->GetEvent(5)->GetLabel(), "Foot Strike");
    TS_ASSERT_EQUALS(acq->GetEvent(5)->GetContext(), "Right");
    TS_ASSERT_DELTA(acq->GetEvent(5)->GetTime(), 2.55, 1e-5);
    TS_ASSERT_EQUALS(acq->GetEvent(5)->GetFrame(), 255);
    
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(281,0), 1044.55872, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(281,1), 684.46423, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues().coeff(281,2), 321.40161, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(8)->GetValues().coeff(264,0), 1051.29175, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(8)->GetValues().coeff(264,1), 29.31635, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(8)->GetValues().coeff(264,2), 361.58163, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(366,0), 2014.28467, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(366,1), 621.17505, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues().coeff(366,2), 392.28842, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(13)->GetValues().coeff(198,0), 280.98044, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(13)->GetValues().coeff(198,1), 51.40823, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(13)->GetValues().coeff(198,2), 257.18488, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(181,0), -1.48131, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(181,1), 664.50085, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues().coeff(181,2), 226.28722, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(10)->GetValues().coeff(266,0), 968.69519, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(10)->GetValues().coeff(266,1), 603.55554, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(10)->GetValues().coeff(266,2), 283.48630, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(12)->GetValues().coeff(386,0), 2328.99854, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(12)->GetValues().coeff(386,1), 47.40369, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(377,0), 2063.97363, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(377,1), 1020.57556, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(377,2), 338.18256, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(6)->GetValues().coeff(270,0), 1042.16150, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(6)->GetValues().coeff(270,1), 338.96271, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(6)->GetValues().coeff(270,2), 355.34717, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(12)->GetValues().coeff(333,0), 1492.63354, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(12)->GetValues().coeff(333,1), 92.17363, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(12)->GetValues().coeff(333,2), 312.85123, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(478,0), 2921.15601, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(478,1), 990.99744, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(478,2), 382.96732, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(264,0), 882.30737, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(264,1), 1009.00555, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues().coeff(264,2), 318.38019, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(6)->GetValues().coeff(270,0), 1042.16150, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(6)->GetValues().coeff(270,1), 338.96271, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(6)->GetValues().coeff(270,2), 355.34717, 1e-5);
    
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "r should");
    TS_ASSERT_EQUALS(acq->GetPoint(1)->GetLabel(), "l should");
    TS_ASSERT_EQUALS(acq->GetPoint(2)->GetLabel(), "c7");
    TS_ASSERT_EQUALS(acq->GetPoint(3)->GetLabel(), "sacrum");
    TS_ASSERT_EQUALS(acq->GetPoint(4)->GetLabel(), "r asis");
    TS_ASSERT_EQUALS(acq->GetPoint(5)->GetLabel(), "r hip");
    TS_ASSERT_EQUALS(acq->GetPoint(6)->GetLabel(), "r knee");
    TS_ASSERT_EQUALS(acq->GetPoint(7)->GetLabel(), "r ankle");
    TS_ASSERT_EQUALS(acq->GetPoint(8)->GetLabel(), "r met");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetLabel(), "l asis");
    TS_ASSERT_EQUALS(acq->GetPoint(10)->GetLabel(), "l hip");
    TS_ASSERT_EQUALS(acq->GetPoint(11)->GetLabel(), "l knee");
    TS_ASSERT_EQUALS(acq->GetPoint(12)->GetLabel(), "l ankle");
    TS_ASSERT_EQUALS(acq->GetPoint(13)->GetLabel(), "l met");
  };
  
  CXXTEST_TEST(FileBlO05_NoLabel)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(EliteFilePathIN + "BlO05.RIF");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 100.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 18);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 2000);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 100.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
  };
};

CXXTEST_SUITE_REGISTRATION(RICFileReaderTest)
CXXTEST_TEST_REGISTRATION(RICFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(RICFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(RICFileReaderTest, File1123xa01)
CXXTEST_TEST_REGISTRATION(RICFileReaderTest, FileBlO05_NoLabel)
#endif
