#ifndef DelsysEMGFileReaderTest_h
#define DelsysEMGFileReaderTest_h

#include <btkAcquisitionFileReader.h>

CXXTEST_SUITE(DelsysEMGFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("foo.emg");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: foo.emg"));
  };

  CXXTEST_TEST(FileFormatV3)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(DelsysEMGFilePathIN + "Set1[Rep2]_v3.emg");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    btk::DelsysEMGFileIO::Pointer io = static_pointer_cast<btk::DelsysEMGFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetFileVersion(),3);
    TS_WARN("Need more tests to validate the content of the acquisition.");
  };
};

CXXTEST_SUITE_REGISTRATION(DelsysEMGFileReaderTest)
CXXTEST_TEST_REGISTRATION(DelsysEMGFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(DelsysEMGFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(DelsysEMGFileReaderTest, FileFormatV3)
#endif
