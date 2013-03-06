#ifndef DownSampleFilterTest_h
#define DownSampleFilterTest_h

#include <btkDownsampleFilter.h>
#include <btkConvert.h>

CXXTEST_SUITE(DownsampleFilterTest)
{
  CXXTEST_TEST(WrenchRatioOne)
  {
    btk::Wrench::Pointer w = btk::Wrench::New("Test", 10);
    w->GetPosition()->SetValues(Eigen::Matrix<double,10,3>::Random());
    w->GetForce()->SetValues(Eigen::Matrix<double,10,3>::Random());
    w->GetMoment()->SetValues(Eigen::Matrix<double,10,3>::Random());
    
    btk::DownsampleFilter<btk::Wrench>::Pointer ds = btk::DownsampleFilter<btk::Wrench>::New();
    ds->SetInput(w);
    ds->SetUpDownRatio(1);
    ds->Update();
    for (int i = 0 ; i < 10 ; ++i)
    {
      TS_ASSERT_EQUALS(ds->GetOutput()->GetPosition()->GetValues()(i,0), w->GetPosition()->GetValues()(i,0));
      TS_ASSERT_EQUALS(ds->GetOutput()->GetPosition()->GetValues()(i,1), w->GetPosition()->GetValues()(i,1));
      TS_ASSERT_EQUALS(ds->GetOutput()->GetPosition()->GetValues()(i,2), w->GetPosition()->GetValues()(i,2));
      TS_ASSERT_EQUALS(ds->GetOutput()->GetForce()->GetValues()(i,0), w->GetForce()->GetValues()(i,0));
      TS_ASSERT_EQUALS(ds->GetOutput()->GetForce()->GetValues()(i,1), w->GetForce()->GetValues()(i,1));
      TS_ASSERT_EQUALS(ds->GetOutput()->GetForce()->GetValues()(i,2), w->GetForce()->GetValues()(i,2));
      TS_ASSERT_EQUALS(ds->GetOutput()->GetMoment()->GetValues()(i,0), w->GetMoment()->GetValues()(i,0));
      TS_ASSERT_EQUALS(ds->GetOutput()->GetMoment()->GetValues()(i,1), w->GetMoment()->GetValues()(i,1));
      TS_ASSERT_EQUALS(ds->GetOutput()->GetMoment()->GetValues()(i,2), w->GetMoment()->GetValues()(i,2));
    }
  };
  
  CXXTEST_TEST(WrenchRatioOneOverTwo)
  {
    btk::Wrench::Pointer w = btk::Wrench::New("Test", 10);
    w->GetPosition()->SetValues(Eigen::Matrix<double,10,3>::Random());
    w->GetForce()->SetValues(Eigen::Matrix<double,10,3>::Random());
    w->GetMoment()->SetValues(Eigen::Matrix<double,10,3>::Random());
    
    btk::DownsampleFilter<btk::Wrench>::Pointer ds = btk::DownsampleFilter<btk::Wrench>::New();
    ds->SetInput(w);
    ds->SetUpDownRatio(2);
    ds->Update();
    TS_ASSERT_EQUALS(ds->GetOutput()->GetPosition()->GetFrameNumber(), 5);
    TS_ASSERT_EQUALS(ds->GetOutput()->GetForce()->GetFrameNumber(), 5);
    TS_ASSERT_EQUALS(ds->GetOutput()->GetMoment()->GetFrameNumber(), 5);
    TS_ASSERT_EQUALS(ds->GetOutput()->GetPosition()->GetLabel(), w->GetPosition()->GetLabel());
    TS_ASSERT_EQUALS(ds->GetOutput()->GetForce()->GetLabel(), w->GetForce()->GetLabel());
    TS_ASSERT_EQUALS(ds->GetOutput()->GetMoment()->GetLabel(), w->GetMoment()->GetLabel());
    
    for (int i = 0 ; i < 5 ; ++i)
    {
      TS_ASSERT_EQUALS(ds->GetOutput()->GetPosition()->GetValues()(i,0), w->GetPosition()->GetValues()(i*2,0));
      TS_ASSERT_EQUALS(ds->GetOutput()->GetForce()->GetValues()(i,1), w->GetForce()->GetValues()(i*2,1));
      TS_ASSERT_EQUALS(ds->GetOutput()->GetMoment()->GetValues()(i,2), w->GetMoment()->GetValues()(i*2,2));
    }
  };
  
  CXXTEST_TEST(WrenchCollectionRatioOne)
  {
    btk::WrenchCollection::Pointer wc = btk::WrenchCollection::New();
    for (int i = 0 ; i < 3 ; ++i)
    {
      btk::Wrench::Pointer w = btk::Wrench::New("Test" + btk::ToString(i), 10);
      w->GetPosition()->SetValues(Eigen::Matrix<double,10,3>::Random());
      w->GetForce()->SetValues(Eigen::Matrix<double,10,3>::Random());
      w->GetMoment()->SetValues(Eigen::Matrix<double,10,3>::Random());
      wc->InsertItem(w);
    }
    
    btk::DownsampleFilter<btk::WrenchCollection>::Pointer ds = btk::DownsampleFilter<btk::WrenchCollection>::New();
    ds->SetInput(wc);
    ds->SetUpDownRatio(1);
    ds->Update();
    TS_ASSERT_EQUALS(ds->GetOutput()->GetItemNumber(), 3);
    for (int j = 0 ; j < 3 ; ++j)
    {
      for (int i = 0 ; i < 10 ; ++i)
      {
        TS_ASSERT_EQUALS(ds->GetOutput()->GetItem(j)->GetPosition()->GetValues()(i,0), wc->GetItem(j)->GetPosition()->GetValues()(i,0));
        TS_ASSERT_EQUALS(ds->GetOutput()->GetItem(j)->GetPosition()->GetValues()(i,1), wc->GetItem(j)->GetPosition()->GetValues()(i,1));
        TS_ASSERT_EQUALS(ds->GetOutput()->GetItem(j)->GetPosition()->GetValues()(i,2), wc->GetItem(j)->GetPosition()->GetValues()(i,2));
        TS_ASSERT_EQUALS(ds->GetOutput()->GetItem(j)->GetForce()->GetValues()(i,0), wc->GetItem(j)->GetForce()->GetValues()(i,0));
        TS_ASSERT_EQUALS(ds->GetOutput()->GetItem(j)->GetForce()->GetValues()(i,1), wc->GetItem(j)->GetForce()->GetValues()(i,1));
        TS_ASSERT_EQUALS(ds->GetOutput()->GetItem(j)->GetForce()->GetValues()(i,2), wc->GetItem(j)->GetForce()->GetValues()(i,2));
        TS_ASSERT_EQUALS(ds->GetOutput()->GetItem(j)->GetMoment()->GetValues()(i,0), wc->GetItem(j)->GetMoment()->GetValues()(i,0));
        TS_ASSERT_EQUALS(ds->GetOutput()->GetItem(j)->GetMoment()->GetValues()(i,1), wc->GetItem(j)->GetMoment()->GetValues()(i,1));
        TS_ASSERT_EQUALS(ds->GetOutput()->GetItem(j)->GetMoment()->GetValues()(i,2), wc->GetItem(j)->GetMoment()->GetValues()(i,2));
      }
    }
  };
  
  CXXTEST_TEST(WrenchCollectionRatioOneOverTwo)
  {
    btk::WrenchCollection::Pointer wc = btk::WrenchCollection::New();
    for (int i = 0 ; i < 3 ; ++i)
    {
      btk::Wrench::Pointer w = btk::Wrench::New("Test" + btk::ToString(i), 10);
      w->GetPosition()->SetValues(Eigen::Matrix<double,10,3>::Random());
      w->GetForce()->SetValues(Eigen::Matrix<double,10,3>::Random());
      w->GetMoment()->SetValues(Eigen::Matrix<double,10,3>::Random());
      wc->InsertItem(w);
    }
    
    btk::DownsampleFilter<btk::WrenchCollection>::Pointer ds = btk::DownsampleFilter<btk::WrenchCollection>::New();
    ds->SetInput(wc);
    ds->SetUpDownRatio(2);
    ds->Update();
    TS_ASSERT_EQUALS(ds->GetOutput()->GetItemNumber(), 3);
    for (int i = 0 ; i < 3 ; ++i)
    {
      TS_ASSERT_EQUALS(ds->GetOutput()->GetItem(i)->GetPosition()->GetLabel(), wc->GetItem(i)->GetPosition()->GetLabel());
      TS_ASSERT_EQUALS(ds->GetOutput()->GetItem(i)->GetForce()->GetLabel(), wc->GetItem(i)->GetForce()->GetLabel());
      TS_ASSERT_EQUALS(ds->GetOutput()->GetItem(i)->GetMoment()->GetLabel(), wc->GetItem(i)->GetMoment()->GetLabel());
      for (int j = 0 ; j < 5 ; ++j)
      {
        TS_ASSERT_EQUALS(ds->GetOutput()->GetItem(i)->GetPosition()->GetValues()(j,0), wc->GetItem(i)->GetPosition()->GetValues()(j*2,0));
        TS_ASSERT_EQUALS(ds->GetOutput()->GetItem(i)->GetForce()->GetValues()(j,1), wc->GetItem(i)->GetForce()->GetValues()(j*2,1));
        TS_ASSERT_EQUALS(ds->GetOutput()->GetItem(i)->GetMoment()->GetValues()(j,2), wc->GetItem(i)->GetMoment()->GetValues()(j*2,2));
      }
    }
  };
};

CXXTEST_SUITE_REGISTRATION(DownsampleFilterTest)
CXXTEST_TEST_REGISTRATION(DownsampleFilterTest, WrenchRatioOne)
CXXTEST_TEST_REGISTRATION(DownsampleFilterTest, WrenchRatioOneOverTwo)
CXXTEST_TEST_REGISTRATION(DownsampleFilterTest, WrenchCollectionRatioOne)
CXXTEST_TEST_REGISTRATION(DownsampleFilterTest, WrenchCollectionRatioOneOverTwo)
#endif
