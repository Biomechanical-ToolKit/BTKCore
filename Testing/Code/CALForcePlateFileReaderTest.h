#ifndef CALForcePlateFileReaderTest_h
#define CALForcePlateFileReaderTest_h

#include <btkAcquisitionFileReader.h>

CXXTEST_SUITE(CALForcePlateFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("test.cal");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: test.cal"));
  };
  
  CXXTEST_TEST(Forcepla)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(CALForcePlateFilePathIN + "Forcepla.cal");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("BTK_PARTIAL_FP_CONFIG")->GetChild("USED")->GetInfo()->ToInt(0), 2);
    
    std::vector<int> type = acq->GetMetaData()->GetChild("BTK_PARTIAL_FP_CONFIG")->GetChild("TYPE")->GetInfo()->ToInt();
    TS_ASSERT_EQUALS(type.size(), 2);
    TS_ASSERT_EQUALS(type[0], 4);
    TS_ASSERT_EQUALS(type[1], 4);
    
    std::vector<float> origin = acq->GetMetaData()->GetChild("BTK_PARTIAL_FP_CONFIG")->GetChild("ORIGIN")->GetInfo()->ToFloat();
    TS_ASSERT_DELTA(origin[0], -0.94, 1e-5);
    TS_ASSERT_DELTA(origin[1], -0.917, 1e-5);
    TS_ASSERT_DELTA(origin[2], -59.428, 1e-5);
    TS_ASSERT_DELTA(origin[3], -1.944, 1e-5);
    TS_ASSERT_DELTA(origin[4], -1.885, 1e-5);
    TS_ASSERT_DELTA(origin[5], -52.979, 1e-5);
    
    std::vector<float> corners = acq->GetMetaData()->GetChild("BTK_PARTIAL_FP_CONFIG")->GetChild("CORNERS")->GetInfo()->ToFloat();
    TS_ASSERT_DELTA(corners[0], -306.4, 1e-5);
    TS_ASSERT_DELTA(corners[1], -620.7, 1e-4);
    TS_ASSERT_DELTA(corners[2], 0.0, 1e-5);
    TS_ASSERT_DELTA(corners[3], -306.4, 1e-5);
    TS_ASSERT_DELTA(corners[4], -11.1, 1e-5);
    TS_ASSERT_DELTA(corners[5], 0.0, 1e-5);
    TS_ASSERT_DELTA(corners[6], 608.0, 1e-5);
    TS_ASSERT_DELTA(corners[7], -11.1, 1e-5);
    TS_ASSERT_DELTA(corners[8], 0.0, 1e-5);
    TS_ASSERT_DELTA(corners[9], 608.0, 1e-5);
    TS_ASSERT_DELTA(corners[10], -620.7, 1e-4);
    TS_ASSERT_DELTA(corners[11], 0.0, 1e-5);
    TS_ASSERT_DELTA(corners[12], -9.30001, 1e-5);
    TS_ASSERT_DELTA(corners[13], -10.1, 1e-5);
    TS_ASSERT_DELTA(corners[14], 0.0, 1e-5);
    TS_ASSERT_DELTA(corners[15], -9.30001, 1e-5);
    TS_ASSERT_DELTA(corners[16], 599.5, 1e-5);
    TS_ASSERT_DELTA(corners[17], 0.0, 1e-5);
    TS_ASSERT_DELTA(corners[18], 905.1, 1e-4);
    TS_ASSERT_DELTA(corners[19], 599.5, 1e-5);
    TS_ASSERT_DELTA(corners[20], 0.0, 1e-5);
    TS_ASSERT_DELTA(corners[21], 905.1, 1e-4);
    TS_ASSERT_DELTA(corners[22], -10.1, 1e-5);
    TS_ASSERT_DELTA(corners[23], 0.0, 1e-5);
    
    std::vector<float> calMatrix = acq->GetMetaData()->GetChild("BTK_PARTIAL_FP_CONFIG")->GetChild("CAL_MATRIX")->GetInfo()->ToFloat();
    TS_ASSERT_DELTA(calMatrix[0], 2.9446, 1e-5);
    TS_ASSERT_DELTA(calMatrix[1], -0.0191, 1e-5);
    TS_ASSERT_DELTA(calMatrix[2], -0.0253, 1e-5);
    TS_ASSERT_DELTA(calMatrix[3], 0.0019 * 1000, 1e-5);
    TS_ASSERT_DELTA(calMatrix[4], 0.0031 * 1000, 1e-5);
    TS_ASSERT_DELTA(calMatrix[5], 0.0029 * 1000, 1e-5);
    TS_ASSERT_DELTA(calMatrix[6], 0.0048, 1e-5);
    TS_ASSERT_DELTA(calMatrix[7], 2.9490, 1e-5);
    TS_ASSERT_DELTA(calMatrix[8], -0.0038, 1e-5);
    TS_ASSERT_DELTA(calMatrix[9], 0.0030 * 1000, 1e-5);
    TS_ASSERT_DELTA(calMatrix[10], -0.0043 * 1000, 1e-5);
    TS_ASSERT_DELTA(calMatrix[11], -0.0020 * 1000, 1e-5);
    TS_ASSERT_DELTA(calMatrix[12], -0.0091, 1e-5);
    TS_ASSERT_DELTA(calMatrix[13], -0.0129, 1e-5);
    TS_ASSERT_DELTA(calMatrix[14], 11.5203, 1e-5);
    TS_ASSERT_DELTA(calMatrix[15], -0.0108 * 1000, 1e-5);
    TS_ASSERT_DELTA(calMatrix[16], -0.0107 * 1000, 1e-5);
    TS_ASSERT_DELTA(calMatrix[17], 0.0030 * 1000, 1e-5);
    TS_ASSERT_DELTA(calMatrix[18], 0.0, 1e-5);
    TS_ASSERT_DELTA(calMatrix[19], 0.0057, 1e-5);
    TS_ASSERT_DELTA(calMatrix[20], -0.0226, 1e-5);
    TS_ASSERT_DELTA(calMatrix[21], 2.1918 * 1000, 1e-4);
    TS_ASSERT_DELTA(calMatrix[22], 0.0077 * 1000, 1e-5);
    TS_ASSERT_DELTA(calMatrix[23], 0.0 * 1000, 1e-5);
    TS_ASSERT_DELTA(calMatrix[24], -0.0051, 1e-5);
    TS_ASSERT_DELTA(calMatrix[25], -0.0050, 1e-5);
    TS_ASSERT_DELTA(calMatrix[26], -0.0605, 1e-5);
    TS_ASSERT_DELTA(calMatrix[27], 0.0039 * 1000, 1e-5);
    TS_ASSERT_DELTA(calMatrix[28], 1.9772 * 1000, 1e-4);
    TS_ASSERT_DELTA(calMatrix[29], 0.0016 * 1000, 1e-5);
    TS_ASSERT_DELTA(calMatrix[30], 0.0174, 1e-5);
    TS_ASSERT_DELTA(calMatrix[31], 0.0049, 1e-5);
    TS_ASSERT_DELTA(calMatrix[32], -0.0003, 1e-5);
    TS_ASSERT_DELTA(calMatrix[33], 0.0167 * 1000, 1e-5);
    TS_ASSERT_DELTA(calMatrix[34], 0.0001 * 1000, 1e-5);
    TS_ASSERT_DELTA(calMatrix[35], 0.9558 * 1000, 1e-4);
  };
};

CXXTEST_SUITE_REGISTRATION(CALForcePlateFileReaderTest)
CXXTEST_TEST_REGISTRATION(CALForcePlateFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(CALForcePlateFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(CALForcePlateFileReaderTest, Forcepla)
#endif
