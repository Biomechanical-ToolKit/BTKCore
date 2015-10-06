#ifndef AlgorithmTest_h
#define AlgorithmTest_h

#include "AlgorithmTest_classdef.h"

CXXTEST_SUITE(AlgorithmTest)
{
  CXXTEST_TEST(DataFactory)
  {
    btk::DataFactory<btk::Node> dfn("test");
    TS_ASSERT_EQUALS(dfn.inputsNumber(), 0u);
    TS_ASSERT_EQUALS(dfn.outputsNumber(), 1u);
    TS_ASSERT_EQUALS(dfn.update(), btk::Result::Success);
    auto outData = btk::make_shared_node(dfn.output(0)->exportData());
    auto _outData = dfn.output(0)->data();
    TS_ASSERT_DIFFERS(outData->child(0), nullptr);
    TS_ASSERT_EQUALS(outData->child(0)->name(), "test");
    auto tsdfn = dfn.timestamp();
    auto tsout = dfn.output(0)->timestamp();
    TS_ASSERT_EQUALS(dfn.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(dfn.timestamp(), tsdfn);
    dfn.modified();
    TS_ASSERT_EQUALS(dfn.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(dfn.output(0)->timestamp(), tsout);
    TS_ASSERT_DIFFERS(dfn.timestamp(), tsdfn);
    auto outData2 = btk::make_shared_node(dfn.output(0)->exportData());
    auto _outData2 = dfn.output(0)->data();
    TS_ASSERT_EQUALS(outData->child(0), outData2->child(0));
    TS_ASSERT_EQUALS(_outData, _outData2);
  };
  
  CXXTEST_TEST(DataFactoryAssignedOutput)
  {
    btk::DataFactory<btk::Node> dfn("test");
    btk::Node* n = new btk::Node("abracadabra");
    dfn.output(0)->holdData(n);
    TS_ASSERT_EQUALS(dfn.output(0)->data(0), n);
    TS_ASSERT_EQUALS(n->isCastable(btk::static_typeid<btk::Node>()), true);
    TS_ASSERT_EQUALS(dfn.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(dfn.output(0)->information<btk::OutputDataTypeid>(), btk::static_typeid<btk::Node>());
    TS_ASSERT_EQUALS(n->isCastable(dfn.output(0)->information<btk::OutputDataTypeid>()), true);
    TS_ASSERT_EQUALS(dfn.output(0)->data(0), n);
  }
  
  CXXTEST_TEST(CounterAlgorithm)
  {
    btk::DataFactory<btk::Node> dfn("test");
    MyCounterAlgorithm ma;
    ma.setInput(dfn.output());
    TS_ASSERT_EQUALS(ma.inputsNumber(), 1u);
    TS_ASSERT_EQUALS(ma.outputsNumber(), 1u);
    TS_ASSERT_EQUALS(ma.update(), btk::Result::Success);
    auto ts = ma.timestamp();
    TS_ASSERT_LESS_THAN(dfn.timestamp(), ma.timestamp());
    TS_ASSERT_LESS_THAN(ma.timestamp(), ma.output(0)->timestamp());
    auto outData = btk::make_shared_node(ma.output(0)->exportData());
    TS_ASSERT_DIFFERS(outData->child(0), nullptr);
    auto outChildData = outData->child(0);
    TS_ASSERT_EQUALS(outChildData->property("counter").cast<int>(),1);
    TS_ASSERT_EQUALS(ma.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(outChildData->property("counter").cast<int>(),1);
    TS_ASSERT_EQUALS(ts, ma.timestamp());
    ma.modified();
    TS_ASSERT_EQUALS(ma.update(), btk::Result::Success);
    ts = ma.timestamp();
    TS_ASSERT_EQUALS(outChildData->property("counter").cast<int>(),2);
    TS_ASSERT_EQUALS(ma.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(outChildData->property("counter").cast<int>(),2);
    TS_ASSERT_EQUALS(ts, ma.timestamp());
  };
    
  CXXTEST_TEST(PipelineOne)
  {
    btk::DataFactory<MySource> dfms("test");
    MySource* ms = new MySource("counter");
    TS_ASSERT_EQUALS(ms->isCastable(btk::static_typeid<MySource>()), true);
    ms->setCounter(5);
    TS_ASSERT_EQUALS(ms->counter(), 5);
    TS_ASSERT_EQUALS(ms->property("counter").cast<int>(), 5);
    dfms.output(0)->holdData(ms);
    MySourceCounterAlgorithm msca;
    msca.setInput(dfms.output());
    TS_ASSERT_EQUALS(msca.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(dfms.output(0)->data(0), ms);
    auto outData = btk::make_shared_node(msca.output(0)->exportData());
    auto res = outData->child<MySource*>(0);
    TS_ASSERT_DIFFERS(res, nullptr);
    TS_ASSERT_EQUALS(res->counter(), 6);
    TS_ASSERT_EQUALS(msca.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(res->counter(), 6);
    msca.setIncrement(2);
    TS_ASSERT_EQUALS(msca.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(res->counter(), 7);
    msca.setInput(msca.output());
    TS_ASSERT_EQUALS(msca.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(res->counter(), 9);
    TS_ASSERT_EQUALS(msca.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(res->counter(), 11);
    TS_ASSERT_EQUALS(msca.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(res->counter(), 13);
  };
  
  CXXTEST_TEST(PipelineOne_LoopOnData)
  {
    btk::DataFactory<MySource> dfms("test");
    MySource* ms = new MySource("counter");
    TS_ASSERT_EQUALS(ms->isCastable(btk::static_typeid<MySource>()), true);
    ms->setCounter(5);
    TS_ASSERT_EQUALS(ms->counter(), 5);
    TS_ASSERT_EQUALS(ms->property("counter").cast<int>(), 5);
    dfms.output(0)->holdData(ms);
    MySourceCounterAlgorithm msca;
    msca.setInput(dfms.output());
    TS_ASSERT_EQUALS(msca.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(dfms.output(0)->data(0), ms);
    auto outData = btk::make_shared_node(msca.output(0)->exportData());
    auto res = outData->child<MySource*>(0);
    TS_ASSERT_DIFFERS(res, nullptr);
    TS_ASSERT_EQUALS(res->counter(), 6);
    TS_ASSERT_EQUALS(msca.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(res->counter(), 6);
    msca.setIncrement(2);
    TS_ASSERT_EQUALS(msca.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(res->counter(), 7);
    dfms.output(0)->holdData(res);
    TS_ASSERT_EQUALS(msca.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(res->counter(), 9);
    TS_ASSERT_EQUALS(msca.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(res->counter(), 11);
    TS_ASSERT_EQUALS(msca.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(res->counter(), 13);
  };

  CXXTEST_TEST(PipelineTwo)
  {
    btk::DataFactory<MySource> dfms("test");
    MySource* ms = new MySource("counter");
    ms->setCounter(5);
    dfms.output(0)->holdData(ms);
    MySourceCounterAlgorithm msca1, msca2;
    msca1.setInput(dfms.output());
    msca2.setInput(msca1.output());
    TS_ASSERT_EQUALS(msca2.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(msca1.output(0)->data(0)->property("counter").cast<int>(),6);
    TS_ASSERT_EQUALS(msca2.output(0)->data(0)->property("counter").cast<int>(),7);
    msca2.setIncrement(2);
    TS_ASSERT_EQUALS(msca2.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(msca1.output(0)->data(0)->property("counter").cast<int>(),6);
    TS_ASSERT_EQUALS(msca2.output(0)->data(0)->property("counter").cast<int>(),8);
  };
  
  CXXTEST_TEST(PipelineThree)
  {
    btk::DataFactory<MySource> dfms("test");
    MySource* ms = new MySource("counter");
    ms->setCounter(5);
    dfms.output(0)->holdData(ms);
    MySourceCounterAlgorithm msca;
    msca.setInput(dfms.output());
    TS_ASSERT_EQUALS(msca.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(msca.output(0)->data(0)->property("counter").cast<int>(),6);
    TS_ASSERT_EQUALS(msca.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(msca.output(0)->data(0)->property("counter").cast<int>(),6);
    ms->setCounter(6);
    TS_ASSERT_EQUALS(msca.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(msca.output(0)->data(0)->property("counter").cast<int>(),7);
  };
  
  CXXTEST_TEST(DeleteParent)
  {
    btk::DataFactory<MySource> dfms("test");
    MySource* ms = new MySource("counter");
    ms->setCounter(5);
    dfms.output(0)->holdData(ms);
    MySourceCounterAlgorithm* msca = new MySourceCounterAlgorithm;
    msca->setInput(dfms.output());
    TS_ASSERT_EQUALS(msca->update(), btk::Result::Success);
    auto outData = btk::make_shared_node(msca->output(0)->exportData());
    TS_ASSERT_EQUALS(outData->child<MySource*>(0)->counter(),6);
    TS_ASSERT_EQUALS(outData->parents().size(), 0ul);
    TS_ASSERT_EQUALS(outData->child(0)->parents().size(), 2ul);
    delete msca;
    TS_ASSERT_EQUALS(outData->child(0)->parents().size(), 1ul);
  };
  
  CXXTEST_TEST(NewInput)
  {
    btk::DataFactory<MySource> dfms("test");
    MySource* src = new MySource("counter");
    src->setCounter(5);
    dfms.output(0)->holdData(src);
    MySourceCounterAlgorithm msca1, msca2;
    msca1.setInput(dfms.output());
    msca2.setInput(msca1.output());
    TS_ASSERT_EQUALS(msca2.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(msca1.output(0)->data()->child<MySource*>(0)->counter(),6);
    TS_ASSERT_EQUALS(msca2.output(0)->data()->child<MySource*>(0)->counter(),7);
    
    MySource* src2 = new MySource("counter2");
    dfms.output(0)->holdData(src2);
    TS_ASSERT_EQUALS(msca2.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(msca1.output(0)->data()->child<MySource*>(0)->counter(),2);
    TS_ASSERT_EQUALS(msca2.output(0)->data()->child<MySource*>(0)->counter(),3);
    
    MySource* src3 = new MySource("counter3");
    src3->setCounter(5);
    dfms.output(0)->holdData(src3);
    TS_ASSERT_EQUALS(msca2.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(msca1.output(0)->data()->child<MySource*>(0)->counter(),6);
    TS_ASSERT_EQUALS(msca2.output(0)->data()->child<MySource*>(0)->counter(),7);
    
    MySource* src4 = new MySource("counter4");
    src4->setCounter(10);
    dfms.output(0)->holdData(src4);
    msca2.setIncrement(2);
    TS_ASSERT_EQUALS(msca2.update(), btk::Result::Success);
    TS_ASSERT_EQUALS(msca1.output(0)->data()->child<MySource*>(0)->counter(),11);
    TS_ASSERT_EQUALS(msca2.output(0)->data()->child<MySource*>(0)->counter(),13);
  };
  
  CXXTEST_TEST(FromFunction)
  {
    MySource* src = new MySource("counter");
    src->setCounter(3);
    auto out = incrementByTwo(src);
    TS_ASSERT_DIFFERS(out->child<MySource*>(0), nullptr);
    TS_ASSERT_EQUALS(out->child<MySource*>(0)->counter(),5);
    TS_ASSERT_EQUALS(out->child(0)->parents().size(), 1ul);
  };
};

CXXTEST_SUITE_REGISTRATION(AlgorithmTest)
CXXTEST_TEST_REGISTRATION(AlgorithmTest, DataFactory)
CXXTEST_TEST_REGISTRATION(AlgorithmTest, DataFactoryAssignedOutput)
CXXTEST_TEST_REGISTRATION(AlgorithmTest, CounterAlgorithm)
CXXTEST_TEST_REGISTRATION(AlgorithmTest, PipelineOne)
CXXTEST_TEST_REGISTRATION(AlgorithmTest, PipelineOne_LoopOnData)
CXXTEST_TEST_REGISTRATION(AlgorithmTest, PipelineTwo)
CXXTEST_TEST_REGISTRATION(AlgorithmTest, PipelineThree)
CXXTEST_TEST_REGISTRATION(AlgorithmTest, DeleteParent)
CXXTEST_TEST_REGISTRATION(AlgorithmTest, NewInput)
CXXTEST_TEST_REGISTRATION(AlgorithmTest, FromFunction)
  
#endif // AlgorithmTest_h