#ifndef MergeAcquisitionFilterTest_h
#define MergeAcquisitionFilterTest_h

#include <btkMergeAcquisitionFilter.h>
#include <btkAcquisitionFileReader.h>
#include <btkAcquisitionFileWriter.h>
#include <btkForcePlatformsExtractor.h>
#include <btkGroundReactionWrenchFilter.h>
//#include <btkForcePlatformWrenchFilter.h>
#include <btkConvert.h>

CXXTEST_SUITE(MergeAcquisitionFilterTest)
{ 
  CXXTEST_TEST(NoInput)
  {
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->Update();
    btk::Acquisition::Pointer output = merger->GetOutput();
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 0);
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), 0);
    TS_ASSERT_EQUALS(output->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 0);
  };
  
  CXXTEST_TEST(OneInput)
  {
    btk::Acquisition::Pointer test = btk::Acquisition::New();
    test->Init(6, 50, 2, 2);
    test->SetPointFrequency(100);
    
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(0, test);
    merger->Update();
    btk::Acquisition::Pointer output = merger->GetOutput();
    TS_ASSERT_EQUALS(output->GetPointFrequency(), 100);
    TS_ASSERT_EQUALS(output->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(output->GetPointNumber(), 6);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 2);
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 50);
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), 100);
    btk::Point::Pointer ptPoint = output->GetPoint(0);
    TS_ASSERT_EQUALS(ptPoint->GetFrameNumber(), 50);
    btk::Analog::Pointer ptAnalog = output->GetAnalog(0);
    TS_ASSERT_EQUALS(ptAnalog->GetFrameNumber(), 100);
  };
  
  CXXTEST_TEST(TwoInputsFromScratch)
  {
    btk::Acquisition::Pointer i1 = btk::Acquisition::New();
    i1->Init(6, 50);
    
    btk::Acquisition::Pointer i2 = btk::Acquisition::New();
    i2->Init(6, 50);
    i2->SetPointFrequency(100);
    
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(0, i1);
    merger->SetInput(1, i2);
    merger->Update();
    btk::Acquisition::Pointer output = merger->GetOutput();
    TS_ASSERT_EQUALS(output->GetPointFrequency(), 100);
    TS_ASSERT_EQUALS(output->GetAnalogFrequency(), 100);
    TS_ASSERT_EQUALS(output->GetPointNumber(), 12);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 50);
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), 50);
    
    
    int inc = 1;
    for (btk::Acquisition::PointIterator it = output->BeginPoint() ; it != output->EndPoint() ; ++it)
    {  
      if (inc <= 6)
      {
        TS_ASSERT_EQUALS((*it)->GetLabel(), "uname*" + btk::ToString(inc));
      }
      else
      {
        TS_ASSERT_EQUALS((*it)->GetLabel(), "uname*" + btk::ToString(inc - 6) + "_2");
      }
      ++inc;
    }
  };
  
  CXXTEST_TEST(TwoInputsFromScratch_FirstFrame1)
  {
    btk::Acquisition::Pointer i1 = btk::Acquisition::New();
    i1->Init(6, 50);
    
    btk::Acquisition::Pointer i2 = btk::Acquisition::New();
    i2->Init(6, 50);
    i2->SetFirstFrame(25);
    i2->SetPointFrequency(100);
    
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(0, i1);
    merger->SetInput(1, i2);
    merger->Update();
    btk::Acquisition::Pointer output = merger->GetOutput();
    TS_ASSERT_EQUALS(output->GetPointFrequency(), 100);
    TS_ASSERT_EQUALS(output->GetAnalogFrequency(), 100);
    TS_ASSERT_EQUALS(output->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(output->GetPointNumber(), 12);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 74);
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), 74);
  };
  
  CXXTEST_TEST(TwoInputsFromScratch_FirstFrame2)
  {
    btk::Acquisition::Pointer i1 = btk::Acquisition::New();
    i1->Init(6, 50);
    i1->SetFirstFrame(25);
    
    btk::Acquisition::Pointer i2 = btk::Acquisition::New();
    i2->Init(6, 50);
    i2->SetPointFrequency(100);
    
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(0, i1);
    merger->SetInput(1, i2);
    merger->Update();
    btk::Acquisition::Pointer output = merger->GetOutput();
    TS_ASSERT_EQUALS(output->GetPointFrequency(), 100);
    TS_ASSERT_EQUALS(output->GetAnalogFrequency(), 100);
    TS_ASSERT_EQUALS(output->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(output->GetPointNumber(), 12);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 74);
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), 74);
  };
  
  CXXTEST_TEST(TwoInputsFromScratch_Merging1)
  {
    btk::Acquisition::Pointer i1 = btk::Acquisition::New();
    i1->Init(6, 50);
    i1->SetFirstFrame(51);
    
    btk::Acquisition::Pointer i2 = btk::Acquisition::New();
    i2->Init(6, 50);
    i2->SetPointFrequency(100);
    
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(0, i1);
    merger->SetInput(1, i2);
    merger->Update();
    btk::Acquisition::Pointer output = merger->GetOutput();
    TS_ASSERT_EQUALS(output->GetPointFrequency(), 100);
    TS_ASSERT_EQUALS(output->GetAnalogFrequency(), 100);
    TS_ASSERT_EQUALS(output->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(output->GetPointNumber(), 6);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 100);
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), 100);
    
    int inc = 1;
    for (btk::Acquisition::PointIterator it = output->BeginPoint() ; it != output->EndPoint() ; ++it)
    {  
      TS_ASSERT_EQUALS((*it)->GetLabel(), "uname*" + btk::ToString(inc));
      ++inc;
    }
  };
  
  CXXTEST_TEST(TwoInputsFromScratch_Merging2)
  {
    btk::Acquisition::Pointer i1 = btk::Acquisition::New();
    i1->Init(6, 50);
    
    btk::Acquisition::Pointer i2 = btk::Acquisition::New();
    i2->Init(6, 50);
    i2->SetPointFrequency(100);
    i2->SetFirstFrame(51);
    
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(0, i1);
    merger->SetInput(1, i2);
    merger->Update();
    btk::Acquisition::Pointer output = merger->GetOutput();
    TS_ASSERT_EQUALS(output->GetPointFrequency(), 100);
    TS_ASSERT_EQUALS(output->GetAnalogFrequency(), 100);
    TS_ASSERT_EQUALS(output->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(output->GetPointNumber(), 6);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 100);
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), 100);
    
    int inc = 1;
    for (btk::Acquisition::PointIterator it = output->BeginPoint() ; it != output->EndPoint() ; ++it)
    {  
      TS_ASSERT_EQUALS((*it)->GetLabel(), "uname*" + btk::ToString(inc));
      ++inc;
    }
  };
  
  CXXTEST_TEST(TwinsFromFile_Concat)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample09/PlugInC3D.c3d");
    btk::Acquisition::Pointer input = reader->GetOutput();
    
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(0, input);
    merger->SetInput(1, input);
    merger->Update();
    btk::Acquisition::Pointer output = merger->GetOutput();
    TS_ASSERT_EQUALS(output->GetPointFrequency(), input->GetPointFrequency());
    TS_ASSERT_EQUALS(output->GetAnalogFrequency(), input->GetAnalogFrequency());
    TS_ASSERT_EQUALS(output->GetPointNumber(), input->GetPointNumber() * 2);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), input->GetAnalogNumber() * 2);
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), input->GetPointFrameNumber());
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), input->GetAnalogFrameNumber());
    TS_ASSERT_EQUALS(output->GetEventNumber(), input->GetEventNumber());
    
    for (int i = 0 ; i < input->GetPointNumber() ; ++i)
    {
      TS_ASSERT_EQUALS(output->GetPoint(i)->GetLabel(), input->GetPoint(i)->GetLabel());
      TS_ASSERT_EQUALS(output->GetPoint(i + input->GetPointNumber())->GetLabel(), input->GetPoint(i)->GetLabel() + "_2");
      TS_ASSERT(output->GetPoint(i)->GetValues().isApprox(output->GetPoint(i + input->GetPointNumber())->GetValues()));
    }
    for (int i = 0 ; i < input->GetAnalogNumber() ; ++i)
    {  
      TS_ASSERT_EQUALS(output->GetAnalog(i)->GetLabel(), input->GetAnalog(i)->GetLabel());
      TS_ASSERT_EQUALS(output->GetAnalog(i + input->GetAnalogNumber())->GetLabel(), input->GetAnalog(i)->GetLabel() + "_2");
      TS_ASSERT(output->GetAnalog(i)->GetValues().isApprox(output->GetAnalog(i + input->GetAnalogNumber())->GetValues()));
    }
    for (int i = 0 ; i < input->GetEventNumber() ; ++i)
      TS_ASSERT(*(output->GetEvent(i)) == *(input->GetEvent(i)));
    
    btk::MetaData::Pointer md = output->GetMetaData();
    btk::MetaData::Pointer fp = md->GetChild("FORCE_PLATFORM");
    int used = fp->GetChild("USED")->GetInfo()->ToInt(0);
    TS_ASSERT_EQUALS(used, 4);
    btk::MetaDataInfo::Pointer corners = fp->GetChild("CORNERS")->GetInfo();
    TS_ASSERT_EQUALS(corners->GetDimensions()[2], used);
    TS_ASSERT_EQUALS(corners->GetValues().size(), 48);
    std::vector<float> corners2Val = input->GetMetaData()->GetChild("FORCE_PLATFORM")->GetChild("CORNERS")->GetInfo()->ToFloat();
    for (int i = 0 ; i < 24 ; ++i)
    {
      TS_ASSERT_DELTA(corners->ToFloat(i), corners2Val.at(i), 1e-5);
      TS_ASSERT_DELTA(corners->ToFloat(i+24), corners2Val.at(i), 1e-5);
    }
    btk::MetaDataInfo::Pointer channel = fp->GetChild("CHANNEL")->GetInfo();
    TS_ASSERT_EQUALS(channel->GetDimensions()[1], used);
    TS_ASSERT_EQUALS(channel->GetValues().size(), 24);
    std::vector<int> channel2Val = input->GetMetaData()->GetChild("FORCE_PLATFORM")->GetChild("CHANNEL")->GetInfo()->ToInt();
    for (int i = 0 ; i < 12 ; ++i)
    {
      TS_ASSERT_EQUALS(channel->ToInt(i), channel2Val.at(i));
      TS_ASSERT_EQUALS(channel->ToInt(i+12), channel2Val.at(i) + 12);
    }
    btk::MetaDataInfo::Pointer type = fp->GetChild("TYPE")->GetInfo();
    TS_ASSERT_EQUALS(type->GetDimensions()[0], used);
    TS_ASSERT_EQUALS(type->GetValues().size(), 4);
    std::vector<int> type2Val = input->GetMetaData()->GetChild("FORCE_PLATFORM")->GetChild("TYPE")->GetInfo()->ToInt();
    for (int i = 0 ; i < 2 ; ++i)
    {
      TS_ASSERT_EQUALS(type->ToInt(i), type2Val.at(i));
      TS_ASSERT_EQUALS(type->ToInt(i+2), type2Val.at(i));
    }
    btk::MetaDataInfo::Pointer origin = fp->GetChild("ORIGIN")->GetInfo();
    TS_ASSERT_EQUALS(origin->GetDimensions()[1], used);
    TS_ASSERT_EQUALS(origin->GetValues().size(), 12);
    std::vector<float> origin2Val = input->GetMetaData()->GetChild("FORCE_PLATFORM")->GetChild("ORIGIN")->GetInfo()->ToFloat();
    for (int i = 0 ; i < 6 ; ++i)
    {
      TS_ASSERT_DELTA(origin->ToFloat(i), origin2Val.at(i), 1e-5);
      TS_ASSERT_DELTA(origin->ToFloat(i+6), origin2Val.at(i), 1e-5);
    }
    
    btk::MetaData::ConstIterator mdPointIt = md->FindChild("POINT");
    TS_ASSERT(mdPointIt != md->End());
    if (mdPointIt != md->End())
    {
      TS_ASSERT_EQUALS((*mdPointIt)->GetChildNumber(), 2);
      TS_ASSERT((*mdPointIt)->FindChild("X_SCREEN") != (*mdPointIt)->End());
      TS_ASSERT((*mdPointIt)->FindChild("Y_SCREEN") != (*mdPointIt)->End());
    }
    TS_ASSERT(md->FindChild("ANALOG") == md->End());
    TS_ASSERT(md->FindChild("EVENT") == md->End());
    TS_ASSERT(md->FindChild("TRIAL") != md->End());
    TS_ASSERT(md->FindChild("SUBJECTS") != md->End());
    TS_ASSERT(md->FindChild("SEG") != md->End());
    TS_ASSERT(md->FindChild("EVENT_CONTEXT") != md->End());
    TS_ASSERT_EQUALS(md->GetChildNumber(), 6);
    
    std::vector<int> start = md->GetChild("TRIAL")->GetChild("ACTUAL_START_FIELD")->GetInfo()->ToInt();
    std::vector<int> stop = md->GetChild("TRIAL")->GetChild("ACTUAL_END_FIELD")->GetInfo()->ToInt();
    TS_ASSERT_EQUALS(output->GetFirstFrame(), start[0] + start[1] * 65535);
    TS_ASSERT_EQUALS(output->GetLastFrame(), stop[0] + stop[1] * 65535);
  };
  
  CXXTEST_TEST(TwinsFromFile_Concat2)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample09/PlugInC3D.c3d");
    reader->Update();
    btk::Acquisition::Pointer i1 = reader->GetOutput();
    btk::Acquisition::Pointer i2 = i1->Clone();
    i2->GetMetaData()->GetChild("SEG")->GetChild("PREDICTION_ERROR")->GetInfo()->SetValue(0, (float)10.0);
    i2->ResizePointNumber(5);
    
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(0, i1);
    merger->SetInput(1, i2);
    merger->Update();
    btk::Acquisition::Pointer output = merger->GetOutput();
    TS_ASSERT_EQUALS(output->GetPointFrequency(), i1->GetPointFrequency());
    TS_ASSERT_EQUALS(output->GetAnalogFrequency(), i1->GetAnalogFrequency());
    TS_ASSERT_EQUALS(output->GetPointNumber(), i1->GetPointNumber() + 5);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), i1->GetAnalogNumber() * 2);
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), i1->GetPointFrameNumber());
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), i1->GetAnalogFrameNumber());
    TS_ASSERT_EQUALS(output->GetEventNumber(), i1 ->GetEventNumber());
    
    btk::MetaData::Pointer md = output->GetMetaData();
    TS_ASSERT(md->FindChild("ANALOG") == md->End());
    TS_ASSERT(md->FindChild("EVENT") == md->End());
    TS_ASSERT(md->FindChild("TRIAL") != md->End());
    TS_ASSERT(md->FindChild("SUBJECTS") != md->End());
    TS_ASSERT(md->FindChild("SEG") != md->End());
    TS_ASSERT(md->FindChild("SEG_2") != md->End());
    TS_ASSERT(md->FindChild("EVENT_CONTEXT") != md->End());
    TS_ASSERT_EQUALS(md->GetChildNumber(), 7);
    
    std::vector<int> start = md->GetChild("TRIAL")->GetChild("ACTUAL_START_FIELD")->GetInfo()->ToInt();
    std::vector<int> stop = md->GetChild("TRIAL")->GetChild("ACTUAL_END_FIELD")->GetInfo()->ToInt();
    TS_ASSERT_EQUALS(output->GetFirstFrame(), start[0] + start[1] * 65535);
    TS_ASSERT_EQUALS(output->GetLastFrame(), stop[0] + stop[1] * 65535);
  };
  
  CXXTEST_TEST(TwinsFromFile_Merge)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample09/PlugInC3D.c3d");
    reader->Update();
    btk::Acquisition::Pointer i1 = reader->GetOutput();
    btk::Acquisition::Pointer i2 = i1->Clone();
    i2->SetFirstFrame(i1->GetLastFrame() + 1);
    
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(0, i1);
    merger->SetInput(1, i2);
    merger->Update();
    btk::Acquisition::Pointer output = merger->GetOutput();
    TS_ASSERT_EQUALS(output->GetPointFrequency(), i1->GetPointFrequency());
    TS_ASSERT_EQUALS(output->GetAnalogFrequency(), i1->GetAnalogFrequency());
    TS_ASSERT_EQUALS(output->GetPointNumber(), i1->GetPointNumber());
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), i1->GetAnalogNumber());
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), i1->GetPointFrameNumber() * 2);
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), i1->GetAnalogFrameNumber() * 2);
    TS_ASSERT_EQUALS(output->GetEventNumber(), i1->GetEventNumber());
    
    for (int i = 0 ; i < i1->GetEventNumber() ; ++i)
      TS_ASSERT(*(output->GetEvent(i)) == *(i1->GetEvent(i)));
    
    btk::MetaData::Pointer md = output->GetMetaData();
    int used = (*md->FindChild("FORCE_PLATFORM"))->ExtractChildInfo("USED", btk::MetaDataInfo::Integer, 0)->ToInt(0);
    TS_ASSERT_EQUALS(used, 2);
    
    std::vector<int> start = md->GetChild("TRIAL")->GetChild("ACTUAL_START_FIELD")->GetInfo()->ToInt();
    std::vector<int> stop = md->GetChild("TRIAL")->GetChild("ACTUAL_END_FIELD")->GetInfo()->ToInt();
    TS_ASSERT_EQUALS(output->GetFirstFrame(), start[0] + start[1] * 65535);
    TS_ASSERT_EQUALS(output->GetLastFrame(), stop[0] + stop[1] * 65535);
  };
  
  CXXTEST_TEST(TwoFiles_Concat_CalMatrix)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample10/TYPE-2.C3D");
    btk::Acquisition::Pointer input = reader->GetOutput();
    input->Update();
    input->GetMetaData()->GetChild("FORCE_PLATFORM")->RemoveChild("CAL_MATRIX");

    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathIN + "sample10/TYPE-4.C3D");
    btk::Acquisition::Pointer input2 = reader2->GetOutput();

    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(0, input);
    merger->SetInput(1, input2);
    merger->Update();
    btk::Acquisition::Pointer output = merger->GetOutput();

    btk::MetaData::Pointer md = output->GetMetaData();
    btk::MetaData::Pointer fp = md->GetChild("FORCE_PLATFORM");
    int used = fp->GetChild("USED")->GetInfo()->ToInt(0);
    TS_ASSERT_EQUALS(used, 2);
    btk::MetaDataInfo::Pointer calMatrix = fp->GetChild("CAL_MATRIX")->GetInfo();
    TS_ASSERT_EQUALS(calMatrix->GetDimensions()[2], used);
    TS_ASSERT_EQUALS(calMatrix->GetValues().size(), 72);
    std::vector<float> calMatrix2Val = input2->GetMetaData()->GetChild("FORCE_PLATFORM")->GetChild("CAL_MATRIX")->GetInfo()->ToFloat();
    for (int i = 0 ; i < 36 ; ++i)
    {
      TS_ASSERT_DELTA(calMatrix->ToFloat(i), 0.0, 1e-5);
      TS_ASSERT_DELTA(calMatrix->ToFloat(i + 36), calMatrix2Val.at(i), 1e-5);
    }
  };
  
  CXXTEST_TEST(TwoFiles_Concat_NoSameFrameNumber)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(TRCFilePathIN + "MOTEK/T.trc");
    btk::Acquisition::Pointer input = reader->GetOutput();
    
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(TRCFilePathIN + "MOTEK/ballet_reduced.trc");
    btk::Acquisition::Pointer input2 = reader2->GetOutput();

    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(0, input);
    merger->SetInput(1, input2);
    merger->Update();
    btk::Acquisition::Pointer output = merger->GetOutput();
    
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 100);
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), 100);
    TS_ASSERT_EQUALS(output->GetPointNumber(), 82);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 0);
  }
  
  CXXTEST_TEST(ThreeFiles_Concat_TRC_and_ANC_and_CAL)
  {
    btk::AcquisitionFileReader::Pointer trcReader = btk::AcquisitionFileReader::New();
    trcReader->SetFilename(TRCFilePathIN + "Gait.trc");
    btk::AcquisitionFileReader::Pointer ancReader = btk::AcquisitionFileReader::New();
    ancReader->SetFilename(ANCFilePathIN + "Gait.anc");
    btk::AcquisitionFileReader::Pointer calReader = btk::AcquisitionFileReader::New();
    calReader->SetFilename(CALForcePlateFilePathIN + "Forcepla.cal");
    
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(0, trcReader->GetOutput());
    merger->SetInput(1, ancReader->GetOutput());
    merger->SetInput(2, calReader->GetOutput());
    merger->Update();
    btk::Acquisition::Pointer output = merger->GetOutput();
    
    TS_ASSERT_EQUALS(output->GetPointFrequency(), 100.0);
    TS_ASSERT_EQUALS(output->GetAnalogFrequency(), 1000.0);
    TS_ASSERT_EQUALS(output->GetPointNumber(), 33);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 28);
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 487);
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), 4870);
    TS_ASSERT_EQUALS(output->GetEventNumber(), 0);
    
    btk::MetaData::Pointer md = output->GetMetaData();
    TS_ASSERT_EQUALS(md->GetChildNumber(), 2); // FORCE_PLATFORM & ANALOG
    btk::MetaData::Pointer fp = md->GetChild("FORCE_PLATFORM");
    int used = fp->GetChild("USED")->GetInfo()->ToInt(0);
    TS_ASSERT_EQUALS(used, 2);
    btk::MetaDataInfo::Pointer calMatrix = fp->GetChild("CAL_MATRIX")->GetInfo();
    TS_ASSERT_EQUALS(calMatrix->GetDimensions()[2], used);
    TS_ASSERT_EQUALS(calMatrix->GetValues().size(), 72);
  };
  
  CXXTEST_TEST(ThreeFiles_Concat_ANC_and_CAL_and_TRC)
  {
    btk::AcquisitionFileReader::Pointer trcReader = btk::AcquisitionFileReader::New();
    trcReader->SetFilename(TRCFilePathIN + "Gait.trc");
    btk::AcquisitionFileReader::Pointer ancReader = btk::AcquisitionFileReader::New();
    ancReader->SetFilename(ANCFilePathIN + "Gait.anc");
    btk::AcquisitionFileReader::Pointer calReader = btk::AcquisitionFileReader::New();
    calReader->SetFilename(CALForcePlateFilePathIN + "Forcepla.cal");
    
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(2, trcReader->GetOutput());
    merger->SetInput(0, ancReader->GetOutput());
    merger->SetInput(1, calReader->GetOutput());
    merger->Update();
    btk::Acquisition::Pointer output = merger->GetOutput();
    
    TS_ASSERT_EQUALS(output->GetPointFrequency(), 100.0);
    TS_ASSERT_EQUALS(output->GetAnalogFrequency(), 1000.0);
    TS_ASSERT_EQUALS(output->GetPointNumber(), 33);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 28);
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 487);
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), 4870);
    TS_ASSERT_EQUALS(output->GetEventNumber(), 0);
    
    btk::MetaData::Pointer md = output->GetMetaData();
    TS_ASSERT_EQUALS(md->GetChildNumber(), 2); // FORCE_PLATFORM & ANALOG
    btk::MetaData::Pointer fp = md->GetChild("FORCE_PLATFORM");
    int used = fp->GetChild("USED")->GetInfo()->ToInt(0);
    TS_ASSERT_EQUALS(used, 2);
    btk::MetaDataInfo::Pointer calMatrix = fp->GetChild("CAL_MATRIX")->GetInfo();
    TS_ASSERT_EQUALS(calMatrix->GetDimensions()[2], used);
    TS_ASSERT_EQUALS(calMatrix->GetValues().size(), 72);
  };
  
  CXXTEST_TEST(ThreeFiles_Concat_CAL_and_TRC_and_ANC)
  {
    btk::AcquisitionFileReader::Pointer trcReader = btk::AcquisitionFileReader::New();
    trcReader->SetFilename(TRCFilePathIN + "Gait.trc");
    btk::AcquisitionFileReader::Pointer ancReader = btk::AcquisitionFileReader::New();
    ancReader->SetFilename(ANCFilePathIN + "Gait.anc");
    btk::AcquisitionFileReader::Pointer calReader = btk::AcquisitionFileReader::New();
    calReader->SetFilename(CALForcePlateFilePathIN + "Forcepla.cal");
    
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(1, trcReader->GetOutput());
    merger->SetInput(2, ancReader->GetOutput());
    merger->SetInput(0, calReader->GetOutput());
    merger->Update();
    btk::Acquisition::Pointer output = merger->GetOutput();
    
    TS_ASSERT_EQUALS(output->GetPointFrequency(), 100.0);
    TS_ASSERT_EQUALS(output->GetAnalogFrequency(), 1000.0);
    TS_ASSERT_EQUALS(output->GetPointNumber(), 33);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 28);
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 487);
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), 4870);
    TS_ASSERT_EQUALS(output->GetEventNumber(), 0);
    
    btk::MetaData::Pointer md = output->GetMetaData();
    TS_ASSERT_EQUALS(md->GetChildNumber(), 2); // FORCE_PLATFORM & ANALOG
    btk::MetaData::Pointer fp = md->GetChild("FORCE_PLATFORM");
    int used = fp->GetChild("USED")->GetInfo()->ToInt(0);
    TS_ASSERT_EQUALS(used, 2);
    btk::MetaDataInfo::Pointer calMatrix = fp->GetChild("CAL_MATRIX")->GetInfo();
    TS_ASSERT_EQUALS(calMatrix->GetDimensions()[2], used);
    TS_ASSERT_EQUALS(calMatrix->GetValues().size(), 72);
  };
  
  CXXTEST_TEST(C3D_vs_ThreeFiles_Concat_TRC_and_ANC_and_CAL)
  {
    // Merge
    btk::AcquisitionFileReader::Pointer trcReader = btk::AcquisitionFileReader::New();
    trcReader->SetFilename(TRCFilePathIN + "Gait.trc");
    btk::AcquisitionFileReader::Pointer ancReader = btk::AcquisitionFileReader::New();
    ancReader->SetFilename(ANCFilePathIN + "Gait.anc");
    btk::AcquisitionFileReader::Pointer calReader = btk::AcquisitionFileReader::New();
    calReader->SetFilename(CALForcePlateFilePathIN + "Forcepla.cal");
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(0, trcReader->GetOutput());
    merger->SetInput(1, ancReader->GetOutput());
    merger->SetInput(2, calReader->GetOutput());
    merger->Update();
    btk::Acquisition::Pointer output = merger->GetOutput();
    // C3D
    btk::AcquisitionFileReader::Pointer c3dReader = btk::AcquisitionFileReader::New();
    c3dReader->SetFilename(C3DFilePathIN + "others/Gait.c3d");
    btk::Acquisition::Pointer input = c3dReader->GetOutput();
    input->Update();
    TS_ASSERT_EQUALS(output->GetPointFrequency(), input->GetPointFrequency());
    TS_ASSERT_EQUALS(output->GetAnalogFrequency(), input->GetAnalogFrequency());
    TS_ASSERT_EQUALS(output->GetPointNumber(), input->GetPointNumber());
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), input->GetAnalogNumber());
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), input->GetPointFrameNumber());
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), input->GetAnalogFrameNumber());
    TS_ASSERT_EQUALS(output->GetEventNumber(), input->GetEventNumber());
    for (int i = 1 ; i < 50 ; ++i)
    {
      for (int j = 0 ; j < output->GetPointNumber() ; ++j)
      {
        TS_ASSERT_DELTA(output->GetPoint(j)->GetValues()(i,0), input->GetPoint(j)->GetValues()(i,0), 1e-4);
        TS_ASSERT_DELTA(output->GetPoint(j)->GetValues()(i,1), input->GetPoint(j)->GetValues()(i,1), 1e-4);
        TS_ASSERT_DELTA(output->GetPoint(j)->GetValues()(i,2), input->GetPoint(j)->GetValues()(i,2), 1e-4);
      }
      TS_ASSERT_DELTA(output->GetAnalog(0)->GetValues()(i), input->GetAnalog(0)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(1)->GetValues()(i), input->GetAnalog(1)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(2)->GetValues()(i), input->GetAnalog(2)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(3)->GetValues()(i), input->GetAnalog(3)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(4)->GetValues()(i), input->GetAnalog(4)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(5)->GetValues()(i), input->GetAnalog(5)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(6)->GetValues()(i), input->GetAnalog(6)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(7)->GetValues()(i), input->GetAnalog(7)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(8)->GetValues()(i), input->GetAnalog(8)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(9)->GetValues()(i), input->GetAnalog(9)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(10)->GetValues()(i), input->GetAnalog(10)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(11)->GetValues()(i), input->GetAnalog(11)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(12)->GetValues()(i), input->GetAnalog(12)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(13)->GetValues()(i), input->GetAnalog(13)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(14)->GetValues()(i), input->GetAnalog(14)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(15)->GetValues()(i), input->GetAnalog(15)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(16)->GetValues()(i), input->GetAnalog(16)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(17)->GetValues()(i), input->GetAnalog(17)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(18)->GetValues()(i), input->GetAnalog(18)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(19)->GetValues()(i), input->GetAnalog(19)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(20)->GetValues()(i), input->GetAnalog(20)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(21)->GetValues()(i), input->GetAnalog(21)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(22)->GetValues()(i), input->GetAnalog(22)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(23)->GetValues()(i), input->GetAnalog(23)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(24)->GetValues()(i), input->GetAnalog(24)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(25)->GetValues()(i), input->GetAnalog(25)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(26)->GetValues()(i), input->GetAnalog(26)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(27)->GetValues()(i), input->GetAnalog(27)->GetValues()(i), 1e-3);
    }
    // GRWs
    btk::ForcePlatformsExtractor::Pointer pfe = btk::ForcePlatformsExtractor::New();
    btk::GroundReactionWrenchFilter::Pointer grwf = btk::GroundReactionWrenchFilter::New();
    grwf->SetThresholdValue(5.0);
    grwf->SetThresholdState(true);
    pfe->SetInput(output);
    grwf->SetInput(pfe->GetOutput());
    grwf->Update();
    btk::WrenchCollection::Pointer grwOutput = grwf->GetOutput()->Clone();
    pfe->SetInput(input);
    grwf->Update();
    btk::WrenchCollection::Pointer grwInput = grwf->GetOutput();
    TS_ASSERT_EQUALS(grwOutput->GetItemNumber(), 2);
    TS_ASSERT_EQUALS(grwInput->GetItemNumber(), 2);

    for (int i = 1 ; i < 50 ; ++i)
    {
      for (int j = 0 ; j < grwOutput->GetItemNumber() ; ++j)
      {
        // Position
        TS_ASSERT_DELTA(grwOutput->GetItem(j)->GetPosition()->GetValues()(i,0), grwInput->GetItem(j)->GetPosition()->GetValues()(i,0), 1e-2);
        TS_ASSERT_DELTA(grwOutput->GetItem(j)->GetPosition()->GetValues()(i,1), grwInput->GetItem(j)->GetPosition()->GetValues()(i,1), 1e-2);
        TS_ASSERT_DELTA(grwOutput->GetItem(j)->GetPosition()->GetValues()(i,2), grwInput->GetItem(j)->GetPosition()->GetValues()(i,2), 1e-2);
        // Force
        TS_ASSERT_DELTA(grwOutput->GetItem(j)->GetForce()->GetValues()(i,0), grwInput->GetItem(j)->GetForce()->GetValues()(i,0), 1e-2);
        TS_ASSERT_DELTA(grwOutput->GetItem(j)->GetForce()->GetValues()(i,1), grwInput->GetItem(j)->GetForce()->GetValues()(i,1), 1e-2);
        TS_ASSERT_DELTA(grwOutput->GetItem(j)->GetForce()->GetValues()(i,2), grwInput->GetItem(j)->GetForce()->GetValues()(i,2), 1e-2);
        // Moment
        TS_ASSERT_DELTA(grwOutput->GetItem(j)->GetMoment()->GetValues()(i,0), grwInput->GetItem(j)->GetMoment()->GetValues()(i,0), 1e-0);
        TS_ASSERT_DELTA(grwOutput->GetItem(j)->GetMoment()->GetValues()(i,1), grwInput->GetItem(j)->GetMoment()->GetValues()(i,1), 1e-0);
        TS_ASSERT_DELTA(grwOutput->GetItem(j)->GetMoment()->GetValues()(i,2), grwInput->GetItem(j)->GetMoment()->GetValues()(i,2), 1e-0);
      }
    }
  };
  
  CXXTEST_TEST(C3D_vs_exportedC3D)
  {
    // Merge
    btk::AcquisitionFileReader::Pointer trcReader = btk::AcquisitionFileReader::New();
    trcReader->SetFilename(TRCFilePathIN + "Gait.trc");
    btk::AcquisitionFileReader::Pointer ancReader = btk::AcquisitionFileReader::New();
    ancReader->SetFilename(ANCFilePathIN + "Gait.anc");
    btk::AcquisitionFileReader::Pointer calReader = btk::AcquisitionFileReader::New();
    calReader->SetFilename(CALForcePlateFilePathIN + "Forcepla.cal");
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(0, trcReader->GetOutput());
    merger->SetInput(1, ancReader->GetOutput());
    merger->SetInput(2, calReader->GetOutput());
    // C3D
    btk::AcquisitionFileReader::Pointer c3dReader = btk::AcquisitionFileReader::New();
    c3dReader->SetFilename(C3DFilePathIN + "others/Gait.c3d");
    btk::Acquisition::Pointer input = c3dReader->GetOutput();
    input->Update();
    // Exported C3D writer
    btk::AcquisitionFileWriter::Pointer c3dWriter = btk::AcquisitionFileWriter::New();
    c3dWriter->SetFilename(C3DFilePathOUT + "others_Gait.c3d");
    c3dWriter->SetInput(merger->GetOutput());
    c3dWriter->Update();
    // Exported C3D reader
    btk::AcquisitionFileReader::Pointer c3dReader2 = btk::AcquisitionFileReader::New();
    c3dReader2->SetFilename(C3DFilePathOUT + "others_Gait.c3d");
    btk::Acquisition::Pointer output = c3dReader2->GetOutput();
    output->Update();
    //
    TS_ASSERT_EQUALS(output->GetPointFrequency(), input->GetPointFrequency());
    TS_ASSERT_EQUALS(output->GetAnalogFrequency(), input->GetAnalogFrequency());
    TS_ASSERT_EQUALS(output->GetPointNumber(), input->GetPointNumber());
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), input->GetAnalogNumber());
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), input->GetPointFrameNumber());
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), input->GetAnalogFrameNumber());
    TS_ASSERT_EQUALS(output->GetEventNumber(), input->GetEventNumber());
    for (int i = 1 ; i < 50 ; ++i)
    {
      for (int j = 0 ; j < output->GetPointNumber() ; ++j)
      {
        TS_ASSERT_DELTA(output->GetPoint(j)->GetValues()(i,0), input->GetPoint(j)->GetValues()(i,0), 1e-4);
        TS_ASSERT_DELTA(output->GetPoint(j)->GetValues()(i,1), input->GetPoint(j)->GetValues()(i,1), 1e-4);
        TS_ASSERT_DELTA(output->GetPoint(j)->GetValues()(i,2), input->GetPoint(j)->GetValues()(i,2), 1e-4);
      }
      TS_ASSERT_DELTA(output->GetAnalog(0)->GetValues()(i), input->GetAnalog(0)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(1)->GetValues()(i), input->GetAnalog(1)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(2)->GetValues()(i), input->GetAnalog(2)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(3)->GetValues()(i), input->GetAnalog(3)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(4)->GetValues()(i), input->GetAnalog(4)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(5)->GetValues()(i), input->GetAnalog(5)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(6)->GetValues()(i), input->GetAnalog(6)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(7)->GetValues()(i), input->GetAnalog(7)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(8)->GetValues()(i), input->GetAnalog(8)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(9)->GetValues()(i), input->GetAnalog(9)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(10)->GetValues()(i), input->GetAnalog(10)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(11)->GetValues()(i), input->GetAnalog(11)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(12)->GetValues()(i), input->GetAnalog(12)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(13)->GetValues()(i), input->GetAnalog(13)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(14)->GetValues()(i), input->GetAnalog(14)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(15)->GetValues()(i), input->GetAnalog(15)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(16)->GetValues()(i), input->GetAnalog(16)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(17)->GetValues()(i), input->GetAnalog(17)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(18)->GetValues()(i), input->GetAnalog(18)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(19)->GetValues()(i), input->GetAnalog(19)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(20)->GetValues()(i), input->GetAnalog(20)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(21)->GetValues()(i), input->GetAnalog(21)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(22)->GetValues()(i), input->GetAnalog(22)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(23)->GetValues()(i), input->GetAnalog(23)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(24)->GetValues()(i), input->GetAnalog(24)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(25)->GetValues()(i), input->GetAnalog(25)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(26)->GetValues()(i), input->GetAnalog(26)->GetValues()(i), 1e-3);
      TS_ASSERT_DELTA(output->GetAnalog(27)->GetValues()(i), input->GetAnalog(27)->GetValues()(i), 1e-3);
    }
    // Analog gain
    TS_ASSERT_EQUALS(output->GetAnalog(0)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(output->GetAnalog(1)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(output->GetAnalog(2)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(output->GetAnalog(3)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(output->GetAnalog(4)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(output->GetAnalog(5)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(output->GetAnalog(6)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(output->GetAnalog(7)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(output->GetAnalog(8)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(output->GetAnalog(9)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(output->GetAnalog(10)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(output->GetAnalog(11)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(output->GetAnalog(12)->GetGain(), btk::Analog::PlusMinus1);
    TS_ASSERT_EQUALS(output->GetAnalog(13)->GetGain(), btk::Analog::PlusMinus1);
    TS_ASSERT_EQUALS(output->GetAnalog(14)->GetGain(), btk::Analog::PlusMinus2Dot5);
    TS_ASSERT_EQUALS(output->GetAnalog(15)->GetGain(), btk::Analog::PlusMinus2Dot5);
    TS_ASSERT_EQUALS(output->GetAnalog(16)->GetGain(), btk::Analog::PlusMinus1);
    TS_ASSERT_EQUALS(output->GetAnalog(17)->GetGain(), btk::Analog::PlusMinus1);
    TS_ASSERT_EQUALS(output->GetAnalog(18)->GetGain(), btk::Analog::PlusMinus2Dot5);
    TS_ASSERT_EQUALS(output->GetAnalog(19)->GetGain(), btk::Analog::PlusMinus2Dot5);
    TS_ASSERT_EQUALS(output->GetAnalog(20)->GetGain(), btk::Analog::PlusMinus2Dot5);
    TS_ASSERT_EQUALS(output->GetAnalog(21)->GetGain(), btk::Analog::PlusMinus2Dot5);
    TS_ASSERT_EQUALS(output->GetAnalog(22)->GetGain(), btk::Analog::PlusMinus2Dot5);
    TS_ASSERT_EQUALS(output->GetAnalog(23)->GetGain(), btk::Analog::PlusMinus2Dot5);
    TS_ASSERT_EQUALS(output->GetAnalog(24)->GetGain(), btk::Analog::PlusMinus2Dot5);
    TS_ASSERT_EQUALS(output->GetAnalog(25)->GetGain(), btk::Analog::PlusMinus2Dot5);
    TS_ASSERT_EQUALS(output->GetAnalog(26)->GetGain(), btk::Analog::PlusMinus2Dot5);
    TS_ASSERT_EQUALS(output->GetAnalog(27)->GetGain(), btk::Analog::PlusMinus2Dot5);
  };
  
  CXXTEST_TEST(FourFiles_Concat_TRC_and_ANC_and_CAL_and_XLS)
  {
    btk::AcquisitionFileReader::Pointer trcReader = btk::AcquisitionFileReader::New();
    trcReader->SetFilename(TRCFilePathIN + "Gait.trc");
    btk::AcquisitionFileReader::Pointer ancReader = btk::AcquisitionFileReader::New();
    ancReader->SetFilename(ANCFilePathIN + "Gait.anc");
    btk::AcquisitionFileReader::Pointer calReader = btk::AcquisitionFileReader::New();
    calReader->SetFilename(CALForcePlateFilePathIN + "Forcepla.cal");
    btk::AcquisitionFileReader::Pointer xlsReader = btk::AcquisitionFileReader::New();
    xlsReader->SetFilename(XLSOrthotrackFilePathIN + "Gait.xls");
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(0, trcReader->GetOutput());
    merger->SetInput(1, ancReader->GetOutput());
    merger->SetInput(2, calReader->GetOutput());
    merger->SetInput(3, xlsReader->GetOutput());
    merger->Update();
    btk::Acquisition::Pointer output = merger->GetOutput();
    
    TS_ASSERT_EQUALS(output->GetPointFrequency(), 100.0);
    TS_ASSERT_EQUALS(output->GetAnalogFrequency(), 1000.0);
    TS_ASSERT_EQUALS(output->GetPointNumber(), 84);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 28);
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 487);

    TS_ASSERT_EQUALS(output->GetEventNumber(), 8);
    TS_ASSERT_EQUALS(output->GetEvent(0)->GetTime(), (70.0 - 1.0) / 100.0);
    TS_ASSERT_EQUALS(output->GetEvent(1)->GetTime(), (166.0 - 1.0) / 100.0);
    TS_ASSERT_EQUALS(output->GetEvent(2)->GetTime(), (19.0 - 1.0) / 100.0);
    TS_ASSERT_EQUALS(output->GetEvent(3)->GetTime(), (119.0 - 1.0) / 100.0);
    TS_ASSERT_EQUALS(output->GetEvent(4)->GetTime(), (32.0 - 1.0) / 100.0);
    TS_ASSERT_EQUALS(output->GetEvent(5)->GetTime(), (129.0 - 1.0) / 100.0);
    TS_ASSERT_EQUALS(output->GetEvent(6)->GetTime(), (78.0 - 1.0) / 100.0);
    TS_ASSERT_EQUALS(output->GetEvent(7)->GetTime(), (177.0 - 1.0) / 100.0);
    
    TS_ASSERT_EQUALS(output->GetPoint(33)->GetLabel(), "R_HIP_ANGLE");
  };
  
  CXXTEST_TEST(Elite_vs_C3D)
  {
    btk::AcquisitionFileReader::Pointer angReader = btk::AcquisitionFileReader::New();
    angReader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.ANG");
    btk::AcquisitionFileReader::Pointer gr1Reader = btk::AcquisitionFileReader::New();
    gr1Reader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.GR1");
    btk::AcquisitionFileReader::Pointer gr2Reader = btk::AcquisitionFileReader::New();
    gr2Reader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.GR2");
    btk::AcquisitionFileReader::Pointer gr3Reader = btk::AcquisitionFileReader::New();
    gr3Reader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.GR3");
    btk::AcquisitionFileReader::Pointer momReader = btk::AcquisitionFileReader::New();
    momReader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.MOM");
    btk::AcquisitionFileReader::Pointer pwrReader = btk::AcquisitionFileReader::New();
    pwrReader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.PWR");
    btk::AcquisitionFileReader::Pointer rawReader = btk::AcquisitionFileReader::New();
    rawReader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.RAW");
    btk::AcquisitionFileReader::Pointer ricReader = btk::AcquisitionFileReader::New();
    ricReader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.RIC");
    
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(0, angReader->GetOutput());
    merger->SetInput(1, gr1Reader->GetOutput());
    merger->SetInput(2, gr2Reader->GetOutput());
    merger->SetInput(3, gr3Reader->GetOutput());
    merger->SetInput(4, momReader->GetOutput());
    merger->SetInput(5, pwrReader->GetOutput());
    merger->SetInput(6, rawReader->GetOutput());
    merger->SetInput(7, ricReader->GetOutput());
    merger->Update();
    btk::Acquisition::Pointer output = merger->GetOutput();
    
    btk::AcquisitionFileReader::Pointer c3dReader = btk::AcquisitionFileReader::New();
    c3dReader->SetFilename(C3DFilePathIN + "others/561xxa01_anonym.C3D");
    btk::Acquisition::Pointer input = c3dReader->GetOutput();
    input->Update();
    
    TS_ASSERT_EQUALS(output->GetPointFrequency(), 100.0);
    TS_ASSERT_EQUALS(output->GetAnalogFrequency(), 100.0);
    TS_ASSERT_EQUALS(output->GetPointNumber(), 58);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 18);
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 361);
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), 361);
    TS_ASSERT_EQUALS(output->GetEventNumber(), 6);
    
    TS_ASSERT_EQUALS(output->GetPoint(24)->GetLabel(), "r should");
    TS_ASSERT_EQUALS(output->GetPoint(25)->GetLabel(), "r asis");
    TS_ASSERT_EQUALS(output->GetPoint(26)->GetLabel(), "r thigh");
    TS_ASSERT_EQUALS(output->GetPoint(27)->GetLabel(), "r bar 1");
    TS_ASSERT_EQUALS(output->GetPoint(28)->GetLabel(), "r knee 1");
    TS_ASSERT_EQUALS(output->GetPoint(29)->GetLabel(), "r knee 2");
    TS_ASSERT_EQUALS(output->GetPoint(30)->GetLabel(), "r bar 2");
    TS_ASSERT_EQUALS(output->GetPoint(31)->GetLabel(), "r mall");
    TS_ASSERT_EQUALS(output->GetPoint(32)->GetLabel(), "r met");
    TS_ASSERT_EQUALS(output->GetPoint(33)->GetLabel(), "l should");
    TS_ASSERT_EQUALS(output->GetPoint(34)->GetLabel(), "l asis");
    TS_ASSERT_EQUALS(output->GetPoint(35)->GetLabel(), "l thigh");
    TS_ASSERT_EQUALS(output->GetPoint(36)->GetLabel(), "l bar 1");
    TS_ASSERT_EQUALS(output->GetPoint(37)->GetLabel(), "l knee 1");
    TS_ASSERT_EQUALS(output->GetPoint(38)->GetLabel(), "l knee 2");
    TS_ASSERT_EQUALS(output->GetPoint(39)->GetLabel(), "l bar 2");
    TS_ASSERT_EQUALS(output->GetPoint(40)->GetLabel(), "l mall");
    TS_ASSERT_EQUALS(output->GetPoint(41)->GetLabel(), "l met");
    TS_ASSERT_EQUALS(output->GetPoint(42)->GetLabel(), "c7");
    TS_ASSERT_EQUALS(output->GetPoint(43)->GetLabel(), "sacrum");
    TS_ASSERT_EQUALS(output->GetPoint(44)->GetLabel(), "r should_2");
    TS_ASSERT_EQUALS(output->GetPoint(45)->GetLabel(), "l should_2");
    TS_ASSERT_EQUALS(output->GetPoint(46)->GetLabel(), "c7_2");
    TS_ASSERT_EQUALS(output->GetPoint(47)->GetLabel(), "sacrum_2");
    TS_ASSERT_EQUALS(output->GetPoint(48)->GetLabel(), "r asis_2");
    TS_ASSERT_EQUALS(output->GetPoint(49)->GetLabel(), "r hip");
    TS_ASSERT_EQUALS(output->GetPoint(50)->GetLabel(), "r knee");
    TS_ASSERT_EQUALS(output->GetPoint(51)->GetLabel(), "r ankle");
    TS_ASSERT_EQUALS(output->GetPoint(52)->GetLabel(), "r met_2");
    TS_ASSERT_EQUALS(output->GetPoint(53)->GetLabel(), "l asis_2");
    TS_ASSERT_EQUALS(output->GetPoint(54)->GetLabel(), "l hip");
    TS_ASSERT_EQUALS(output->GetPoint(55)->GetLabel(), "l knee");
    TS_ASSERT_EQUALS(output->GetPoint(56)->GetLabel(), "l ankle");
    TS_ASSERT_EQUALS(output->GetPoint(57)->GetLabel(), "l met_2");
    
    TS_ASSERT_EQUALS(output->GetAnalog(0)->GetLabel(), "Fx1");
    TS_ASSERT_EQUALS(output->GetAnalog(1)->GetLabel(), "Fy1");
    TS_ASSERT_EQUALS(output->GetAnalog(2)->GetLabel(), "Fz1");
    TS_ASSERT_EQUALS(output->GetAnalog(3)->GetLabel(), "Px1");
    TS_ASSERT_EQUALS(output->GetAnalog(4)->GetLabel(), "Py1");
    TS_ASSERT_EQUALS(output->GetAnalog(5)->GetLabel(), "Mz1");
    TS_ASSERT_EQUALS(output->GetAnalog(6)->GetLabel(), "Fx2");
    TS_ASSERT_EQUALS(output->GetAnalog(7)->GetLabel(), "Fy2");
    TS_ASSERT_EQUALS(output->GetAnalog(8)->GetLabel(), "Fz2");
    TS_ASSERT_EQUALS(output->GetAnalog(9)->GetLabel(), "Px2");
    TS_ASSERT_EQUALS(output->GetAnalog(10)->GetLabel(), "Py2");
    TS_ASSERT_EQUALS(output->GetAnalog(11)->GetLabel(), "Mz2");
    TS_ASSERT_EQUALS(output->GetAnalog(12)->GetLabel(), "Fx3");
    TS_ASSERT_EQUALS(output->GetAnalog(13)->GetLabel(), "Fy3");
    TS_ASSERT_EQUALS(output->GetAnalog(14)->GetLabel(), "Fz3");
    TS_ASSERT_EQUALS(output->GetAnalog(15)->GetLabel(), "Px3");
    TS_ASSERT_EQUALS(output->GetAnalog(16)->GetLabel(), "Py3");
    TS_ASSERT_EQUALS(output->GetAnalog(17)->GetLabel(), "Mz3");
    
    btk::MetaData::Pointer md = output->GetMetaData();
    btk::MetaData::Pointer fp = md->GetChild("FORCE_PLATFORM");
    int used = fp->GetChild("USED")->GetInfo()->ToInt(0);
    TS_ASSERT_EQUALS(used, 3);
    btk::MetaDataInfo::Pointer corners = fp->GetChild("CORNERS")->GetInfo();
    TS_ASSERT_EQUALS(corners->GetDimensions()[2], used);
    TS_ASSERT_EQUALS(corners->GetValues().size(), 36);
    std::vector<float> corners2Val = input->GetMetaData()->GetChild("FORCE_PLATFORM")->GetChild("CORNERS")->GetInfo()->ToFloat();
    for (int i = 0 ; i < 36 ; ++i)
      TS_ASSERT_DELTA(corners->ToFloat(i), corners2Val.at(i), 1e-5);
    btk::MetaDataInfo::Pointer channel = fp->GetChild("CHANNEL")->GetInfo();
    TS_ASSERT_EQUALS(channel->GetDimensions()[1], used);
    TS_ASSERT_EQUALS(channel->GetValues().size(), 18);
    std::vector<int> channel2Val = input->GetMetaData()->GetChild("FORCE_PLATFORM")->GetChild("CHANNEL")->GetInfo()->ToInt();
    for (int i = 0 ; i < 18 ; ++i)
      TS_ASSERT_EQUALS(channel->ToInt(i), channel2Val.at(i));
    btk::MetaDataInfo::Pointer type = fp->GetChild("TYPE")->GetInfo();
    TS_ASSERT_EQUALS(type->GetDimensions()[0], used);
    TS_ASSERT_EQUALS(type->GetValues().size(), 3);
    std::vector<int> type2Val = input->GetMetaData()->GetChild("FORCE_PLATFORM")->GetChild("TYPE")->GetInfo()->ToInt();
    for (int i = 0 ; i < used ; ++i)
      TS_ASSERT_EQUALS(type->ToInt(i), type2Val.at(i));
    btk::MetaDataInfo::Pointer origin = fp->GetChild("ORIGIN")->GetInfo();
    TS_ASSERT_EQUALS(origin->GetDimensions()[1], used);
    TS_ASSERT_EQUALS(origin->GetValues().size(), 9);
    TS_ASSERT_EQUALS(origin->GetDimensions()[1], used);
    TS_ASSERT_EQUALS(origin->GetValues().size(), 9);
    TS_ASSERT_DELTA(origin->ToFloat(0), 300.0, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(1), 200.0, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(2), -27.0, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(3), 749.0, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(4), 228.5, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(5), -40.0, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(6), 749.0, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(7), 228.5, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(8), -40.0, 1e-5);
    
    for (int i = 0 ; i < output->GetPointFrameNumber() ; ++i)  
    {
      for (int j = 24 ; j < 57 ; ++j)
      {
        TS_ASSERT_EQUALS(output->GetPoint(j)->GetValues().coeff(i,0), input->GetPoint(j-24)->GetValues().coeff(i,0));
        TS_ASSERT_EQUALS(output->GetPoint(j)->GetValues().coeff(i,1), input->GetPoint(j-24)->GetValues().coeff(i,1));
        TS_ASSERT_EQUALS(output->GetPoint(j)->GetValues().coeff(i,2), input->GetPoint(j-24)->GetValues().coeff(i,2));
      }   
    }
    for (int i = 48 ; i < 114 ; ++i)
    {
      for (int j = 0 ; j < 6 ; ++j)
        TS_ASSERT_DELTA(output->GetAnalog(j)->GetValues().coeff(i), input->GetAnalog(j)->GetValues().coeff(i), 1e-4);
    }
    
    for (int i = 103 ; i < 283 ; ++i)
    {
      for (int j = 6 ; j < 12 ; ++j)
        TS_ASSERT_DELTA(output->GetAnalog(j)->GetValues().coeff(i), input->GetAnalog(j)->GetValues().coeff(i), 2e-4);
    }
    
    for (int i = 159 ; i < 313 ; ++i)
    {
      for (int j = 12 ; j < 18 ; ++j)
        TS_ASSERT_DELTA(output->GetAnalog(j)->GetValues().coeff(i), input->GetAnalog(j)->GetValues().coeff(i), 2e-4);
    }
  };
  
  CXXTEST_TEST(EliteGrFirst_vs_C3D)
  {
    btk::AcquisitionFileReader::Pointer angReader = btk::AcquisitionFileReader::New();
    angReader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.ANG");
    btk::AcquisitionFileReader::Pointer gr1Reader = btk::AcquisitionFileReader::New();
    gr1Reader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.GR1");
    btk::AcquisitionFileReader::Pointer gr2Reader = btk::AcquisitionFileReader::New();
    gr2Reader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.GR2");
    btk::AcquisitionFileReader::Pointer gr3Reader = btk::AcquisitionFileReader::New();
    gr3Reader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.GR3");
    btk::AcquisitionFileReader::Pointer momReader = btk::AcquisitionFileReader::New();
    momReader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.MOM");
    btk::AcquisitionFileReader::Pointer pwrReader = btk::AcquisitionFileReader::New();
    pwrReader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.PWR");
    btk::AcquisitionFileReader::Pointer rawReader = btk::AcquisitionFileReader::New();
    rawReader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.RAW");
    btk::AcquisitionFileReader::Pointer ricReader = btk::AcquisitionFileReader::New();
    ricReader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.RIC");
    
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(0, gr1Reader->GetOutput());
    merger->SetInput(1, gr2Reader->GetOutput());
    merger->SetInput(2, angReader->GetOutput());
    merger->SetInput(3, gr3Reader->GetOutput());
    merger->SetInput(4, momReader->GetOutput());
    merger->SetInput(5, pwrReader->GetOutput());
    merger->SetInput(6, rawReader->GetOutput());
    merger->SetInput(7, ricReader->GetOutput());
    merger->Update();
    btk::Acquisition::Pointer output = merger->GetOutput();
    
    btk::AcquisitionFileReader::Pointer c3dReader = btk::AcquisitionFileReader::New();
    c3dReader->SetFilename(C3DFilePathIN + "others/561xxa01_anonym.C3D");
    btk::Acquisition::Pointer input = c3dReader->GetOutput();
    input->Update();
    
    TS_ASSERT_EQUALS(output->GetPointFrequency(), 100.0);
    TS_ASSERT_EQUALS(output->GetAnalogFrequency(), 100.0);
    TS_ASSERT_EQUALS(output->GetPointNumber(), 58);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 18);
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 361);
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), 361);
    TS_ASSERT_EQUALS(output->GetEventNumber(), 6);
    
    TS_ASSERT_EQUALS(output->GetPoint(24)->GetLabel(), "r should");
    TS_ASSERT_EQUALS(output->GetPoint(25)->GetLabel(), "r asis");
    TS_ASSERT_EQUALS(output->GetPoint(26)->GetLabel(), "r thigh");
    TS_ASSERT_EQUALS(output->GetPoint(27)->GetLabel(), "r bar 1");
    TS_ASSERT_EQUALS(output->GetPoint(28)->GetLabel(), "r knee 1");
    TS_ASSERT_EQUALS(output->GetPoint(29)->GetLabel(), "r knee 2");
    TS_ASSERT_EQUALS(output->GetPoint(30)->GetLabel(), "r bar 2");
    TS_ASSERT_EQUALS(output->GetPoint(31)->GetLabel(), "r mall");
    TS_ASSERT_EQUALS(output->GetPoint(32)->GetLabel(), "r met");
    TS_ASSERT_EQUALS(output->GetPoint(33)->GetLabel(), "l should");
    TS_ASSERT_EQUALS(output->GetPoint(34)->GetLabel(), "l asis");
    TS_ASSERT_EQUALS(output->GetPoint(35)->GetLabel(), "l thigh");
    TS_ASSERT_EQUALS(output->GetPoint(36)->GetLabel(), "l bar 1");
    TS_ASSERT_EQUALS(output->GetPoint(37)->GetLabel(), "l knee 1");
    TS_ASSERT_EQUALS(output->GetPoint(38)->GetLabel(), "l knee 2");
    TS_ASSERT_EQUALS(output->GetPoint(39)->GetLabel(), "l bar 2");
    TS_ASSERT_EQUALS(output->GetPoint(40)->GetLabel(), "l mall");
    TS_ASSERT_EQUALS(output->GetPoint(41)->GetLabel(), "l met");
    TS_ASSERT_EQUALS(output->GetPoint(42)->GetLabel(), "c7");
    TS_ASSERT_EQUALS(output->GetPoint(43)->GetLabel(), "sacrum");
    TS_ASSERT_EQUALS(output->GetPoint(44)->GetLabel(), "r should_2");
    TS_ASSERT_EQUALS(output->GetPoint(45)->GetLabel(), "l should_2");
    TS_ASSERT_EQUALS(output->GetPoint(46)->GetLabel(), "c7_2");
    TS_ASSERT_EQUALS(output->GetPoint(47)->GetLabel(), "sacrum_2");
    TS_ASSERT_EQUALS(output->GetPoint(48)->GetLabel(), "r asis_2");
    TS_ASSERT_EQUALS(output->GetPoint(49)->GetLabel(), "r hip");
    TS_ASSERT_EQUALS(output->GetPoint(50)->GetLabel(), "r knee");
    TS_ASSERT_EQUALS(output->GetPoint(51)->GetLabel(), "r ankle");
    TS_ASSERT_EQUALS(output->GetPoint(52)->GetLabel(), "r met_2");
    TS_ASSERT_EQUALS(output->GetPoint(53)->GetLabel(), "l asis_2");
    TS_ASSERT_EQUALS(output->GetPoint(54)->GetLabel(), "l hip");
    TS_ASSERT_EQUALS(output->GetPoint(55)->GetLabel(), "l knee");
    TS_ASSERT_EQUALS(output->GetPoint(56)->GetLabel(), "l ankle");
    TS_ASSERT_EQUALS(output->GetPoint(57)->GetLabel(), "l met_2");
    
    TS_ASSERT_EQUALS(output->GetAnalog(0)->GetLabel(), "Fx1");
    TS_ASSERT_EQUALS(output->GetAnalog(1)->GetLabel(), "Fy1");
    TS_ASSERT_EQUALS(output->GetAnalog(2)->GetLabel(), "Fz1");
    TS_ASSERT_EQUALS(output->GetAnalog(3)->GetLabel(), "Px1");
    TS_ASSERT_EQUALS(output->GetAnalog(4)->GetLabel(), "Py1");
    TS_ASSERT_EQUALS(output->GetAnalog(5)->GetLabel(), "Mz1");
    TS_ASSERT_EQUALS(output->GetAnalog(6)->GetLabel(), "Fx2");
    TS_ASSERT_EQUALS(output->GetAnalog(7)->GetLabel(), "Fy2");
    TS_ASSERT_EQUALS(output->GetAnalog(8)->GetLabel(), "Fz2");
    TS_ASSERT_EQUALS(output->GetAnalog(9)->GetLabel(), "Px2");
    TS_ASSERT_EQUALS(output->GetAnalog(10)->GetLabel(), "Py2");
    TS_ASSERT_EQUALS(output->GetAnalog(11)->GetLabel(), "Mz2");
    TS_ASSERT_EQUALS(output->GetAnalog(12)->GetLabel(), "Fx3");
    TS_ASSERT_EQUALS(output->GetAnalog(13)->GetLabel(), "Fy3");
    TS_ASSERT_EQUALS(output->GetAnalog(14)->GetLabel(), "Fz3");
    TS_ASSERT_EQUALS(output->GetAnalog(15)->GetLabel(), "Px3");
    TS_ASSERT_EQUALS(output->GetAnalog(16)->GetLabel(), "Py3");
    TS_ASSERT_EQUALS(output->GetAnalog(17)->GetLabel(), "Mz3");
    
    btk::MetaData::Pointer md = output->GetMetaData();
    btk::MetaData::Pointer fp = md->GetChild("FORCE_PLATFORM");
    int used = fp->GetChild("USED")->GetInfo()->ToInt(0);
    TS_ASSERT_EQUALS(used, 3);
    btk::MetaDataInfo::Pointer corners = fp->GetChild("CORNERS")->GetInfo();
    TS_ASSERT_EQUALS(corners->GetDimensions()[2], used);
    TS_ASSERT_EQUALS(corners->GetValues().size(), 36);
    std::vector<float> corners2Val = input->GetMetaData()->GetChild("FORCE_PLATFORM")->GetChild("CORNERS")->GetInfo()->ToFloat();
    for (int i = 0 ; i < 36 ; ++i)
      TS_ASSERT_DELTA(corners->ToFloat(i), corners2Val.at(i), 1e-5);
    btk::MetaDataInfo::Pointer channel = fp->GetChild("CHANNEL")->GetInfo();
    TS_ASSERT_EQUALS(channel->GetDimensions()[1], used);
    TS_ASSERT_EQUALS(channel->GetValues().size(), 18);
    std::vector<int> channel2Val = input->GetMetaData()->GetChild("FORCE_PLATFORM")->GetChild("CHANNEL")->GetInfo()->ToInt();
    for (int i = 0 ; i < 18 ; ++i)
      TS_ASSERT_EQUALS(channel->ToInt(i), channel2Val.at(i));
    btk::MetaDataInfo::Pointer type = fp->GetChild("TYPE")->GetInfo();
    TS_ASSERT_EQUALS(type->GetDimensions()[0], used);
    TS_ASSERT_EQUALS(type->GetValues().size(), 3);
    std::vector<int> type2Val = input->GetMetaData()->GetChild("FORCE_PLATFORM")->GetChild("TYPE")->GetInfo()->ToInt();
    for (int i = 0 ; i < used ; ++i)
      TS_ASSERT_EQUALS(type->ToInt(i), type2Val.at(i));
    btk::MetaDataInfo::Pointer origin = fp->GetChild("ORIGIN")->GetInfo();
    TS_ASSERT_EQUALS(origin->GetDimensions()[1], used);
    TS_ASSERT_EQUALS(origin->GetValues().size(), 9);
    TS_ASSERT_EQUALS(origin->GetDimensions()[1], used);
    TS_ASSERT_EQUALS(origin->GetValues().size(), 9);
    TS_ASSERT_DELTA(origin->ToFloat(0), 300.0, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(1), 200.0, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(2), -27.0, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(3), 749.0, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(4), 228.5, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(5), -40.0, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(6), 749.0, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(7), 228.5, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(8), -40.0, 1e-5);
    
    for (int i = 0 ; i < output->GetPointFrameNumber() ; ++i)  
    {
      for (int j = 24 ; j < 57 ; ++j)
      {
        TS_ASSERT_EQUALS(output->GetPoint(j)->GetValues().coeff(i,0), input->GetPoint(j-24)->GetValues().coeff(i,0));
        TS_ASSERT_EQUALS(output->GetPoint(j)->GetValues().coeff(i,1), input->GetPoint(j-24)->GetValues().coeff(i,1));
        TS_ASSERT_EQUALS(output->GetPoint(j)->GetValues().coeff(i,2), input->GetPoint(j-24)->GetValues().coeff(i,2));
      }   
    }
    for (int i = 48 ; i < 114 ; ++i)
    {
      for (int j = 0 ; j < 6 ; ++j)
        TS_ASSERT_DELTA(output->GetAnalog(j)->GetValues().coeff(i), input->GetAnalog(j)->GetValues().coeff(i), 1e-4);
    }
    
    for (int i = 103 ; i < 283 ; ++i)
    {
      for (int j = 6 ; j < 12 ; ++j)
        TS_ASSERT_DELTA(output->GetAnalog(j)->GetValues().coeff(i), input->GetAnalog(j)->GetValues().coeff(i), 2e-4);
    }
    
    for (int i = 159 ; i < 313 ; ++i)
    {
      for (int j = 12 ; j < 18 ; ++j)
        TS_ASSERT_DELTA(output->GetAnalog(j)->GetValues().coeff(i), input->GetAnalog(j)->GetValues().coeff(i), 2e-4);
    }
  };
  
  CXXTEST_TEST(ExportedC3DElite_vs_C3D)
  {
    btk::AcquisitionFileReader::Pointer angReader = btk::AcquisitionFileReader::New();
    angReader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.ANG");
    btk::AcquisitionFileReader::Pointer gr1Reader = btk::AcquisitionFileReader::New();
    gr1Reader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.GR1");
    btk::AcquisitionFileReader::Pointer gr2Reader = btk::AcquisitionFileReader::New();
    gr2Reader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.GR2");
    btk::AcquisitionFileReader::Pointer gr3Reader = btk::AcquisitionFileReader::New();
    gr3Reader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.GR3");
    btk::AcquisitionFileReader::Pointer momReader = btk::AcquisitionFileReader::New();
    momReader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.MOM");
    btk::AcquisitionFileReader::Pointer pwrReader = btk::AcquisitionFileReader::New();
    pwrReader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.PWR");
    btk::AcquisitionFileReader::Pointer rawReader = btk::AcquisitionFileReader::New();
    rawReader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.RAW");
    btk::AcquisitionFileReader::Pointer ricReader = btk::AcquisitionFileReader::New();
    ricReader->SetFilename(EliteFilePathIN + "561XXA01/561XXA01.RIC");
    
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(0, angReader->GetOutput());
    merger->SetInput(1, gr1Reader->GetOutput());
    merger->SetInput(2, gr2Reader->GetOutput());
    merger->SetInput(3, gr3Reader->GetOutput());
    merger->SetInput(4, momReader->GetOutput());
    merger->SetInput(5, pwrReader->GetOutput());
    merger->SetInput(6, rawReader->GetOutput());
    merger->SetInput(7, ricReader->GetOutput());
    merger->Update();
    
    btk::AcquisitionFileWriter::Pointer c3dWriter = btk::AcquisitionFileWriter::New();
    c3dWriter->SetFilename(C3DFilePathOUT + "561xxa01_exported.C3D");
    c3dWriter->SetInput(merger->GetOutput());
    c3dWriter->Update();
    
    btk::AcquisitionFileReader::Pointer c3dReader = btk::AcquisitionFileReader::New();
    c3dReader->SetFilename(C3DFilePathIN + "others/561xxa01_anonym.C3D");
    btk::Acquisition::Pointer input = c3dReader->GetOutput();
    input->Update();
    
    btk::AcquisitionFileReader::Pointer c3dReader2 = btk::AcquisitionFileReader::New();
    c3dReader2->SetFilename(C3DFilePathOUT + "561xxa01_exported.C3D");
    btk::Acquisition::Pointer output = c3dReader2->GetOutput();
    output->Update();
    
    TS_ASSERT_EQUALS(output->GetPointFrequency(), 100.0);
    TS_ASSERT_EQUALS(output->GetAnalogFrequency(), 100.0);
    TS_ASSERT_EQUALS(output->GetPointNumber(), 58);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 18);
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 361);
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), 361);
    TS_ASSERT_EQUALS(output->GetEventNumber(), 6);
    
    TS_ASSERT_EQUALS(output->GetPoint(24)->GetLabel(), "r should");
    TS_ASSERT_EQUALS(output->GetPoint(25)->GetLabel(), "r asis");
    TS_ASSERT_EQUALS(output->GetPoint(26)->GetLabel(), "r thigh");
    TS_ASSERT_EQUALS(output->GetPoint(27)->GetLabel(), "r bar 1");
    TS_ASSERT_EQUALS(output->GetPoint(28)->GetLabel(), "r knee 1");
    TS_ASSERT_EQUALS(output->GetPoint(29)->GetLabel(), "r knee 2");
    TS_ASSERT_EQUALS(output->GetPoint(30)->GetLabel(), "r bar 2");
    TS_ASSERT_EQUALS(output->GetPoint(31)->GetLabel(), "r mall");
    TS_ASSERT_EQUALS(output->GetPoint(32)->GetLabel(), "r met");
    TS_ASSERT_EQUALS(output->GetPoint(33)->GetLabel(), "l should");
    TS_ASSERT_EQUALS(output->GetPoint(34)->GetLabel(), "l asis");
    TS_ASSERT_EQUALS(output->GetPoint(35)->GetLabel(), "l thigh");
    TS_ASSERT_EQUALS(output->GetPoint(36)->GetLabel(), "l bar 1");
    TS_ASSERT_EQUALS(output->GetPoint(37)->GetLabel(), "l knee 1");
    TS_ASSERT_EQUALS(output->GetPoint(38)->GetLabel(), "l knee 2");
    TS_ASSERT_EQUALS(output->GetPoint(39)->GetLabel(), "l bar 2");
    TS_ASSERT_EQUALS(output->GetPoint(40)->GetLabel(), "l mall");
    TS_ASSERT_EQUALS(output->GetPoint(41)->GetLabel(), "l met");
    TS_ASSERT_EQUALS(output->GetPoint(42)->GetLabel(), "c7");
    TS_ASSERT_EQUALS(output->GetPoint(43)->GetLabel(), "sacrum");
    TS_ASSERT_EQUALS(output->GetPoint(44)->GetLabel(), "r should_2");
    TS_ASSERT_EQUALS(output->GetPoint(45)->GetLabel(), "l should_2");
    TS_ASSERT_EQUALS(output->GetPoint(46)->GetLabel(), "c7_2");
    TS_ASSERT_EQUALS(output->GetPoint(47)->GetLabel(), "sacrum_2");
    TS_ASSERT_EQUALS(output->GetPoint(48)->GetLabel(), "r asis_2");
    TS_ASSERT_EQUALS(output->GetPoint(49)->GetLabel(), "r hip");
    TS_ASSERT_EQUALS(output->GetPoint(50)->GetLabel(), "r knee");
    TS_ASSERT_EQUALS(output->GetPoint(51)->GetLabel(), "r ankle");
    TS_ASSERT_EQUALS(output->GetPoint(52)->GetLabel(), "r met_2");
    TS_ASSERT_EQUALS(output->GetPoint(53)->GetLabel(), "l asis_2");
    TS_ASSERT_EQUALS(output->GetPoint(54)->GetLabel(), "l hip");
    TS_ASSERT_EQUALS(output->GetPoint(55)->GetLabel(), "l knee");
    TS_ASSERT_EQUALS(output->GetPoint(56)->GetLabel(), "l ankle");
    TS_ASSERT_EQUALS(output->GetPoint(57)->GetLabel(), "l met_2");
    
    TS_ASSERT_EQUALS(output->GetAnalog(0)->GetLabel(), "Fx1");
    TS_ASSERT_EQUALS(output->GetAnalog(1)->GetLabel(), "Fy1");
    TS_ASSERT_EQUALS(output->GetAnalog(2)->GetLabel(), "Fz1");
    TS_ASSERT_EQUALS(output->GetAnalog(3)->GetLabel(), "Px1");
    TS_ASSERT_EQUALS(output->GetAnalog(4)->GetLabel(), "Py1");
    TS_ASSERT_EQUALS(output->GetAnalog(5)->GetLabel(), "Mz1");
    TS_ASSERT_EQUALS(output->GetAnalog(6)->GetLabel(), "Fx2");
    TS_ASSERT_EQUALS(output->GetAnalog(7)->GetLabel(), "Fy2");
    TS_ASSERT_EQUALS(output->GetAnalog(8)->GetLabel(), "Fz2");
    TS_ASSERT_EQUALS(output->GetAnalog(9)->GetLabel(), "Px2");
    TS_ASSERT_EQUALS(output->GetAnalog(10)->GetLabel(), "Py2");
    TS_ASSERT_EQUALS(output->GetAnalog(11)->GetLabel(), "Mz2");
    TS_ASSERT_EQUALS(output->GetAnalog(12)->GetLabel(), "Fx3");
    TS_ASSERT_EQUALS(output->GetAnalog(13)->GetLabel(), "Fy3");
    TS_ASSERT_EQUALS(output->GetAnalog(14)->GetLabel(), "Fz3");
    TS_ASSERT_EQUALS(output->GetAnalog(15)->GetLabel(), "Px3");
    TS_ASSERT_EQUALS(output->GetAnalog(16)->GetLabel(), "Py3");
    TS_ASSERT_EQUALS(output->GetAnalog(17)->GetLabel(), "Mz3");
    
    btk::MetaData::Pointer md = output->GetMetaData();
    btk::MetaData::Pointer fp = md->GetChild("FORCE_PLATFORM");
    int used = fp->GetChild("USED")->GetInfo()->ToInt(0);
    TS_ASSERT_EQUALS(used, 3);
    btk::MetaDataInfo::Pointer corners = fp->GetChild("CORNERS")->GetInfo();
    TS_ASSERT_EQUALS(corners->GetDimensions()[2], used);
    TS_ASSERT_EQUALS(corners->GetValues().size(), 36);
    std::vector<float> corners2Val = input->GetMetaData()->GetChild("FORCE_PLATFORM")->GetChild("CORNERS")->GetInfo()->ToFloat();
    for (int i = 0 ; i < 36 ; ++i)
      TS_ASSERT_DELTA(corners->ToFloat(i), corners2Val.at(i), 1e-5);
    btk::MetaDataInfo::Pointer channel = fp->GetChild("CHANNEL")->GetInfo();
    TS_ASSERT_EQUALS(channel->GetDimensions()[1], used);
    TS_ASSERT_EQUALS(channel->GetValues().size(), 18);
    std::vector<int> channel2Val = input->GetMetaData()->GetChild("FORCE_PLATFORM")->GetChild("CHANNEL")->GetInfo()->ToInt();
    for (int i = 0 ; i < 18 ; ++i)
      TS_ASSERT_EQUALS(channel->ToInt(i), channel2Val.at(i));
    btk::MetaDataInfo::Pointer type = fp->GetChild("TYPE")->GetInfo();
    TS_ASSERT_EQUALS(type->GetDimensions()[0], used);
    TS_ASSERT_EQUALS(type->GetValues().size(), 3);
    std::vector<int> type2Val = input->GetMetaData()->GetChild("FORCE_PLATFORM")->GetChild("TYPE")->GetInfo()->ToInt();
    for (int i = 0 ; i < used ; ++i)
      TS_ASSERT_EQUALS(type->ToInt(i), type2Val.at(i));
    btk::MetaDataInfo::Pointer origin = fp->GetChild("ORIGIN")->GetInfo();
    TS_ASSERT_EQUALS(origin->GetDimensions()[1], used);
    TS_ASSERT_EQUALS(origin->GetValues().size(), 9);
    TS_ASSERT_EQUALS(origin->GetDimensions()[1], used);
    TS_ASSERT_EQUALS(origin->GetValues().size(), 9);
    TS_ASSERT_DELTA(origin->ToFloat(0), 300.0, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(1), 200.0, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(2), -27.0, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(3), 749.0, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(4), 228.5, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(5), -40.0, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(6), 749.0, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(7), 228.5, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(8), -40.0, 1e-5);
    
    for (int i = 0 ; i < output->GetPointFrameNumber() ; ++i)  
    {
      for (int j = 24 ; j < 57 ; ++j)
      {
        TS_ASSERT_EQUALS(output->GetPoint(j)->GetValues().coeff(i,0), input->GetPoint(j-24)->GetValues().coeff(i,0));
        TS_ASSERT_EQUALS(output->GetPoint(j)->GetValues().coeff(i,1), input->GetPoint(j-24)->GetValues().coeff(i,1));
        TS_ASSERT_EQUALS(output->GetPoint(j)->GetValues().coeff(i,2), input->GetPoint(j-24)->GetValues().coeff(i,2));
      }   
    }
    for (int i = 48 ; i < 114 ; ++i)
    {
      for (int j = 0 ; j < 6 ; ++j)
        TS_ASSERT_DELTA(output->GetAnalog(j)->GetValues().coeff(i), input->GetAnalog(j)->GetValues().coeff(i), 1e-4);
    }
    
    for (int i = 103 ; i < 283 ; ++i)
    {
      for (int j = 6 ; j < 12 ; ++j)
        TS_ASSERT_DELTA(output->GetAnalog(j)->GetValues().coeff(i), input->GetAnalog(j)->GetValues().coeff(i), 2e-4);
    }
    
    for (int i = 159 ; i < 313 ; ++i)
    {
      for (int j = 12 ; j < 18 ; ++j)
        TS_ASSERT_DELTA(output->GetAnalog(j)->GetValues().coeff(i), input->GetAnalog(j)->GetValues().coeff(i), 2e-4);
    }
  };
  
  CXXTEST_TEST(Elite200)
  {
    btk::AcquisitionFileReader::Pointer rawReader = btk::AcquisitionFileReader::New();
    rawReader->SetFilename(EliteFilePathIN + "1216XB01/1216XB01.RAW");
    btk::AcquisitionFileReader::Pointer gr1Reader = btk::AcquisitionFileReader::New();
    gr1Reader->SetFilename(EliteFilePathIN + "1216XB01/1216XB01.GR1");
    btk::AcquisitionFileReader::Pointer gr2Reader = btk::AcquisitionFileReader::New();
    gr2Reader->SetFilename(EliteFilePathIN + "1216XB01/1216XB01.GR2");
    
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(0, rawReader->GetOutput());
    merger->SetInput(1, gr1Reader->GetOutput());
    merger->SetInput(2, gr2Reader->GetOutput());
    
    btk::Acquisition::Pointer output = merger->GetOutput();
    output->Update();
    
    TS_ASSERT_EQUALS(output->GetPointFrequency(), 100.0);
    TS_ASSERT_EQUALS(output->GetAnalogFrequency(), 200.0);
    TS_ASSERT_EQUALS(output->GetPointNumber(), 3);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 12);
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 657);
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), 657 * 2);
    TS_ASSERT_EQUALS(output->GetEventNumber(), 0);
    
    TS_ASSERT_DELTA(output->GetAnalog(1)->GetValues()(562), 0.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(0)->GetValues()(562), 0.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(2)->GetValues()(562), 0.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(4)->GetValues()(562), 206.11985 - 200.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(3)->GetValues()(562),-192.98107 + 300.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(5)->GetValues()(562), 0.0, 1e-5);
    
    TS_ASSERT_DELTA(output->GetAnalog(7)->GetValues()(719), 0.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(6)->GetValues()(719), 0.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(8)->GetValues()(719), 0.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(10)->GetValues()(719), 129.23571 - 200.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(9)->GetValues()(719),-305.97125 + 300.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(11)->GetValues()(719), 0.0, 1e-5);
    
    TS_ASSERT_EQUALS(output->GetPoint(0)->GetLabel(), "M1");
    TS_ASSERT_EQUALS(output->GetPoint(1)->GetLabel(), "M2");
    TS_ASSERT_EQUALS(output->GetPoint(2)->GetLabel(), "M3");
  };
  
  CXXTEST_TEST(Elite2000)
  {
    btk::AcquisitionFileReader::Pointer rawReader = btk::AcquisitionFileReader::New();
    rawReader->SetFilename(EliteFilePathIN + "1216XE01/1216XE01.RAW");
    btk::AcquisitionFileReader::Pointer gr1Reader = btk::AcquisitionFileReader::New();
    gr1Reader->SetFilename(EliteFilePathIN + "1216XE01/1216XE01.GR1");
    btk::AcquisitionFileReader::Pointer gr2Reader = btk::AcquisitionFileReader::New();
    gr2Reader->SetFilename(EliteFilePathIN + "1216XE01/1216XE01.GR2");
    
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(0, rawReader->GetOutput());
    merger->SetInput(1, gr1Reader->GetOutput());
    merger->SetInput(2, gr2Reader->GetOutput());
    
    btk::Acquisition::Pointer output = merger->GetOutput();
    output->Update();
    
    TS_ASSERT_EQUALS(output->GetPointFrequency(), 100.0);
    TS_ASSERT_EQUALS(output->GetAnalogFrequency(), 2000.0);
    TS_ASSERT_EQUALS(output->GetPointNumber(), 3);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 12);
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 554);
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), 554 * 20);
    TS_ASSERT_EQUALS(output->GetEventNumber(), 0);
    
    TS_ASSERT_DELTA(output->GetAnalog(1)->GetValues()(2322*2), 0.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(0)->GetValues()(2322*2), 0.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(2)->GetValues()(2322*2), 0.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(4)->GetValues()(2322*2), 225.43112 - 200.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(3)->GetValues()(2322*2),-142.27230 + 300.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(5)->GetValues()(2322*2), 0.0, 1e-5);
    
    TS_ASSERT_DELTA(output->GetAnalog(7)->GetValues()(2983*2), 0.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(6)->GetValues()(2983*2), 0.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(8)->GetValues()(2983*2), 0.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(10)->GetValues()(2983*2), 62.26385 - 200.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(9)->GetValues()(2983*2),-344.44421 + 300.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(11)->GetValues()(2983*2), 0.0, 1e-5);
    
    TS_ASSERT_EQUALS(output->GetPoint(0)->GetLabel(), "M1");
    TS_ASSERT_EQUALS(output->GetPoint(1)->GetLabel(), "M2");
    TS_ASSERT_EQUALS(output->GetPoint(2)->GetLabel(), "M3");
  };
  
  CXXTEST_TEST(Elite2000Reversed)
  {
    btk::AcquisitionFileReader::Pointer rawReader = btk::AcquisitionFileReader::New();
    rawReader->SetFilename(EliteFilePathIN + "1216XE01/1216XE01.RAW");
    btk::AcquisitionFileReader::Pointer gr1Reader = btk::AcquisitionFileReader::New();
    gr1Reader->SetFilename(EliteFilePathIN + "1216XE01/1216XE01.GR1");
    btk::AcquisitionFileReader::Pointer gr2Reader = btk::AcquisitionFileReader::New();
    gr2Reader->SetFilename(EliteFilePathIN + "1216XE01/1216XE01.GR2");
    
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(2, rawReader->GetOutput());
    merger->SetInput(0, gr1Reader->GetOutput());
    merger->SetInput(1, gr2Reader->GetOutput());
    
    btk::Acquisition::Pointer output = merger->GetOutput();
    output->Update();
    
    TS_ASSERT_EQUALS(output->GetPointFrequency(), 100.0);
    TS_ASSERT_EQUALS(output->GetAnalogFrequency(), 2000.0);
    TS_ASSERT_EQUALS(output->GetPointNumber(), 3);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 12);
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 554);
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), 554 * 20);
    TS_ASSERT_EQUALS(output->GetEventNumber(), 0);
    
    TS_ASSERT_DELTA(output->GetAnalog(1)->GetValues()(2322*2), 0.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(0)->GetValues()(2322*2), 0.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(2)->GetValues()(2322*2), 0.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(4)->GetValues()(2322*2), 225.43112 - 200.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(3)->GetValues()(2322*2),-142.27230 + 300.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(5)->GetValues()(2322*2), 0.0, 1e-5);
    
    TS_ASSERT_DELTA(output->GetAnalog(7)->GetValues()(2983*2), 0.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(6)->GetValues()(2983*2), 0.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(8)->GetValues()(2983*2), 0.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(10)->GetValues()(2983*2), 62.26385 - 200.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(9)->GetValues()(2983*2),-344.44421 + 300.0, 1e-5);
    TS_ASSERT_DELTA(output->GetAnalog(11)->GetValues()(2983*2), 0.0, 1e-5);
    
    TS_ASSERT_EQUALS(output->GetPoint(0)->GetLabel(), "M1");
    TS_ASSERT_EQUALS(output->GetPoint(1)->GetLabel(), "M2");
    TS_ASSERT_EQUALS(output->GetPoint(2)->GetLabel(), "M3");
  };
  
  CXXTEST_TEST(DifferentAnalogFrequencies)
  {
    btk::AcquisitionFileReader::Pointer emgReader = btk::AcquisitionFileReader::New();
    emgReader->SetFilename(EliteFilePathIN + "1123xa01/1123xa01.EMG");
    btk::AcquisitionFileReader::Pointer gr1Reader = btk::AcquisitionFileReader::New();
    gr1Reader->SetFilename(EliteFilePathIN + "1123xa01/1123xa01.GR1");
    
    btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
    merger->SetInput(2, emgReader->GetOutput());
    merger->SetInput(0, gr1Reader->GetOutput());
    
    btk::Acquisition::Pointer output = merger->GetOutput();
    output->Update();
    
    TS_ASSERT_EQUALS(output->GetPointFrequency(), 100.0);
    TS_ASSERT_EQUALS(output->GetAnalogFrequency(), 100.0);
    TS_ASSERT_EQUALS(output->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 6);
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 46);
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(), 46);
    TS_ASSERT_EQUALS(output->GetEventNumber(), 0);
  };
};

CXXTEST_SUITE_REGISTRATION(MergeAcquisitionFilterTest)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, NoInput)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, OneInput)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, TwoInputsFromScratch)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, TwoInputsFromScratch_FirstFrame1)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, TwoInputsFromScratch_FirstFrame2)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, TwoInputsFromScratch_Merging1)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, TwoInputsFromScratch_Merging2)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, TwinsFromFile_Concat)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, TwinsFromFile_Concat2)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, TwinsFromFile_Merge)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, TwoFiles_Concat_CalMatrix)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, TwoFiles_Concat_NoSameFrameNumber)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, ThreeFiles_Concat_TRC_and_ANC_and_CAL)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, ThreeFiles_Concat_ANC_and_CAL_and_TRC)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, ThreeFiles_Concat_CAL_and_TRC_and_ANC)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, C3D_vs_ThreeFiles_Concat_TRC_and_ANC_and_CAL)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, C3D_vs_exportedC3D)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, FourFiles_Concat_TRC_and_ANC_and_CAL_and_XLS)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, Elite_vs_C3D)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, EliteGrFirst_vs_C3D)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, ExportedC3DElite_vs_C3D)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, Elite200)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, Elite2000)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, Elite2000Reversed)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, DifferentAnalogFrequencies)
#endif
