#ifndef SpecializedPointsExtractorTest_h
#define SpecializedPointsExtractorTest_h

#include <btkSpecializedPointsExtractor.h>
#include <btkAcquisitionFileReader.h>
#include <btkPoint.h>

CXXTEST_SUITE(SpecializedPointsExtractorTest)
{
  CXXTEST_TEST(PlugInGait)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample09/PlugInC3D.c3d");
    btk::SpecializedPointsExtractor::Pointer specialPointExtractor = btk::SpecializedPointsExtractor::New();
    specialPointExtractor->SetInput(reader->GetOutput());
    btk::PointCollection::Pointer markers = specialPointExtractor->GetOutput();
    specialPointExtractor->SetPointType(btk::Point::Marker);
    specialPointExtractor->Update();
    TS_ASSERT_EQUALS(markers->GetItemNumber(), 54);
    TS_ASSERT_EQUALS(markers->GetItem(0)->GetLabel(), "RKNE");
    TS_ASSERT_EQUALS(markers->GetItem(1)->GetLabel(), "RTOE");
    TS_ASSERT_EQUALS(markers->GetItem(2)->GetLabel(), "RTIB");
    TS_ASSERT_EQUALS(markers->GetItem(3)->GetLabel(), "RASI");
    TS_ASSERT_EQUALS(markers->GetItem(4)->GetLabel(), "RTHI");
    TS_ASSERT_EQUALS(markers->GetItem(5)->GetLabel(), "RHEE");
    TS_ASSERT_EQUALS(markers->GetItem(6)->GetLabel(), "LKNE");
    TS_ASSERT_EQUALS(markers->GetItem(7)->GetLabel(), "RANK");
    TS_ASSERT_EQUALS(markers->GetItem(8)->GetLabel(), "RCLA");
    TS_ASSERT_EQUALS(markers->GetItem(45)->GetLabel(), "RTIL");
    TS_ASSERT_EQUALS(markers->GetItem(46)->GetLabel(), "RFOO");
    TS_ASSERT_EQUALS(markers->GetItem(47)->GetLabel(), "RFOP");
    TS_ASSERT_EQUALS(markers->GetItem(48)->GetLabel(), "RFOA");
    TS_ASSERT_EQUALS(markers->GetItem(49)->GetLabel(), "RFOL");
    TS_ASSERT_EQUALS(markers->GetItem(50)->GetLabel(), "RTOO");
    TS_ASSERT_EQUALS(markers->GetItem(51)->GetLabel(), "RTOP");
    TS_ASSERT_EQUALS(markers->GetItem(52)->GetLabel(), "RTOA");
    TS_ASSERT_EQUALS(markers->GetItem(53)->GetLabel(), "RTOL");

  };

  CXXTEST_TEST(Eb015pi)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pi.c3d");
    btk::SpecializedPointsExtractor::Pointer specialPointExtractor = btk::SpecializedPointsExtractor::New();
    specialPointExtractor->SetInput(reader->GetOutput());
    btk::PointCollection::Pointer markers = specialPointExtractor->GetOutput();
    specialPointExtractor->SetPointType(btk::Point::Marker);
    specialPointExtractor->Update();
    TS_ASSERT_EQUALS(markers->GetItem(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(markers->GetItem(12)->GetLabel(), "LSK3");
    TS_ASSERT_EQUALS(markers->GetItem(25)->GetLabel(), "pv4");
  };

  CXXTEST_TEST(NoPoint)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample19/sample19.c3d");
    btk::SpecializedPointsExtractor::Pointer specialPointExtractor = btk::SpecializedPointsExtractor::New();
    specialPointExtractor->SetInput(reader->GetOutput());
    btk::PointCollection::Pointer markers = specialPointExtractor->GetOutput();
    specialPointExtractor->SetPointType(btk::Point::Marker);
    specialPointExtractor->Update();
    TS_ASSERT_EQUALS(markers->GetItemNumber(), 0);
  };

  CXXTEST_TEST(NoParameter)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample24/MotionMonitorC3D.c3d");
    btk::SpecializedPointsExtractor::Pointer specialPointExtractor = btk::SpecializedPointsExtractor::New();
    specialPointExtractor->SetInput(reader->GetOutput());
    btk::PointCollection::Pointer markers = specialPointExtractor->GetOutput();
    specialPointExtractor->SetPointType(btk::Point::Marker);
    specialPointExtractor->Update();
    TS_ASSERT_EQUALS(markers->GetItemNumber(), 8);
  };


  CXXTEST_TEST(PlugInGait2)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample09/PlugInC3D.c3d");
    btk::SpecializedPointsExtractor::Pointer specialPointExtractor = btk::SpecializedPointsExtractor::New();
    specialPointExtractor->SetInput(reader->GetOutput());
    btk::PointCollection::Pointer markers = specialPointExtractor->GetOutput();
    specialPointExtractor->SetPointType(btk::Point::Angle);
    specialPointExtractor->Update();
    TS_ASSERT_EQUALS(markers->GetItemNumber(), 10);
    TS_ASSERT_EQUALS(markers->GetItem(0)->GetLabel(), "LHipAngles");
    TS_ASSERT_EQUALS(markers->GetItem(9)->GetLabel(), "RFootProgressAngles");
    specialPointExtractor->SetPointType(btk::Point::Force);
    specialPointExtractor->Update();
    TS_ASSERT_EQUALS(markers->GetItemNumber(), 8);
    TS_ASSERT_EQUALS(markers->GetItem(0)->GetLabel(), "LGroundReactionForce");
    TS_ASSERT_EQUALS(markers->GetItem(7)->GetLabel(), "RHipForce");
    specialPointExtractor->SetPointType(btk::Point::Moment);
    specialPointExtractor->Update();
    TS_ASSERT_EQUALS(markers->GetItemNumber(), 8);
    TS_ASSERT_EQUALS(markers->GetItem(0)->GetLabel(), "LGroundReactionMoment");
    TS_ASSERT_EQUALS(markers->GetItem(7)->GetLabel(), "RHipMoment");
    specialPointExtractor->SetPointType(btk::Point::Power);
    specialPointExtractor->Update();
    TS_ASSERT_EQUALS(markers->GetItemNumber(), 6);
    TS_ASSERT_EQUALS(markers->GetItem(0)->GetLabel(), "LAnklePower");
    TS_ASSERT_EQUALS(markers->GetItem(5)->GetLabel(), "RHipPower");
  };

  CXXTEST_TEST(Eb015pi2)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pi.c3d");
    btk::SpecializedPointsExtractor::Pointer specialPointExtractor = btk::SpecializedPointsExtractor::New();
    specialPointExtractor->SetInput(reader->GetOutput());
    btk::PointCollection::Pointer markers = specialPointExtractor->GetOutput();
    specialPointExtractor->SetPointType(btk::Point::Angle);
    specialPointExtractor->Update();
    TS_ASSERT_EQUALS(markers->GetItemNumber(), 0);
    specialPointExtractor->SetPointType(btk::Point::Force);
    specialPointExtractor->Update();
    TS_ASSERT_EQUALS(markers->GetItemNumber(), 0);
    specialPointExtractor->SetPointType(btk::Point::Moment);
    specialPointExtractor->Update();
    TS_ASSERT_EQUALS(markers->GetItemNumber(), 0);
    specialPointExtractor->SetPointType(btk::Point::Power);
    specialPointExtractor->Update();
    TS_ASSERT_EQUALS(markers->GetItemNumber(), 0);
    specialPointExtractor->SetPointType(btk::Point::Scalar);
    specialPointExtractor->Update();
    TS_ASSERT_EQUALS(markers->GetItemNumber(), 0);

  };
};

CXXTEST_SUITE_REGISTRATION(SpecializedPointsExtractorTest)
CXXTEST_TEST_REGISTRATION(SpecializedPointsExtractorTest, PlugInGait)
CXXTEST_TEST_REGISTRATION(SpecializedPointsExtractorTest, Eb015pi)
CXXTEST_TEST_REGISTRATION(SpecializedPointsExtractorTest, NoPoint)
CXXTEST_TEST_REGISTRATION(SpecializedPointsExtractorTest, NoParameter)
CXXTEST_TEST_REGISTRATION(SpecializedPointsExtractorTest, PlugInGait2)
CXXTEST_TEST_REGISTRATION(SpecializedPointsExtractorTest, Eb015pi2)
#endif

