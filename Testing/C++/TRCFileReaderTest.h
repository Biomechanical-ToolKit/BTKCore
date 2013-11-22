#ifndef TRCFileReaderTest_h
#define TRCFileReaderTest_h

#include <btkAcquisitionFileReader.h>
#include <btkTRCFileIO.h>
#include <btkConvert.h>

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
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::TRCFileIOException &e, e.what(), std::string("Unexpected end of file."));
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
  
  CXXTEST_TEST(Unamed1)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(TRCFilePathIN + "10002-20130626160334-Unnamed.trc");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 400.0);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 400.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 10);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 882);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    
    for (int i = 0 ; i < 10 ; ++i)
    {  
      TS_ASSERT_EQUALS(acq->GetPoint(i)->GetLabel(), "Unlabel#"+btk::ToString(i+1));
    }
    
    for (int i = 0 ; i < 10 ; ++i)
    {
      for (int j = 0 ; j < 11 ; ++j)
      {
        TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(j,0), 0.0, 1e-15);
        TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(j,1), 0.0, 1e-15);
        TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(j,2), 0.0, 1e-15);
        TS_ASSERT_DELTA(acq->GetPoint(i)->GetResiduals()(j), -1.0, 1e-15);
      }
    }
    
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(11,0), 19.91857, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(11,1), 1362.18750, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(11,2), -124.33382, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(11), 0.0, 1e-15);
    for (int i = 1 ; i < 10 ; ++i)
    {
      for (int j = 11 ; j < 26 ; ++j)
      {
        TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(j,0), 0.0, 1e-15);
        TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(j,1), 0.0, 1e-15);
        TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(j,2), 0.0, 1e-15);
        TS_ASSERT_DELTA(acq->GetPoint(i)->GetResiduals()(j), -1.0, 1e-15);
      }
    }
    
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(356,0), -37.94589, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(356,1), 909.69788, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(356,2), -160.10370, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(356), 0.0, 1e-15);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues()(356,0), -161.65665, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues()(356,1), 1223.79883, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues()(356,2), -198.66595, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetResiduals()(356), 0.0, 1e-15);
    for (int i = 2 ; i < 10 ; ++i)
    {
      for (int j = 356 ; j < 363 ; ++j)
      {
        TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(j,0), 0.0, 1e-15);
        TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(j,1), 0.0, 1e-15);
        TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(j,2), 0.0, 1e-15);
        TS_ASSERT_DELTA(acq->GetPoint(i)->GetResiduals()(j), -1.0, 1e-15);
      }
    }
    
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(540,0), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(540,1), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(540,2), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(540), -1.0, 1e-15);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues()(540,0), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues()(540,1), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues()(540,2), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetResiduals()(540), -1.0, 1e-15);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues()(540,0), 832.84674, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues()(540,1), 1561.89258, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues()(540,2), -699.47595, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetResiduals()(540), 0.0, 1e-15);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(540,0), -1985.56189, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(540,1), 362.87326, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(540,2), -248.83156, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetResiduals()(540), 0.0, 1e-15);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues()(540,0), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues()(540,1), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetValues()(540,2), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(4)->GetResiduals()(540), -1.0, 1e-15);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues()(540,0), -26.17541, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues()(540,1), 526.26477, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetValues()(540,2), -348.95346, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(9)->GetResiduals()(540), 0.0, 1e-15);
    
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(881,0), -799.76202, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(881,1), 801.21472, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(881,2), -391.54608, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(881), 0.0, 1e-15);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues()(881,0), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues()(881,1), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues()(881,2), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetResiduals()(881), -1.0, 1e-15);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues()(881,0), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues()(881,1), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues()(881,2), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(2)->GetResiduals()(881), -1.0, 1e-15);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(881,0), -1837.97595, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(881,1), -1082.82910, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(881,2), -1182.05762, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetResiduals()(881), 0.0, 1e-15);
    for (int i = 4 ; i < 10 ; ++i)
    {
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(881,0), 0.0, 1e-15);
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(881,1), 0.0, 1e-15);
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(881,2), 0.0, 1e-15);
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetResiduals()(881), -1.0, 1e-15);
    }
  };
  
  CXXTEST_TEST(Unamed2)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(TRCFilePathIN + "10002-20130731010438-Unnamed.trc");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 400.0);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 400.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 37);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 1313);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(0,0), -352.45120, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(0,1), 985.40039, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(0,2), 288.96054, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0), 0.0, 1e-15);
    TS_ASSERT_DELTA(acq->GetPoint(12)->GetValues()(0,0), 799.99994, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(12)->GetValues()(0,1), 1593.03601, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(12)->GetValues()(0,2), 427.78055, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(12)->GetResiduals()(0), 0.0, 1e-15);
    
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(893,0), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(893,1), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(893,2), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(893), -1.0, 1e-15);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues()(893,0), -133.13820, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues()(893,1), 1641.26953, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues()(893,2), 367.13284, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetResiduals()(893), 0.0, 1e-15);
    for (int i = 2 ; i < 5 ; ++i)
    {
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(893,0), 0.0, 1e-15);
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(893,1), 0.0, 1e-15);
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(893,2), 0.0, 1e-15);
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetResiduals()(893), -1.0, 1e-15);
    }
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues()(893,0), -599.99548, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues()(893,1), 189.93408, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetValues()(893,2), 565.01599, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(5)->GetResiduals()(893), 0.0, 1e-15);
    for (int i = 6 ; i < 9 ; ++i)
    {
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(893,0), 0.0, 1e-15);
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(893,1), 0.0, 1e-15);
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(893,2), 0.0, 1e-15);
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetResiduals()(893), -1.0, 1e-15);
    }
    TS_ASSERT_DELTA(acq->GetPoint(19)->GetValues()(893,0), -318.83157, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(19)->GetValues()(893,1), 55.89437, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(19)->GetValues()(893,2), 260.76419, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(19)->GetResiduals()(893), 0.0, 1e-15);
    for (int i = 20 ; i < 26 ; ++i)
    {
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(893,0), 0.0, 1e-15);
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(893,1), 0.0, 1e-15);
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(893,2), 0.0, 1e-15);
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetResiduals()(893), -1.0, 1e-15);
    }
    TS_ASSERT_DELTA(acq->GetPoint(27)->GetValues()(893,0), -376.94412, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(27)->GetValues()(893,1), 1453.94983, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(27)->GetValues()(893,2), 208.30200, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(27)->GetResiduals()(893), 0.0, 1e-15);
    TS_ASSERT_DELTA(acq->GetPoint(28)->GetValues()(893,0), -419.75079, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(28)->GetValues()(893,1), 1230.84387, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(28)->GetValues()(893,2), 1282.72046, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(28)->GetResiduals()(893), 0.0, 1e-15);
    for (int i = 29 ; i < 32 ; ++i)
    {
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(893,0), 0.0, 1e-15);
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(893,1), 0.0, 1e-15);
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(893,2), 0.0, 1e-15);
      TS_ASSERT_DELTA(acq->GetPoint(i)->GetResiduals()(893), -1.0, 1e-15);
    }
    
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(1312,0), 386.76334, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(1312,1), 1372.59546, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(1312,2), 745.81812, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(1312), 0.0, 1e-15);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues()(1312,0), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues()(1312,1), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues()(1312,2), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetResiduals()(1312), -1.0, 1e-15);
    TS_ASSERT_DELTA(acq->GetPoint(31)->GetValues()(1312,0), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(31)->GetValues()(1312,1), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(31)->GetValues()(1312,2), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(31)->GetResiduals()(1312), -1.0, 1e-15);
    TS_ASSERT_DELTA(acq->GetPoint(32)->GetValues()(1312,0), 950.22644, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(32)->GetValues()(1312,1), 1470.34619, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(32)->GetValues()(1312,2), 951.17596, 1e-5);
    TS_ASSERT_DELTA(acq->GetPoint(32)->GetResiduals()(1312), 0.0, 1e-15);
  };
};

CXXTEST_SUITE_REGISTRATION(TRCFileReaderTest)
CXXTEST_TEST_REGISTRATION(TRCFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(TRCFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(TRCFileReaderTest, TruncatedFile)
CXXTEST_TEST_REGISTRATION(TRCFileReaderTest, MOTEK_T)
CXXTEST_TEST_REGISTRATION(TRCFileReaderTest, MOTEK_ballet)
CXXTEST_TEST_REGISTRATION(TRCFileReaderTest, KneeWithOcclusion)
CXXTEST_TEST_REGISTRATION(TRCFileReaderTest, Unamed1)
CXXTEST_TEST_REGISTRATION(TRCFileReaderTest, Unamed2)
#endif
