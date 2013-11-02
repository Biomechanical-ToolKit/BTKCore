#ifndef MeasureFrameExtractorTest_h
#define MeasureFrameExtractorTest_h

#include <btkMeasureFrameExtractor.h>
#include <btkAcquisitionFileReader.h>
#include <btkPoint.h>
#include <btkAnalog.h>

CXXTEST_SUITE(MeasureFrameExtractorTest)
{
  CXXTEST_TEST(Test1)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pi.c3d");
    reader->Update();
    btk::MeasureFrameExtractor<btk::Point>::Pointer pointExtractor = btk::MeasureFrameExtractor<btk::Point>::New();
    btk::MeasureFrameExtractor<btk::Analog>::Pointer analogExtractor = btk::MeasureFrameExtractor<btk::Analog>::New();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    pointExtractor->SetInput(acq->GetPoints());
    analogExtractor->SetInput(acq->GetAnalogs());
    btk::Point::Pointer PointFrame = pointExtractor->GetOutput();
    btk::Analog::Pointer AnalogFrame = analogExtractor->GetOutput();
    PointFrame->Update();
    AnalogFrame->Update();
    TS_ASSERT_EQUALS(PointFrame->GetFrameNumber(), 26);
    TS_ASSERT_EQUALS(AnalogFrame->GetFrameNumber(), 16);
    TS_ASSERT_DELTA(AnalogFrame->GetValues()(0,0), -26.6599, 0.001);
  };
};

CXXTEST_SUITE_REGISTRATION(MeasureFrameExtractorTest)
CXXTEST_TEST_REGISTRATION(MeasureFrameExtractorTest, Test1)
#endif
