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
  
  CXXTEST_TEST(Gait)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(ANCFilePathIN + "Gait.anc");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 1000.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 5345);
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
    TS_ASSERT_EQUALS(channel.size(), 12);
    TS_ASSERT_EQUALS(dims.size(), 2);
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
  };
};

CXXTEST_SUITE_REGISTRATION(ANCFileReaderTest)
CXXTEST_TEST_REGISTRATION(ANCFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(ANCFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(ANCFileReaderTest, Gait)
#endif
