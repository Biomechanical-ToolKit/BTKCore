#ifndef SubAcquisitionFilterTest_h
#define SubAcquisitionFilterTest_h

#include <btkSubAcquisitionFilter.h>

CXXTEST_SUITE(SubAcquisitionFilterTest)
{
  CXXTEST_TEST(TestAllNoEffect)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(10,25,5,2);
    acq->AppendEvent(btk::Event::New());
    acq->SetFirstFrame(10);
    acq->SetPointFrequency(25.0);
    
    btk::SubAcquisitionFilter::Pointer sub = btk::SubAcquisitionFilter::New();
    sub->SetInput(acq);
    sub->Update();
    
    btk::Acquisition::Pointer output = sub->GetOutput();
    TS_ASSERT_EQUALS(output->GetPoints(), acq->GetPoints());
    TS_ASSERT_EQUALS(output->GetAnalogs(), acq->GetAnalogs());
    TS_ASSERT_EQUALS(output->GetEvents(), acq->GetEvents());
    TS_ASSERT_EQUALS(output->GetMetaData(), acq->GetMetaData());
    TS_ASSERT_EQUALS(output->GetFirstFrame(), acq->GetFirstFrame());
    TS_ASSERT_EQUALS(output->GetPointFrequency(), acq->GetPointFrequency());
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), acq->GetPointFrameNumber());
    TS_ASSERT_EQUALS(output->GetPointNumber(), acq->GetPointNumber());
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), acq->GetAnalogNumber());
    TS_ASSERT_EQUALS(output->GetEventNumber(), 1)
    TS_ASSERT_EQUALS(output->GetNumberAnalogSamplePerFrame(), acq->GetNumberAnalogSamplePerFrame());
  };
  
  CXXTEST_TEST(TestAllSubFrame)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(10,25,5,2);
    acq->AppendEvent(btk::Event::New("", 12));
    acq->SetFirstFrame(10);
    acq->SetPointFrequency(25.0);
    for (int i = 0 ; i < 10 ; ++i)
      acq->GetPoint(i)->GetValues().coeffRef(0,0) = 10.0;
    for (int i = 0 ; i < 5 ; ++i)
      acq->GetAnalog(i)->GetValues().coeffRef(2) = 10.0;
    
    btk::SubAcquisitionFilter::Pointer sub = btk::SubAcquisitionFilter::New();
    sub->SetInput(acq);
    sub->SetFramesIndex(0,9); // Ten first frames
    sub->Update();
    
    btk::Acquisition::Pointer output = sub->GetOutput();
    TS_ASSERT_EQUALS(output->GetFirstFrame(), acq->GetFirstFrame());
    TS_ASSERT_EQUALS(output->GetPointFrequency(), acq->GetPointFrequency());
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 10);
    TS_ASSERT_EQUALS(output->GetNumberAnalogSamplePerFrame(), acq->GetNumberAnalogSamplePerFrame());
    TS_ASSERT_EQUALS(output->GetEventNumber(), 1)
    TS_ASSERT_EQUALS(output->GetPointNumber(), acq->GetPointNumber());
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), acq->GetAnalogNumber());
    TS_ASSERT_EQUALS(output->GetMetaData(), acq->GetMetaData());
    TS_ASSERT_EQUALS(output->GetEvent(0), acq->GetEvent(0))
    
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_EQUALS(output->GetPoint(i)->GetValues().coeff(0,0), 10.0);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(output->GetAnalog(i)->GetValues().coeff(2), 10.0);
  };
  
  CXXTEST_TEST(TestAllSubFrameBis)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(10,25,5,2);
    acq->AppendEvent(btk::Event::New("", 10));
    acq->SetFirstFrame(10);
    acq->SetPointFrequency(25.0);
    for (int i = 0 ; i < 10 ; ++i)
      acq->GetPoint(i)->GetValues().coeffRef(0,0) = 10.0;
    for (int i = 0 ; i < 5 ; ++i)
      acq->GetAnalog(i)->GetValues().coeffRef(2) = 10.0;
    
    btk::SubAcquisitionFilter::Pointer sub = btk::SubAcquisitionFilter::New();
    sub->SetInput(acq);
    sub->SetFramesIndex(1,9); // Nine first frames
    sub->Update();
    
    btk::Acquisition::Pointer output = sub->GetOutput();
    TS_ASSERT_EQUALS(output->GetFirstFrame(), acq->GetFirstFrame());
    TS_ASSERT_EQUALS(output->GetPointFrequency(), acq->GetPointFrequency());
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 9);
    TS_ASSERT_EQUALS(output->GetNumberAnalogSamplePerFrame(), acq->GetNumberAnalogSamplePerFrame());
    TS_ASSERT_EQUALS(output->GetEventNumber(), 0)
    TS_ASSERT_EQUALS(output->GetPointNumber(), acq->GetPointNumber());
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), acq->GetAnalogNumber());
    TS_ASSERT_EQUALS(output->GetMetaData(), acq->GetMetaData());
    
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_EQUALS(output->GetPoint(i)->GetValues().coeff(0,0), 0.0);
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(output->GetAnalog(i)->GetValues().coeff(0), 10.0);
  };
  
  CXXTEST_TEST(TestOnlyPoints)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(10,25,5,2);
    acq->AppendEvent(btk::Event::New("", 12));
    acq->SetFirstFrame(10);
    acq->SetPointFrequency(25.0);
    for (int i = 0 ; i < 10 ; ++i)
      acq->GetPoint(i)->GetValues().coeffRef(0,0) = 10.0;
    for (int i = 0 ; i < 5 ; ++i)
      acq->GetAnalog(i)->GetValues().coeffRef(2) = 10.0;
    
    btk::SubAcquisitionFilter::Pointer sub = btk::SubAcquisitionFilter::New();
    sub->SetInput(acq);
    sub->SetFramesIndex(0,9);
    sub->SetExtractionOption(btk::SubAcquisitionFilter::PointsOnly);
    sub->Update();
    
    btk::Acquisition::Pointer output = sub->GetOutput();
    TS_ASSERT_EQUALS(output->GetFirstFrame(), acq->GetFirstFrame());
    TS_ASSERT_EQUALS(output->GetPointFrequency(), acq->GetPointFrequency());
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 10);
    TS_ASSERT_EQUALS(output->GetNumberAnalogSamplePerFrame(), acq->GetNumberAnalogSamplePerFrame());
    TS_ASSERT_EQUALS(output->GetEventNumber(), 0)
    TS_ASSERT_EQUALS(output->GetPointNumber(), acq->GetPointNumber());
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(output->GetMetaData(), acq->GetMetaData());
    
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_EQUALS(output->GetPoint(i)->GetValues().coeff(0,0), 10.0);
  };
  
  CXXTEST_TEST(TestOnlyFivePoints)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(10,25,5,2);
    acq->AppendEvent(btk::Event::New("", 12));
    acq->SetFirstFrame(10);
    acq->SetPointFrequency(25.0);
    for (int i = 0 ; i < 10 ; ++i)
      acq->GetPoint(i)->GetValues().coeffRef(0,0) = 10.0;
    for (int i = 0 ; i < 5 ; ++i)
      acq->GetAnalog(i)->GetValues().coeffRef(2) = 10.0;
    
    btk::SubAcquisitionFilter::Pointer sub = btk::SubAcquisitionFilter::New();
    sub->SetInput(acq);
    sub->SetFramesIndex(0,9);
    std::list<int> ids; ids.push_back(0); ids.push_back(2); ids.push_back(4); ids.push_back(6); ids.push_back(8);
    sub->SetExtractionOption(btk::SubAcquisitionFilter::PointsOnly, ids);
    sub->Update();
    
    btk::Acquisition::Pointer output = sub->GetOutput();
    TS_ASSERT_EQUALS(output->GetFirstFrame(), acq->GetFirstFrame());
    TS_ASSERT_EQUALS(output->GetPointFrequency(), acq->GetPointFrequency());
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 10);
    TS_ASSERT_EQUALS(output->GetNumberAnalogSamplePerFrame(), acq->GetNumberAnalogSamplePerFrame());
    TS_ASSERT_EQUALS(output->GetEventNumber(), 0)
    TS_ASSERT_EQUALS(output->GetPointNumber(), 5);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(output->GetMetaData(), acq->GetMetaData());
    
    TS_ASSERT_EQUALS(output->GetPoint(0)->GetLabel(), "uname*1");
    TS_ASSERT_EQUALS(output->GetPoint(1)->GetLabel(), "uname*3");
    TS_ASSERT_EQUALS(output->GetPoint(2)->GetLabel(), "uname*5");
    TS_ASSERT_EQUALS(output->GetPoint(3)->GetLabel(), "uname*7");
    TS_ASSERT_EQUALS(output->GetPoint(4)->GetLabel(), "uname*9");
    
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(output->GetPoint(i)->GetValues().coeff(0,0), 10.0);
  };
  
  CXXTEST_TEST(TestOnlyAnalogs)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(10,25,5,2);
    acq->AppendEvent(btk::Event::New("", 12));
    acq->SetFirstFrame(10);
    acq->SetPointFrequency(25.0);
    for (int i = 0 ; i < 10 ; ++i)
      acq->GetPoint(i)->GetValues().coeffRef(0,0) = 10.0;
    for (int i = 0 ; i < 5 ; ++i)
      acq->GetAnalog(i)->GetValues().coeffRef(2) = 10.0;
    
    btk::SubAcquisitionFilter::Pointer sub = btk::SubAcquisitionFilter::New();
    sub->SetInput(acq);
    sub->SetFramesIndex(0,9);
    sub->SetExtractionOption(btk::SubAcquisitionFilter::AnalogsOnly);
    sub->Update();
    
    btk::Acquisition::Pointer output = sub->GetOutput();
    TS_ASSERT_EQUALS(output->GetFirstFrame(), acq->GetFirstFrame());
    TS_ASSERT_EQUALS(output->GetPointFrequency(), acq->GetPointFrequency());
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 10);
    TS_ASSERT_EQUALS(output->GetNumberAnalogSamplePerFrame(), acq->GetNumberAnalogSamplePerFrame());
    TS_ASSERT_EQUALS(output->GetEventNumber(), 0)
    TS_ASSERT_EQUALS(output->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 5);
    TS_ASSERT_EQUALS(output->GetMetaData(), acq->GetMetaData());
    
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_EQUALS(output->GetAnalog(i)->GetValues().coeff(2), 10.0);
  };
  
  CXXTEST_TEST(TestOnlyTwoAnalogsSubFrame)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(10,25,5,2);
    acq->AppendEvent(btk::Event::New("", 12));
    acq->SetFirstFrame(10);
    acq->SetPointFrequency(25.0);
    for (int i = 0 ; i < 10 ; ++i)
      acq->GetPoint(i)->GetValues().coeffRef(0,0) = 10.0;
    for (int i = 0 ; i < 5 ; ++i)
      acq->GetAnalog(i)->GetValues().coeffRef(2) = 10.0;
    
    btk::SubAcquisitionFilter::Pointer sub = btk::SubAcquisitionFilter::New();
    sub->SetInput(acq);
    sub->SetFramesIndex(1,9);
    std::list<int> ids; ids.push_back(1); ids.push_back(3);
    sub->SetExtractionOption(btk::SubAcquisitionFilter::AnalogsOnly,ids);
    sub->Update();
    
    btk::Acquisition::Pointer output = sub->GetOutput();
    TS_ASSERT_EQUALS(output->GetFirstFrame(), acq->GetFirstFrame());
    TS_ASSERT_EQUALS(output->GetPointFrequency(), acq->GetPointFrequency());
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 9);
    TS_ASSERT_EQUALS(output->GetNumberAnalogSamplePerFrame(), acq->GetNumberAnalogSamplePerFrame());
    TS_ASSERT_EQUALS(output->GetEventNumber(), 0)
    TS_ASSERT_EQUALS(output->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 2);
    TS_ASSERT_EQUALS(output->GetMetaData(), acq->GetMetaData());
    
    TS_ASSERT_EQUALS(output->GetAnalog(0)->GetLabel(), "uname*2");
    TS_ASSERT_EQUALS(output->GetAnalog(1)->GetLabel(), "uname*4");
    
    for (int i = 0 ; i < 2 ; ++i)
      TS_ASSERT_EQUALS(output->GetAnalog(i)->GetValues().coeff(0), 10.0);
  };
  
  CXXTEST_TEST(TestOnlyEventsSubFrame)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(10,25,5,2);
    acq->AppendEvent(btk::Event::New("", 10));
    acq->AppendEvent(btk::Event::New("", 12));
    acq->AppendEvent(btk::Event::New("", 15));
    acq->AppendEvent(btk::Event::New("", 20));
    acq->AppendEvent(btk::Event::New("", 21));
    acq->AppendEvent(btk::Event::New("", 25));
    acq->SetFirstFrame(10);
    acq->SetPointFrequency(25.0);
    for (int i = 0 ; i < 10 ; ++i)
      acq->GetPoint(i)->GetValues().coeffRef(0,0) = 10.0;
    for (int i = 0 ; i < 5 ; ++i)
      acq->GetAnalog(i)->GetValues().coeffRef(2) = 10.0;
    
    btk::SubAcquisitionFilter::Pointer sub = btk::SubAcquisitionFilter::New();
    sub->SetInput(acq);
    sub->SetFramesIndex(1,9);
    sub->SetExtractionOption(btk::SubAcquisitionFilter::EventsOnly);
    sub->Update();
    
    btk::Acquisition::Pointer output = sub->GetOutput();
    TS_ASSERT_EQUALS(output->GetFirstFrame(), acq->GetFirstFrame());
    TS_ASSERT_EQUALS(output->GetPointFrequency(), acq->GetPointFrequency());
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(), 9);
    TS_ASSERT_EQUALS(output->GetNumberAnalogSamplePerFrame(), acq->GetNumberAnalogSamplePerFrame());
    TS_ASSERT_EQUALS(output->GetEventNumber(), 2)
    TS_ASSERT_EQUALS(output->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(output->GetMetaData(), acq->GetMetaData());
    
    TS_ASSERT_EQUALS(output->GetEvent(0)->GetFrame(), 12);
    TS_ASSERT_EQUALS(output->GetEvent(1)->GetFrame(), 15);
  };
};

CXXTEST_SUITE_REGISTRATION(SubAcquisitionFilterTest)
CXXTEST_TEST_REGISTRATION(SubAcquisitionFilterTest, TestAllNoEffect)
CXXTEST_TEST_REGISTRATION(SubAcquisitionFilterTest, TestAllSubFrame)
CXXTEST_TEST_REGISTRATION(SubAcquisitionFilterTest, TestAllSubFrameBis)
CXXTEST_TEST_REGISTRATION(SubAcquisitionFilterTest, TestOnlyPoints)
CXXTEST_TEST_REGISTRATION(SubAcquisitionFilterTest, TestOnlyFivePoints)
CXXTEST_TEST_REGISTRATION(SubAcquisitionFilterTest, TestOnlyAnalogs)
CXXTEST_TEST_REGISTRATION(SubAcquisitionFilterTest, TestOnlyTwoAnalogsSubFrame)
CXXTEST_TEST_REGISTRATION(SubAcquisitionFilterTest, TestOnlyEventsSubFrame)

#endif // SubAcquisitionFilterTest_h