#ifndef CumtrapzTest_h
#define CumtrapzTest_h

#include <btkEigen/Core/Cumtrapz.h>

CXXTEST_SUITE(CumtrapzTest)
{
  CXXTEST_TEST(ColumnVectorXd)
  {
    Eigen::Matrix<double, Eigen::Dynamic, 1> y(6,1), z;
    y << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0;
    z = btkEigen::cumtrapz(y);
    TS_ASSERT_DELTA(z.coeff(0), 0.0, 1e-15);
    TS_ASSERT_DELTA(z.coeff(1), 1.5, 1e-15);
    TS_ASSERT_DELTA(z.coeff(2), 4.0, 1e-15);
    TS_ASSERT_DELTA(z.coeff(3), 7.5, 1e-15);
    TS_ASSERT_DELTA(z.coeff(4), 12.0, 1e-15);
    TS_ASSERT_DELTA(z.coeff(5), 17.5, 1e-15);
  };
  
  CXXTEST_TEST(MatrixX2d)
  {
    Eigen::Matrix<double, Eigen::Dynamic, 2> y(6,2), z;
    y << 1.0, 7.0, 2.0, 8.0, 3.0, 9.0, 4.0, 10.0, 5.0, 11.0, 6.0, 12.0;
    z = btkEigen::cumtrapz(y);
    TS_ASSERT_DELTA(z.coeff(0,0), 0.0, 1e-15);
    TS_ASSERT_DELTA(z.coeff(1,0), 1.5, 1e-15);
    TS_ASSERT_DELTA(z.coeff(2,0), 4.0, 1e-15);
    TS_ASSERT_DELTA(z.coeff(3,0), 7.5, 1e-15);
    TS_ASSERT_DELTA(z.coeff(4,0), 12.0, 1e-15);
    TS_ASSERT_DELTA(z.coeff(5,0), 17.5, 1e-15);
    TS_ASSERT_DELTA(z.coeff(0,1), 0.0, 1e-15);
    TS_ASSERT_DELTA(z.coeff(1,1), 7.5, 1e-15);
    TS_ASSERT_DELTA(z.coeff(2,1), 16.0, 1e-15);
    TS_ASSERT_DELTA(z.coeff(3,1), 25.5, 1e-15);
    TS_ASSERT_DELTA(z.coeff(4,1), 36.0, 1e-15);
    TS_ASSERT_DELTA(z.coeff(5,1), 47.5, 1e-15);
  };
  
  CXXTEST_TEST(Matrix2Xd)
  {
    Eigen::Matrix<double, 2, Eigen::Dynamic> y(2,6), z;
    y << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0;
    z = btkEigen::cumtrapz(y);
    TS_ASSERT_DELTA(z.coeff(0,0), 0.0, 1e-15);
    TS_ASSERT_DELTA(z.coeff(0,1), 0.0, 1e-15);
    TS_ASSERT_DELTA(z.coeff(0,2), 0.0, 1e-15);
    TS_ASSERT_DELTA(z.coeff(0,3), 0.0, 1e-15);
    TS_ASSERT_DELTA(z.coeff(0,4), 0.0, 1e-15);
    TS_ASSERT_DELTA(z.coeff(0,5), 0.0, 1e-15);
    TS_ASSERT_DELTA(z.coeff(1,0), 4.0, 1e-15);
    TS_ASSERT_DELTA(z.coeff(1,1), 5.0, 1e-15);
    TS_ASSERT_DELTA(z.coeff(1,2), 6.0, 1e-15);
    TS_ASSERT_DELTA(z.coeff(1,3), 7.0, 1e-15);
    TS_ASSERT_DELTA(z.coeff(1,4), 8.0, 1e-15);
    TS_ASSERT_DELTA(z.coeff(1,5), 9.0, 1e-15);
  };
  
  CXXTEST_TEST(MatrixX2d_Col)
  {
    Eigen::Matrix<double, Eigen::Dynamic, 2> y(6,2);
    Eigen::Matrix<double, Eigen::Dynamic, 1> z1, z2;
    y << 1.0, 7.0, 2.0, 8.0, 3.0, 9.0, 4.0, 10.0, 5.0, 11.0, 6.0, 12.0;
    z1 = btkEigen::cumtrapz(y.col(0));
    z2 = btkEigen::cumtrapz(y.col(1));
    TS_ASSERT_DELTA(z1.coeff(0), 0.0, 1e-15);
    TS_ASSERT_DELTA(z1.coeff(1), 1.5, 1e-15);
    TS_ASSERT_DELTA(z1.coeff(2), 4.0, 1e-15);
    TS_ASSERT_DELTA(z1.coeff(3), 7.5, 1e-15);
    TS_ASSERT_DELTA(z1.coeff(4), 12.0, 1e-15);
    TS_ASSERT_DELTA(z1.coeff(5), 17.5, 1e-15);
    TS_ASSERT_DELTA(z2.coeff(0), 0.0, 1e-15);
    TS_ASSERT_DELTA(z2.coeff(1), 7.5, 1e-15);
    TS_ASSERT_DELTA(z2.coeff(2), 16.0, 1e-15);
    TS_ASSERT_DELTA(z2.coeff(3), 25.5, 1e-15);
    TS_ASSERT_DELTA(z2.coeff(4), 36.0, 1e-15);
    TS_ASSERT_DELTA(z2.coeff(5), 47.5, 1e-15);
  };
};

CXXTEST_SUITE_REGISTRATION(CumtrapzTest)
CXXTEST_TEST_REGISTRATION(CumtrapzTest, ColumnVectorXd)
CXXTEST_TEST_REGISTRATION(CumtrapzTest, MatrixX2d)
CXXTEST_TEST_REGISTRATION(CumtrapzTest, Matrix2Xd)
CXXTEST_TEST_REGISTRATION(CumtrapzTest, MatrixX2d_Col)

#endif // CumtrapzTest_h