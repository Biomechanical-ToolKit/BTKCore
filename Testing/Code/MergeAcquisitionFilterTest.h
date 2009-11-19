#ifndef MergeAcquisitionFilterTest_h
#define MergeAcquisitionFilterTest_h

#include <btkMergeAcquisitionFilter.h>
#include <btkAcquisitionFileReader.h>
#include <btkForcePlatformsExtractor.h>
#include <btkGroundReactionWrenchFilter.h>
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
  }
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
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, ThreeFiles_Concat_TRC_and_ANC_and_CAL)
CXXTEST_TEST_REGISTRATION(MergeAcquisitionFilterTest, C3D_vs_ThreeFiles_Concat_TRC_and_ANC_and_CAL)
#endif
