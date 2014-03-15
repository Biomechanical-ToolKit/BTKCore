#ifndef CLBFileReaderTest_h
#define CLBFileReaderTest_h

#include <btkAcquisitionFileReader.h>
#include <btkConvert.h>

CXXTEST_SUITE(CLBFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("foo.clb");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: foo.clb"));
  };

  CXXTEST_TEST(NoScale)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(CLBFilePathIN + "NoScale.clb");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 2000);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 500);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 2000);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 10);
    TS_ASSERT_EQUALS(acq->GetPointUnit(), "mm");
    
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("ANALOG")->GetChild("BOARD")->GetInfo()->ToString(0), "ADA16-32/2(CB)F");
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("ANALOG")->GetChild("FORMAT")->GetInfo()->ToString(0), "UNSIGNED");
    
    for (int i = 0 ; i < 10 ; ++i)
    {
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetLabel().compare("Channel " + btk::ToString(i)), 0);
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetScale(), 20.0/65536.0);
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetOffset(), 32768.0);
    }
    
    TS_ASSERT_EQUALS(static_cast<int>(acq->GetAnalog(0)->GetValues()(0) / acq->GetAnalog(0)->GetScale()) + acq->GetAnalog(0)->GetOffset(), 32749);
    TS_ASSERT_EQUALS(static_cast<int>(acq->GetAnalog(0)->GetValues()(1) / acq->GetAnalog(0)->GetScale()) + acq->GetAnalog(0)->GetOffset(), 32746);
    TS_ASSERT_EQUALS(static_cast<int>(acq->GetAnalog(0)->GetValues()(2) / acq->GetAnalog(0)->GetScale()) + acq->GetAnalog(0)->GetOffset(), 32745);
    
    TS_ASSERT_EQUALS(static_cast<int>(acq->GetAnalog(1)->GetValues()(0) / acq->GetAnalog(1)->GetScale()) + acq->GetAnalog(1)->GetOffset(), 32770);
    TS_ASSERT_EQUALS(static_cast<int>(acq->GetAnalog(1)->GetValues()(1) / acq->GetAnalog(1)->GetScale()) + acq->GetAnalog(1)->GetOffset(), 32771);
    TS_ASSERT_EQUALS(static_cast<int>(acq->GetAnalog(1)->GetValues()(2) / acq->GetAnalog(1)->GetScale()) + acq->GetAnalog(1)->GetOffset(), 32770);
    
    TS_ASSERT_EQUALS(static_cast<int>(acq->GetAnalog(9)->GetValues()(0) / acq->GetAnalog(9)->GetScale()) + acq->GetAnalog(9)->GetOffset(), 32774);
    TS_ASSERT_EQUALS(static_cast<int>(acq->GetAnalog(9)->GetValues()(1) / acq->GetAnalog(9)->GetScale()) + acq->GetAnalog(9)->GetOffset(), 32771);
    TS_ASSERT_EQUALS(static_cast<int>(acq->GetAnalog(9)->GetValues()(2) / acq->GetAnalog(9)->GetScale()) + acq->GetAnalog(9)->GetOffset(), 32772);
  };
  
  CXXTEST_TEST(Truncated)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(CLBFilePathIN + "Truncated.clb");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::Exception &e, e.what(), std::string("Unexpected end of file."));
  };
};

CXXTEST_SUITE_REGISTRATION(CLBFileReaderTest)
CXXTEST_TEST_REGISTRATION(CLBFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(CLBFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(CLBFileReaderTest, NoScale)
CXXTEST_TEST_REGISTRATION(CLBFileReaderTest, Truncated)
#endif
