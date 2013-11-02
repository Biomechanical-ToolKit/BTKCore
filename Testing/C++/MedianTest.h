#ifndef MedianTest_h
#define MedianTest_h

CXXTEST_SUITE(MedianTest)
{
  CXXTEST_TEST(VectorXd)
  {
    Eigen::Matrix<double, Eigen::Dynamic, 1> data;
    data.resize(4,1);  data << 1.0, 8.0, 5.0, 7.0;
    TS_ASSERT_DELTA(data.median(), 6.0, 1e-15);
    data.resize(5,1);  data << 1.0, 8.0, 5.0, 7.0, 3.0;
    TS_ASSERT_DELTA(data.median(), 5.0, 1e-15);
    data.resize(5,1);  data << 1.0, 8.0, 5.0, 7.0, 1.0;
    TS_ASSERT_DELTA(data.median(), 5.0, 1e-15);
    data.resize(5,1);  data << 1.0, 8.0, 5.0, 1.0, 1.0;
    TS_ASSERT_DELTA(data.median(), 1.0, 1e-15);
    data.resize(2,1);  data << 1.0, 8.0;
    TS_ASSERT_DELTA(data.median(), 4.5, 1e-15);
    data.resize(1,1);  data << 0.5;
    TS_ASSERT_DELTA(data.median(), 0.5, 1e-15);
  };
  
  CXXTEST_TEST(RowVectorXd)
  {
    Eigen::Matrix<double, 1, Eigen::Dynamic> data;
    data.resize(1,4);  data << 1.0, 8.0, 5.0, 7.0;
    TS_ASSERT_DELTA(data.median(), 6.0, 1e-15);
    data.resize(1,5);  data << 1.0, 8.0, 5.0, 7.0, 3.0;
    TS_ASSERT_DELTA(data.median(), 5.0, 1e-15);
    data.resize(1,5);  data << 1.0, 8.0, 5.0, 7.0, 1.0;
    TS_ASSERT_DELTA(data.median(), 5.0, 1e-15);
    data.resize(1,5);  data << 1.0, 8.0, 5.0, 1.0, 1.0;
    TS_ASSERT_DELTA(data.median(), 1.0, 1e-15);
    data.resize(1,2);  data << 1.0, 8.0;
    TS_ASSERT_DELTA(data.median(), 4.5, 1e-15);
    data.resize(1,1);  data << 0.5;
    TS_ASSERT_DELTA(data.median(), 0.5, 1e-15);
  };
  
  CXXTEST_TEST(MatrixXd_colwise)
  {
    Eigen::Matrix<double, Eigen::Dynamic, 3> data;
    data.resize(6,3);  data << 0.313803798336819, 6.732991328090547, 4.368957185030585, 1.160349385283249, 8.187858082704581, 1.625225008684635, 2.508550429013066, 9.489298045196573, 3.098208693314368, 7.596749456650356, 8.743153274054658, 6.811094223541295, 8.983096251825591, 3.937363024907196, 9.340549840356969, 2.234204874958241, 9.369623161748992, 9.473892316240427;
    Eigen::Matrix<double, 1, 3> m = data.colwise().median();
    TS_ASSERT_DELTA(m(0), 2.371377651985653, 1e-15);
    TS_ASSERT_DELTA(m(1), 8.465505678379619, 1e-15);
    TS_ASSERT_DELTA(m(2), 5.590025704285940, 1e-15);
  };
  
  CXXTEST_TEST(MatrixXd_rowwise)
  {
    Eigen::Matrix<double, 3, Eigen::Dynamic> data;
    data.resize(3, 6);  data << 0.313803798336819, 7.596749456650356, 6.732991328090547, 8.743153274054658, 4.368957185030585, 6.811094223541295, 1.160349385283249, 8.983096251825591, 8.187858082704581, 3.937363024907196, 1.625225008684635, 9.340549840356969, 2.508550429013066, 2.234204874958241, 9.489298045196573, 9.369623161748992, 3.098208693314368, 9.473892316240427;
    Eigen::Matrix<double, 3, 1> m = data.rowwise().median();
    TS_ASSERT_DELTA(m(0), 6.772042775815921, 1e-15);
    TS_ASSERT_DELTA(m(1), 6.062610553805889, 1e-15);
    TS_ASSERT_DELTA(m(2), 6.233915927531680, 1e-15);
  };
};

CXXTEST_SUITE_REGISTRATION(MedianTest)
CXXTEST_TEST_REGISTRATION(MedianTest, VectorXd)
CXXTEST_TEST_REGISTRATION(MedianTest, RowVectorXd)
CXXTEST_TEST_REGISTRATION(MedianTest, MatrixXd_colwise)
CXXTEST_TEST_REGISTRATION(MedianTest, MatrixXd_rowwise)
  
  

#endif // MedianTest_h