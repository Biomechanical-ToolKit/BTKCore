#ifndef TRCFileWriterTest_h
#define TRCFileWriterTest_h

#include <btkAcquisitionFileWriter.h>
#include <btkTRCFileIO.h>

CXXTEST_SUITE(TRCFileWriterTest)
{
  CXXTEST_TEST(NoFileNoInput)
  {
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    TS_ASSERT_THROWS_EQUALS(writer->Update(), const btk::AcquisitionFileWriterException &e, e.what(), std::string("Filename must be specified."));
  };

  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(TRCFilePathIN + "MOTEK/T.trc");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(reader->GetOutput());
    TS_ASSERT_THROWS_EQUALS(writer->Update(), const btk::AcquisitionFileWriterException &e, e.what(), std::string("Filename must be specified."));
  };


  CXXTEST_TEST(NoFileWithIO)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(TRCFilePathIN + "MOTEK/T.trc");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    btk::TRCFileIO::Pointer io = btk::TRCFileIO::New();
    writer->SetAcquisitionIO(io);
    TS_ASSERT_THROWS_EQUALS(writer->Update(), const btk::AcquisitionFileWriterException &e, e.what(), std::string("Filename must be specified."));
  };

  CXXTEST_TEST(MOTEK_T_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(TRCFilePathIN + "MOTEK/T.trc");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(TRCFilePathOUT + "MOTEK_T_rewrited.trc");
    writer->Update();

    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(TRCFilePathOUT + "MOTEK_T_rewrited.trc");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    TS_ASSERT_EQUALS(acq->GetPointUnit(), acq2->GetPointUnit());
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues()(3,0), acq2->GetPoint(1)->GetValues()(3,0), 0.000001);
  };

  CXXTEST_TEST(Knee_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(TRCFilePathIN + "Knee.trc");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(TRCFilePathOUT + "Knee_rewrited.trc");
    writer->Update();

    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(TRCFilePathOUT + "Knee_rewrited.trc");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();

    TS_ASSERT_DELTA(acq->GetPoint(70)->GetResiduals()(82), -1.0, 0.000001);

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    TS_ASSERT_EQUALS(acq->GetPointUnit(), acq2->GetPointUnit());
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues()(3,0), acq2->GetPoint(1)->GetValues()(3,0), 0.000001);
    TS_ASSERT_DELTA(acq->GetPoint(70)->GetResiduals()(82), acq2->GetPoint(70)->GetResiduals()(82), 0.000001);
  };

  CXXTEST_TEST(Gait_from_c3d)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "Gait.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(TRCFilePathOUT + "Gait_from_c3d.trc");
    writer->Update();
    
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader->SetAcquisitionIO(); // Reset IO
    reader->SetFilename(TRCFilePathIN + "Gait.trc");
    reader->Update();
    reader2->SetFilename(TRCFilePathOUT + "Gait_from_c3d.trc");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();

    for(int i = 0 ; i < 33 ; ++i)
    {
      for(int j = 0 ; j < 487 ; ++j)
      {
        TS_ASSERT_DELTA(acq->GetPoint(i)->GetValues()(j), acq2->GetPoint(i)->GetValues()(j), 0.0001);
      }
    }

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    TS_ASSERT_EQUALS(acq2->GetPoint(0)->GetLabel(), "R.Shoulder");
    TS_ASSERT_EQUALS(acq2->GetPoint(1)->GetLabel(), "R.Offset");
    TS_ASSERT_EQUALS(acq2->GetPoint(2)->GetLabel(), "R.Elbow");
    TS_ASSERT_EQUALS(acq2->GetPoint(3)->GetLabel(), "R.Wrist");
    TS_ASSERT_EQUALS(acq2->GetPoint(4)->GetLabel(), "L.Shoulder");
    TS_ASSERT_EQUALS(acq2->GetPoint(5)->GetLabel(), "L.Elbow");
    TS_ASSERT_EQUALS(acq2->GetPoint(6)->GetLabel(), "L.Wrist");
    TS_ASSERT_EQUALS(acq2->GetPoint(21)->GetLabel(), "L.Toe");
    TS_ASSERT_EQUALS(acq2->GetPoint(22)->GetLabel(), "R.Knee.Medial");
    TS_ASSERT_EQUALS(acq2->GetPoint(23)->GetLabel(), "R.Ankle.Medial");
    TS_ASSERT_EQUALS(acq2->GetPoint(24)->GetLabel(), "L.Knee.Medial");
    TS_ASSERT_EQUALS(acq2->GetPoint(25)->GetLabel(), "L.Ankle.Medial");
    TS_ASSERT_EQUALS(acq2->GetPoint(26)->GetLabel(), "R.Foot.Medial");
    TS_ASSERT_EQUALS(acq2->GetPoint(27)->GetLabel(), "R.Foot.Lateral");
  };
};

CXXTEST_SUITE_REGISTRATION(TRCFileWriterTest)
CXXTEST_TEST_REGISTRATION(TRCFileWriterTest, NoFileNoInput)
CXXTEST_TEST_REGISTRATION(TRCFileWriterTest, NoFile)
CXXTEST_TEST_REGISTRATION(TRCFileWriterTest, NoFileWithIO)
CXXTEST_TEST_REGISTRATION(TRCFileWriterTest, MOTEK_T_rewrited)
CXXTEST_TEST_REGISTRATION(TRCFileWriterTest, Knee_rewrited)
//CXXTEST_TEST_REGISTRATION(TRCFileWriterTest, Gait_from_c3d)
  
#endif
