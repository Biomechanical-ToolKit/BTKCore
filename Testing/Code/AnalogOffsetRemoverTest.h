#ifndef AnalogOffsetRemoverTest_h
#define AnalogOffsetRemoverTest_h

#include <btkAnalogOffsetRemover.h>

CXXTEST_SUITE(AnalogOffsetRemoverTest)
{
  CXXTEST_TEST(Test0)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(0,25,1,1);
    acq->GetAnalog(0)->GetValues().setConstant(5.0);
    
    btk::AnalogOffsetRemover::Pointer remover = btk::AnalogOffsetRemover::New();
    remover->SetRawInput(acq);
    remover->SetOffsetInput(acq);
    remover->Update();
    
    btk::Acquisition::Pointer output = remover->GetOutput();
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 1);
    TS_ASSERT_EQUALS(output->GetAnalog(0)->GetValues().sum(), 0.0);
  };
  
  CXXTEST_TEST(Test1234)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(0,25,4,1);
    acq->GetAnalog(0)->GetValues().setConstant(5.0);
    acq->GetAnalog(1)->GetValues().setConstant(4.0);
    acq->GetAnalog(2)->GetValues().setConstant(3.0);
    acq->GetAnalog(3)->GetValues().setConstant(2.0);
    
    btk::Acquisition::Pointer acq2 = btk::Acquisition::New();
    acq2->Init(0,25,4,1);
    acq2->GetAnalog(0)->GetValues().setConstant(1.0);
    acq2->GetAnalog(1)->GetValues().setConstant(2.0);
    acq2->GetAnalog(2)->GetValues().setConstant(3.0);
    acq2->GetAnalog(3)->GetValues().setConstant(4.0);
    
    btk::AnalogOffsetRemover::Pointer remover = btk::AnalogOffsetRemover::New();
    remover->SetRawInput(acq);
    remover->SetOffsetInput(acq2);
    remover->Update();
    
    btk::Acquisition::Pointer output = remover->GetOutput();
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 4);
    TS_ASSERT_EQUALS(output->GetAnalog(0)->GetValues().sum() / 25.0, 4.0);
    TS_ASSERT_EQUALS(output->GetAnalog(1)->GetValues().sum() / 25.0, 2.0);
    TS_ASSERT_EQUALS(output->GetAnalog(2)->GetValues().sum() / 25.0, 0.0);
    TS_ASSERT_EQUALS(output->GetAnalog(3)->GetValues().sum() / 25.0, -2.0);
  };
  
  CXXTEST_TEST(Test3Over4)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(0,25,4,1);
    acq->GetAnalog(0)->GetValues().setConstant(5.0);
    acq->GetAnalog(1)->GetValues().setConstant(4.0);
    acq->GetAnalog(2)->GetValues().setConstant(3.0);
    acq->GetAnalog(3)->GetValues().setConstant(2.0);
    
    btk::Acquisition::Pointer acq2 = btk::Acquisition::New();
    acq2->Init(0,25,4,1);
    acq2->GetAnalog(0)->GetValues().setConstant(1.0);
    acq2->GetAnalog(1)->GetValues().setConstant(2.0);
    acq2->GetAnalog(2)->GetValues().setConstant(3.0);
    
    btk::AnalogOffsetRemover::Pointer remover = btk::AnalogOffsetRemover::New();
    remover->SetRawInput(acq);
    remover->SetOffsetInput(acq2);
    remover->Update();
    
    btk::Acquisition::Pointer output = remover->GetOutput();
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 4);
    TS_ASSERT_EQUALS(output->GetAnalog(0)->GetValues().sum() / 25.0, 4.0);
    TS_ASSERT_EQUALS(output->GetAnalog(1)->GetValues().sum() / 25.0, 2.0);
    TS_ASSERT_EQUALS(output->GetAnalog(2)->GetValues().sum() / 25.0, 0.0);
    TS_ASSERT_EQUALS(output->GetAnalog(3)->GetValues().sum() / 25.0, 2.0);
  };
  
  CXXTEST_TEST(Test4Over3)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(0,25,3,1);
    acq->GetAnalog(0)->GetValues().setConstant(5.0);
    acq->GetAnalog(1)->GetValues().setConstant(4.0);
    acq->GetAnalog(2)->GetValues().setConstant(3.0);
    
    btk::Acquisition::Pointer acq2 = btk::Acquisition::New();
    acq2->Init(0,25,4,1);
    acq2->GetAnalog(0)->GetValues().setConstant(1.0);
    acq2->GetAnalog(1)->GetValues().setConstant(2.0);
    acq2->GetAnalog(2)->GetValues().setConstant(3.0);
    acq2->GetAnalog(3)->GetValues().setConstant(4.0);
    
    btk::AnalogOffsetRemover::Pointer remover = btk::AnalogOffsetRemover::New();
    remover->SetRawInput(acq);
    remover->SetOffsetInput(acq2);
    remover->Update();
    
    btk::Acquisition::Pointer output = remover->GetOutput();
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 3);
    TS_ASSERT_EQUALS(output->GetAnalog(0)->GetValues().sum() / 25.0, 4.0);
    TS_ASSERT_EQUALS(output->GetAnalog(1)->GetValues().sum() / 25.0, 2.0);
    TS_ASSERT_EQUALS(output->GetAnalog(2)->GetValues().sum() / 25.0, 0.0);
  };
  
  CXXTEST_TEST(TestNoCommonLabel)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(0,25,3,1);
    acq->GetAnalog(0)->SetLabel("FOO");
    acq->GetAnalog(1)->SetLabel("BAR");
    acq->GetAnalog(2)->SetLabel("FOOBAR");
    
    btk::Acquisition::Pointer acq2 = btk::Acquisition::New();
    acq2->Init(0,25,3,1);
    acq2->GetAnalog(0)->GetValues().setConstant(1.0);
    acq2->GetAnalog(1)->GetValues().setConstant(2.0);
    acq2->GetAnalog(2)->GetValues().setConstant(3.0);
    
    btk::AnalogOffsetRemover::Pointer remover = btk::AnalogOffsetRemover::New();
    remover->SetRawInput(acq);
    remover->SetOffsetInput(acq2);
    remover->Update();
    
    btk::Acquisition::Pointer output = remover->GetOutput();
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 3);
    TS_ASSERT_EQUALS(output->GetAnalog(0)->GetValues().sum(), 0.0);
    TS_ASSERT_EQUALS(output->GetAnalog(1)->GetValues().sum(), 0.0);
    TS_ASSERT_EQUALS(output->GetAnalog(2)->GetValues().sum(), 0.0);
  };
  
  CXXTEST_TEST(TestOneCommonLabel)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(0,25,3,1);
    acq->GetAnalog(0)->SetLabel("FOO");
    acq->GetAnalog(1)->SetLabel("BAR");
    acq->GetAnalog(2)->SetLabel("FOOBAR");
    
    btk::Acquisition::Pointer acq2 = btk::Acquisition::New();
    acq2->Init(0,25,3,1);
    acq2->GetAnalog(0)->GetValues().setConstant(1.0);
    acq2->GetAnalog(1)->GetValues().setConstant(2.0);
    acq2->GetAnalog(2)->GetValues().setConstant(3.0);
    acq2->GetAnalog(2)->SetLabel("FOO");
    
    btk::AnalogOffsetRemover::Pointer remover = btk::AnalogOffsetRemover::New();
    remover->SetRawInput(acq);
    remover->SetOffsetInput(acq2);
    remover->Update();
    
    btk::Acquisition::Pointer output = remover->GetOutput();
    TS_ASSERT_EQUALS(output->GetAnalogNumber(), 3);
    TS_ASSERT_EQUALS(output->GetAnalog(0)->GetValues().sum() / 25.0, -3.0);
    TS_ASSERT_EQUALS(output->GetAnalog(1)->GetValues().sum() / 25.0, 0.0);
    TS_ASSERT_EQUALS(output->GetAnalog(2)->GetValues().sum() / 25.0, 0.0);
  };
};

CXXTEST_SUITE_REGISTRATION(AnalogOffsetRemoverTest)
CXXTEST_TEST_REGISTRATION(AnalogOffsetRemoverTest, Test0)
CXXTEST_TEST_REGISTRATION(AnalogOffsetRemoverTest, Test1234)
CXXTEST_TEST_REGISTRATION(AnalogOffsetRemoverTest, Test3Over4)
CXXTEST_TEST_REGISTRATION(AnalogOffsetRemoverTest, Test4Over3)
CXXTEST_TEST_REGISTRATION(AnalogOffsetRemoverTest, TestNoCommonLabel)
CXXTEST_TEST_REGISTRATION(AnalogOffsetRemoverTest, TestOneCommonLabel)
CXXTEST_TEST_REGISTRATION(AnalogOffsetRemoverTest, TestFromFile)

#endif // AnalogOffsetRemoverTest_h