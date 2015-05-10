#ifndef EventTest_h
#define EventTest_h

#include <btkJoint.h>
#include <btkNode.h>
#include <btkSegment.h>

CXXTEST_SUITE(JointTest)
{
  CXXTEST_TEST(Constructor)
  {
    btk::Node root("root");
    btk::Segment prox("prox", 0, 0, &root);
    btk::Segment dist("dist", 0, 0, &root);
    btk::Joint joint("test",&prox,&dist,&root);
    
    TS_ASSERT_EQUALS(joint.name(), std::string("test"));
    
    TS_ASSERT_EQUALS(prox.parents().size(),2ul);
    TS_ASSERT_EQUALS(dist.parents().size(),2ul);
    TS_ASSERT_EQUALS(joint.proximalSegment(),&prox);
    TS_ASSERT_EQUALS(joint.distalSegment(),&dist);
  };
  
  CXXTEST_TEST(Mutator)
  {
    btk::Node root("root");
    btk::Segment prox("prox", 0, 0, &root);
    btk::Segment dist("dist", 0, 0, &root);
    btk::Joint joint("test",nullptr,nullptr,&root);
    
    TS_ASSERT_EQUALS(joint.proximalSegment(),nullptr);
    TS_ASSERT_EQUALS(joint.distalSegment(),nullptr);
    
    joint.setProximalSegment(&prox);
    joint.setDistalSegment(&dist);
    
    TS_ASSERT_EQUALS(prox.parents().size(),2ul);
    TS_ASSERT_EQUALS(dist.parents().size(),2ul);
    TS_ASSERT_EQUALS(joint.proximalSegment(),&prox);
    TS_ASSERT_EQUALS(joint.distalSegment(),&dist);
  };
  
  CXXTEST_TEST(Clone)
  {
    TS_WARN("TODO");
  };
};

CXXTEST_SUITE_REGISTRATION(JointTest)
CXXTEST_TEST_REGISTRATION(JointTest, Constructor)
CXXTEST_TEST_REGISTRATION(JointTest, Mutator)
CXXTEST_TEST_REGISTRATION(JointTest, Clone)
  
#endif // EventTest_h