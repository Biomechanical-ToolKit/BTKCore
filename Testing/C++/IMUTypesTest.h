#ifndef IMUTypesTest_h
#define IMUTypesTest_h

#include <btkIMUTypes.h>

CXXTEST_SUITE(IMUTypesTest)
{
  CXXTEST_TEST(IMUType1)
  {
    btk::IMU::Pointer imu = btk::IMUType1::New();
    TS_ASSERT_EQUALS(imu->GetLabel(), "IMU");
    TS_ASSERT_EQUALS(imu->GetDescription(), "");
    TS_ASSERT_EQUALS(imu->GetType(), 1);
    TS_ASSERT_EQUALS(imu->GetFrequency(), 0.0);
    TS_ASSERT_EQUALS(imu->GetFrameNumber(), 0);
    TS_ASSERT_EQUALS(imu->GetChannelNumber(), 6);
    TS_ASSERT(imu->GetCalMatrix().data() == 0);
    TS_ASSERT_EQUALS(imu->GetAccelerometerX(), imu->GetChannel(0));
    TS_ASSERT_EQUALS(imu->GetAccelerometerY(), imu->GetChannel(1));
    TS_ASSERT_EQUALS(imu->GetAccelerometerZ(), imu->GetChannel(2));
    TS_ASSERT_EQUALS(imu->GetGyroscopeX(), imu->GetChannel(3));
    TS_ASSERT_EQUALS(imu->GetGyroscopeY(), imu->GetChannel(4));
    TS_ASSERT_EQUALS(imu->GetGyroscopeZ(), imu->GetChannel(5));
  };
  
  CXXTEST_TEST(IMUType1_noInit)
  {
    btk::IMU::Pointer imu = btk::IMUType1::New("LAD", "aayTER", false);
    TS_ASSERT_EQUALS(imu->GetLabel(), "LAD");
    TS_ASSERT_EQUALS(imu->GetDescription(), "aayTER");
    TS_ASSERT_EQUALS(imu->GetType(), 1);
    TS_ASSERT_EQUALS(imu->GetFrequency(), 0.0);
    TS_ASSERT_EQUALS(imu->GetFrameNumber(), 0);
    TS_ASSERT_EQUALS(imu->GetChannelNumber(), 0);
    TS_ASSERT(imu->GetCalMatrix().data() == 0);
  };
  
  CXXTEST_TEST(IMUType1_Clone)
  {
    btk::IMU::Pointer imu_ = btk::IMUType1::New("MyIMU", "blabla");
    imu_->SetFrameNumber(10);
    imu_->SetFrequency(1.0);
    imu_->GetCalMatrix().setIdentity(6,6);
    btk::IMU::Pointer imu = imu_->Clone();
    TS_ASSERT_EQUALS(imu->GetLabel(), "MyIMU");
    TS_ASSERT_EQUALS(imu->GetDescription(), "blabla");
    TS_ASSERT_EQUALS(imu->GetType(), 1);
    TS_ASSERT_EQUALS(imu->GetFrequency(), 1.0);
    TS_ASSERT_EQUALS(imu->GetFrameNumber(), 10);
    TS_ASSERT_EQUALS(imu->GetChannelNumber(), 6);
    TS_ASSERT(imu->GetCalMatrix().isIdentity());
    TS_ASSERT_EQUALS(imu->GetCalMatrix().rows(), 6);
    TS_ASSERT_EQUALS(imu->GetCalMatrix().cols(), 6);
    TS_ASSERT_EQUALS(imu->GetAccelerometerX(), imu->GetChannel(0));
    TS_ASSERT_EQUALS(imu->GetAccelerometerY(), imu->GetChannel(1));
    TS_ASSERT_EQUALS(imu->GetAccelerometerZ(), imu->GetChannel(2));
    TS_ASSERT_EQUALS(imu->GetGyroscopeX(), imu->GetChannel(3));
    TS_ASSERT_EQUALS(imu->GetGyroscopeY(), imu->GetChannel(4));
    TS_ASSERT_EQUALS(imu->GetGyroscopeZ(), imu->GetChannel(5));
  };

  CXXTEST_TEST(IMUType2)
  {
    btk::IMU::Pointer imu = btk::IMUType2::New();
    TS_ASSERT_EQUALS(imu->GetLabel(), "IMU");
    TS_ASSERT_EQUALS(imu->GetDescription(), "");
    TS_ASSERT_EQUALS(imu->GetType(), 2);
    TS_ASSERT_EQUALS(imu->GetFrequency(), 0.0);
    TS_ASSERT_EQUALS(imu->GetFrameNumber(), 0);
    TS_ASSERT_EQUALS(imu->GetChannelNumber(), 6);
    TS_ASSERT(imu->GetCalMatrix().data() == 0);
    TS_ASSERT_EQUALS(imu->GetAccelerometerX(), imu->GetChannel(0));
    TS_ASSERT_EQUALS(imu->GetAccelerometerY(), imu->GetChannel(1));
    TS_ASSERT_EQUALS(imu->GetAccelerometerZ(), imu->GetChannel(2));
    TS_ASSERT_EQUALS(imu->GetGyroscopeX(), imu->GetChannel(3));
    TS_ASSERT_EQUALS(imu->GetGyroscopeY(), imu->GetChannel(4));
    TS_ASSERT_EQUALS(imu->GetGyroscopeZ(), imu->GetChannel(5));
  };
  
  CXXTEST_TEST(IMUType2_noInit)
  {
    btk::IMU::Pointer imu = btk::IMUType2::New("LAD", "aayTER", false);
    TS_ASSERT_EQUALS(imu->GetLabel(), "LAD");
    TS_ASSERT_EQUALS(imu->GetDescription(), "aayTER");
    TS_ASSERT_EQUALS(imu->GetType(), 2);
    TS_ASSERT_EQUALS(imu->GetFrequency(), 0.0);
    TS_ASSERT_EQUALS(imu->GetFrameNumber(), 0);
    TS_ASSERT_EQUALS(imu->GetChannelNumber(), 0);
    TS_ASSERT(imu->GetCalMatrix().data() == 0);
  };
    
  CXXTEST_TEST(IMUType2_Clone)
  {
    btk::IMU::Pointer imu_ = btk::IMUType2::New("MyIMU", "blabla");
    imu_->SetFrameNumber(10);
    imu_->SetFrequency(1.0);
    imu_->GetCalMatrix().setIdentity(6,6);
    btk::IMU::Pointer imu = imu_->Clone();
    TS_ASSERT_EQUALS(imu->GetLabel(), "MyIMU");
    TS_ASSERT_EQUALS(imu->GetDescription(), "blabla");
    TS_ASSERT_EQUALS(imu->GetType(), 2);
    TS_ASSERT_EQUALS(imu->GetFrequency(), 1.0);
    TS_ASSERT_EQUALS(imu->GetFrameNumber(), 10);
    TS_ASSERT_EQUALS(imu->GetChannelNumber(), 6);
    TS_ASSERT(imu->GetCalMatrix().isIdentity());
    TS_ASSERT_EQUALS(imu->GetCalMatrix().rows(), 6);
    TS_ASSERT_EQUALS(imu->GetCalMatrix().cols(), 6);
    TS_ASSERT_EQUALS(imu->GetAccelerometerX(), imu->GetChannel(0));
    TS_ASSERT_EQUALS(imu->GetAccelerometerY(), imu->GetChannel(1));
    TS_ASSERT_EQUALS(imu->GetAccelerometerZ(), imu->GetChannel(2));
    TS_ASSERT_EQUALS(imu->GetGyroscopeX(), imu->GetChannel(3));
    TS_ASSERT_EQUALS(imu->GetGyroscopeY(), imu->GetChannel(4));
    TS_ASSERT_EQUALS(imu->GetGyroscopeZ(), imu->GetChannel(5));
  };
};

CXXTEST_SUITE_REGISTRATION(IMUTypesTest)
CXXTEST_TEST_REGISTRATION(IMUTypesTest, IMUType1)
CXXTEST_TEST_REGISTRATION(IMUTypesTest, IMUType1_noInit)
CXXTEST_TEST_REGISTRATION(IMUTypesTest, IMUType1_Clone)
CXXTEST_TEST_REGISTRATION(IMUTypesTest, IMUType2)
CXXTEST_TEST_REGISTRATION(IMUTypesTest, IMUType2_noInit)
CXXTEST_TEST_REGISTRATION(IMUTypesTest, IMUType2_Clone)

#endif // IMUTypesTest_h
