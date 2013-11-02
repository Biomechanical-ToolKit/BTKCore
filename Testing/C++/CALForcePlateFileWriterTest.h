#ifndef CALForcePlateFileWriterTest_h
#define CALForcePlateFileWriterTest_h

#include <btkAcquisitionFileWriter.h>
#include <btkAcquisitionFileReader.h>
#include <btkCALForcePlateFileIO.h>

CXXTEST_SUITE(CALForcePlateFileWriterTest)
{
  CXXTEST_TEST(NoFileNoInput)
  {
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    TS_ASSERT_THROWS_EQUALS(writer->Update(), const btk::AcquisitionFileWriterException &e, e.what(), std::string("Filename must be specified."));
  };

  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(CALForcePlateFilePathIN + "Forcepla.cal");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(reader->GetOutput());
    TS_ASSERT_THROWS_EQUALS(writer->Update(), const btk::AcquisitionFileWriterException &e, e.what(), std::string("Filename must be specified."));
  };


  CXXTEST_TEST(NoFileWithIO)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(CALForcePlateFilePathIN + "Forcepla.cal");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    btk::CALForcePlateFileIO::Pointer io = btk::CALForcePlateFileIO::New();
    writer->SetAcquisitionIO(io);
    TS_ASSERT_THROWS_EQUALS(writer->Update(), const btk::AcquisitionFileWriterException &e, e.what(), std::string("Filename must be specified."));
  };
  
  CXXTEST_TEST(Forcepla_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(CALForcePlateFilePathIN + "Forcepla.cal");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(CALForcePlateFilePathOUT + "Forcepla_rewrited.cal");
    writer->Update();
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(CALForcePlateFilePathOUT + "Forcepla_rewrited.cal");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader2->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("BTK_PARTIAL_FP_CONFIG")->GetChild("USED")->GetInfo()->ToInt(0), 2);
    
    std::vector<int> type = acq->GetMetaData()->GetChild("BTK_PARTIAL_FP_CONFIG")->GetChild("TYPE")->GetInfo()->ToInt();
    TS_ASSERT_EQUALS((int)type.size(), 2);
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
  
  CXXTEST_TEST(Forcepla_rewrited_bis)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(CALForcePlateFilePathIN + "Forcepla.cal");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(CALForcePlateFilePathOUT + "Forcepla_rewrited.cal");
    writer->Update();
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(CALForcePlateFilePathOUT + "Forcepla_rewrited.cal");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();
    
    btk::MetaData::Pointer fp = acq->GetMetaData()->GetChild("BTK_PARTIAL_FP_CONFIG");
    btk::MetaData::Pointer fp2 = acq2->GetMetaData()->GetChild("BTK_PARTIAL_FP_CONFIG");
    TS_ASSERT_EQUALS(fp->GetChild("USED")->GetInfo()->ToInt(), fp2->GetChild("USED")->GetInfo()->ToInt());
    TS_ASSERT_EQUALS(fp->GetChild("TYPE")->GetInfo()->ToInt(), fp2->GetChild("TYPE")->GetInfo()->ToInt());
    TS_ASSERT_EQUALS(fp->GetChild("ORIGIN")->GetInfo()->ToDouble(), fp2->GetChild("ORIGIN")->GetInfo()->ToDouble());
    btk::MetaDataInfo::Pointer cornersInfo = fp->GetChild("CORNERS")->GetInfo();
    btk::MetaDataInfo::Pointer cornersInfo2 = fp2->GetChild("CORNERS")->GetInfo();
    for (int i = 0 ; i < 24 ; ++i)
    {
      TS_ASSERT_DELTA(cornersInfo->ToDouble(i), cornersInfo2->ToDouble(i), 1e-5);
    }
    TS_ASSERT_EQUALS(fp->GetChild("CAL_MATRIX")->GetInfo()->ToDouble(), fp2->GetChild("CAL_MATRIX")->GetInfo()->ToDouble());
  };
  
  CXXTEST_TEST(shd01_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "others/shd01.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(CALForcePlateFilePathOUT + "shd01.cal");
    writer->Update();
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(CALForcePlateFilePathOUT + "shd01.cal");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();
    
    btk::MetaData::Pointer fp = acq->GetMetaData()->GetChild("FORCE_PLATFORM");
    btk::MetaData::Pointer fp2 = acq2->GetMetaData()->GetChild("BTK_PARTIAL_FP_CONFIG");
    TS_ASSERT_EQUALS(fp->GetChild("USED")->GetInfo()->ToInt(), fp2->GetChild("USED")->GetInfo()->ToInt());
    TS_ASSERT_EQUALS(fp->GetChild("TYPE")->GetInfo()->ToInt(), fp2->GetChild("TYPE")->GetInfo()->ToInt());
    TS_ASSERT_EQUALS(fp->GetChild("ORIGIN")->GetInfo()->ToDouble(), fp2->GetChild("ORIGIN")->GetInfo()->ToDouble());
    TS_ASSERT_EQUALS(fp->GetChild("CORNERS")->GetInfo()->ToDouble(), fp2->GetChild("CORNERS")->GetInfo()->ToDouble());
    // Shd01.cal contains scaled calibration matrix
    btk::MetaDataInfo::Pointer fpCalMat = fp->GetChild("CAL_MATRIX")->GetInfo();
    btk::MetaDataInfo::Pointer fpCalMat2 = fp2->GetChild("CAL_MATRIX")->GetInfo();
    double s = 20.0 / 65536.0;
    double s1 = -1.0 * acq->GetAnalog(0)->GetScale() / s;
    double s2 = -1.0 * acq->GetAnalog(1)->GetScale() / s;
    double s3 = -1.0 * acq->GetAnalog(2)->GetScale() / s;
    double s4 = -1.0 * acq->GetAnalog(3)->GetScale() / s;
    double s5 = -1.0 * acq->GetAnalog(4)->GetScale() / s;
    double s6 = -1.0 * acq->GetAnalog(5)->GetScale() / s;
    // Col #1
    TS_ASSERT_DELTA(fpCalMat->ToDouble(0), fpCalMat2->ToDouble(0) / s1, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(1), fpCalMat2->ToDouble(1) / s1, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(2), fpCalMat2->ToDouble(2) / s1, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(3), fpCalMat2->ToDouble(3) / s1, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(4), fpCalMat2->ToDouble(4) / s1, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(5), fpCalMat2->ToDouble(5) / s1, 1e-5);
    // Col #2
    TS_ASSERT_DELTA(fpCalMat->ToDouble(0+6), fpCalMat2->ToDouble(0+6) / s2, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(1+6), fpCalMat2->ToDouble(1+6) / s2, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(2+6), fpCalMat2->ToDouble(2+6) / s2, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(3+6), fpCalMat2->ToDouble(3+6) / s2, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(4+6), fpCalMat2->ToDouble(4+6) / s2, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(5+6), fpCalMat2->ToDouble(5+6) / s2, 1e-5);
    // Col #3
    TS_ASSERT_DELTA(fpCalMat->ToDouble(0+12), fpCalMat2->ToDouble(0+12) / s3, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(1+12), fpCalMat2->ToDouble(1+12) / s3, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(2+12), fpCalMat2->ToDouble(2+12) / s3, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(3+12), fpCalMat2->ToDouble(3+12) / s3, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(4+12), fpCalMat2->ToDouble(4+12) / s3, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(5+12), fpCalMat2->ToDouble(5+12) / s3, 1e-5);
    // Col #4
    TS_ASSERT_DELTA(fpCalMat->ToDouble(0+18), fpCalMat2->ToDouble(0+18) / s4, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(1+18), fpCalMat2->ToDouble(1+18) / s4, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(2+18), fpCalMat2->ToDouble(2+18) / s4, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(3+18), fpCalMat2->ToDouble(3+18) / s4, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(4+18), fpCalMat2->ToDouble(4+18) / s4, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(5+18), fpCalMat2->ToDouble(5+18) / s4, 1e-5);
    // Col #5
    TS_ASSERT_DELTA(fpCalMat->ToDouble(0+24), fpCalMat2->ToDouble(0+24) / s5, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(1+24), fpCalMat2->ToDouble(1+24) / s5, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(2+24), fpCalMat2->ToDouble(2+24) / s5, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(3+24), fpCalMat2->ToDouble(3+24) / s5, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(4+24), fpCalMat2->ToDouble(4+24) / s5, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(5+24), fpCalMat2->ToDouble(5+24) / s5, 1e-5);
    // Col #6
    TS_ASSERT_DELTA(fpCalMat->ToDouble(0+30), fpCalMat2->ToDouble(0+30) / s6, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(1+30), fpCalMat2->ToDouble(1+30) / s6, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(2+30), fpCalMat2->ToDouble(2+30) / s6, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(3+30), fpCalMat2->ToDouble(3+30) / s6, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(4+30), fpCalMat2->ToDouble(4+30) / s6, 1e-5);
    TS_ASSERT_DELTA(fpCalMat->ToDouble(5+30), fpCalMat2->ToDouble(5+30) / s6, 1e-5);
  };
};

CXXTEST_SUITE_REGISTRATION(CALForcePlateFileWriterTest)
CXXTEST_TEST_REGISTRATION(CALForcePlateFileWriterTest, NoFileNoInput)
CXXTEST_TEST_REGISTRATION(CALForcePlateFileWriterTest, NoFile)
CXXTEST_TEST_REGISTRATION(CALForcePlateFileWriterTest, NoFileWithIO)
CXXTEST_TEST_REGISTRATION(CALForcePlateFileWriterTest, Forcepla_rewrited)
CXXTEST_TEST_REGISTRATION(CALForcePlateFileWriterTest, Forcepla_rewrited_bis)
CXXTEST_TEST_REGISTRATION(CALForcePlateFileWriterTest, shd01_rewrited)
#endif