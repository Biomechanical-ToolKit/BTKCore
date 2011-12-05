#ifndef ANCFileWriterTest_h
#define ANCFileWriterTest_h

#include <btkAcquisitionFileWriter.h>
#include <btkAcquisitionFileReader.h>
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
      for(int j = 0 ; j < 5346 ; ++j)
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
  
  CXXTEST_TEST(Res16bits_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(ANCFilePathIN + "Res16bits.anc");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(ANCFilePathOUT + "Res16bits_rewrited.anc");
    writer->Update();
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(ANCFilePathOUT + "Res16bits_rewrited.anc");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());

    for(int i = 0 ; i < acq->GetAnalogNumber() ; ++i)
    {
      for(int j = 0 ; j < acq->GetAnalogFrameNumber() ; ++j)
      {
        TS_ASSERT_DELTA(acq->GetAnalog(i)->GetValues()(j), acq2->GetAnalog(i)->GetValues()(j), 1e-5);
      }
    }
  };
  
  CXXTEST_TEST(Shd01_from_c3d)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "others/Shd01.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(ANCFilePathOUT + "Shd01_from_c3d.anc");
    writer->Update();
    
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(ANCFilePathOUT + "Shd01_from_c3d.anc");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();

    TS_ASSERT_EQUALS(acq2->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq2->GetPointFrequency(), 1000.0);
    TS_ASSERT_EQUALS(acq2->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(acq2->GetPointFrameNumber(), acq->GetAnalogFrameNumber());
    TS_ASSERT_EQUALS(acq2->GetAnalogFrequency(), acq->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq2->GetAnalogNumber(), acq->GetAnalogNumber());
    TS_ASSERT_EQUALS(acq2->GetAnalog(0)->GetLabel(), "Fx");
    TS_ASSERT_EQUALS(acq2->GetAnalog(1)->GetLabel(), "Fy");
    TS_ASSERT_EQUALS(acq2->GetAnalog(2)->GetLabel(), "Fz");
    TS_ASSERT_EQUALS(acq2->GetAnalog(3)->GetLabel(), "Mx");
    TS_ASSERT_EQUALS(acq2->GetAnalog(4)->GetLabel(), "My");
    TS_ASSERT_EQUALS(acq2->GetAnalog(5)->GetLabel(), "Mz");
    
    double s = 20.0 / 65536.0;
    double s1 = -1.0 * acq->GetAnalog(0)->GetScale() / s;
    double s2 = -1.0 * acq->GetAnalog(1)->GetScale() / s;
    double s3 = -1.0 * acq->GetAnalog(2)->GetScale() / s;
    double s4 = -1.0 * acq->GetAnalog(3)->GetScale() / s;
    double s5 = -1.0 * acq->GetAnalog(4)->GetScale() / s;
    double s6 = -1.0 * acq->GetAnalog(5)->GetScale() / s;
    
    for(int j = 0 ; j < 1 ; ++j)
    {
      TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(j), acq2->GetAnalog(0)->GetValues()(j) * s1, 1e-5);
      TS_ASSERT_DELTA(acq->GetAnalog(1)->GetValues()(j), acq2->GetAnalog(1)->GetValues()(j) * s2, 1e-5);
      TS_ASSERT_DELTA(acq->GetAnalog(2)->GetValues()(j), acq2->GetAnalog(2)->GetValues()(j) * s3, 1e-5);
      TS_ASSERT_DELTA(acq->GetAnalog(3)->GetValues()(j), acq2->GetAnalog(3)->GetValues()(j) * s4, 1e-5);
      TS_ASSERT_DELTA(acq->GetAnalog(4)->GetValues()(j), acq2->GetAnalog(4)->GetValues()(j) * s5, 1e-5);
      TS_ASSERT_DELTA(acq->GetAnalog(5)->GetValues()(j), acq2->GetAnalog(5)->GetValues()(j) * s6, 1e-5);
    }
  };
};

CXXTEST_SUITE_REGISTRATION(ANCFileWriterTest)
CXXTEST_TEST_REGISTRATION(ANCFileWriterTest, NoFileNoInput)
CXXTEST_TEST_REGISTRATION(ANCFileWriterTest, NoFile)
CXXTEST_TEST_REGISTRATION(ANCFileWriterTest, NoFileWithIO)
CXXTEST_TEST_REGISTRATION(ANCFileWriterTest, Gait_rewrited)
CXXTEST_TEST_REGISTRATION(ANCFileWriterTest, Gait_from_c3d)
CXXTEST_TEST_REGISTRATION(ANCFileWriterTest, Res16bits_rewrited)
CXXTEST_TEST_REGISTRATION(ANCFileWriterTest, Shd01_from_c3d) 
#endif
