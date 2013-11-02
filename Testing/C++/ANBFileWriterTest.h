#ifndef ANBFileWriterTest_h
#define ANBFileWriterTest_h

#include <btkAcquisitionFileWriter.h>
#include <btkAcquisitionFileReader.h>
#include <btkANBFileIO.h>

CXXTEST_SUITE(ANBFileWriterTest)
{
  CXXTEST_TEST(NoFileNoInput)
  {
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    TS_ASSERT_THROWS_EQUALS(writer->Update(), const btk::AcquisitionFileWriterException &e, e.what(), std::string("Filename must be specified."));
  };

  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(ANBFilePathIN + "Gait.anb");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(reader->GetOutput());
    TS_ASSERT_THROWS_EQUALS(writer->Update(), const btk::AcquisitionFileWriterException &e, e.what(), std::string("Filename must be specified."));
  };


  CXXTEST_TEST(NoFileWithIO)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(ANBFilePathIN + "Gait.anb");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    btk::ANBFileIO::Pointer io = btk::ANBFileIO::New();
    writer->SetAcquisitionIO(io);
    TS_ASSERT_THROWS_EQUALS(writer->Update(), const btk::AcquisitionFileWriterException &e, e.what(), std::string("Filename must be specified."));
  };

  CXXTEST_TEST(Gait_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(ANBFilePathIN + "Gait.anb");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(ANBFilePathOUT + "Gait_rewrited.anb");
    writer->Update();

    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(ANBFilePathOUT + "Gait_rewrited.anb");
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
    writer->SetFilename(ANBFilePathOUT + "Gait_from_c3d.anb");
    writer->Update();
    
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader->SetAcquisitionIO(); // Reset IO
    reader->SetFilename(ANBFilePathIN + "Gait.anb");
    reader->Update();
    reader2->SetFilename(ANBFilePathOUT + "Gait_from_c3d.anb");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();
    
    for(int i = 12 ; i < 28 ; ++i)
    {
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetGain(), acq2->GetAnalog(i)->GetGain());
      for(int j = 0 ; j < 4870 ; ++j)
      {
        TS_ASSERT_DELTA(acq->GetAnalog(i)->GetValues()(j), acq2->GetAnalog(i)->GetValues()(j), 0.001);
      }
    }
    
    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
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
  
  CXXTEST_TEST(NewAcquisition)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(0,10,6);
  
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(acq);
    writer->SetFilename(ANBFilePathOUT + "new_acquisition.anb");
    writer->Update();
    
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(ANBFilePathOUT + "new_acquisition.anb");
    reader->Update();
    btk::Acquisition::Pointer acq2 = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq2->GetPointFrequency(), 50.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq2->GetAnalogFrequency(), 50.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    TS_ASSERT_EQUALS(acq2->GetAnalog(0)->GetLabel(), "uname*1");
    TS_ASSERT_EQUALS(acq2->GetAnalog(1)->GetLabel(), "uname*2");
    TS_ASSERT_EQUALS(acq2->GetAnalog(2)->GetLabel(), "uname*3");
    TS_ASSERT_EQUALS(acq2->GetAnalog(3)->GetLabel(), "uname*4");
    TS_ASSERT_EQUALS(acq2->GetAnalog(4)->GetLabel(), "uname*5");
    TS_ASSERT_EQUALS(acq2->GetAnalog(5)->GetLabel(), "uname*6");
    
    double scale = 20.0 / 4096.0;
    for(int i = 0 ; i < 6 ; ++i)
    {
      TS_ASSERT_EQUALS(acq2->GetAnalog(i)->GetGain(), btk::Analog::PlusMinus10);
      TS_ASSERT_DELTA(acq2->GetAnalog(i)->GetScale(), scale, 1e-5);
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetOffset(), acq2->GetAnalog(i)->GetOffset());
      for(int j = 0 ; j < 10 ; ++j)
      {
        TS_ASSERT_DELTA(acq->GetAnalog(i)->GetValues()(j), acq2->GetAnalog(i)->GetValues()(j), 1e-5);
      }
    }
  };
  
  CXXTEST_TEST(NewAcquisitionBis)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(0,10,7);
    acq->SetPointFrequency(100.0);
    acq->SetAnalogResolution(btk::Acquisition::Bit16);
    btk::Analog::Pointer analog0 = acq->GetAnalog(0);
    btk::Analog::Pointer analog1 = acq->GetAnalog(1);
    btk::Analog::Pointer analog2 = acq->GetAnalog(2);
    btk::Analog::Pointer analog3 = acq->GetAnalog(3);
    btk::Analog::Pointer analog4 = acq->GetAnalog(4);
    btk::Analog::Pointer analog5 = acq->GetAnalog(5);
    btk::Analog::Pointer analog6 = acq->GetAnalog(6);
    analog0->SetLabel("Is");
    analog1->SetLabel("There");
    analog2->SetLabel("Any");
    analog3->SetLabel("Problem");
    analog4->SetLabel("In");
    analog5->SetLabel("This");
    analog6->SetLabel("Test");
    analog0->SetGain(btk::Analog::PlusMinus5);
    analog1->SetGain(btk::Analog::PlusMinus5);
    analog2->SetGain(btk::Analog::PlusMinus5);
    analog3->SetGain(btk::Analog::PlusMinus5);
    analog4->SetGain(btk::Analog::PlusMinus5);
    analog5->SetGain(btk::Analog::PlusMinus5);
    analog6->SetGain(btk::Analog::PlusMinus5);
    
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(acq);
    writer->SetFilename(ANBFilePathOUT + "new_acquisition.anb");
    writer->Update();
    
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(ANBFilePathOUT + "new_acquisition.anb");
    reader->Update();
    btk::Acquisition::Pointer acq2 = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    
    double scale = 10.0 / 65536.0;
    for(int i = 0 ; i < 7 ; ++i)
    {
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetLabel(), acq2->GetAnalog(i)->GetLabel());
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetDescription(), acq2->GetAnalog(i)->GetDescription());
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetGain(), acq2->GetAnalog(i)->GetGain());
      TS_ASSERT_DELTA(acq2->GetAnalog(i)->GetScale(), scale, 1e-5);
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetOffset(), acq2->GetAnalog(i)->GetOffset());
      for(int j = 0 ; j < 10 ; ++j)
      {
        TS_ASSERT_DELTA(acq->GetAnalog(i)->GetValues()(j), acq2->GetAnalog(i)->GetValues()(j), 1e-5);
      }
    }
  };
};

CXXTEST_SUITE_REGISTRATION(ANBFileWriterTest)
CXXTEST_TEST_REGISTRATION(ANBFileWriterTest, NoFileNoInput)
CXXTEST_TEST_REGISTRATION(ANBFileWriterTest, NoFile)
CXXTEST_TEST_REGISTRATION(ANBFileWriterTest, NoFileWithIO)
CXXTEST_TEST_REGISTRATION(ANBFileWriterTest, Gait_rewrited)
CXXTEST_TEST_REGISTRATION(ANBFileWriterTest, Gait_from_c3d)  
CXXTEST_TEST_REGISTRATION(ANBFileWriterTest, NewAcquisition)
CXXTEST_TEST_REGISTRATION(ANBFileWriterTest, NewAcquisitionBis)
#endif
