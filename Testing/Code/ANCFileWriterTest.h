#ifndef ANCFileWriterTest_h
#define ANCFileWriterTest_h

#include <btkAcquisitionFileWriter.h>
#include <btkANCFileIO.h>

CXXTEST_SUITE(ANCFileWriterTest)
{
  CXXTEST_TEST(NoFileNoInput)
  {
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    TS_ASSERT_THROWS_EQUALS(writer->Update(), const btk::AcquisitionFileWriterException &e, e.what(), std::string("Filename must be specified."));
  };

  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(ANCFilePathIN + "Gait.anc");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(reader->GetOutput());
    TS_ASSERT_THROWS_EQUALS(writer->Update(), const btk::AcquisitionFileWriterException &e, e.what(), std::string("Filename must be specified."));
  };


  CXXTEST_TEST(NoFileWithIO)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(ANCFilePathIN + "Gait.anc");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    btk::ANCFileIO::Pointer io = btk::ANCFileIO::New();
    writer->SetAcquisitionIO(io);
    TS_ASSERT_THROWS_EQUALS(writer->Update(), const btk::AcquisitionFileWriterException &e, e.what(), std::string("Filename must be specified."));
  };

  CXXTEST_TEST(Gait_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(ANCFilePathIN + "Gait.anc");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(ANCFilePathOUT + "Gait_rewrited.anc");
    writer->Update();
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(ANCFilePathOUT + "Gait_rewrited.anc");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());

    for(int i = 0 ; i < 28 ; ++i)
    {
      for(int j = 0 ; j < 5345 ; ++j)
      {
        TS_ASSERT_DELTA(acq->GetAnalog(i)->GetValues()(j), acq2->GetAnalog(i)->GetValues()(j), 0.0001);
      }
    }
  };

  CXXTEST_TEST(Gait_from_c3d)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "others/Gait.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(ANCFilePathOUT + "Gait_from_c3d.anc");
    writer->Update();
    
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader->SetAcquisitionIO(); // Reset IO
    reader->SetFilename(ANCFilePathIN + "Gait.anc");
    reader->Update();
    reader2->SetFilename(ANCFilePathOUT + "Gait_from_c3d.anc");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();

    for(int i = 12 ; i < 28 ; ++i)
    {
      for(int j = 0 ; j < 4870 ; ++j)
      {
        TS_ASSERT_DELTA(acq->GetAnalog(i)->GetValues()(j), acq2->GetAnalog(i)->GetValues()(j), 0.001);
      }
    }

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    // TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber()); // Not the same number of frames due to a truncated C3D file.
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    std::string boardInfo = acq2->GetMetaData()->GetChild("ANALOG")->GetChild("BOARD")->GetInfo()->ToString(0);
    TS_ASSERT_EQUALS(boardInfo, "Unknown");
    TS_ASSERT_EQUALS(acq2->GetAnalog(0)->GetLabel(), "f1x");
    TS_ASSERT_EQUALS(acq2->GetAnalog(1)->GetLabel(), "f1y");
    TS_ASSERT_EQUALS(acq2->GetAnalog(2)->GetLabel(), "f1z");
    TS_ASSERT_EQUALS(acq2->GetAnalog(3)->GetLabel(), "m1x");
    TS_ASSERT_EQUALS(acq2->GetAnalog(4)->GetLabel(), "m1y");
    TS_ASSERT_EQUALS(acq2->GetAnalog(5)->GetLabel(), "m1z");
    TS_ASSERT_EQUALS(acq2->GetAnalog(6)->GetLabel(), "f2x");
    TS_ASSERT_EQUALS(acq2->GetAnalog(21)->GetLabel(), "R Biceps");
    TS_ASSERT_EQUALS(acq2->GetAnalog(22)->GetLabel(), "L Tibialis");
    TS_ASSERT_EQUALS(acq2->GetAnalog(23)->GetLabel(), "R Tibialis");
    TS_ASSERT_EQUALS(acq2->GetAnalog(24)->GetLabel(), "L Sol");
    TS_ASSERT_EQUALS(acq2->GetAnalog(25)->GetLabel(), "R Sol");
    TS_ASSERT_EQUALS(acq2->GetAnalog(26)->GetLabel(), "L Jum Int");
    TS_ASSERT_EQUALS(acq2->GetAnalog(27)->GetLabel(), "R Jum Int");
  };
};

CXXTEST_SUITE_REGISTRATION(ANCFileWriterTest)
CXXTEST_TEST_REGISTRATION(ANCFileWriterTest, NoFileNoInput)
CXXTEST_TEST_REGISTRATION(ANCFileWriterTest, NoFile)
CXXTEST_TEST_REGISTRATION(ANCFileWriterTest, NoFileWithIO)
CXXTEST_TEST_REGISTRATION(ANCFileWriterTest, Gait_rewrited)
CXXTEST_TEST_REGISTRATION(ANCFileWriterTest, Gait_from_c3d)  
#endif
