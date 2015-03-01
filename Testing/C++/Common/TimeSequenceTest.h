#ifndef TimeSequenceTest_h
#define TimeSequenceTest_h

#include <btkTimeSequence.h>
#include <Eigen/Dense>

CXXTEST_SUITE(TimeSequenceTest)
{
  CXXTEST_TEST(Accessor)
  {
    btk::TimeSequence foo("1D4C_Signal(markers)",4,0,100.0,"mm",btk::TimeSequence::Marker);
    btk::TimeSequence toto("1D4C_Signal(markers)",4,10,100.0,"mm",btk::TimeSequence::Marker,0.125);
    btk::TimeSequence bar("1D1C_Signal(analog)",1,100,1000.0,"V",btk::TimeSequence::Analog);
    btk::TimeSequence bar2("2D64x64C_Signal(pressure)",{32,32},2,200.0,"Pa");
    
    TS_ASSERT_EQUALS(foo.data(),nullptr);
    TS_ASSERT_EQUALS(foo.name(),"1D4C_Signal(markers)");
    TS_ASSERT_EQUALS(foo.unit(),"mm");
    TS_ASSERT_EQUALS(foo.sampleRate(),100.0);
    TS_ASSERT_EQUALS(foo.startTime(),0.0);
    TS_ASSERT_EQUALS(foo.dimensions().size(),1ul);
    TS_ASSERT_EQUALS(foo.dimensions().front(),4);
    TS_ASSERT_EQUALS(foo.samples(),0);
    TS_ASSERT_EQUALS(foo.components(),4);
    TS_ASSERT_EQUALS(foo.elements(),0);
    TS_ASSERT_EQUALS(foo.duration(),0.0);
    
    TS_ASSERT_EQUALS(toto.startTime(),0.125);
    
    TS_ASSERT_DIFFERS(toto.data(),nullptr);
    TS_ASSERT_DIFFERS(bar.data(),nullptr);
    TS_ASSERT_EQUALS(toto.duration(),0.1);
    TS_ASSERT_EQUALS(toto.duration(),bar.duration());
    
    TS_ASSERT_EQUALS(bar2.dimensions().size(),2ul);
    TS_ASSERT_EQUALS(bar2.components(),1024);
    TS_ASSERT_EQUALS(bar2.elements(),2048);
    
    Eigen::Matrix<double,Eigen::Dynamic,3> rv = Eigen::Matrix<double,Eigen::Dynamic,3>::Random(10,3);
    Eigen::Map<Eigen::Matrix<double,Eigen::Dynamic,3>> values(toto.data(),toto.samples(),3);
    values = rv;
    // btk::TimeSequence::Values<btk::Dynamic,3> toto_values = toto.values<btk::Dynamic,3>();
    // btk::TimeSequence::Values<5,4,btk::OuterStride<>> toto_values_slice = toto.values<5,4>(btk::OuterStride<>(toto.samples()));    
    
    for (int i = 0 ; i < 30 ; ++i)
      TS_ASSERT_EQUALS(toto.data()[i],rv.data()[i]);
  };
};

CXXTEST_SUITE_REGISTRATION(TimeSequenceTest)
CXXTEST_TEST_REGISTRATION(TimeSequenceTest, Accessor)

#endif // TimeSequenceTest_h