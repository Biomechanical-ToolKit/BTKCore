#ifndef PrecentileTest_h
#define PrecentileTest_h

#include <algorithm> // std::sort

CXXTEST_SUITE(PrecentileTest)
{
  CXXTEST_TEST(VectorXd_p1)
  {
    Eigen::Matrix<double,Eigen::Dynamic,1> data;
    data.resize(4,1);  data << 1.0, 8.0, 5.0, 7.0;
    TS_ASSERT_DELTA(data.percentile(50.0), 6.0, 1e-15);
    TS_ASSERT_DELTA(data.percentile(0.0), 1.0, 1e-15);
    TS_ASSERT_DELTA(data.percentile(100.0), 8.0, 1e-15);
    
    data.resize(10,1); data << 44.847908873498511, 56.882229392661344, 4.386126294272817, 24.171297247003128, 17.289433662316060, 28.983967968375094, 20.506750835132479, 64.761435971631315, 8.705507758166213, 14.568222566626162;
    Eigen::Matrix<double,10,1> sorted = data; std::sort(sorted.data(),sorted.data()+10);
    TS_ASSERT_DELTA(data.percentile(0.0), sorted(0), 1e-15);
    TS_ASSERT_DELTA(data.percentile(5.0), sorted(0), 1e-15);
    TS_ASSERT_DELTA(data.percentile(15.0), sorted(1), 1e-15);
    TS_ASSERT_DELTA(data.percentile(25.0), sorted(2), 1e-15);
    TS_ASSERT_DELTA(data.percentile(35.0), sorted(3), 1e-15);
    TS_ASSERT_DELTA(data.percentile(45.0), sorted(4), 1e-15);
    TS_ASSERT_DELTA(data.percentile(55.0), sorted(5), 1e-15);
    TS_ASSERT_DELTA(data.percentile(65.0), sorted(6), 1e-15);
    TS_ASSERT_DELTA(data.percentile(75.0), sorted(7), 1e-15);
    TS_ASSERT_DELTA(data.percentile(85.0), sorted(8), 1e-15);
    TS_ASSERT_DELTA(data.percentile(95.0), sorted(9), 1e-15);
    TS_ASSERT_DELTA(data.percentile(100.0), sorted(9), 1e-15);
    TS_ASSERT_DELTA(data.percentile(89.0), 60.033912024249332, 1e-15);
    TS_ASSERT_DELTA(data.percentile(62.0), 27.540166751963504, 1e-15);
    TS_ASSERT_DELTA(data.percentile(40.0), 18.898092248724268, 1e-15);
    TS_ASSERT_DELTA(data.percentile(36.0), 17.611165379597701, 1e-15);
    TS_ASSERT_DELTA(data.percentile(65.0), 28.983967968375094, 1e-15);
    TS_ASSERT_DELTA(data.percentile(73.0), 41.675120692473833, 5e-15); // Minor difference compared to Matlab result
    TS_ASSERT_DELTA(data.percentile(73.0), 41.675120692473833, 5e-15); // Minor difference compared to Matlab result
    TS_ASSERT_DELTA(data.percentile(96.0), 64.761435971631315, 1e-15);
    TS_ASSERT_DELTA(data.percentile(05.0),  4.386126294272817, 1e-15);
    TS_ASSERT_DELTA(data.percentile(42.0), 19.541555683287552, 5e-15); // Minor difference compared to Matlab result
  };
  
  CXXTEST_TEST(RowVectorXd_p1)
  {
    Eigen::Matrix<double,1,Eigen::Dynamic> data;
    data.resize(1,4);  data << 1.0, 8.0, 5.0, 7.0;
    TS_ASSERT_DELTA(data.percentile(50.0), 6.0, 1e-15);
    TS_ASSERT_DELTA(data.percentile(0.0), 1.0, 1e-15);
    TS_ASSERT_DELTA(data.percentile(100.0), 8.0, 1e-15);
    
    data.resize(1,10); data << 44.847908873498511, 56.882229392661344, 4.386126294272817, 24.171297247003128, 17.289433662316060, 28.983967968375094, 20.506750835132479, 64.761435971631315, 8.705507758166213, 14.568222566626162;
    Eigen::Matrix<double,1,10> sorted = data; std::sort(sorted.data(),sorted.data()+10);
    TS_ASSERT_DELTA(data.percentile(0.0), sorted(0), 1e-15);
    TS_ASSERT_DELTA(data.percentile(5.0), sorted(0), 1e-15);
    TS_ASSERT_DELTA(data.percentile(15.0), sorted(1), 1e-15);
    TS_ASSERT_DELTA(data.percentile(25.0), sorted(2), 1e-15);
    TS_ASSERT_DELTA(data.percentile(35.0), sorted(3), 1e-15);
    TS_ASSERT_DELTA(data.percentile(45.0), sorted(4), 1e-15);
    TS_ASSERT_DELTA(data.percentile(55.0), sorted(5), 1e-15);
    TS_ASSERT_DELTA(data.percentile(65.0), sorted(6), 1e-15);
    TS_ASSERT_DELTA(data.percentile(75.0), sorted(7), 1e-15);
    TS_ASSERT_DELTA(data.percentile(85.0), sorted(8), 1e-15);
    TS_ASSERT_DELTA(data.percentile(95.0), sorted(9), 1e-15);
    TS_ASSERT_DELTA(data.percentile(100.0), sorted(9), 1e-15);
    TS_ASSERT_DELTA(data.percentile(89.0), 60.033912024249332, 1e-15);
    TS_ASSERT_DELTA(data.percentile(62.0), 27.540166751963504, 1e-15);
    TS_ASSERT_DELTA(data.percentile(40.0), 18.898092248724268, 1e-15);
    TS_ASSERT_DELTA(data.percentile(36.0), 17.611165379597701, 1e-15);
    TS_ASSERT_DELTA(data.percentile(65.0), 28.983967968375094, 1e-15);
    TS_ASSERT_DELTA(data.percentile(73.0), 41.675120692473833, 5e-15); // Minor difference compared to Matlab result
    TS_ASSERT_DELTA(data.percentile(73.0), 41.675120692473833, 5e-15); // Minor difference compared to Matlab result
    TS_ASSERT_DELTA(data.percentile(96.0), 64.761435971631315, 1e-15);
    TS_ASSERT_DELTA(data.percentile(05.0),  4.386126294272817, 1e-15);
    TS_ASSERT_DELTA(data.percentile(42.0), 19.541555683287552, 5e-15); // Minor difference compared to Matlab result
  };
  
  CXXTEST_TEST(VectorXd_p2)
  {
    Eigen::Matrix<double,Eigen::Dynamic,1> data;
    Eigen::Matrix<double,2,1> p;
      
    data.resize(4,1);  data << 1.0, 8.0, 5.0, 7.0;
    p = data.percentile(50.0, 0.0);
    TS_ASSERT_DELTA(p(0), 6.0, 1e-15);
    TS_ASSERT_DELTA(p(1), 1.0, 1e-15);
    p = data.percentile(0.0, 0.0);
    TS_ASSERT_DELTA(p(0), 1.0, 1e-15);
    TS_ASSERT_DELTA(p(1), 1.0, 1e-15);
    p = data.percentile(50.0, 50.0);
    TS_ASSERT_DELTA(p(0), 6.0, 1e-15);
    TS_ASSERT_DELTA(p(1), 6.0, 1e-15);
    p = data.percentile(0.0, 100.0);
    TS_ASSERT_DELTA(p(0), 1.0, 1e-15);
    TS_ASSERT_DELTA(p(1), 8.0, 1e-15);
    
    data.resize(10,1); data << 44.847908873498511, 56.882229392661344, 4.386126294272817, 24.171297247003128, 17.289433662316060, 28.983967968375094, 20.506750835132479, 64.761435971631315, 8.705507758166213, 14.568222566626162;
    p = data.percentile(25.0, 75.0);
    TS_ASSERT_DELTA(p(0), 14.568222566626162, 1e-15);
    TS_ASSERT_DELTA(p(1), 44.847908873498511, 1e-15);
    p = data.percentile(36.0, 73.0);
    TS_ASSERT_DELTA(p(0), 17.611165379597701, 1e-15);
    TS_ASSERT_DELTA(p(1), 41.675120692473833, 5e-15);
  };
  
  CXXTEST_TEST(RowVectorXd_p2)
  {
    Eigen::Matrix<double,1,Eigen::Dynamic> data;
    Eigen::Matrix<double,1,2> p;
      
    data.resize(1,4);  data << 1.0, 8.0, 5.0, 7.0;
    p = data.percentile(50.0, 0.0);
    TS_ASSERT_DELTA(p(0), 6.0, 1e-15);
    TS_ASSERT_DELTA(p(1), 1.0, 1e-15);
    p = data.percentile(0.0, 0.0);
    TS_ASSERT_DELTA(p(0), 1.0, 1e-15);
    TS_ASSERT_DELTA(p(1), 1.0, 1e-15);
    p = data.percentile(50.0, 50.0);
    TS_ASSERT_DELTA(p(0), 6.0, 1e-15);
    TS_ASSERT_DELTA(p(1), 6.0, 1e-15);
    p = data.percentile(0.0, 100.0);
    TS_ASSERT_DELTA(p(0), 1.0, 1e-15);
    TS_ASSERT_DELTA(p(1), 8.0, 1e-15);
    
    data.resize(1,10); data << 44.847908873498511, 56.882229392661344, 4.386126294272817, 24.171297247003128, 17.289433662316060, 28.983967968375094, 20.506750835132479, 64.761435971631315, 8.705507758166213, 14.568222566626162;
    p = data.percentile(25.0, 75.0);
    TS_ASSERT_DELTA(p(0), 14.568222566626162, 1e-15);
    TS_ASSERT_DELTA(p(1), 44.847908873498511, 1e-15);
    p = data.percentile(36.0, 73.0);
    TS_ASSERT_DELTA(p(0), 17.611165379597701, 1e-15);
    TS_ASSERT_DELTA(p(1), 41.675120692473833, 5e-15);
  };
  
  CXXTEST_TEST(Vector10d_p3)
  {
    Eigen::Matrix<double,10,1> data;
    Eigen::Matrix<double,3,1> p;
      
    data << 44.847908873498511, 56.882229392661344, 4.386126294272817, 24.171297247003128, 17.289433662316060, 28.983967968375094, 20.506750835132479, 64.761435971631315, 8.705507758166213, 14.568222566626162;
    p = data.percentile(25.0, 50.0, 75.0);
    TS_ASSERT_DELTA(p(0), 14.568222566626162, 1e-15);
    TS_ASSERT_DELTA(p(1), 22.339024041067802, 1e-15);
    TS_ASSERT_DELTA(p(2), 44.847908873498511, 1e-15);
    p = data.percentile(10.0, 50.0, 90.0);
    TS_ASSERT_DELTA(p(0), 6.545817026219515, 1e-15);
    TS_ASSERT_DELTA(p(1), 22.339024041067802, 1e-15);
    TS_ASSERT_DELTA(p(2), 60.821832682146329, 1e-15);
  };
  
  CXXTEST_TEST(Vector10d_pX)
  {
    Eigen::Matrix<double,10,1> data;
    Eigen::Matrix<double,Eigen::Dynamic,1> p, v(5,1);
      
    data << 44.847908873498511, 56.882229392661344, 4.386126294272817, 24.171297247003128, 17.289433662316060, 28.983967968375094, 20.506750835132479, 64.761435971631315, 8.705507758166213, 14.568222566626162;
    v << 0.0, 25.0, 50.0, 75.0, 100.0;
    p = data.percentile(v);
    TS_ASSERT_DELTA(p(0), 4.386126294272817, 1e-15);
    TS_ASSERT_DELTA(p(1), 14.568222566626162, 1e-15);
    TS_ASSERT_DELTA(p(2), 22.339024041067802, 1e-15);
    TS_ASSERT_DELTA(p(3), 44.847908873498511, 1e-15);
    TS_ASSERT_DELTA(p(4), 64.761435971631315, 1e-15);
    v << 89.0, 62.0, 40.0, 36.0, 65.0;
    p = data.percentile(v);
    TS_ASSERT_DELTA(p(0), 60.033912024249332, 1e-15);
    TS_ASSERT_DELTA(p(1), 27.540166751963504, 1e-15);
    TS_ASSERT_DELTA(p(2), 18.898092248724268, 1e-15);
    TS_ASSERT_DELTA(p(3), 17.611165379597701, 1e-15);
    TS_ASSERT_DELTA(p(4), 28.983967968375094, 1e-15);
  };
  
  CXXTEST_TEST(RowVector10d_pX)
  {
    Eigen::Matrix<double,1,10> data;
    Eigen::Matrix<double,1,Eigen::Dynamic> p, v(1,5);
      
    data << 44.847908873498511, 56.882229392661344, 4.386126294272817, 24.171297247003128, 17.289433662316060, 28.983967968375094, 20.506750835132479, 64.761435971631315, 8.705507758166213, 14.568222566626162;
    v << 0.0, 25.0, 50.0, 75.0, 100.0;
      p = data.percentile(v);
    TS_ASSERT_DELTA(p(0), 4.386126294272817, 1e-15);
    TS_ASSERT_DELTA(p(1), 14.568222566626162, 1e-15);
    TS_ASSERT_DELTA(p(2), 22.339024041067802, 1e-15);
    TS_ASSERT_DELTA(p(3), 44.847908873498511, 1e-15);
    TS_ASSERT_DELTA(p(4), 64.761435971631315, 1e-15);
    v << 89.0, 62.0, 40.0, 36.0, 65.0;
    p = data.percentile(v);
    TS_ASSERT_DELTA(p(0), 60.033912024249332, 1e-15);
    TS_ASSERT_DELTA(p(1), 27.540166751963504, 1e-15);
    TS_ASSERT_DELTA(p(2), 18.898092248724268, 1e-15);
    TS_ASSERT_DELTA(p(3), 17.611165379597701, 1e-15);
    TS_ASSERT_DELTA(p(4), 28.983967968375094, 1e-15);
  };
};

CXXTEST_SUITE_REGISTRATION(PrecentileTest)
CXXTEST_TEST_REGISTRATION(PrecentileTest, VectorXd_p1)
CXXTEST_TEST_REGISTRATION(PrecentileTest, RowVectorXd_p1)
CXXTEST_TEST_REGISTRATION(PrecentileTest, VectorXd_p2)
CXXTEST_TEST_REGISTRATION(PrecentileTest, RowVectorXd_p2)
CXXTEST_TEST_REGISTRATION(PrecentileTest, Vector10d_p3)
CXXTEST_TEST_REGISTRATION(PrecentileTest, Vector10d_pX)
CXXTEST_TEST_REGISTRATION(PrecentileTest, RowVector10d_pX)

#endif // PrecentileTest_h