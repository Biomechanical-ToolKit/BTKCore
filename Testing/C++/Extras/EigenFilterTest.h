#ifndef EigenFilterTest_h
#define EigenFilterTest_h

#include <btkEigen/SignalProcessing/Filter.h>
#include <btkEigen/SignalProcessing/IIRFilterDesign.h>

#include "EigenFilt_Util.h"

CXXTEST_SUITE(EigenFilterTest)
{
  CXXTEST_TEST(Butterworth_LowPass_2_0Dot5)
  {
    Eigen::Matrix<double,Eigen::Dynamic,1> x;
    generateRawData(x);
    Eigen::Matrix<double,Eigen::Dynamic,1> a,b;
    btkEigen::butter(&b, &a, 2, 0.5);
    Eigen::Matrix<double,Eigen::Dynamic,1> y = btkEigen::filter(b, a, x);
    
    Eigen::Matrix<double,Eigen::Dynamic,1> result(81,1);
    result << 0.078994393959063, 0.293680461518291, 0.609380263216319, 0.946431952108866, 1.149836173246444, 1.197611331288984, 1.232450178628516, 1.254621765667304, 1.019843119537853, 0.571983234713189, 
              0.269176503179677, 0.099607686508477,-0.172421820904401,-0.500779451295233,-0.711226206533147,-0.840222509725156,-0.830419999274584,-0.661291278146493,-0.488105261850835,-0.315180913270960,
              -0.112032247792902, 0.167090500562476, 0.476541594399677, 0.836334705655723, 1.201400420133043, 1.274854279299765, 1.140329566649642, 1.048678594787334, 0.966635438734780, 0.747496053757893, 
              0.404434589342732, 0.086235153615051,-0.205089573144154,-0.470258647788757,-0.726457650870339,-0.905243136643278,-0.828927377498028,-0.632743662037768,-0.504080465007461,-0.364170395195024,
              -0.154161885286635, 0.224808007840451, 0.665711683031317, 0.843881197919049, 0.919507586060895, 1.148025160831078, 1.334850948766730, 1.226109441098812, 0.895946914775277, 0.649412068757044,
              0.473242635616601, 0.147984746940540,-0.242538325450954,-0.576553839054406,-0.719547594074781,-0.641508996017767,-0.634892053949205,-0.672322742093992,-0.464487790441573,-0.131292243846836, 
              0.146262271061159, 0.404358040748561, 0.606842273082192, 0.750541284154185, 0.947178416749771, 1.090718719493036, 1.068618334381885, 1.080543912175484, 1.039055308050256, 0.741475258209657,
              0.417167318398736, 0.188711190787009,-0.095447600154962,-0.424576582058042,-0.675569990839884,-0.877678660751538,-0.915391589117641,-0.702574712465922,-0.498670536168534,-0.334128214554144,
              -0.061874352581106;
    
    TS_ASSERT_EQUALS(x.rows(), y.rows());
    TS_ASSERT_EQUALS(x.cols(), y.cols());
    for (int i = 0 ; i < x.rows() ; ++i)
      TSM_ASSERT_DELTA("Sample #" + std::to_string(i), y.coeff(i), result.coeff(i), 1e-15);
  };
  
  CXXTEST_TEST(FilterWindowAverage_NoInitState_FixedSize)
  {
    Eigen::Matrix<double, 16, 1> signal;
    signal << 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0;
    Eigen::Matrix<double, 5, 1> b = Eigen::Matrix<double, 5, 1>::Constant(0.2); // ones(5,1) / 5
    Eigen::Matrix<double, 1, 1> a; a << 1.0;
    signal = btkEigen::filter(b, a, signal);
    Eigen::Matrix<double, 16, 1> ref;
    ref << 0.20, 0.44, 0.72, 1.04, 1.40, 1.60, 1.80, 2.00, 2.20, 2.40, 2.60, 2.80, 3.00, 3.20, 3.40, 3.60;
    for (int i = 0 ; i < signal.rows() ; ++i)
    {
      TSM_ASSERT_DELTA("Row #" + std::to_string(i), signal(i), ref(i), 1e-15);
    }
  }
  
  CXXTEST_TEST(FilterOrder2_NoInitState_FixedSize)
  {
    Eigen::Matrix<double, 16, 1> signal;
    signal << 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4.0;
    Eigen::Matrix<double, 2, 1> b; b << 1.0, -1.0;
    Eigen::Matrix<double, 2, 1> a; a << 1.0, -0.995;
    signal = btkEigen::filter(b, a, signal);
    Eigen::Matrix<double, 16, 1> ref;
    ref << 1.0, 1.1950, 1.3890250, 1.5820798750, 1.7741694756250, 1.965298628246875, 2.155472135105641, 2.344694774430113, 2.532971300557961, 2.720306444055171, 2.906704911834896, 3.092171387275721, 3.276710530339342, 3.460326977687646, 3.643025342799207, 3.824810216085212;
    for (int i = 0 ; i < signal.rows() ; ++i)
    {
      TSM_ASSERT_DELTA("Row #" + std::to_string(i), signal(i), ref(i), 1e-15);
    }
  }
};

CXXTEST_SUITE_REGISTRATION(EigenFilterTest)
CXXTEST_TEST_REGISTRATION(EigenFilterTest, Butterworth_LowPass_2_0Dot5)
CXXTEST_TEST_REGISTRATION(EigenFilterTest, FilterWindowAverage_NoInitState_FixedSize)
CXXTEST_TEST_REGISTRATION(EigenFilterTest, FilterOrder2_NoInitState_FixedSize)

#endif // EigenFilterTest_h