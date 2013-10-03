#ifndef PointTest_h
#define PointTest_h

#include <btkPoint.h>

CXXTEST_SUITE(PointTest)
{
  CXXTEST_TEST(Constructor)
  {
    btk::Point::Pointer test = btk::Point::New("HEEL_R", 200);
    TS_ASSERT_EQUALS(test->GetLabel(), "HEEL_R");
    TS_ASSERT_EQUALS(test->GetDescription(), "");
    TS_ASSERT_EQUALS(test->GetValues().rows(), 200);
    TS_ASSERT_EQUALS(test->GetValues().cols(), 3);
    TS_ASSERT_EQUALS(test->GetResiduals().rows(), 200);
    TS_ASSERT_EQUALS(test->GetResiduals().cols(), 1);
    TS_ASSERT_EQUALS(test->GetType(), btk::Point::Marker);
  };
  
  CXXTEST_TEST(FrameNumber)
  {
    btk::Point::Pointer test = btk::Point::New("HEEL_R", 200);
    unsigned long int t1 = test->GetTimestamp();
    test->SetFrameNumber(225);
    TS_ASSERT_EQUALS(test->GetValues().rows(), 225);
    TS_ASSERT_EQUALS(test->GetResiduals().rows(), 225);
    TS_ASSERT(t1 != test->GetTimestamp());
  };
  
  CXXTEST_TEST(DataWithParent)
  {
    btk::Point::Pointer point = btk::Point::New("HEEL_R");
    btk::Point::Data::Pointer data = btk::Point::Data::New(5);
    point->SetData(data);
    TS_ASSERT(point->GetData() == data);
    data->GetValues().coeffRef(0) = 0.123;
    data->Modified();
    unsigned long int t1 = point->GetTimestamp();
    unsigned long int t2 = data->GetTimestamp();
    TS_ASSERT_EQUALS(point->GetValues().coeff(0), 0.123);
    TS_ASSERT(t1 > t2);
  };
  
  CXXTEST_TEST(DataWithoutParent)
  {
    btk::Point::Pointer point = btk::Point::New("HEEL_R");
    btk::Point::Data::Pointer data = btk::Point::Data::New(5);
    point->SetData(data, false);
    TS_ASSERT(point->GetData() == data);
    data->GetValues().coeffRef(0) = 0.123;
    data->Modified();
    unsigned long int t1 = point->GetTimestamp();
    unsigned long int t2 = data->GetTimestamp();
    TS_ASSERT_EQUALS(point->GetValues().coeff(0), 0.123);
    TS_ASSERT(t1 < t2);
  };
  
  CXXTEST_TEST(EigenDataFromMap)
  {
    double data[12] = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0};
    btk::Point::Values val = btk::Point::Values::Map(data,4,3);
    TS_ASSERT_EQUALS(val.coeff(2,2), 11.0);
    val.coeffRef(2,2) *= 2.0;
    TS_ASSERT_EQUALS(val.coeff(2,2), 22.0);
    TS_ASSERT_EQUALS(data[10], 11.0);
  };
  
  CXXTEST_TEST(EigenDataMapCopied)
  {
    double data[12] = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0};
    Eigen::Map<btk::Point::Values> val(data,4,3);
    TS_ASSERT_EQUALS(val.coeff(2,2), 11.0);
    btk::Point::Values assigned = val;
    TS_ASSERT_EQUALS(assigned.coeff(2,2), 11.0);
    assigned.coeffRef(2,2) *= 2.0;
    TS_ASSERT_EQUALS(val.coeff(2,2), 11.0);
    TS_ASSERT_EQUALS(assigned.coeff(2,2), 22.0);
  };
  
  CXXTEST_TEST(EigenDataRowMajorFromMap)
  {
    double data[12] = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0};    
    btk::Point::Values val = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajorBit>::Map(data,4,3);
    TS_ASSERT_EQUALS(val.coeff(2,2), 9.0);
    val.coeffRef(2,2) *= 2.0;
    TS_ASSERT_EQUALS(val.coeff(2,2), 18.0);
    TS_ASSERT_EQUALS(data[8], 9.0);
    TS_ASSERT_EQUALS(data[10], 11.0);
  };
  
  CXXTEST_TEST(EigenDataRowMajorFromMapSwap)
  {
    double data[12] = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0};    
    Eigen::Map< Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajorBit> > val = Eigen::Map< Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajorBit> >(data,4,3);
    btk::Point::Values assigned = btk::Point::Values::Zero(4,3);
    val.swap(assigned);
    TS_ASSERT_EQUALS(assigned.coeff(2,2), 9.0);
    assigned.coeffRef(2,2) *= 2.0;
    TS_ASSERT_EQUALS(assigned.coeff(2,2), 18.0);
    TS_ASSERT_EQUALS(data[8], 0.0);
    TS_ASSERT_EQUALS(data[10], 0.0);
  };
};

CXXTEST_SUITE_REGISTRATION(PointTest)
CXXTEST_TEST_REGISTRATION(PointTest, Constructor)
CXXTEST_TEST_REGISTRATION(PointTest, FrameNumber)
CXXTEST_TEST_REGISTRATION(PointTest, DataWithParent)
CXXTEST_TEST_REGISTRATION(PointTest, DataWithoutParent)
CXXTEST_TEST_REGISTRATION(PointTest, EigenDataFromMap)
CXXTEST_TEST_REGISTRATION(PointTest, EigenDataMapCopied)
CXXTEST_TEST_REGISTRATION(PointTest, EigenDataRowMajorFromMap)
CXXTEST_TEST_REGISTRATION(PointTest, EigenDataRowMajorFromMapSwap)
#endif
