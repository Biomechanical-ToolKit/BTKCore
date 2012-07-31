#ifndef WrenchDirectionAngleFilterTest_h
#define WrenchDirectionAngleFilterTest_h

#include "btkWrenchDirectionAngleFilter.h"

CXXTEST_SUITE(WrenchDirectionAngleFilterTest)
{
  CXXTEST_TEST(OneFrame)
  {
    btk::Wrench::Pointer w = btk::Wrench::New();
    w->SetFrameNumber(6);
    
    w->GetForce()->GetValues()(0,0) = 100.0;
    w->GetForce()->GetValues()(0,1) = 100.0;
    w->GetForce()->GetValues()(0,2) = 100.0;
    
    w->GetForce()->GetValues()(1,0) = 0.5;
    w->GetForce()->GetValues()(1,1) = 0.5;
    w->GetForce()->GetValues()(1,2) = sqrt(3.0)/2.0;
    
    w->GetForce()->GetValues()(2,0) = 0.0;
    w->GetForce()->GetValues()(2,1) = 0.0;
    w->GetForce()->GetValues()(2,2) = 1.0;
    
    w->GetForce()->GetValues()(3,0) = -100.0;
    w->GetForce()->GetValues()(3,1) = 0.0;
    w->GetForce()->GetValues()(3,2) = 100.0;
    
    w->GetForce()->GetValues()(4,0) = -40.0;
    w->GetForce()->GetValues()(4,1) = -40.0;
    w->GetForce()->GetValues()(4,2) = -40.0;
    
    w->GetPosition()->GetResiduals()(5,0) = -1.0;
    w->GetPosition()->GetMasks()(5,0) = -1.0;
    
    btk::WrenchCollection::Pointer input = btk::WrenchCollection::New();
    input->InsertItem(w);
    
    btk::WrenchDirectionAngleFilter::Pointer wdaf = btk::WrenchDirectionAngleFilter::New();
    wdaf->SetInput(input);
    btk::PointCollection::Pointer output = wdaf->GetOutput();
    output->Update();

    TS_ASSERT_EQUALS(output->GetItem(0)->GetFrameNumber(), 6);
    
    TS_ASSERT_DELTA(output->GetItem(0)->GetValues()(0,0), 45.0, 1e-15);
    TS_ASSERT_DELTA(output->GetItem(0)->GetValues()(0,1), 45.0, 1e-15);
    TS_ASSERT_DELTA(output->GetItem(0)->GetValues()(0,2), 45.0, 1e-15);
    
    TS_ASSERT_DELTA(output->GetItem(0)->GetValues()(1,0), 60.0, 5e-14); // 5e-14: due to trigonometric function
    TS_ASSERT_DELTA(output->GetItem(0)->GetValues()(1,1), 60.0, 5e-14);
    TS_ASSERT_DELTA(output->GetItem(0)->GetValues()(1,2), 45.0, 1e-15);
    
    TS_ASSERT_DELTA(output->GetItem(0)->GetValues()(2,0), 90.0, 1e-15);
    TS_ASSERT_DELTA(output->GetItem(0)->GetValues()(2,1), 90.0, 1e-15);
    TS_ASSERT_DELTA(output->GetItem(0)->GetValues()(2,2), 0.0, 1e-15);
    
    TS_ASSERT_DELTA(output->GetItem(0)->GetValues()(3,0), 90.0, 1e-15);
    TS_ASSERT_DELTA(output->GetItem(0)->GetValues()(3,1), 135.0, 1e-15);
    TS_ASSERT_DELTA(output->GetItem(0)->GetValues()(3,2), 180.0, 1e-15);
    
    TS_ASSERT_DELTA(output->GetItem(0)->GetValues()(4,0), 225.0, 1e-15);
    TS_ASSERT_DELTA(output->GetItem(0)->GetValues()(4,1), 225.0, 1e-15);
    TS_ASSERT_DELTA(output->GetItem(0)->GetValues()(4,2), 225.0, 1e-15);
    
    TS_ASSERT_DELTA(output->GetItem(0)->GetValues()(5,0), 0.0, 1e-15);
    TS_ASSERT_DELTA(output->GetItem(0)->GetValues()(5,1), 0.0, 1e-15);
    TS_ASSERT_DELTA(output->GetItem(0)->GetValues()(5,2), 0.0, 1e-15);
  };
};

CXXTEST_SUITE_REGISTRATION(WrenchDirectionAngleFilterTest)
CXXTEST_TEST_REGISTRATION(WrenchDirectionAngleFilterTest, OneFrame)

#endif // WrenchDirectionAngleFilterTest_h