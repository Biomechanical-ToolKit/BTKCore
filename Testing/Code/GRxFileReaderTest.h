#ifndef GRxFileReaderTest_h
#define GRxFileReaderTest_h

#include <btkAcquisitionFileReader.h>

CXXTEST_SUITE(GRxFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("test.gr1");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: test.gr1"));
  };
  
  CXXTEST_TEST(File1123xa01_1)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(EliteFilePathIN + "1123xa01/1123xa01.GR1");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 175);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 100.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 46);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 46);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 100.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 6);
    
    btk::MetaData::Pointer md = acq->GetMetaData();
    btk::MetaData::Pointer fp = md->GetChild("FORCE_PLATFORM");
    int used = fp->GetChild("USED")->GetInfo()->ToInt(0);
    TS_ASSERT_EQUALS(used, 1);
    btk::MetaDataInfo::Pointer corners = fp->GetChild("CORNERS")->GetInfo();
    std::vector<double> cornersVal = corners->ToDouble();
    TS_ASSERT_DELTA(cornersVal[0], 90.4063, 1e-4);
    TS_ASSERT_DELTA(cornersVal[1], -1.80849, 1e-5);
    TS_ASSERT_DELTA(cornersVal[2], 290.0700, 1e-5);
    TS_ASSERT_DELTA(cornersVal[3], 690.38678, 1e-5);
    TS_ASSERT_DELTA(cornersVal[4], -3.06236, 1e-5);
    TS_ASSERT_DELTA(cornersVal[5], 285.39300, 1e-5);
    TS_ASSERT_DELTA(cornersVal[6], 693.50647, 1e-5);
    TS_ASSERT_DELTA(cornersVal[7], -2.23016, 1e-5);
    TS_ASSERT_DELTA(cornersVal[8], 685.37988, 1e-5);
    TS_ASSERT_DELTA(cornersVal[9], 93.52607, 1e-5);
    TS_ASSERT_DELTA(cornersVal[10], -0.97630, 1e-5);
    TS_ASSERT_DELTA(cornersVal[11], 690.05688, 1e-5);
    
    TS_ASSERT_EQUALS(corners->GetDimensions()[2], used);
    TS_ASSERT_EQUALS(corners->GetValues().size(), 12);
    btk::MetaDataInfo::Pointer channel = fp->GetChild("CHANNEL")->GetInfo();
    TS_ASSERT_EQUALS(channel->GetDimensions()[1], used);
    TS_ASSERT_EQUALS(channel->GetValues().size(), 6);
    for (int i = 0 ; i < 6 ; ++i)
      TS_ASSERT_EQUALS(channel->ToInt(i), i+1);
    btk::MetaDataInfo::Pointer type = fp->GetChild("TYPE")->GetInfo();
    TS_ASSERT_EQUALS(type->GetDimensions()[0], used);
    TS_ASSERT_EQUALS(type->GetValues().size(), 1);
    for (int i = 0 ; i < used ; ++i)
      TS_ASSERT_EQUALS(type->ToInt(i), 1);
    btk::MetaDataInfo::Pointer origin = fp->GetChild("ORIGIN")->GetInfo();
    TS_ASSERT_EQUALS(origin->GetDimensions()[1], used);
    TS_ASSERT_EQUALS(origin->GetValues().size(), 3);
    TS_ASSERT_DELTA(origin->ToFloat(0), 300.0, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(1), 200.0, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(2), -11.0, 1e-5);
    
    TS_ASSERT_DELTA(acq->GetAnalog(1)->GetValues().coeff(0), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues().coeff(0), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues().coeff(0), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(4)->GetValues().coeff(0), 223.975 - 200.0, 1e-3);
    TS_ASSERT_DELTA(acq->GetAnalog(3)->GetValues().coeff(0),-86.499 + 300.0, 1e-3);
    TS_ASSERT_DELTA(acq->GetAnalog(5)->GetValues().coeff(0), 0.0, 1e-5);
    
    TS_ASSERT_DELTA(acq->GetAnalog(1)->GetValues().coeff(25),   39.345703125000, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues().coeff(25),    6.670898437500, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues().coeff(25), -218.522949218750, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(4)->GetValues().coeff(25),  293.897888183594 - 200.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(3)->GetValues().coeff(25),   -63.315734863281 + 300.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(5)->GetValues().coeff(25), 4411.003417968750, 1e-5);
    
    TS_ASSERT_DELTA(acq->GetAnalog(1)->GetValues().coeff(45),   0.0            , 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues().coeff(45),   0.0            , 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues().coeff(45),   0.0            , 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(4)->GetValues().coeff(45), 213.8834991455078 - 200.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(3)->GetValues().coeff(45),-146.2087402343750 + 300.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(5)->GetValues().coeff(45),   0.0            , 1e-5);
  };
  
  CXXTEST_TEST(File1123xa01_2)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(EliteFilePathIN + "1123xa01/1123xa01.GR2");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 211);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 100.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 48);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 48);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 100.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 6);
    
    btk::MetaData::Pointer platform = acq->GetMetaData()->GetChild("FORCE_PLATFORM");
    std::vector<double> corners = platform->GetChild("CORNERS")->GetInfo()->ToDouble();
    TS_ASSERT_DELTA(corners[0],  402.1354064941407 , 1e-5);
    TS_ASSERT_DELTA(corners[1],  -4.5329937934875  , 1e-5);
    TS_ASSERT_DELTA(corners[2],  -116.2665023803711, 1e-5);
    TS_ASSERT_DELTA(corners[3],  1002.086975097656 , 1e-5);
    TS_ASSERT_DELTA(corners[4],  -2.959216117859   , 1e-5);
    TS_ASSERT_DELTA(corners[5],  -123.729202270508 , 1e-5);
    TS_ASSERT_DELTA(corners[6],  1007.060974121094 , 1e-5);
    TS_ASSERT_DELTA(corners[7],  -2.627995967865   , 1e-5);
    TS_ASSERT_DELTA(corners[8],  276.239685058594  , 1e-5);
    TS_ASSERT_DELTA(corners[9],  407.1098022460937 , 1e-5);
    TS_ASSERT_DELTA(corners[10], -4.2017750740051  , 1e-5);
    TS_ASSERT_DELTA(corners[11], 283.7025146484375 , 1e-5);
    
    TS_ASSERT_DELTA(acq->GetAnalog(1)->GetValues().coeff(0), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues().coeff(0), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues().coeff(0), 0.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(4)->GetValues().coeff(0), 99.7479553222656 - 200.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(3)->GetValues().coeff(0),-159.5763854980469 + 300.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(5)->GetValues().coeff(0), 0.0, 1e-5);
    
    TS_ASSERT_DELTA(acq->GetAnalog(1)->GetValues().coeff(39), -10.0195312500000, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues().coeff(39), -34.1967773437500, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues().coeff(39),-227.9765625000000, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(4)->GetValues().coeff(39),  35.1036682128906 - 200.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(3)->GetValues().coeff(39),-221.0007629394531 + 300.0, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(5)->GetValues().coeff(39), 488.6576538085937, 1e-5);
  };
  
  CXXTEST_TEST(File1216XB01)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(EliteFilePathIN + "1216XB01/1216XB01.GR1");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 563);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 200.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 197);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 197);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 6);
  };
  
  CXXTEST_TEST(File1216XE01)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(EliteFilePathIN + "1216XE01/1216XE01.GR1");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 4645);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 2000.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 1609);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 1609);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 2000.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 6);
  };
};

CXXTEST_SUITE_REGISTRATION(GRxFileReaderTest)
CXXTEST_TEST_REGISTRATION(GRxFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(GRxFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(GRxFileReaderTest, File1123xa01_1)
CXXTEST_TEST_REGISTRATION(GRxFileReaderTest, File1123xa01_2)
CXXTEST_TEST_REGISTRATION(GRxFileReaderTest, File1216XB01)
CXXTEST_TEST_REGISTRATION(GRxFileReaderTest, File1216XE01)
#endif
