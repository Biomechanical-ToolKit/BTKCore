#ifndef EigenIIRFilterDesignTest_h
#define EigenIIRFilterDesignTest_h

#include <btkEigen/SignalProcessing/IIRFilterDesign.h>

CXXTEST_SUITE(EigenIIRFilterDesignTest)
{
  CXXTEST_TEST(Butter_low_2_0Dot5)
  {
    Eigen::Matrix<double,Eigen::Dynamic,1> a,b;
    btkEigen::butter(&b, &a, 2, 0.5);
    TS_ASSERT_EQUALS(b.rows(), 3);
    TS_ASSERT_DELTA(b(0), 0.29289321881345243, 1e-15);
    TS_ASSERT_DELTA(b(1), 0.58578643762690485, 1e-15);
    TS_ASSERT_DELTA(b(2), 0.29289321881345243, 1e-15);
    TS_ASSERT_EQUALS(a.rows(), 3);
    TS_ASSERT_DELTA(a(0), 1.0, 1e-15);
    TS_ASSERT_DELTA(a(1), -1.3007071811330761e-16, 1e-15);
    TS_ASSERT_DELTA(a(2), 0.17157287525380988, 1e-15);
  };
  
  CXXTEST_TEST(Butter_low_5_0Dot1)
  {
    Eigen::Matrix<double,Eigen::Dynamic,1> a,b;
    btkEigen::butter(&b, &a, 5, 0.1);
    TS_ASSERT_EQUALS(b.rows(), 6);
    TS_ASSERT_DELTA(b(0), 5.9795780370003215e-05, 1e-15);
    TS_ASSERT_DELTA(b(1), 0.00029897890185001593, 1e-15);
    TS_ASSERT_DELTA(b(2), 0.00059795780370003196, 1e-15);
    TS_ASSERT_DELTA(b(3), 0.00059795780370003196, 1e-15);
    TS_ASSERT_DELTA(b(4), 0.00029897890185001593, 1e-15);
    TS_ASSERT_DELTA(b(5), 5.9795780370003215e-05, 1e-15);
    TS_ASSERT_EQUALS(a.rows(), 6);
    TS_ASSERT_DELTA(a(0), 1.0                 , 1e-15);
    TS_ASSERT_DELTA(a(1), -3.9845431196123355 , 5e-15); // Not exactly the same value than SciPy ... which is also not exactly the same than Matlab
    TS_ASSERT_DELTA(a(2), 6.4348670902758665  , 1e-14); // Not exactly the same value than SciPy ... which is also not exactly the same than Matlab 
    TS_ASSERT_DELTA(a(3), -5.2536151703522673 , 1e-14); // Not exactly the same value than SciPy ... which is also not exactly the same than Matlab
    TS_ASSERT_DELTA(a(4), 2.1651329097241305  , 1e-15);
    TS_ASSERT_DELTA(a(5), -0.35992824506355597, 1e-15);
  };
  
  CXXTEST_TEST(Butter_high_3_0Dot4)
  {
    Eigen::Matrix<double,Eigen::Dynamic,1> a,b;
    btkEigen::butter(&b, &a, 3, 0.4, btkEigen::HighPass);
    TS_ASSERT_EQUALS(b.rows(), 4);
    TS_ASSERT_DELTA(b(0),  0.2569156012484633 , 1e-15);
    TS_ASSERT_DELTA(b(1), -0.77074680374538984, 1e-15);
    TS_ASSERT_DELTA(b(2),  0.77074680374538984, 1e-15);
    TS_ASSERT_DELTA(b(3), -0.2569156012484633 , 1e-15);
    TS_ASSERT_EQUALS(a.rows(), 4);
    TS_ASSERT_DELTA(a(0),  1.0                 , 1e-15);
    TS_ASSERT_DELTA(a(1), -0.5772405248063025  , 1e-15);
    TS_ASSERT_DELTA(a(2),  0.42178704868956135 , 1e-15);
    TS_ASSERT_DELTA(a(3), -0.056297236491842595, 1e-15);
  };
  
  CXXTEST_TEST(Butter_pass_7_0Dot2_0Dot8)
  {
    Eigen::Matrix<double,Eigen::Dynamic,1> a,b;
    double wn[2] = {0.2, 0.8};
    btkEigen::butter(&b, &a, 7, wn, btkEigen::BandPass);
    TS_ASSERT_EQUALS(b.rows(), 15);
    TS_ASSERT_DELTA(b(0),  0.045311437068742688   , 1e-15);
    TS_ASSERT_DELTA(b(1),  0.0                    , 1e-15);
    TS_ASSERT_DELTA(b(2),  -0.31718005948119882   , 5e-14);
    TS_ASSERT_DELTA(b(3),  -2.7524238472248408e-16, 1e-15);
    TS_ASSERT_DELTA(b(4),  0.95154017844359873    , 5e-14);
    TS_ASSERT_DELTA(b(5),  1.5138331159736624e-15 , 5e-14);
    TS_ASSERT_DELTA(b(6),  -1.5859002974060057    , 1e-13);
    TS_ASSERT_DELTA(b(7),  -3.1308821262182565e-15, 5e-15);
    TS_ASSERT_DELTA(b(8),  1.5859002974060057     , 1e-13);
    TS_ASSERT_DELTA(b(9),  1.5138331159736624e-15 , 5e-15);
    TS_ASSERT_DELTA(b(10), -0.95154017844359873   , 5e-14);
    TS_ASSERT_DELTA(b(11), -2.7524238472248408e-16, 1e-15);
    TS_ASSERT_DELTA(b(12), 0.31718005948119882    , 5e-14);
    TS_ASSERT_DELTA(b(13), 0.0                    , 1e-15);
    TS_ASSERT_DELTA(b(14), -0.045311437068742688  , 1e-15);
    TS_ASSERT_EQUALS(a.rows(), 15);
    TS_ASSERT_DELTA(a(0),  1.0                    , 1e-15);
    TS_ASSERT_DELTA(a(1),  -1.2234040176410024e-15, 1e-15);
    TS_ASSERT_DELTA(a(2),  -1.3892801471571392    , 1e-13);
    TS_ASSERT_DELTA(a(3),  2.2987039786588711e-15 , 1e-14);
    TS_ASSERT_DELTA(a(4),  1.6750236175286821     , 5e-13);
    TS_ASSERT_DELTA(a(5),  1.2807372214118088e-14 , 1e-13);
    TS_ASSERT_DELTA(a(6),  -1.0538973163757603    , 1e-12); // Not so accurate? Is it due to the use of the comb function with double?
    TS_ASSERT_DELTA(a(7),  1.3056810625272839e-14 , 1e-13);
    TS_ASSERT_DELTA(a(8),  0.50855151093463624    , 5e-13); // Not so accurate?
    TS_ASSERT_DELTA(a(9),  -2.5520129858487823e-14, 1e-13);
    TS_ASSERT_DELTA(a(10), -0.14482945329972546   , 5e-13);
    TS_ASSERT_DELTA(a(11), 3.1717384177005001e-15 , 1e-14); // Note: With Eigen3 the result is negative: -3.53945e-15
    TS_ASSERT_DELTA(a(12), 0.026252219253782492   , 1e-14);
    TS_ASSERT_DELTA(a(13), -1.8499567508325164e-16, 1e-15);
    TS_ASSERT_DELTA(a(14), -0.00202968024924362   , 1e-15);
  };
  
  CXXTEST_TEST(Butter_stop_3_0Dot5_0Dot6)
  {
    Eigen::Matrix<double,Eigen::Dynamic,1> a,b;
    double wn[2] = {0.5, 0.6};
    btkEigen::butter(&b, &a, 3, wn, btkEigen::BandStop);
    TS_ASSERT_EQUALS(b.rows(), 7);
    TS_ASSERT_DELTA(b(0), 0.72944072263908233, 1e-15);
    TS_ASSERT_DELTA(b(1), 0.69319236363069792, 1e-15);
    TS_ASSERT_DELTA(b(2), 2.4079039370190274 , 5e-15);
    TS_ASSERT_DELTA(b(3), 1.4095702843311675 , 5e-15);
    TS_ASSERT_DELTA(b(4), 2.4079039370190274 , 5e-15);
    TS_ASSERT_DELTA(b(5), 0.69319236363069758, 1e-15);
    TS_ASSERT_DELTA(b(6), 0.72944072263908233, 1e-15);
    TS_ASSERT_EQUALS(a.rows(), 7);
    TS_ASSERT_DELTA(a(0), 1.0                , 1e-15);
    TS_ASSERT_DELTA(a(1), 0.85117298823343734, 1e-15);
    TS_ASSERT_DELTA(a(2), 2.6168620698809031 , 1e-13);
    TS_ASSERT_DELTA(a(3), 1.3863847272613972 , 5e-15);
    TS_ASSERT_DELTA(a(4), 2.1257518811232239 , 5e-15);
    TS_ASSERT_DELTA(a(5), 0.55839729609772959, 1e-15);
    TS_ASSERT_DELTA(a(6), 0.53207536831209168, 1e-15);
  };

  // ----------------------------------------------------------------------- //

  CXXTEST_TEST(Internal_buttap_2)
  {
    Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1> z, p;
    double k;
    btkEigen::buttap(&z, &p, &k,2);
    TS_ASSERT_EQUALS(z.rows(), 0);
    TS_ASSERT_EQUALS(p.rows(), 2);
    TS_ASSERT_DELTA(p(0).real(), -0.70710678118654746, 1e-15);
    TS_ASSERT_DELTA(p(0).imag(), 0.70710678118654757, 1e-15);
    TS_ASSERT_DELTA(p(1).real(), -0.70710678118654757, 1e-15);
    TS_ASSERT_DELTA(p(1).imag(), -0.70710678118654746, 1e-15);
    TS_ASSERT_EQUALS(k,1.0);
  };
  
  CXXTEST_TEST(Internal_buttap_3)
  {
    Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1> z, p;
    double k;
    btkEigen::buttap(&z, &p, &k,3);
    TS_ASSERT_EQUALS(z.rows(), 0);
    TS_ASSERT_EQUALS(p.rows(), 3);
    TS_ASSERT_DELTA(p(0).real(), -0.49999999999999994, 1e-15);
    TS_ASSERT_DELTA(p(0).imag(), 0.86602540378443871, 1e-15);
    TS_ASSERT_DELTA(p(1).real(), -1.0, 1e-15);
    TS_ASSERT_DELTA(p(1).imag(), 6.123233995736766e-17, 1e-15);
    TS_ASSERT_DELTA(p(2).real(), -0.50000000000000033, 1e-15);
    TS_ASSERT_DELTA(p(2).imag(), -0.86602540378443849, 1e-15);
    TS_ASSERT_EQUALS(k,1.0);
  };
  
  CXXTEST_TEST(Internal_poly)
  {
    Eigen::Matrix<double, Eigen::Dynamic, 1> a(3), b;
    a << -0.5, 0.0, 0.5;
    btkEigen::poly(&b,a);
    TS_ASSERT_EQUALS(b.rows(), 4);
    TS_ASSERT_DELTA(b(0), 1.0, 1e-15);
    TS_ASSERT_DELTA(b(1), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(2), -0.25, 1e-15);
    TS_ASSERT_DELTA(b(3), 0.0, 1e-15);
  };
  
  CXXTEST_TEST(Internal_zpk2tf_identity)
  {
    typedef Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1> Coeffs;
    Coeffs z, p, a, b;
    Coeffs a_r = Coeffs::Constant(1, std::complex<double>(1.0, 0.0));
    Coeffs b_r = a_r;
    double k = 1.0;
    btkEigen::zpk2tf(&b, &a, z, p, k);
    TS_ASSERT_EQUALS(b.rows(), b_r.rows());
    TS_ASSERT(b.isApprox(b_r, 1e-15));
    TS_ASSERT_EQUALS(a.rows(), a_r.rows());
    TS_ASSERT(a.isApprox(a_r, 1e-15));
  };
  
  CXXTEST_TEST(Internal_zpk2tf_bis)
  {
    Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1> z, p, a, b;
    double k;
    btkEigen::buttap(&z, &p, &k, 3);
    btkEigen::zpk2tf(&b, &a, z, p, k);
    TS_ASSERT_EQUALS(b.rows(), 1);
    TS_ASSERT_DELTA(b(0).real(), 1.0, 1e-15);
    TS_ASSERT_EQUALS(a.rows(), 4);
    TS_ASSERT_DELTA(a(0).real(), 1.0, 1e-15);
    TS_ASSERT_DELTA(a(0).imag(), 0.0, 1e-15);
    TS_ASSERT_DELTA(a(1).real(), 2.0000000000000004, 1e-15);
    TS_ASSERT_DELTA(a(1).imag(), -3.3306690738754696e-16, 1e-15);
    TS_ASSERT_DELTA(a(2).real(), 2.0000000000000004, 1e-15);
    TS_ASSERT_DELTA(a(2).imag(), -5.5511151231257827e-16, 1e-15);
    TS_ASSERT_DELTA(a(3).real(), 1.0, 1e-15);
    TS_ASSERT_DELTA(a(3).imag(), -4.9960036108132044e-16, 1e-15);
  };
  
  CXXTEST_TEST(Internal_normalize)
  {
    // Test for false positive on allclose in normalize() in filter_design.py
    // Test to make sure the allclose call within signal.normalize does not
    // choose false positives. Then check against a known output from MATLAB
    // to make sure the fix doesn't break anything.

    // These are the coefficients returned from `[b,a] = cheby1(8, 0.5, 0.048)'
    // in MATLAB. There are at least 15 significant figures in each
    // coefficient, so it makes sense to test for errors on the order of
    // 1e-13 (this can always be relaxed if different platforms have
    // different rounding errors)
    Eigen::Matrix<double, Eigen::Dynamic, 1> b_matlab(9), a_matlab(9), b_norm_in(9), a_norm_in(9);
    b_matlab  << 2.150733144728282e-11, 1.720586515782626e-10,
                 6.022052805239190e-10, 1.204410561047838e-09,
                 1.505513201309798e-09, 1.204410561047838e-09,
                 6.022052805239190e-10, 1.720586515782626e-10,
                 2.150733144728282e-11;
    a_matlab << 1.000000000000000e+00, -7.782402035027959e+00,
                2.654354569747454e+01, -5.182182531666387e+01,
                6.334127355102684e+01, -4.963358186631157e+01,
                2.434862182949389e+01, -6.836925348604676e+00,
                8.412934944449140e-01;
    // This is the input to signal.normalize after passing through the
    // equivalent steps in signal.iirfilter as was done for MATLAB
    b_norm_in << 1.5543135865293012e-06, 1.2434508692234413e-05,
                 4.3520780422820447e-05, 8.7041560845640893e-05,
                 1.0880195105705122e-04, 8.7041560845640975e-05,
                 4.3520780422820447e-05, 1.2434508692234413e-05,
                 1.5543135865293012e-06;
    a_norm_in << 7.2269025909127173e+04, -5.6242661430467968e+05,
                 1.9182761917308895e+06, -3.7451128364682454e+06,
                 4.5776121393762771e+06, -3.5869706138592605e+06,
                 1.7596511818472347e+06, -4.9409793515707983e+05,
                 6.0799461347219651e+04;

    btkEigen::normalize(&b_norm_in, &a_norm_in);

    // The test on b works for decimal=14 but the one for a does not. For
    // the sake of consistency, both of these are decimal=13. If something
    // breaks on another platform, it is probably fine to relax this lower.
    TS_ASSERT_EQUALS(b_matlab.rows(), b_norm_in.rows());
    for (int i = 0 ; i < 9 ; ++i)
      TS_ASSERT_DELTA(b_matlab.coeff(i), b_norm_in.coeff(i), 1e-13);
    TS_ASSERT_EQUALS(a_matlab.rows(), a_norm_in.rows());
    for (int i = 0 ; i < 9 ; ++i)
      TS_ASSERT_DELTA(a_matlab.coeff(i), a_norm_in.coeff(i), 1e-13);
  };
  
  CXXTEST_TEST(Internal_lp2lp_default)
  {
    Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1> z, p, a, b;
    double k;
    btkEigen::buttap(&z, &p, &k, 3);
    btkEigen::zpk2tf(&b, &a, z, p, k);
    btkEigen::lp2lp(&b, &a);
    TS_ASSERT_EQUALS(b.rows(), 1);
    TS_ASSERT_DELTA(b(0).real(), 1.0, 1e-15);
    TS_ASSERT_DELTA(b(0).real(), 1.0, 1e-15);
    TS_ASSERT_EQUALS(a.rows(), 4);
    TS_ASSERT_DELTA(a(0).real(), 1.0, 1e-15);
    TS_ASSERT_DELTA(a(0).imag(), 0.0, 1e-15);
    TS_ASSERT_DELTA(a(1).real(), 2.0000000000000004, 1e-15);
    TS_ASSERT_DELTA(a(1).imag(), -3.3306690738754696e-16, 1e-15);
    TS_ASSERT_DELTA(a(2).real(), 2.0000000000000004, 1e-15);
    TS_ASSERT_DELTA(a(2).imag(), -5.5511151231257827e-16, 1e-15);
    TS_ASSERT_DELTA(a(3).real(), 1.0, 1e-15);
    TS_ASSERT_DELTA(a(3).imag(), -4.9960036108132044e-16, 1e-15);
  };
  
  CXXTEST_TEST(Internal_lp2lp_half)
  {
    const double wo = 0.5;
    Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1> z, p, a, b;
    double k;
    btkEigen::buttap(&z, &p, &k, 3);
    btkEigen::zpk2tf(&b, &a, z, p, k);
    btkEigen::lp2lp(&b, &a, wo);
    TS_ASSERT_EQUALS(b.rows(), 1);
    TS_ASSERT_DELTA(b(0).real(), 0.125, 1e-15);
    TS_ASSERT_DELTA(b(0).imag(), 0.0, 1e-15);
    TS_ASSERT_EQUALS(a.rows(), 4);
    TS_ASSERT_DELTA(a(0).real(), 1.0, 1e-15);
    TS_ASSERT_DELTA(a(0).imag(), 0.0, 1e-15);
    TS_ASSERT_DELTA(a(1).real(), 1.0000000000000004, 1e-15);
    TS_ASSERT_DELTA(a(1).imag(), -1.6653345369377348e-16, 1e-15);
    TS_ASSERT_DELTA(a(2).real(), 0.50000000000000011, 1e-15);
    TS_ASSERT_DELTA(a(2).imag(), -1.3877787807814457e-16, 1e-15);
    TS_ASSERT_DELTA(a(3).real(), 0.125, 1e-15);
    TS_ASSERT_DELTA(a(3).imag(), -6.2450045135165055e-17, 1e-15);
  };
  
  CXXTEST_TEST(Internal_lp2hp_thirth)
  {
    const double wo = 0.33;
    Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1> z, p, a, b;
    double k;
    btkEigen::buttap(&z, &p, &k, 4);
    btkEigen::zpk2tf(&b, &a, z, p, k);
    btkEigen::lp2hp(&b, &a, wo);
    TS_ASSERT_EQUALS(b.rows(), 5);
    TS_ASSERT_DELTA(b(0).real(), 1.0, 1e-15);
    TS_ASSERT_DELTA(b(0).imag(), 1.6653345369377348e-16, 1e-15);
    TS_ASSERT_DELTA(b(1).real(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(1).imag(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(2).real(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(2).imag(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(3).real(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(3).imag(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(4).real(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(4).imag(), 0.0, 1e-15);
    TS_ASSERT_EQUALS(a.rows(), 5);
    TS_ASSERT_DELTA(a(0).real(), 1.0, 1e-15);
    TS_ASSERT_DELTA(a(0).imag(), 0.0, 1e-15);
    TS_ASSERT_DELTA(a(1).real(), 0.86233155681840856, 1e-15);
    TS_ASSERT_DELTA(a(1).imag(), 1.0696969257346787e-16, 1e-15);
    TS_ASSERT_DELTA(a(2).real(), 0.37180785694243013, 1e-15);
    TS_ASSERT_DELTA(a(2).imag(), 6.1918446527103342e-17, 1e-15);
    TS_ASSERT_DELTA(a(3).real(), 0.093907906537524719, 1e-15);
    TS_ASSERT_DELTA(a(3).imag(), 1.1648999521250655e-17, 1e-15);
    TS_ASSERT_DELTA(a(4).real(), 0.011859210000000002, 1e-15);
    TS_ASSERT_DELTA(a(4).imag(), 1.9749551993797357e-18, 1e-15);
  };
  
  CXXTEST_TEST(Internal_lp2bp_default)
  {
    Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1> z, p, a, b;
    double k;
    btkEigen::buttap(&z, &p, &k, 5);
    btkEigen::zpk2tf(&b, &a, z, p, k);
    btkEigen::lp2bp(&b, &a);
    TS_ASSERT_EQUALS(b.rows(), 6);
    TS_ASSERT_DELTA(b(0).real(), 1.0, 1e-15);
    TS_ASSERT_DELTA(b(0).imag(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(1).real(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(1).imag(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(2).real(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(2).imag(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(3).real(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(3).imag(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(4).real(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(4).imag(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(5).real(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(5).imag(), 0.0, 1e-15);
    TS_ASSERT_EQUALS(a.rows(), 11);
    TS_ASSERT_DELTA(a(0).real(),  1.0, 1e-15);
    TS_ASSERT_DELTA(a(0).imag(),  0.0, 1e-15);
    TS_ASSERT_DELTA(a(1).real(),  3.2360679774997898, 1e-15);
    TS_ASSERT_DELTA(a(1).imag(),  0.0, 1e-15);
    TS_ASSERT_DELTA(a(2).real(),  10.236067977499788, 1e-15);
    TS_ASSERT_DELTA(a(2).imag(), -4.4408920985006262e-16, 1e-15);
    TS_ASSERT_DELTA(a(3).real(),  18.180339887498953, 1e-14);
    TS_ASSERT_DELTA(a(3).imag(), -4.4408920985006262e-16, 5e-15);
    TS_ASSERT_DELTA(a(4).real(),  28.944271909999152, 1e-14);
    TS_ASSERT_DELTA(a(4).imag(), -1.8873791418627661e-15, 5e-15);
    TS_ASSERT_DELTA(a(5).real(),  30.888543819998318, 1e-14);
    TS_ASSERT_DELTA(a(5).imag(), -1.0547118733938987e-15, 5e-15);
    TS_ASSERT_DELTA(a(6).real(),  28.944271909999152, 1e-14);
    TS_ASSERT_DELTA(a(6).imag(), -1.8873791418627661e-15, 5e-15);
    TS_ASSERT_DELTA(a(7).real(),  18.180339887498953, 1e-14);
    TS_ASSERT_DELTA(a(7).imag(), -4.4408920985006262e-16, 5e-15);
    TS_ASSERT_DELTA(a(8).real(),  10.236067977499788, 1e-15);
    TS_ASSERT_DELTA(a(8).imag(), -4.4408920985006262e-16, 1e-15);
    TS_ASSERT_DELTA(a(9).real(),  3.2360679774997898, 1e-15);
    TS_ASSERT_DELTA(a(9).imag(),  0.0, 1e-15);
    TS_ASSERT_DELTA(a(10).real(), 1.0, 1e-15);
    TS_ASSERT_DELTA(a(10).imag(), 0.0, 1e-15);
  };
  
  CXXTEST_TEST(Internal_lp2bp)
  {
    Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1> z, p, a, b;
    double k;
    btkEigen::buttap(&z, &p, &k, 5);
    btkEigen::zpk2tf(&b, &a, z, p, k);
    btkEigen::lp2bp(&b, &a, 0.3, 0.8);
    TS_ASSERT_EQUALS(b.rows(), 6);
    TS_ASSERT_DELTA(b(0).real(), 0.32768, 1e-15);
    TS_ASSERT_DELTA(b(0).imag(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(1).real(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(1).imag(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(2).real(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(2).imag(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(3).real(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(3).imag(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(4).real(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(4).imag(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(5).real(), 0.0, 1e-15);
    TS_ASSERT_DELTA(b(5).imag(), 0.0, 1e-15);
    TS_ASSERT_EQUALS(a.rows(), 11);
    TS_ASSERT_DELTA(a(0).real(),  1.0, 1e-15);
    TS_ASSERT_DELTA(a(0).imag(),  0.0, 1e-15);
    TS_ASSERT_DELTA(a(1).real(),  2.5888543819998318, 1e-15);
    TS_ASSERT_DELTA(a(1).imag(),  0.0, 1e-15);
    TS_ASSERT_DELTA(a(2).real(),  3.8010835055998653, 5e-15);
    TS_ASSERT_DELTA(a(2).imag(), -2.8421709430404013e-16, 1e-15);
    TS_ASSERT_DELTA(a(3).real(),  3.6128543819998318, 5e-15);
    TS_ASSERT_DELTA(a(3).imag(), -2.2737367544323206e-16, 1e-15);
    TS_ASSERT_DELTA(a(4).real(),  2.311285990095878, 1e-15);
    TS_ASSERT_DELTA(a(4).imag(), -3.0411229090532291e-16, 1e-15);
    TS_ASSERT_DELTA(a(5).real(),  0.93605434777157248, 1e-15);
    TS_ASSERT_DELTA(a(5).imag(), -9.5496943686157494e-17, 1e-15);
    TS_ASSERT_DELTA(a(6).real(),  0.20801573910862897, 1e-15);
    TS_ASSERT_DELTA(a(6).imag(), -2.7370106181479062e-17, 1e-15);
    TS_ASSERT_DELTA(a(7).real(),  0.029264120494198641, 1e-15);
    TS_ASSERT_DELTA(a(7).imag(), -1.8417267710901798e-18, 1e-15);
    TS_ASSERT_DELTA(a(8).real(),  0.0027709898755823013, 1e-15);
    TS_ASSERT_DELTA(a(8).imag(), -2.0719426174764523e-19, 1e-15);
    TS_ASSERT_DELTA(a(9).real(),  0.00016985473600300896, 1e-15);
    TS_ASSERT_DELTA(a(9).imag(),  0.0, 1e-15);
    TS_ASSERT_DELTA(a(10).real(), 5.9048999999999991e-06, 1e-15);
    TS_ASSERT_DELTA(a(10).imag(), 0.0, 1e-15);
  };
  
  CXXTEST_TEST(Internal_lp2bs)
  {
    Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1> z, p, a, b;
    double k;
    btkEigen::buttap(&z, &p, &k, 5);
    btkEigen::zpk2tf(&b, &a, z, p, k);
    btkEigen::lp2bs(&b, &a, 0.3, 0.8);
    TS_ASSERT_EQUALS(b.rows(), 11);
    TS_ASSERT_DELTA(b(0).real(),  1.0000000000000002    , 1e-15);
    TS_ASSERT_DELTA(b(0).imag(),  1.6653345369377356e-16, 1e-15);
    TS_ASSERT_DELTA(b(1).real(),  0.0                   , 1e-15);
    TS_ASSERT_DELTA(b(1).imag(),  0.0                   , 1e-15);
    TS_ASSERT_DELTA(b(2).real(),  0.44999999999999996   , 1e-15);
    TS_ASSERT_DELTA(b(2).imag(),  7.4940054162198059e-17, 1e-15);
    TS_ASSERT_DELTA(b(3).real(),  0.0                   , 1e-15);
    TS_ASSERT_DELTA(b(3).imag(),  0.0                   , 1e-15);
    TS_ASSERT_DELTA(b(4).real(),  0.080999999999999989  , 1e-15);
    TS_ASSERT_DELTA(b(4).imag(),  1.3489209749195653e-17, 1e-15);
    TS_ASSERT_DELTA(b(5).real(),  0.0                   , 1e-15);
    TS_ASSERT_DELTA(b(5).imag(),  0.0                   , 1e-15);
    TS_ASSERT_DELTA(b(6).real(),  0.0072899999999999996 , 1e-15);
    TS_ASSERT_DELTA(b(6).imag(),  1.2140288774276087e-18, 1e-15);
    TS_ASSERT_DELTA(b(7).real(),  0.0                   , 1e-15);
    TS_ASSERT_DELTA(b(7).imag(),  0.0                   , 1e-15);
    TS_ASSERT_DELTA(b(8).real(),  0.00032804999999999987, 1e-15);
    TS_ASSERT_DELTA(b(8).imag(),  5.4631299484242381e-20, 1e-15);
    TS_ASSERT_DELTA(b(9).real(),  0.0                   , 1e-15);
    TS_ASSERT_DELTA(b(9).imag(),  0.0                   , 1e-15);
    TS_ASSERT_DELTA(b(10).real(), 5.9049000000000008e-06, 1e-15);
    TS_ASSERT_DELTA(b(10).imag(), 9.8336339071636331e-22, 1e-15);
    TS_ASSERT_EQUALS(a.rows(), 11);
    TS_ASSERT_DELTA(a(0).real(),  1.0                    , 1e-15);
    TS_ASSERT_DELTA(a(0).imag(),  0.0                    , 1e-15);
    TS_ASSERT_DELTA(a(1).real(),  2.5888543819998322     , 1e-15);
    TS_ASSERT_DELTA(a(1).imag(),  -1.2958348505371001e-17, 1e-15);
    TS_ASSERT_DELTA(a(2).real(),  3.8010835055998666     , 1e-15);
    TS_ASSERT_DELTA(a(2).imag(),  2.7385041549974336e-16 , 1e-15);
    TS_ASSERT_DELTA(a(3).real(),  3.6128543819998336     , 1e-15);
    TS_ASSERT_DELTA(a(3).imag(),  2.1441532693786116e-16 , 1e-15);
    TS_ASSERT_DELTA(a(4).real(),  2.3112859900958784     , 1e-15);
    TS_ASSERT_DELTA(a(4).imag(),  2.9467861319341292e-16 , 1e-15);
    TS_ASSERT_DELTA(a(5).real(),  0.93605434777157281    , 1e-15);
    TS_ASSERT_DELTA(a(5).imag(),  9.3374366200977735e-17 , 1e-15);
    TS_ASSERT_DELTA(a(6).real(),  0.20801573910862903    , 1e-15);
    TS_ASSERT_DELTA(a(6).imag(),  2.6521075187407153e-17 , 1e-15);
    TS_ASSERT_DELTA(a(7).real(),  0.029264120494198648   , 1e-15);
    TS_ASSERT_DELTA(a(7).imag(),  1.7367641481966751e-18 , 1e-15);
    TS_ASSERT_DELTA(a(8).real(),  0.0027709898755823026  , 1e-15);
    TS_ASSERT_DELTA(a(8).imag(),  1.99636952899313e-19   , 1e-15);
    TS_ASSERT_DELTA(a(9).real(),  0.00016985473600300896 , 1e-15);
    TS_ASSERT_DELTA(a(9).imag(),  -8.5019724543739762e-22, 1e-15);
    TS_ASSERT_DELTA(a(10).real(), 5.9049e-06             , 1e-15);
    TS_ASSERT_DELTA(a(10).imag(), 0.0                    , 1e-15);
  };
  
  CXXTEST_TEST(Internal_bilinear)
  {
    const double wo = 0.5;
    const double fs = 2.0;
    Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1> z, p, a_, b_;
    Eigen::Matrix<double, Eigen::Dynamic, 1> a, b;
    double k;
    btkEigen::buttap(&z, &p, &k, 3);
    btkEigen::zpk2tf(&b_, &a_, z, p, k);
    btkEigen::lp2lp(&b_, &a_, wo);
    btkEigen::bilinear(&b, &a, b_, a_, fs);
    TS_ASSERT_EQUALS(b.rows(), 4);
    TS_ASSERT_DELTA(b(0), 0.0015220700152207001, 1e-15);
    TS_ASSERT_DELTA(b(1), 0.0045662100456621002, 1e-15);
    TS_ASSERT_DELTA(b(2), 0.0045662100456621002, 1e-15);
    TS_ASSERT_DELTA(b(3), 0.0015220700152207001, 1e-15);
    TS_ASSERT_EQUALS(a.rows(), 4);
    TS_ASSERT_DELTA(a(0), 1.0, 1e-15);
    TS_ASSERT_DELTA(a(1), -2.5038051750380514, 1e-15);
    TS_ASSERT_DELTA(a(2), 2.1232876712328763, 1e-15);
    TS_ASSERT_DELTA(a(3), -0.60730593607305938, 1e-15);
  }
};

CXXTEST_SUITE_REGISTRATION(EigenIIRFilterDesignTest)
CXXTEST_TEST_REGISTRATION(EigenIIRFilterDesignTest, Butter_low_2_0Dot5)
CXXTEST_TEST_REGISTRATION(EigenIIRFilterDesignTest, Butter_low_5_0Dot1)
CXXTEST_TEST_REGISTRATION(EigenIIRFilterDesignTest, Butter_high_3_0Dot4)
CXXTEST_TEST_REGISTRATION(EigenIIRFilterDesignTest, Butter_pass_7_0Dot2_0Dot8)
CXXTEST_TEST_REGISTRATION(EigenIIRFilterDesignTest, Butter_stop_3_0Dot5_0Dot6)
CXXTEST_TEST_REGISTRATION(EigenIIRFilterDesignTest, Internal_buttap_2)
CXXTEST_TEST_REGISTRATION(EigenIIRFilterDesignTest, Internal_buttap_3)
CXXTEST_TEST_REGISTRATION(EigenIIRFilterDesignTest, Internal_poly)
CXXTEST_TEST_REGISTRATION(EigenIIRFilterDesignTest, Internal_zpk2tf_identity)
CXXTEST_TEST_REGISTRATION(EigenIIRFilterDesignTest, Internal_zpk2tf_bis)
CXXTEST_TEST_REGISTRATION(EigenIIRFilterDesignTest, Internal_normalize)
CXXTEST_TEST_REGISTRATION(EigenIIRFilterDesignTest, Internal_lp2lp_default)
CXXTEST_TEST_REGISTRATION(EigenIIRFilterDesignTest, Internal_lp2lp_half)
CXXTEST_TEST_REGISTRATION(EigenIIRFilterDesignTest, Internal_lp2hp_thirth)
CXXTEST_TEST_REGISTRATION(EigenIIRFilterDesignTest, Internal_lp2bp_default)
CXXTEST_TEST_REGISTRATION(EigenIIRFilterDesignTest, Internal_lp2bp)
CXXTEST_TEST_REGISTRATION(EigenIIRFilterDesignTest, Internal_lp2bs)
CXXTEST_TEST_REGISTRATION(EigenIIRFilterDesignTest, Internal_bilinear)

#endif // EigenIIRFilterDesignTest_h