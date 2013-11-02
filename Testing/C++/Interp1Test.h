#ifndef CumtrapzTest_h
#define CumtrapzTest_h

#include <btkEigen/Interpolation/Interp1.h>

CXXTEST_SUITE(Interp1Test)
{
  CXXTEST_TEST(LinearXd)
  {
    Eigen::Matrix<double, Eigen::Dynamic, 1> x(3,1), y(3,1), xi(7,1), yi;
    x << 3.0, 6.0, 9.0;
    y << 6.0, 12.0, 18.0;
    xi << 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0;
    btkEigen::interp1().linear(&yi, x, y, xi);
    
    TS_ASSERT_EQUALS(yi.count(), 7);
    TS_ASSERT_EQUALS(yi.rows(), 7);
    TS_ASSERT_EQUALS(yi.cols(), 1);
    TS_ASSERT_DELTA(yi.coeff(0), 6.0, 1e-15);
    TS_ASSERT_DELTA(yi.coeff(1), 8.0, 1e-15);
    TS_ASSERT_DELTA(yi.coeff(2), 10.0, 1e-15);
    TS_ASSERT_DELTA(yi.coeff(3), 12.0, 1e-15);
    TS_ASSERT_DELTA(yi.coeff(4), 14.0, 1e-15);
    TS_ASSERT_DELTA(yi.coeff(5), 16.0, 1e-15);
    TS_ASSERT_DELTA(yi.coeff(6), 18.0, 1e-15);
  };
  
  CXXTEST_TEST(PchipXd_internals_sign)
  {
    Eigen::Matrix<double, Eigen::Dynamic, 1> x(10);
    x << 3.0, 6.0, -1.0, 44.12, 0.0, -0.27, -74.486, 0.0, 1.1, -110.0;
    Eigen::Matrix<double, Eigen::Dynamic, 1> ones(10); ones.setOnes();
    
    Eigen::Matrix<double, Eigen::Dynamic, 1> s = (x.array() > 0.0).select(ones,0.0) - (x.array() < 0.0).select(ones,0.0);
    
    TS_ASSERT_EQUALS(s.rows(), 10);
    TS_ASSERT_EQUALS(s.cols(), 1);
    TS_ASSERT_DELTA(s.coeff(0), 1.0, 1e-15);
    TS_ASSERT_DELTA(s.coeff(1), 1.0, 1e-15);
    TS_ASSERT_DELTA(s.coeff(2), -1.0, 1e-15);
    TS_ASSERT_DELTA(s.coeff(3), 1.0, 1e-15);
    TS_ASSERT_DELTA(s.coeff(4), 0.0, 1e-15);
    TS_ASSERT_DELTA(s.coeff(5), -1.0, 1e-15);
    TS_ASSERT_DELTA(s.coeff(6), -1.0, 1e-15);
    TS_ASSERT_DELTA(s.coeff(7), 0.0, 1e-15);
    TS_ASSERT_DELTA(s.coeff(8), 1.0, 1e-15);
    TS_ASSERT_DELTA(s.coeff(9), -1.0, 1e-15);
  };
  
  CXXTEST_TEST(PchipXd_internals_sign_bis)
  {
    Eigen::Matrix<double, Eigen::Dynamic, 1> x(10);
    x << 3.0, 6.0, -1.0, 44.12, 0.0, -0.27, -74.486, 0.0, 1.1, -110.0;
    
    Eigen::Matrix<double, Eigen::Dynamic, 1> s(10); s.setZero();
    s = (x.array() > 0.0).select(1.0,s) - (x.array() < 0.0).select(1.0,s);
    
    TS_ASSERT_EQUALS(s.rows(), 10);
    TS_ASSERT_EQUALS(s.cols(), 1);
    TS_ASSERT_DELTA(s.coeff(0), 1.0, 1e-15);
    TS_ASSERT_DELTA(s.coeff(1), 1.0, 1e-15);
    TS_ASSERT_DELTA(s.coeff(2), -1.0, 1e-15);
    TS_ASSERT_DELTA(s.coeff(3), 1.0, 1e-15);
    TS_ASSERT_DELTA(s.coeff(4), 0.0, 1e-15);
    TS_ASSERT_DELTA(s.coeff(5), -1.0, 1e-15);
    TS_ASSERT_DELTA(s.coeff(6), -1.0, 1e-15);
    TS_ASSERT_DELTA(s.coeff(7), 0.0, 1e-15);
    TS_ASSERT_DELTA(s.coeff(8), 1.0, 1e-15);
    TS_ASSERT_DELTA(s.coeff(9), -1.0, 1e-15);
  };
  
  CXXTEST_TEST(PchipXd_internals_condition)
  {
    Eigen::Matrix<double, Eigen::Dynamic, 1> x(10);
    x << 3.0, 6.0, -1.0, 44.12, 0.0, -0.27, -74.486, 0.0, 1.1, -110.0;
    
    Eigen::Matrix<double, Eigen::Dynamic, 1> smk(10); smk.setZero();
    smk = (x.array() > 0.0).select(1.0,smk) - (x.array() < 0.0).select(1.0,smk);
    
    Eigen::Matrix<double, Eigen::Dynamic, 1> condition(9); condition.setZero();
    condition = (smk.segment(0,9).array() != smk.segment(1,9).array()).select(1.0,condition) + (smk.segment(0,9).array() == 0).select(1.0,condition) + (smk.segment(1,9).array() == 0).select(1.0,condition);
    
    Eigen::Matrix<double, Eigen::Dynamic, 1> dk(9); dk.setConstant(5.0);
    dk = condition.select(0.0,dk);
    
    TS_ASSERT_EQUALS(condition.rows(), 9);
    TS_ASSERT_EQUALS(condition.cols(), 1);
    TS_ASSERT_DELTA(condition.coeff(0), 0.0, 1e-15);
    TS_ASSERT_DELTA(condition.coeff(1), 1.0, 1e-15);
    TS_ASSERT_DELTA(condition.coeff(2), 1.0, 1e-15);
    TS_ASSERT_DELTA(condition.coeff(3), 2.0, 1e-15);
    TS_ASSERT_DELTA(condition.coeff(4), 2.0, 1e-15);
    TS_ASSERT_DELTA(condition.coeff(5), 0.0, 1e-15);
    TS_ASSERT_DELTA(condition.coeff(6), 2.0, 1e-15);
    TS_ASSERT_DELTA(condition.coeff(7), 2.0, 1e-15);
    TS_ASSERT_DELTA(condition.coeff(8), 1.0, 1e-15);
    
    TS_ASSERT_DELTA(dk.coeff(0), 5.0, 1e-15);
    TS_ASSERT_DELTA(dk.coeff(1), 0.0, 1e-15);
    TS_ASSERT_DELTA(dk.coeff(2), 0.0, 1e-15);
    TS_ASSERT_DELTA(dk.coeff(3), 0.0, 1e-15);
    TS_ASSERT_DELTA(dk.coeff(4), 0.0, 1e-15);
    TS_ASSERT_DELTA(dk.coeff(5), 5.0, 1e-15);
    TS_ASSERT_DELTA(dk.coeff(6), 0.0, 1e-15);
    TS_ASSERT_DELTA(dk.coeff(7), 0.0, 1e-15);
    TS_ASSERT_DELTA(dk.coeff(8), 0.0, 1e-15);
  };
  
  CXXTEST_TEST(PchipXd_internals)
  {
    int num = 10;
    typedef Eigen::Matrix<double, Eigen::Dynamic, 1> VectorType;
    VectorType x(num), y(num);
    x << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0;
    y << -5.24537281, -0.23271432, -7.00403019, 8.8274458, 5.62689008, -8.90664265, -4.27131617, -2.59928097, 1.94098585, 1.2027007;
    
    VectorType hk = x.segment(1,num-1) - x.segment(0,num-1);
    VectorType mk = (y.segment(1,num-1) - y.segment(0,num-1)).cwiseQuotient(hk);
    VectorType w1 = 2.0 * hk.segment(1,num-2) + hk.segment(0,num-2);
    VectorType w2 = hk.segment(1,num-2) + 2.0 * hk.segment(0,num-2);
    VectorType whmean = (w1 + w2).cwiseInverse().cwiseProduct((w1.cwiseQuotient(mk.segment(1,num-2)) + w2.cwiseQuotient(mk.segment(0,num-2))));
    VectorType smk(num-1); smk.setZero(); smk = (mk.array() > 0.0).select(1.0, smk) - (mk.array() < 0.0).select(1.0, smk);
    VectorType condition(num-2); condition.setZero(); condition = (smk.segment(0,num-2).array() != smk.segment(1,num-2).array()).select(1.0,condition) + (smk.segment(0,num-2).array() == 0).select(1.0,condition) + (smk.segment(1,num-2).array() == 0).select(1.0,condition);
    VectorType dk(num);
    dk.segment(1,num-2) = condition.select(0.0, whmean.cwiseInverse());
    dk.coeffRef(0) = ((2.0*hk.coeff(0)+hk.coeff(1))*mk.coeff(0) - hk.coeff(0)*mk.coeff(1)) / (hk.coeff(0)+hk.coeff(1));
    double temp_dks = dk.coeff(0);
    if (sign(dk.coeff(0)) != sign(mk.coeff(0)))
      dk.coeffRef(0) = 0.0;
    else if ((sign(mk.coeff(0)) != sign(mk.coeff(1))) && (std::fabs(dk.coeff(0)) > std::fabs(3.0*mk.coeff(0))))
      dk.coeffRef(0) = 3.0*mk.coeff(0);
    dk.coeffRef(num-1) = ((2.0*hk.coeff(num-2)+hk.coeff(num-3))*mk.coeff(num-2) - hk.coeff(num-2)*mk.coeff(num-3)) / (hk.coeff(num-2)+hk.coeff(num-3));
    double temp_dke = dk.coeff(num-1);
    if (sign(dk.coeff(num-1)) != sign(mk.coeff(num-2)))
      dk.coeffRef(num-1) = 0.0;
    else if ((sign(mk.coeff(num-2)) != sign(mk.coeff(num-3))) && (std::fabs(dk.coeff(num-1)) > std::fabs(3.0*mk.coeff(num-2))))
      dk.coeffRef(num-1) = 3.0*mk.coeff(num-2);
    
    VectorType hk_(9), mk_(9), smk_(9), condition_(8), w1_(8), w2_(8), whmean_(8), dk_(10);
    hk_ << 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0;
    mk_ << 5.01265848, -6.77131586, 15.83147599, -3.20055572,-14.53353273,  4.63532648,  1.6720352 ,  4.54026682, -0.73828515;
    smk_ << 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0;
    condition_ << 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0;
    w1_ << 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0;
    w2_ << 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0;
    whmean_ << 0.02590658,-0.04225824,-0.12464022,-0.19062607, 0.07346407, 0.40690403, 0.40916245,-0.56711941;
    dk_ << 10.90464567, 0.0, 0.0, 0.0, -5.24587226, 0.0, 2.45758193, 2.44401702, 0.0, -2.21485545;
    
    TS_ASSERT_EQUALS(hk.rows(), hk_.rows());
    for (int i = 0 ; i < hk_.rows() ; ++i)
      TS_ASSERT_DELTA(hk.coeff(i), hk_.coeff(i), 1e-8);
    
    TS_ASSERT_EQUALS(mk.rows(), mk_.rows());
    for (int i = 0 ; i < mk_.rows() ; ++i)
      TS_ASSERT_DELTA(mk.coeff(i), mk_.coeff(i), 1e-8);
    
    TS_ASSERT_EQUALS(smk.rows(), smk_.rows());
    for (int i = 0 ; i < smk_.rows() ; ++i)
      TS_ASSERT_DELTA(smk.coeff(i), smk_.coeff(i), 1e-8);
    
    TS_ASSERT_EQUALS(condition.rows(), condition_.rows());
    for (int i = 0 ; i < condition_.rows() ; ++i)
      TS_ASSERT_DELTA(condition.coeff(i), condition_.coeff(i), 1e-8);
    
    TS_ASSERT_EQUALS(w1.rows(), w1_.rows());
    for (int i = 0 ; i < w1_.rows() ; ++i)
      TS_ASSERT_DELTA(w1.coeff(i), w1_.coeff(i), 1e-8);
    
    TS_ASSERT_EQUALS(w2.rows(), w2_.rows());
    for (int i = 0 ; i < w2_.rows() ; ++i)
      TS_ASSERT_DELTA(w2.coeff(i), w2_.coeff(i), 1e-8);
    
    TS_ASSERT_EQUALS(whmean.rows(), whmean_.rows());
    for (int i = 0 ; i < whmean_.rows() ; ++i)
      TS_ASSERT_DELTA(whmean.coeff(i), whmean_.coeff(i), 1e-8);
    
    TS_ASSERT_EQUALS(dk.rows(), dk_.rows());
    for (int i = 0 ; i < dk_.rows() ; ++i)
      TS_ASSERT_DELTA(dk.coeff(i), dk_.coeff(i), 1e-8);
    TS_ASSERT_DELTA(temp_dks, 10.90464567, 1e-8);
    TS_ASSERT_DELTA(temp_dke, -3.377561135, 1e-8);
  };
  
  CXXTEST_TEST(PchipXd)
  {
    Eigen::Matrix<double, Eigen::Dynamic, 1> x(10), y(10), xi(25), yi, yi_(25);
    x << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0;
    y << -5.245373, -0.232714, -7.004030, 8.827446, 5.626890, -8.906643, -4.271316, -2.599281, 1.940986, 1.202701;
    xi << 1.138631, 1.387214, 1.730132, 2.520910, 2.651600, 2.700595, 3.113019, 3.666887, 4.178427, 4.316361, 4.922727, 5.021054, 5.058313, 5.381125, 5.923080, 6.630567, 6.829714, 6.842039, 7.180979, 7.585501, 7.702235, 7.981414, 8.022047, 8.390746, 9.364474;
    yi_ << -3.861444, -1.987144, -0.551022, -3.830633, -5.110989, -5.546501, -6.443077, 4.727645, 8.695336, 8.428076, 6.026405, 5.501991, 5.213124, 0.137055, -8.690551, -6.062783, -4.916877, -4.857000, -3.894057, -3.323010, -3.161600, -2.643911, -2.541224, -0.706884, 1.905240;
    btkEigen::interp1().pchip(&yi, x, y, xi);
    
    TS_ASSERT_EQUALS(yi.rows(), yi_.rows());
    for (int i = 0 ; i < yi_.rows() ; ++i)
      TS_ASSERT_DELTA(yi_.coeff(i), yi_.coeff(i), 1e-6);
  };
  
  CXXTEST_TEST(PchipXd_bis)
  {
    Eigen::Matrix<double, Eigen::Dynamic, 1> x(10), y(10), xi(27), yi, yi_(27);
    x << 1.0, 2.0, 4.0, 5.0, 6.0, 7.0, 8.0, 10.0, 20.0, 25.0;
    y << -5.245373, -0.232714, -7.004030, 8.827446, 5.626890, -8.906643, -4.271316, -2.599281, 1.940986, 1.202701;
    xi << 1.0, 25.0, 8.352387, 13.204208, 13.258518, 20.623065, 20.075954, 16.463635, 10.086625, 20.477931, 13.787814, 9.417450, 23.536037, 22.022627, 14.203752, 15.939402, 15.089073, 5.985815, 8.229912, 12.302160, 6.531716, 21.263411, 5.674343, 6.422123, 5.096993, 6.463943, 11.456768;
    yi_ << -5.245373, 1.202701, -3.799015, -0.567421, -0.534354, 1.933117, 1.940876, 1.148358, -2.544639, 1.936426, -0.217731, -2.941939, 1.610474, 1.845948, 0.022799, 0.919982, 0.504631, 5.701120, -3.948408, -1.128858, -2.942033, 1.906469, 7.200952, -0.695340, 8.787522, -1.554554, -1.668538;
    btkEigen::interp1().pchip(&yi, x, y, xi);
    
    TS_ASSERT_EQUALS(yi.rows(), yi_.rows());
    for (int i = 0 ; i < yi_.rows() ; ++i)
      TS_ASSERT_DELTA(yi_.coeff(i), yi_.coeff(i), 1e-6);
  };
};

CXXTEST_SUITE_REGISTRATION(Interp1Test)
CXXTEST_TEST_REGISTRATION(Interp1Test, LinearXd)
CXXTEST_TEST_REGISTRATION(Interp1Test, PchipXd_internals_sign)
CXXTEST_TEST_REGISTRATION(Interp1Test, PchipXd_internals_sign_bis)
CXXTEST_TEST_REGISTRATION(Interp1Test, PchipXd_internals_condition)
CXXTEST_TEST_REGISTRATION(Interp1Test, PchipXd_internals)
CXXTEST_TEST_REGISTRATION(Interp1Test, PchipXd)
CXXTEST_TEST_REGISTRATION(Interp1Test, PchipXd_bis)

#endif // CumtrapzTest_h