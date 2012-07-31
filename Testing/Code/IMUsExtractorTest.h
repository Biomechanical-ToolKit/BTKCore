#ifndef IMUsExtractorTest_h
#define IMUsExtractorTest_h

#include <btkIMUsExtractor.h>
#include <btkMetaDataUtils.h>

CXXTEST_SUITE(IMUsExtractorTest)
{
  CXXTEST_TEST(NoAcquisition)
  {
    btk::IMUsExtractor::Pointer imuse = btk::IMUsExtractor::New();
    btk::IMUCollection::Pointer output = imuse->GetOutput();
    output->Update();
    TS_ASSERT_EQUALS(output->GetItemNumber(), 0);
  }
  
  CXXTEST_TEST(NoIMU)
  {
    btk::IMUsExtractor::Pointer imuse = btk::IMUsExtractor::New();
    imuse->SetInput(btk::Acquisition::New());
    btk::IMUCollection::Pointer output = imuse->GetOutput();
    output->Update();
    TS_ASSERT_EQUALS(output->GetItemNumber(), 0);
  }
  
  CXXTEST_TEST(OneAcquisition)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(0,200,6);
    btk::MetaData::Pointer imu = btk::MetaDataCreateChild(acq->GetMetaData(), "IMU");
    btk::MetaDataCreateChild(imu, "USED", (int16_t)1);
    btk::MetaDataCreateChild(imu, "LABELS", std::vector<std::string>(1,"Foo"));
    btk::MetaDataCreateChild(imu, "DESCRIPTIONS", std::vector<std::string>(1,"BAR"));
    btk::MetaDataCreateChild(imu, "TYPE", std::vector<int16_t>(1,1));
    std::vector<int16_t> channels(6); channels[0] = 1; channels[1] = 2; channels[2] = 3; channels[3] = 4; channels[4] = 5; channels[5] = 6;
    btk::MetaDataCreateChild2D(imu, "CHANNEL", channels, 6);
    
    btk::IMUsExtractor::Pointer imuse = btk::IMUsExtractor::New();
    imuse->SetInput(acq);
    btk::IMUCollection::Pointer output = imuse->GetOutput();
    output->Update();
    TS_ASSERT_EQUALS(output->GetItemNumber(), 1);
    TS_ASSERT_EQUALS(output->GetItem(0)->GetLabel(), std::string("Foo"));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetDescription(), std::string("BAR"));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetAccelerometerX(), acq->GetAnalog(0));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetAccelerometerY(), acq->GetAnalog(1));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetAccelerometerZ(), acq->GetAnalog(2));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetGyroscopeX(), acq->GetAnalog(3));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetGyroscopeY(), acq->GetAnalog(4));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetGyroscopeZ(), acq->GetAnalog(5));
  }
  
  CXXTEST_TEST(OneAcquisition_NoDescriptions)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(0,200,6);
    btk::MetaData::Pointer imu = btk::MetaDataCreateChild(acq->GetMetaData(), "IMU");
    btk::MetaDataCreateChild(imu, "USED", (int16_t)1);
    btk::MetaDataCreateChild(imu, "LABELS", std::vector<std::string>(1,"Foo"));
    btk::MetaDataCreateChild(imu, "TYPE", std::vector<int16_t>(1,1));
    std::vector<int16_t> channels(6); channels[0] = 6; channels[1] = 5; channels[2] = 4; channels[3] = 3; channels[4] = 2; channels[5] = 1;
    btk::MetaDataCreateChild2D(imu, "CHANNEL", channels, 6);
    
    btk::IMUsExtractor::Pointer imuse = btk::IMUsExtractor::New();
    imuse->SetInput(acq);
    btk::IMUCollection::Pointer output = imuse->GetOutput();
    output->Update();
    TS_ASSERT_EQUALS(output->GetItemNumber(), 1);
    TS_ASSERT_EQUALS(output->GetItem(0)->GetLabel(), std::string("Foo"));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetDescription(), std::string(""));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetAccelerometerX(), acq->GetAnalog(5));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetAccelerometerY(), acq->GetAnalog(4));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetAccelerometerZ(), acq->GetAnalog(3));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetGyroscopeX(), acq->GetAnalog(2));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetGyroscopeY(), acq->GetAnalog(1));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetGyroscopeZ(), acq->GetAnalog(0));
  }
  
  CXXTEST_TEST(OneAcquisition_NoLabelsAndDescriptions)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(0,200,6);
    btk::MetaData::Pointer imu = btk::MetaDataCreateChild(acq->GetMetaData(), "IMU");
    btk::MetaDataCreateChild(imu, "USED", (int16_t)1);
    btk::MetaDataCreateChild(imu, "TYPE", std::vector<int16_t>(1,1));
    std::vector<int16_t> channels(6); channels[0] = 6; channels[1] = 5; channels[2] = 4; channels[3] = 3; channels[4] = 2; channels[5] = 1;
    btk::MetaDataCreateChild2D(imu, "CHANNEL", channels, 6);
    
    btk::IMUsExtractor::Pointer imuse = btk::IMUsExtractor::New();
    imuse->SetInput(acq);
    btk::IMUCollection::Pointer output = imuse->GetOutput();
    output->Update();
    TS_ASSERT_EQUALS(output->GetItemNumber(), 1);
    TS_ASSERT_EQUALS(output->GetItem(0)->GetLabel(), std::string("IMU #1"));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetDescription(), std::string(""));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetAccelerometerX(), acq->GetAnalog(5));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetAccelerometerY(), acq->GetAnalog(4));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetAccelerometerZ(), acq->GetAnalog(3));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetGyroscopeX(), acq->GetAnalog(2));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetGyroscopeY(), acq->GetAnalog(1));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetGyroscopeZ(), acq->GetAnalog(0));
  }
  
  CXXTEST_TEST(OneAcquisition_MissingChannels)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(0,200,6);
    btk::MetaData::Pointer imu = btk::MetaDataCreateChild(acq->GetMetaData(), "IMU");
    btk::MetaDataCreateChild(imu, "USED", (int16_t)1);
    btk::MetaDataCreateChild(imu, "TYPE", std::vector<int16_t>(1,1));
    std::vector<int16_t> channels(6); channels[0] = 6; channels[1] = 5; channels[2] = -1; channels[3] = -1; channels[4] = 2; channels[5] = 1;
    btk::MetaDataCreateChild2D(imu, "CHANNEL", channels, 6);
    
    btk::IMUsExtractor::Pointer imuse = btk::IMUsExtractor::New();
    imuse->SetInput(acq);
    btk::IMUCollection::Pointer output = imuse->GetOutput();
    output->Update();
    TS_ASSERT_EQUALS(output->GetItemNumber(), 1);
    TS_ASSERT_EQUALS(output->GetItem(0)->GetLabel(), std::string("IMU #1"));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetDescription(), std::string(""));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetAccelerometerX(), acq->GetAnalog(5));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetAccelerometerY(), acq->GetAnalog(4));
    TS_ASSERT(output->GetItem(0)->GetAccelerometerZ().get() == 0);
    TS_ASSERT(output->GetItem(0)->GetGyroscopeX().get() == 0);
    TS_ASSERT_EQUALS(output->GetItem(0)->GetGyroscopeY(), acq->GetAnalog(1));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetGyroscopeZ(), acq->GetAnalog(0));
  }
  
  CXXTEST_TEST(OneAcquisition_TwoSensors)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(0,200,15);
    btk::MetaData::Pointer imu = btk::MetaDataCreateChild(acq->GetMetaData(), "IMU");
    btk::MetaDataCreateChild(imu, "USED", (int16_t)2);
    btk::MetaDataCreateChild(imu, "LABELS", std::vector<std::string>(2,"Foo"));
    btk::MetaDataCreateChild(imu, "TYPE", std::vector<int16_t>(2,1));
    std::vector<int16_t> channels(18); 
    channels[0] = 1; channels[1] = 2; channels[2] = 3; channels[3] = 4; channels[4] = 5; channels[5] = 6; channels[6] = -1; channels[7] = -1; channels[8] = -1;
    channels[9] = 7; channels[10] = 8; channels[11] = 9; channels[12] = 10; channels[13] = 11; channels[14] = 12; channels[15] = 13; channels[16] = 14; channels[17] = 15;
    btk::MetaDataCreateChild2D(imu, "CHANNEL", channels, 9);
    
    btk::IMUsExtractor::Pointer imuse = btk::IMUsExtractor::New();
    imuse->SetInput(acq);
    btk::IMUCollection::Pointer output = imuse->GetOutput();
    output->Update();
    
    TS_ASSERT_EQUALS(output->GetItemNumber(), 2);

    TS_ASSERT_EQUALS(output->GetItem(0)->GetLabel(), std::string("Foo"));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetDescription(), std::string(""));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetAccelerometerX(), acq->GetAnalog(0));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetAccelerometerY(), acq->GetAnalog(1));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetAccelerometerZ(), acq->GetAnalog(2));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetGyroscopeX(), acq->GetAnalog(3));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetGyroscopeY(), acq->GetAnalog(4));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetGyroscopeZ(), acq->GetAnalog(5));

    TS_ASSERT_EQUALS(output->GetItem(1)->GetLabel(), std::string("Foo"));
    TS_ASSERT_EQUALS(output->GetItem(1)->GetDescription(), std::string(""));
    TS_ASSERT_EQUALS(output->GetItem(1)->GetAccelerometerX(), acq->GetAnalog(6));
    TS_ASSERT_EQUALS(output->GetItem(1)->GetAccelerometerY(), acq->GetAnalog(7));
    TS_ASSERT_EQUALS(output->GetItem(1)->GetAccelerometerZ(), acq->GetAnalog(8));
    TS_ASSERT_EQUALS(output->GetItem(1)->GetGyroscopeX(), acq->GetAnalog(9));
    TS_ASSERT_EQUALS(output->GetItem(1)->GetGyroscopeY(), acq->GetAnalog(10));
    TS_ASSERT_EQUALS(output->GetItem(1)->GetGyroscopeZ(), acq->GetAnalog(11));
    TS_ASSERT_EQUALS(output->GetItem(1)->GetChannel(6), acq->GetAnalog(12));
    TS_ASSERT_EQUALS(output->GetItem(1)->GetChannel(7), acq->GetAnalog(13));
    TS_ASSERT_EQUALS(output->GetItem(1)->GetChannel(8), acq->GetAnalog(14));
  }
  
  CXXTEST_TEST(TwoAcquisitions)
  {
    // Acquisition #1
    btk::Acquisition::Pointer acq1 = btk::Acquisition::New();
    acq1->Init(0,200,6);
    btk::MetaData::Pointer imu = btk::MetaDataCreateChild(acq1->GetMetaData(), "IMU");
    btk::MetaDataCreateChild(imu, "USED", (int16_t)1);
    btk::MetaDataCreateChild(imu, "DESCRIPTIONS", std::vector<std::string>(1,"BAR"));
    btk::MetaDataCreateChild(imu, "TYPE", std::vector<int16_t>(1,1));
    std::vector<int16_t> channels(6); channels[0] = 1; channels[1] = 2; channels[2] = 3; channels[3] = 4; channels[4] = 5; channels[5] = 6;
    btk::MetaDataCreateChild2D(imu, "CHANNEL", channels, 6);
    
    // Acquisition #2 (two sensors)
    btk::Acquisition::Pointer acq2 = btk::Acquisition::New();
    acq2->Init(0,200,15);
    imu = btk::MetaDataCreateChild(acq2->GetMetaData(), "IMU");
    btk::MetaDataCreateChild(imu, "USED", (int16_t)2);
    btk::MetaDataCreateChild(imu, "LABELS", std::vector<std::string>(2,"Foo"));
    btk::MetaDataCreateChild(imu, "TYPE", std::vector<int16_t>(2,1));
    channels.resize(18); 
    channels[0] = 1; channels[1] = 2; channels[2] = 3; channels[3] = 4; channels[4] = 5; channels[5] = 6; channels[6] = -1; channels[7] = -1; channels[8] = -1;
    channels[9] = 7; channels[10] = 8; channels[11] = 9; channels[12] = 10; channels[13] = 11; channels[14] = 12; channels[15] = 13; channels[16] = 14; channels[17] = 15;
    btk::MetaDataCreateChild2D(imu, "CHANNEL", channels, 9);
    
    btk::IMUsExtractor::Pointer imuse = btk::IMUsExtractor::New();
    imuse->SetInput(0, acq1);
    imuse->SetInput(1, acq2);
    btk::IMUCollection::Pointer output = imuse->GetOutput();
    output->Update();
    
    TS_ASSERT_EQUALS(output->GetItemNumber(), 3);
    
    TS_ASSERT_EQUALS(output->GetItem(0)->GetLabel(), std::string("IMU #1"));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetDescription(), std::string("BAR"));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetAccelerometerX(), acq1->GetAnalog(0));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetAccelerometerY(), acq1->GetAnalog(1));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetAccelerometerZ(), acq1->GetAnalog(2));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetGyroscopeX(), acq1->GetAnalog(3));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetGyroscopeY(), acq1->GetAnalog(4));
    TS_ASSERT_EQUALS(output->GetItem(0)->GetGyroscopeZ(), acq1->GetAnalog(5));

    TS_ASSERT_EQUALS(output->GetItem(1)->GetLabel(), std::string("Foo"));
    TS_ASSERT_EQUALS(output->GetItem(1)->GetDescription(), std::string(""));
    TS_ASSERT_EQUALS(output->GetItem(1)->GetAccelerometerX(), acq2->GetAnalog(0));
    TS_ASSERT_EQUALS(output->GetItem(1)->GetAccelerometerY(), acq2->GetAnalog(1));
    TS_ASSERT_EQUALS(output->GetItem(1)->GetAccelerometerZ(), acq2->GetAnalog(2));
    TS_ASSERT_EQUALS(output->GetItem(1)->GetGyroscopeX(), acq2->GetAnalog(3));
    TS_ASSERT_EQUALS(output->GetItem(1)->GetGyroscopeY(), acq2->GetAnalog(4));
    TS_ASSERT_EQUALS(output->GetItem(1)->GetGyroscopeZ(), acq2->GetAnalog(5));

    TS_ASSERT_EQUALS(output->GetItem(2)->GetLabel(), std::string("Foo"));
    TS_ASSERT_EQUALS(output->GetItem(2)->GetDescription(), std::string(""));
    TS_ASSERT_EQUALS(output->GetItem(2)->GetAccelerometerX(), acq2->GetAnalog(6));
    TS_ASSERT_EQUALS(output->GetItem(2)->GetAccelerometerY(), acq2->GetAnalog(7));
    TS_ASSERT_EQUALS(output->GetItem(2)->GetAccelerometerZ(), acq2->GetAnalog(8));
    TS_ASSERT_EQUALS(output->GetItem(2)->GetGyroscopeX(), acq2->GetAnalog(9));
    TS_ASSERT_EQUALS(output->GetItem(2)->GetGyroscopeY(), acq2->GetAnalog(10));
    TS_ASSERT_EQUALS(output->GetItem(2)->GetGyroscopeZ(), acq2->GetAnalog(11));
    TS_ASSERT_EQUALS(output->GetItem(2)->GetChannel(6), acq2->GetAnalog(12));
    TS_ASSERT_EQUALS(output->GetItem(2)->GetChannel(7), acq2->GetAnalog(13));
    TS_ASSERT_EQUALS(output->GetItem(2)->GetChannel(8), acq2->GetAnalog(14));
  }
  
  CXXTEST_TEST(TwoAcquisitions_NoIMU)
  {
    btk::IMUsExtractor::Pointer imuse = btk::IMUsExtractor::New();
    imuse->SetInput(0,btk::Acquisition::New());
    imuse->SetInput(1,btk::Acquisition::New());
    btk::IMUCollection::Pointer output = imuse->GetOutput();
    output->Update();
    TS_ASSERT_EQUALS(output->GetItemNumber(), 0);
  }
};
  
CXXTEST_SUITE_REGISTRATION(IMUsExtractorTest)
CXXTEST_TEST_REGISTRATION(IMUsExtractorTest, NoAcquisition)
CXXTEST_TEST_REGISTRATION(IMUsExtractorTest, NoIMU)
CXXTEST_TEST_REGISTRATION(IMUsExtractorTest, OneAcquisition)
CXXTEST_TEST_REGISTRATION(IMUsExtractorTest, OneAcquisition_NoDescriptions)
CXXTEST_TEST_REGISTRATION(IMUsExtractorTest, OneAcquisition_NoLabelsAndDescriptions)
CXXTEST_TEST_REGISTRATION(IMUsExtractorTest, OneAcquisition_MissingChannels)
CXXTEST_TEST_REGISTRATION(IMUsExtractorTest, OneAcquisition_TwoSensors)
CXXTEST_TEST_REGISTRATION(IMUsExtractorTest, TwoAcquisitions)
CXXTEST_TEST_REGISTRATION(IMUsExtractorTest, TwoAcquisitions_NoIMU)

#endif