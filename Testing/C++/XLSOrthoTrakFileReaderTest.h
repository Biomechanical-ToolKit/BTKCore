#ifndef XLSOrthoTrakFileReaderTest_h
#define XLSOrthoTrakFileReaderTest_h

#include <btkAcquisitionFileReader.h>

CXXTEST_SUITE(XLSOrthoTrakFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("test.xls");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: test.xls"));
  };
  
  CXXTEST_TEST(Gait)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(XLSOrthoTrakFilePathIN + "Gait.xls");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChildNumber(), 15);
    TS_ASSERT_DELTA(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("Avg_Step_Width")->GetInfo()->ToDouble(0), 17.660 * 10.0, 1e-4);
    TS_ASSERT_DELTA(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("R_Velocity")->GetInfo()->ToDouble(0), 145.237 * 10.0, 1e-4);
    TS_ASSERT_DELTA(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("R_Stride_Len")->GetInfo()->ToDouble(0), 141.311 * 10.0, 2e-4);
    TS_ASSERT_DELTA(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("R_Cadence")->GetInfo()->ToDouble(0), 125.000, 1e-4);
    TS_ASSERT_DELTA(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("L_Velocity")->GetInfo()->ToDouble(0), 146.260 * 10.0, 1e-4);
    TS_ASSERT_DELTA(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("L_Stride_Len")->GetInfo()->ToDouble(0), 144.033 * 10.0, 1e-4);
    TS_ASSERT_DELTA(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("L_Cadence")->GetInfo()->ToDouble(0), 120.000, 1e-4);
    TS_ASSERT_DELTA(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("R_Support_Time")->GetInfo()->ToDouble(0), 61.458, 1e-4);
    TS_ASSERT_DELTA(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("L_Support_Time")->GetInfo()->ToDouble(0), 59.000, 1e-4);
    TS_ASSERT_DELTA(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("R_Non_Support")->GetInfo()->ToDouble(0), 38.542, 1e-4);
    TS_ASSERT_DELTA(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("L_Non_Support")->GetInfo()->ToDouble(0), 41.000, 1e-4);
    TS_ASSERT_DELTA(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("R_Step_Len")->GetInfo()->ToDouble(0), 73.464 * 10.0, 1e-4);
    TS_ASSERT_DELTA(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("R_Step_Len")->GetInfo()->ToDouble(1), 70.742 * 10.0, 1e-4);
    TS_ASSERT_DELTA(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("L_Step_Len")->GetInfo()->ToDouble(0), 70.569 * 10.0, 1e-4);
    TS_ASSERT_DELTA(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("R_Dbl_Support")->GetInfo()->ToDouble(0), 8.333, 1e-4);
    TS_ASSERT_DELTA(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("L_Dbl_Support")->GetInfo()->ToDouble(0), 13.000, 1e-4);
    
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 8);
    TS_ASSERT_EQUALS(acq->GetEvent(0)->GetLabel(), "Foot Strike");
    TS_ASSERT_EQUALS(acq->GetEvent(0)->GetContext(), "Right");
    TS_ASSERT_EQUALS(acq->GetEvent(0)->GetId(), 1);
    TS_ASSERT_EQUALS(acq->GetEvent(0)->GetFrame(), 137 + 70 + 1);
    TS_ASSERT_EQUALS(acq->GetEvent(0)->GetDetectionFlags(), btk::Event::FromForcePlatform);
    TS_ASSERT_EQUALS(acq->GetEvent(1)->GetFrame(), 137 + 166 + 1);
    TS_ASSERT_EQUALS(acq->GetEvent(1)->GetLabel(), "Foot Strike");
    TS_ASSERT_EQUALS(acq->GetEvent(1)->GetContext(), "Right");
    TS_ASSERT_EQUALS(acq->GetEvent(2)->GetFrame(), 137 + 19 + 1);
    TS_ASSERT_EQUALS(acq->GetEvent(3)->GetFrame(), 137 + 119 + 1);
    TS_ASSERT_EQUALS(acq->GetEvent(3)->GetDetectionFlags(), btk::Event::FromForcePlatform);
    TS_ASSERT_EQUALS(acq->GetEvent(4)->GetFrame(), 137 + 32 + 1);
    TS_ASSERT_EQUALS(acq->GetEvent(5)->GetFrame(), 137 + 129 + 1);
    TS_ASSERT_EQUALS(acq->GetEvent(6)->GetLabel(), "Foot Off");
    TS_ASSERT_EQUALS(acq->GetEvent(6)->GetContext(), "Left");
    TS_ASSERT_EQUALS(acq->GetEvent(6)->GetFrame(), 137 + 78 + 1);
    TS_ASSERT_EQUALS(acq->GetEvent(7)->GetFrame(), 137 + 177 + 1);
    
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 51);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "R_HIP_ANGLE");
    TS_ASSERT_EQUALS(acq->GetPoint(1)->GetLabel(), "R_KNEE_ANGLE");
    TS_ASSERT_EQUALS(acq->GetPoint(2)->GetLabel(), "R_ANK_ANGLE");
    TS_ASSERT_EQUALS(acq->GetPoint(3)->GetLabel(), "R_Elbow_ANGLE");
    TS_ASSERT_EQUALS(acq->GetPoint(4)->GetLabel(), "R_Shl_ANGLE");
    TS_ASSERT_EQUALS(acq->GetPoint(5)->GetLabel(), "L_HIP_ANGLE");
    TS_ASSERT_EQUALS(acq->GetPoint(6)->GetLabel(), "L_KNEE_ANGLE");
    TS_ASSERT_EQUALS(acq->GetPoint(7)->GetLabel(), "L_ANK_ANGLE");
    TS_ASSERT_EQUALS(acq->GetPoint(8)->GetLabel(), "L_Elbow_ANGLE");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetLabel(), "L_Shl_ANGLE");
    TS_ASSERT_EQUALS(acq->GetPoint(10)->GetLabel(), "Head_ANGLE");
    TS_ASSERT_EQUALS(acq->GetPoint(11)->GetLabel(), "Trunk_ANGLE");
    TS_ASSERT_EQUALS(acq->GetPoint(12)->GetLabel(), "Pelvis_ANGLE");
    TS_ASSERT_EQUALS(acq->GetPoint(13)->GetLabel(), "R_Foot_ANGLE");
    TS_ASSERT_EQUALS(acq->GetPoint(14)->GetLabel(), "L_Foot_ANGLE");
    TS_ASSERT_EQUALS(acq->GetPoint(15)->GetLabel(), "R_HIP_FORCE");
    TS_ASSERT_EQUALS(acq->GetPoint(16)->GetLabel(), "R_KNEE_FORCE");
    TS_ASSERT_EQUALS(acq->GetPoint(17)->GetLabel(), "R_ANK_FORCE");
    TS_ASSERT_EQUALS(acq->GetPoint(18)->GetLabel(), "R_GRF_FORCE");
    TS_ASSERT_EQUALS(acq->GetPoint(19)->GetLabel(), "L_HIP_FORCE");
    TS_ASSERT_EQUALS(acq->GetPoint(20)->GetLabel(), "L_KNEE_FORCE");
    TS_ASSERT_EQUALS(acq->GetPoint(21)->GetLabel(), "L_ANK_FORCE");
    TS_ASSERT_EQUALS(acq->GetPoint(22)->GetLabel(), "L_GRF_FORCE");
    TS_ASSERT_EQUALS(acq->GetPoint(23)->GetLabel(), "R_HIP_MOMENT");
    TS_ASSERT_EQUALS(acq->GetPoint(24)->GetLabel(), "R_KNEE_MOMENT");
    TS_ASSERT_EQUALS(acq->GetPoint(25)->GetLabel(), "R_ANK_MOMENT");
    TS_ASSERT_EQUALS(acq->GetPoint(26)->GetLabel(), "L_HIP_MOMENT");
    TS_ASSERT_EQUALS(acq->GetPoint(27)->GetLabel(), "L_KNEE_MOMENT");
    TS_ASSERT_EQUALS(acq->GetPoint(28)->GetLabel(), "L_ANK_MOMENT");
    TS_ASSERT_EQUALS(acq->GetPoint(29)->GetLabel(), "R_HIP_POWER");
    TS_ASSERT_EQUALS(acq->GetPoint(30)->GetLabel(), "R_KNEE_POWER");
    TS_ASSERT_EQUALS(acq->GetPoint(31)->GetLabel(), "R_ANK_POWER");     
    TS_ASSERT_EQUALS(acq->GetPoint(32)->GetLabel(), "L_HIP_POWER");
    TS_ASSERT_EQUALS(acq->GetPoint(33)->GetLabel(), "L_KNEE_POWER");
    TS_ASSERT_EQUALS(acq->GetPoint(34)->GetLabel(), "L_ANK_POWER");
    TS_ASSERT_EQUALS(acq->GetPoint(35)->GetLabel(), "L RECTUS");
    TS_ASSERT_EQUALS(acq->GetPoint(36)->GetLabel(), "R RECTUS");
    TS_ASSERT_EQUALS(acq->GetPoint(37)->GetLabel(), "L VASTE EXT");
    TS_ASSERT_EQUALS(acq->GetPoint(38)->GetLabel(), "R VASTE EXT");
    TS_ASSERT_EQUALS(acq->GetPoint(39)->GetLabel(), "L GRF");
    TS_ASSERT_EQUALS(acq->GetPoint(40)->GetLabel(), "R GRF");
    TS_ASSERT_EQUALS(acq->GetPoint(41)->GetLabel(), "L ISCHIO");
    TS_ASSERT_EQUALS(acq->GetPoint(42)->GetLabel(), "R ISCHIO");
    TS_ASSERT_EQUALS(acq->GetPoint(43)->GetLabel(), "L BICEPS");
    TS_ASSERT_EQUALS(acq->GetPoint(44)->GetLabel(), "R BICEPS");
    TS_ASSERT_EQUALS(acq->GetPoint(45)->GetLabel(), "L TIBIALIS");
    TS_ASSERT_EQUALS(acq->GetPoint(46)->GetLabel(), "R TIBIALIS");
    TS_ASSERT_EQUALS(acq->GetPoint(47)->GetLabel(), "L SOL");
    TS_ASSERT_EQUALS(acq->GetPoint(48)->GetLabel(), "R SOL");
    TS_ASSERT_EQUALS(acq->GetPoint(49)->GetLabel(), "L JUM INT");
    TS_ASSERT_EQUALS(acq->GetPoint(50)->GetLabel(), "R JUM INT");
    
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetType(), btk::Point::Angle); // "R_HIP_ANGLE"
    TS_ASSERT_EQUALS(acq->GetPoint(1)->GetType(), btk::Point::Angle); // "R_KNEE_ANGLE"
    TS_ASSERT_EQUALS(acq->GetPoint(14)->GetType(), btk::Point::Angle); // "L_Foot_ANGLE"
    TS_ASSERT_EQUALS(acq->GetPoint(15)->GetType(), btk::Point::Force); // "R_HIP_FORCE"
    TS_ASSERT_EQUALS(acq->GetPoint(16)->GetType(), btk::Point::Force); // "R_KNEE_FORCE"
    TS_ASSERT_EQUALS(acq->GetPoint(17)->GetType(), btk::Point::Force); // "R_ANK_FORCE"
    TS_ASSERT_EQUALS(acq->GetPoint(18)->GetType(), btk::Point::Force); // "R_GRF_FORCE"
    TS_ASSERT_EQUALS(acq->GetPoint(25)->GetType(), btk::Point::Moment); // "R_ANK_MOMENT"
    TS_ASSERT_EQUALS(acq->GetPoint(26)->GetType(), btk::Point::Moment); // "L_HIP_MOMENT"
    TS_ASSERT_EQUALS(acq->GetPoint(44)->GetType(), btk::Point::Scalar); // "R BICEPS"
    
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(1, 0), -0.558, 1e-4);
    // Last value
    TS_ASSERT_DELTA(acq->GetPoint(50)->GetValues().coeff(acq->GetPointFrameNumber()-1, 0), 25.751, 1e-4);
  };
  
  CXXTEST_TEST(EmptySpatialParameters)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(XLSOrthoTrakFilePathIN + "EmptySpatialParameters.xls");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 1357);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 15);

    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChildNumber(), 15);
    TS_ASSERT_DELTA(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("Avg_Step_Width")->GetInfo()->ToDouble(0), 36.53 * 10, 1e-4);
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("R_Velocity")->GetInfo()->GetValues().empty(), true);
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("R_Stride_Len")->GetInfo()->GetValues().empty(), true);
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("R_Cadence")->GetInfo()->GetValues().empty(), true);
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("L_Velocity")->GetInfo()->GetValues().empty(), true);
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("L_Stride_Len")->GetInfo()->GetValues().empty(), true);
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("L_Cadence")->GetInfo()->GetValues().empty(), true);
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("R_Support_Time")->GetInfo()->ToDouble(0), 0.0);
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("L_Support_Time")->GetInfo()->ToDouble(0), 0.0);
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("R_Non_Support")->GetInfo()->ToDouble(0), 0.0);
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("L_Non_Support")->GetInfo()->ToDouble(0), 0.0);
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("R_Step_Len")->GetInfo()->GetValues().empty(), true);
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("L_Step_Len")->GetInfo()->GetValues().empty(), true);
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("R_Dbl_Support")->GetInfo()->ToDouble(0), 0.0);
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("SPATIOTEMP")->GetChild("L_Dbl_Support")->GetInfo()->ToDouble(0), 0.0);
    
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    
    // First frame
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(0,0), 62.767, 1e-4);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(0,1), -15.171, 1e-4);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(0,2), 9.519, 1e-4);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetValues().coeff(0,0), 13.240, 1e-4);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetValues().coeff(0,1), -3.836, 1e-4);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetValues().coeff(0,2), 0.000, 1e-4);
    // Last frame
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(acq->GetPointFrameNumber()-1, 0), 63.938, 1e-4);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(acq->GetPointFrameNumber()-1, 1), -13.137, 1e-4);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues().coeff(acq->GetPointFrameNumber()-1, 2), 15.398, 1e-4);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetValues().coeff(acq->GetPointFrameNumber()-1, 0), 20.452, 1e-4);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetValues().coeff(acq->GetPointFrameNumber()-1, 1), -1.318, 1e-4);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetValues().coeff(acq->GetPointFrameNumber()-1, 2), 0.0, 1e-4);
  };
};

CXXTEST_SUITE_REGISTRATION(XLSOrthoTrakFileReaderTest)
CXXTEST_TEST_REGISTRATION(XLSOrthoTrakFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(XLSOrthoTrakFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(XLSOrthoTrakFileReaderTest, Gait)
CXXTEST_TEST_REGISTRATION(XLSOrthoTrakFileReaderTest, EmptySpatialParameters)
#endif
