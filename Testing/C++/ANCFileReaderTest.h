#ifndef ANCFileReaderTest_h
#define ANCFileReaderTest_h

#include <btkAcquisitionFileReader.h>

CXXTEST_SUITE(ANCFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("test.anc");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: test.anc"));
  };
  
  CXXTEST_TEST(Truncated)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(ANCFilePathIN + "Truncated.anc");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::Exception &e, e.what(), std::string("Unexpected end of file."));
  };

  CXXTEST_TEST(Gait)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(ANCFilePathIN + "Gait.anc");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 1000.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 5346);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 1000.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 28);
    TS_ASSERT_EQUALS(acq->GetPointUnit(), "mm");
    std::string boardInfo = acq->GetMetaData()->GetChild("ANALOG")->GetChild("BOARD")->GetInfo()->ToString(0);
    TS_ASSERT_EQUALS(boardInfo, "National PCI-6071E");
    TS_ASSERT_EQUALS(acq->GetAnalog(0)->GetLabel(), "f1x");
    TS_ASSERT_EQUALS(acq->GetAnalog(1)->GetLabel(), "f1y");
    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetLabel(), "f1z");
    TS_ASSERT_EQUALS(acq->GetAnalog(3)->GetLabel(), "m1x");
    TS_ASSERT_EQUALS(acq->GetAnalog(4)->GetLabel(), "m1y");
    TS_ASSERT_EQUALS(acq->GetAnalog(5)->GetLabel(), "m1z");
    TS_ASSERT_EQUALS(acq->GetAnalog(6)->GetLabel(), "f2x");
    TS_ASSERT_EQUALS(acq->GetAnalog(21)->GetLabel(), "R Biceps");
    TS_ASSERT_EQUALS(acq->GetAnalog(22)->GetLabel(), "L Tibialis");
    TS_ASSERT_EQUALS(acq->GetAnalog(23)->GetLabel(), "R Tibialis");
    TS_ASSERT_EQUALS(acq->GetAnalog(24)->GetLabel(), "L Sol");
    TS_ASSERT_EQUALS(acq->GetAnalog(25)->GetLabel(), "R Sol");
    TS_ASSERT_EQUALS(acq->GetAnalog(26)->GetLabel(), "L Jum Int");
    TS_ASSERT_EQUALS(acq->GetAnalog(27)->GetLabel(), "R Jum Int");

    TS_ASSERT_DELTA(acq->GetAnalog(13)->GetValues()(0), -0.2627, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(13)->GetValues()(1), -0.3169, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(13)->GetValues()(2), -0.2481, 0.0001);
    
    std::vector<int16_t> channel = acq->GetMetaData()->GetChild("BTK_PARTIAL_FP_CHAN")->GetChild("CHANNEL")->GetInfo()->ToInt16();
    std::vector<uint8_t> dims = acq->GetMetaData()->GetChild("BTK_PARTIAL_FP_CHAN")->GetChild("CHANNEL")->GetInfo()->GetDimensions();
    TS_ASSERT_EQUALS((int)channel.size(), 12);
    TS_ASSERT_EQUALS((int)dims.size(), 2);
    TS_ASSERT_EQUALS(dims.at(0), 6);
    TS_ASSERT_EQUALS(dims.at(1), 2);
    TS_ASSERT_EQUALS(channel.at(0), 1);
    TS_ASSERT_EQUALS(channel.at(1), 2);
    TS_ASSERT_EQUALS(channel.at(2), 3);
    TS_ASSERT_EQUALS(channel.at(3), 4);
    TS_ASSERT_EQUALS(channel.at(4), 5);
    TS_ASSERT_EQUALS(channel.at(5), 6);
    TS_ASSERT_EQUALS(channel.at(6), 7);
    TS_ASSERT_EQUALS(channel.at(7), 8);
    TS_ASSERT_EQUALS(channel.at(8), 9);
    TS_ASSERT_EQUALS(channel.at(9), 10);
    TS_ASSERT_EQUALS(channel.at(10), 11);
    TS_ASSERT_EQUALS(channel.at(11), 12);
    
    // Analog gain
    TS_ASSERT_EQUALS(acq->GetAnalog(0)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq->GetAnalog(1)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq->GetAnalog(3)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq->GetAnalog(4)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq->GetAnalog(5)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq->GetAnalog(6)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq->GetAnalog(7)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq->GetAnalog(8)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq->GetAnalog(9)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq->GetAnalog(10)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq->GetAnalog(11)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq->GetAnalog(12)->GetGain(), btk::Analog::PlusMinus1);
    TS_ASSERT_EQUALS(acq->GetAnalog(13)->GetGain(), btk::Analog::PlusMinus1);
    TS_ASSERT_EQUALS(acq->GetAnalog(14)->GetGain(), btk::Analog::PlusMinus2Dot5);
    TS_ASSERT_EQUALS(acq->GetAnalog(15)->GetGain(), btk::Analog::PlusMinus2Dot5);
    TS_ASSERT_EQUALS(acq->GetAnalog(16)->GetGain(), btk::Analog::PlusMinus1);
    TS_ASSERT_EQUALS(acq->GetAnalog(17)->GetGain(), btk::Analog::PlusMinus1);
    TS_ASSERT_EQUALS(acq->GetAnalog(18)->GetGain(), btk::Analog::PlusMinus2Dot5);
    TS_ASSERT_EQUALS(acq->GetAnalog(19)->GetGain(), btk::Analog::PlusMinus2Dot5);
    TS_ASSERT_EQUALS(acq->GetAnalog(20)->GetGain(), btk::Analog::PlusMinus2Dot5);
    TS_ASSERT_EQUALS(acq->GetAnalog(21)->GetGain(), btk::Analog::PlusMinus2Dot5);
    TS_ASSERT_EQUALS(acq->GetAnalog(22)->GetGain(), btk::Analog::PlusMinus2Dot5);
    TS_ASSERT_EQUALS(acq->GetAnalog(23)->GetGain(), btk::Analog::PlusMinus2Dot5);
    TS_ASSERT_EQUALS(acq->GetAnalog(24)->GetGain(), btk::Analog::PlusMinus2Dot5);
    TS_ASSERT_EQUALS(acq->GetAnalog(25)->GetGain(), btk::Analog::PlusMinus2Dot5);
    TS_ASSERT_EQUALS(acq->GetAnalog(26)->GetGain(), btk::Analog::PlusMinus2Dot5);
    TS_ASSERT_EQUALS(acq->GetAnalog(27)->GetGain(), btk::Analog::PlusMinus2Dot5);
  };
  
  CXXTEST_TEST(Res16Bits)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(ANCFilePathIN + "Res16bits.anc");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 1000.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 1000);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 1000);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 1000.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 81);
    
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathIN + "others/Res16bits.c3d");
    reader2->Update();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency() * 10.0);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber() * 10.0);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), acq2->GetAnalogFrameNumber());
    
    for (int i = 0 ; i < acq->GetAnalogNumber() ; ++i)
    {
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetLabel(), acq2->GetAnalog(i)->GetLabel());
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetOffset(), acq2->GetAnalog(i)->GetOffset());
      TS_ASSERT_DELTA(acq->GetAnalog(i)->GetScale(), acq2->GetAnalog(i)->GetScale(), 1e-5);
      for (int j = 0 ; j < acq->GetAnalogFrameNumber() ; j+=50)
      {
        TS_ASSERT_DELTA(acq->GetAnalog(i)->GetValues()(j), acq2->GetAnalog(i)->GetValues()(j), 1e-5);
      }
    }
  }
};

CXXTEST_SUITE_REGISTRATION(ANCFileReaderTest)
CXXTEST_TEST_REGISTRATION(ANCFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(ANCFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(ANCFileReaderTest, Truncated)
CXXTEST_TEST_REGISTRATION(ANCFileReaderTest, Gait)
CXXTEST_TEST_REGISTRATION(ANCFileReaderTest, Res16Bits)
#endif
