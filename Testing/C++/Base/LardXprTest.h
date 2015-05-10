#ifndef LardXprTest_h
#define LardXprTest_h

#include <btklardXpr.h>

CXXTEST_SUITE(LardXprTest)
{
  CXXTEST_TEST(ArrayAssignment)
  {
    btk::lard::Vector a(10);
    a.values() << -1.0 , -1.0 , -1.0 , 
                   1.0 ,  1.0 ,  1.0 , 
                   0.5 ,  0.5 ,  0.5 , 
                   0.5 ,  0.5 ,  0.5 , 
                   1.0 ,  1.0 ,  1.0 , 
                   0.65,  0.65,  0.65, 
                   0.45,  0.45,  0.45, 
                   1.0 ,  1.0 ,  1.0 , 
                   0.0 ,  0.0 ,  0.0 , 
                  -0.4 , -0.4 , -0.4;
    a.residuals() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    btk::lard::Vector b = a;
    for (int i = 0 ; i < 30 ; ++i)
      TS_ASSERT_EQUALS(a.values().data()[i], b.values().data()[i]);
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_EQUALS(a.residuals().data()[i], b.residuals().data()[i]);
  };
  
  CXXTEST_TEST(ArrayNorm)
  {
    btk::lard::Vector a(10);
    a.values() << -1.0 , -1.0 , -1.0 , 
                   1.0 ,  1.0 ,  1.0 , 
                   0.5 ,  0.5 ,  0.5 , 
                   0.5 ,  0.5 ,  0.5 , 
                   1.0 ,  1.0 ,  1.0 , 
                   0.65,  0.65,  0.65, 
                   0.45,  0.45,  0.45, 
                   1.0 ,  1.0 ,  1.0 , 
                   0.0 ,  0.0 ,  0.0 , 
                  -0.4 , -0.4 , -0.4;
    a.residuals() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    
    btk::lard::Scalar b = a.norm();

    TS_ASSERT_DELTA(b.values().coeff(0), 0.0, 1e-15);
    TS_ASSERT_DELTA(b.values().coeff(1), 1.732050807568877, 1e-15);
    TS_ASSERT_DELTA(b.values().coeff(2), 0.866025403784439, 1e-15);
    TS_ASSERT_DELTA(b.values().coeff(3), 0.866025403784439, 1e-15);
    TS_ASSERT_DELTA(b.values().coeff(4), 1.732050807568877, 1e-15);
    TS_ASSERT_DELTA(b.values().coeff(5), 1.12583302491977, 1e-15);
    TS_ASSERT_DELTA(b.values().coeff(6), 0.779422863405995, 1e-15);
    TS_ASSERT_DELTA(b.values().coeff(7), 1.732050807568877, 1e-15);
    TS_ASSERT_DELTA(b.values().coeff(8), 0.0, 1e-15);
    TS_ASSERT_DELTA(b.values().coeff(9), 0.0, 1e-15);

    TS_ASSERT_EQUALS(b.residuals().coeff(0), -1.0);
    TS_ASSERT_EQUALS(b.residuals().coeff(1), 0.0);
    TS_ASSERT_EQUALS(b.residuals().coeff(2), 0.0);
    TS_ASSERT_EQUALS(b.residuals().coeff(3), 0.0);
    TS_ASSERT_EQUALS(b.residuals().coeff(4), 0.0);
    TS_ASSERT_EQUALS(b.residuals().coeff(5), 0.0);
    TS_ASSERT_EQUALS(b.residuals().coeff(6), 0.0);
    TS_ASSERT_EQUALS(b.residuals().coeff(7), 0.0);
    TS_ASSERT_EQUALS(b.residuals().coeff(8), 0.0);
    TS_ASSERT_EQUALS(b.residuals().coeff(9), -1.0);
  };
  
  CXXTEST_TEST(ArrayMean)
  {  
    btk::lard::Scalar a(10), b(10);
    btk::lard::Trajectory c(10);
    a.values() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    a.residuals() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    b.values() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    b.residuals().setConstant(10,-1.0);
    c.values() << -1.0 , -1.0 , -1.0 , 
                   1.0 ,  1.0 ,  1.0 , 
                   0.5 ,  0.5 ,  0.5 , 
                   0.5 ,  0.5 ,  0.5 , 
                   1.0 ,  1.0 ,  1.0 , 
                   0.65,  0.65,  0.65, 
                   0.45,  0.45,  0.45, 
                   1.0 ,  1.0 ,  1.0 , 
                   0.0 ,  0.0 ,  0.0 , 
                  -0.4 , -0.4 , -0.4;
    c.residuals() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    
    btk::lard::Scalar a_ = a.mean();
    btk::lard::Scalar b_ = b.mean();
    btk::lard::Trajectory c_ = c.mean();

    TS_ASSERT_EQUALS(a_.values().coeff(0), 0.6375);
    TS_ASSERT_EQUALS(a_.residuals().coeff(0), 0.0);
    TS_ASSERT_EQUALS(a_.values().rows(), 1);
    TS_ASSERT_EQUALS(a_.residuals().rows(), 1);
    
    TS_ASSERT_EQUALS(b_.values().coeff(0), 0.0);
    TS_ASSERT_EQUALS(b_.residuals().coeff(0), -1.0);
    TS_ASSERT_EQUALS(b_.values().rows(), 1);
    TS_ASSERT_EQUALS(b_.residuals().rows(), 1);
    
    TS_ASSERT_EQUALS(c_.values().coeff(0,0), 0.6375);
    TS_ASSERT_EQUALS(c_.values().coeff(0,1), 0.6375);
    TS_ASSERT_EQUALS(c_.values().coeff(0,2), 0.6375);
    TS_ASSERT_EQUALS(c_.residuals().coeff(0), 0.0);
    TS_ASSERT_EQUALS(c_.values().rows(), 1);
    TS_ASSERT_EQUALS(c_.residuals().rows(), 1);
  };

  CXXTEST_TEST(ArrayScaledDifference)
  {
    btk::lard::Scalar a(10), b(10);
    a.values().setRandom();
    a.residuals() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    b.values().setRandom();
    b.residuals() <<  0.5, 0.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0,  0.0;
    
    btk::lard::Scalar c = 0.5 * (a - b);

    TS_ASSERT_EQUALS(c.residuals().coeff(0), -1.0);
    TS_ASSERT_EQUALS(c.residuals().coeff(9), -1.0);
    for (int i = 1 ; i < 9 ; ++i)
      TS_ASSERT_EQUALS(c.values().coeff(i), (a.values().coeff(i) - b.values().coeff(i)) / 2.0);
    TS_ASSERT_EQUALS(c.residuals().coeff(0), -1.0);
    TS_ASSERT_EQUALS(c.residuals().coeff(1), 0.0);
    TS_ASSERT_EQUALS(c.residuals().coeff(2), 0.0);
    TS_ASSERT_EQUALS(c.residuals().coeff(3), 0.0);
    TS_ASSERT_EQUALS(c.residuals().coeff(4), 0.0);
    TS_ASSERT_EQUALS(c.residuals().coeff(5), 0.0);
    TS_ASSERT_EQUALS(c.residuals().coeff(6), 0.0);
    TS_ASSERT_EQUALS(c.residuals().coeff(7), 0.0);
    TS_ASSERT_EQUALS(c.residuals().coeff(8), 0.0);
    TS_ASSERT_EQUALS(c.residuals().coeff(9), -1.0);
  };
  
  CXXTEST_TEST(ArrayNormalized)
  {
    btk::lard::Vector a(10);
    a.values().setRandom();
    a.values().row(9) << 0.0, 0.0, 0.0;
    a.residuals() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    btk::lard::Vector b(10);
    b.values().setRandom();
    b.residuals() << 1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, 0.4;
    btk::lard::Vector c = a.normalized();
    btk::lard::Vector d = (b-a).normalized();

    Eigen::Matrix<double,Eigen::Dynamic,1> norm = c.values().matrix().rowwise().norm();
    for (int i = 1 ; i < 9 ; ++i)
      TS_ASSERT_DELTA(norm.coeff(i), 1.0, 1e-16);
    TS_ASSERT_DELTA(norm.coeff(0), 0.0, 1e-16);
    TS_ASSERT_DELTA(norm.coeff(9), 0.0, 1e-16);
    
    norm = d.values().matrix().rowwise().norm();
    for (int i = 1 ; i < 9 ; ++i)
      TS_ASSERT_DELTA(norm.coeff(i), 1.0, 1e-15);
    TS_ASSERT_DELTA(norm.coeff(0), 0.0, 1e-16);
    TS_ASSERT_DELTA(norm.coeff(9), 0.0, 1e-16);
  };

  CXXTEST_TEST(ArrayCross)
  {
    btk::lard::Vector a(4), b(4);
    a.values() << 1.0, 0.0, 0.0,
                  0.0, 1.0, 0.0,
                  0.0, 0.0, 1.0,
                  0.0, 0.0, 0.0;
    a.residuals() << 1.0, 0.5, 0.5, -1.0;
    b.values() << 0.0, 1.0, 0.0,
                  0.0, 1.0, 0.0,
                  1.0, 0.0, 0.0,
                  0.0, 0.0, 0.0;
    b.residuals() << 1.0, 0.5, 0.5, 1.0;
    btk::lard::Vector c = a.cross(b);

    const btk::lard::Vector::Values& cv = c.values();
    const btk::lard::Vector::Residuals& cr = c.residuals();

    TS_ASSERT_DELTA(cv.coeff(0,0), 0.0, 1e-15);
    TS_ASSERT_DELTA(cv.coeff(0,1), 0.0, 1e-15);
    TS_ASSERT_DELTA(cv.coeff(0,2), 1.0, 1e-15);

    TS_ASSERT_DELTA(cv.coeff(1,0), 0.0, 1e-15);
    TS_ASSERT_DELTA(cv.coeff(1,1), 0.0, 1e-15);
    TS_ASSERT_DELTA(cv.coeff(1,2), 0.0, 1e-15);

    TS_ASSERT_DELTA(cv.coeff(2,0), 0.0, 1e-15);
    TS_ASSERT_DELTA(cv.coeff(2,1), 1.0, 1e-15);
    TS_ASSERT_DELTA(cv.coeff(2,2), 0.0, 1e-15);

    TS_ASSERT_DELTA(cv.coeff(3,0), 0.0, 1e-15);
    TS_ASSERT_DELTA(cv.coeff(3,1), 0.0, 1e-15);
    TS_ASSERT_DELTA(cv.coeff(3,2), 0.0, 1e-15);

    TS_ASSERT_EQUALS(cr.coeff(0), 0.0);
    TS_ASSERT_EQUALS(cr.coeff(1), 0.0);
    TS_ASSERT_EQUALS(cr.coeff(2), 0.0);
    TS_ASSERT_EQUALS(cr.coeff(3), -1.0);
  };

  CXXTEST_TEST(ArrayCrossBis)
  {
    btk::lard::Vector a(4), b(4), c(4);
    a.values().setZero();
    a.residuals().setZero();
    b.values() << 0.0, 0.4, 0.0,
                  0.0, 2.0, 0.0,
                  0.9, 0.0, 0.0,
                  0.0, 0.0, 0.0;
    b.residuals() << 1.0, 0.5, 0.5, 1.0;
    c.values() << 2.0, 0.0, 0.0,
                  0.0, 3.0, 0.0,
                  0.0, 0.0, 1.5,
                  0.0, 0.0, 0.0;
    c.residuals() << 1.0, 0.5, 0.5, -1.0;
    btk::lard::Vector d = (c-a).normalized().cross((b-a).normalized());

    const btk::lard::Vector::Values& dv = d.values();
    const btk::lard::Vector::Residuals& dr = d.residuals();

    TS_ASSERT_DELTA(dv.coeff(0,0), 0.0, 1e-15);
    TS_ASSERT_DELTA(dv.coeff(0,1), 0.0, 1e-15);
    TS_ASSERT_DELTA(dv.coeff(0,2), 1.0, 1e-15);

    TS_ASSERT_DELTA(dv.coeff(1,0), 0.0, 1e-15);
    TS_ASSERT_DELTA(dv.coeff(1,1), 0.0, 1e-15);
    TS_ASSERT_DELTA(dv.coeff(1,2), 0.0, 1e-15);

    TS_ASSERT_DELTA(dv.coeff(2,0), 0.0, 1e-15);
    TS_ASSERT_DELTA(dv.coeff(2,1), 1.0, 1e-15);
    TS_ASSERT_DELTA(dv.coeff(2,2), 0.0, 1e-15);

    TS_ASSERT_DELTA(dv.coeff(3,0), 0.0, 1e-15);
    TS_ASSERT_DELTA(dv.coeff(3,1), 0.0, 1e-15);
    TS_ASSERT_DELTA(dv.coeff(3,2), 0.0, 1e-15);

    TS_ASSERT_EQUALS(dr.coeff(0), 0.0);
    TS_ASSERT_EQUALS(dr.coeff(1), 0.0);
    TS_ASSERT_EQUALS(dr.coeff(2), 0.0);
    TS_ASSERT_EQUALS(dr.coeff(3), -1.0);
  };
  
  CXXTEST_TEST(ArrayOperatorDouble)
  {
    btk::lard::Trajectory c(10);
    c.values() << -1.0 , -1.0 , -1.0 , 
                   1.0 ,  1.0 ,  1.0 , 
                   0.5 ,  0.5 ,  0.5 , 
                   0.5 ,  0.5 ,  0.5 , 
                   1.0 ,  1.0 ,  1.0 , 
                   0.65,  0.65,  0.65, 
                   0.45,  0.45,  0.45, 
                   1.0 ,  1.0 ,  1.0 , 
                   0.0 ,  0.0 ,  0.0 , 
                  -0.4 , -0.4 , -0.4;
    c.residuals() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    double norm = 0.0;
    
    norm = c.mean().norm();
    TS_ASSERT_DELTA(norm, 1.104182389825159, 1e-15);
    
    c.residuals().setConstant(-1.0);
    norm = c.mean().norm();
    TS_ASSERT_DELTA(norm, 0.0, 1e-15);
  };
  
  CXXTEST_TEST(ArrayReplicate)
  {
    btk::lard::Trajectory a(2);
    a.values() << -1.0 , -1.0 , -1.0 , 
                   1.0 ,  1.0 ,  1.0;
    a.residuals() << -1.0, 1.0;
    
    btk::lard::Trajectory b = a.replicate(3);
    const btk::lard::Trajectory::Values& bv = b.values();
    const btk::lard::Trajectory::Residuals& br = b.residuals();
    TS_ASSERT_EQUALS(b.rows(), 6);
    
    TS_ASSERT_DELTA(bv.coeff(0,0),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(0,1),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(0,2),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(1,0),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(1,1),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(1,2),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(2,0),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(2,1),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(2,2),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(3,0),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(3,1),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(3,2),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(4,0),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(4,1),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(4,2),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(5,0),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(5,1),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(5,2),   1.0, 1e-15);
    
    TS_ASSERT_DELTA(br.coeff(0), -1.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(1),  1.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(2), -1.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(3),  1.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(4), -1.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(5),  1.0, 1e-15);
  };
  
  CXXTEST_TEST(ArrayReplicateBis)
  {
    btk::lard::Trajectory::Values av(2,3);
    av << -1.0 , -1.0 , -1.0 , 
           1.0 ,  1.0 ,  1.0;
           
    btk::lard::Trajectory b = btk::lard::Trajectory(av,btk::lard::Trajectory::Residuals::Zero(av.rows())).replicate(3);
    const btk::lard::Trajectory::Values& bv = b.values();
    const btk::lard::Trajectory::Residuals& br = b.residuals();
    TS_ASSERT_EQUALS(b.rows(), 6);
    
    TS_ASSERT_DELTA(bv.coeff(0,0),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(0,1),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(0,2),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(1,0),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(1,1),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(1,2),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(2,0),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(2,1),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(2,2),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(3,0),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(3,1),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(3,2),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(4,0),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(4,1),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(4,2),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(5,0),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(5,1),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(5,2),   1.0, 1e-15);
    
    TS_ASSERT_DELTA(br.coeff(0), 0.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(1), 0.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(2), 0.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(3), 0.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(4), 0.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(5), 0.0, 1e-15);
  };
  
  CXXTEST_TEST(MotionConstructor)
  {
    btk::lard::Trajectory a(1), b(1), c(1);
    a.values() << 0.0, 0.0, 0.0; a.residuals() << 0.0;
    b.values() << 9.9, 0.0, 0.0; b.residuals() << 0.0;
    c.values() << 0.0,-4.1, 0.0; c.residuals() << 0.0;
      
    auto u = (b-a).normalized();
    auto v = (c-a).normalized();
    btk::lard::Motion motion(u, v, u.cross(v), (a+b+c)/3.0);
    TS_ASSERT_EQUALS(motion.isValid(),true);
    TS_ASSERT_EQUALS(motion.isOccluded(),false);
    
    const btk::lard::Motion::Values& mv = motion.values();
    TS_ASSERT_DELTA(mv.coeff(0,0),   1.0, 1e-15);
    TS_ASSERT_DELTA(mv.coeff(0,1),   0.0, 1e-15);
    TS_ASSERT_DELTA(mv.coeff(0,2),   0.0, 1e-15);
    TS_ASSERT_DELTA(mv.coeff(0,3),   0.0, 1e-15);
    TS_ASSERT_DELTA(mv.coeff(0,4),  -1.0, 1e-15);
    TS_ASSERT_DELTA(mv.coeff(0,5),   0.0, 1e-15);
    TS_ASSERT_DELTA(mv.coeff(0,6),   0.0, 1e-15);
    TS_ASSERT_DELTA(mv.coeff(0,7),   0.0, 1e-15);
    TS_ASSERT_DELTA(mv.coeff(0,8),  -1.0, 1e-15);
    TS_ASSERT_DELTA(mv.coeff(0,9),   3.3, 1e-15);
    TS_ASSERT_DELTA(mv.coeff(0,10),  -1.366666666666667, 1e-15);
    TS_ASSERT_DELTA(mv.coeff(0,11),  0.0, 1e-15);
    
    TS_ASSERT_DELTA(motion.residuals().coeff(0), 0.0, 1e-15);
  };
  
  CXXTEST_TEST(MotionInverse)
  {
    btk::lard::Motion motion(3);
    motion.values() << 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 10.0, -43.2, 100.19,
                       0.299252, 0.947355, 0.113874, -0.361734, 0.002203, 0.932279, 0.882948, -0.320179, 0.343350, 10.0, -43.2, 100.19,
                       1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0;
    btk::lard::Motion::Values inv = motion.inverse().values();
    // Row #1
    TS_ASSERT_DELTA(inv.coeff(0, 0), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(0, 1), -1.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(0, 2), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(0, 3), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(0, 4), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(0, 5), -1.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(0, 6), 1.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(0, 7), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(0, 8), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(0, 9), -100.190000, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(0,10), 10.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(0,11), -43.200000, 1e-15);
    // Row #2
    TS_ASSERT_DELTA(inv.coeff(1, 0), 0.299252, 5e-7);
    TS_ASSERT_DELTA(inv.coeff(1, 1), -0.361734, 5e-7);
    TS_ASSERT_DELTA(inv.coeff(1, 2), 0.882948, 5e-7);
    TS_ASSERT_DELTA(inv.coeff(1, 3), 0.947355, 5e-7);
    TS_ASSERT_DELTA(inv.coeff(1, 4), 0.002203, 5e-7);
    TS_ASSERT_DELTA(inv.coeff(1, 5), -0.320179, 5e-7);
    TS_ASSERT_DELTA(inv.coeff(1, 6), 0.113874, 5e-7);
    TS_ASSERT_DELTA(inv.coeff(1, 7), 0.932279, 5e-7);
    TS_ASSERT_DELTA(inv.coeff(1, 8), 0.343350, 5e-7);
    TS_ASSERT_DELTA(inv.coeff(1, 9), 26.524142, 5e-5);
    TS_ASSERT_DELTA(inv.coeff(1,10), -89.692480, 5e-5);
    TS_ASSERT_DELTA(inv.coeff(1,11), -57.061415, 5e-5);
    // Row #3
    TS_ASSERT_DELTA(inv.coeff(2, 0), 1.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(2, 1), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(2, 2), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(2, 3), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(2, 4), 1.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(2, 5), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(2, 6), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(2, 7), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(2, 8), 1.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(2, 9), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(2,10), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(2,11), 0.0, 1e-15);
  };
  
  CXXTEST_TEST(MotionTransformMotion)
  {
    btk::lard::Motion motion(3);
    motion.values() << 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 10.0, -43.2, 100.19,
                       0.299252, 0.947355, 0.113874, -0.361734, 0.002203, 0.932279, 0.882948, -0.320179, 0.343350, 10.0, -43.2, 100.19,
                       1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0;
    btk::lard::Motion::Values eye = motion.transform(motion.inverse()).values();
    
    // Row #1
    TS_ASSERT_DELTA(eye.coeff(0, 0), 1.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(0, 1), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(0, 2), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(0, 3), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(0, 4), 1.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(0, 5), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(0, 6), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(0, 7), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(0, 8), 1.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(0, 9), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(0,10), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(0,11), 0.0, 1e-15);
    // Row #2
    TS_ASSERT_DELTA(eye.coeff(1, 0), 1.0, 1e-6);
    TS_ASSERT_DELTA(eye.coeff(1, 1), 0.0, 1e-6);
    TS_ASSERT_DELTA(eye.coeff(1, 2), 0.0, 1e-6);
    TS_ASSERT_DELTA(eye.coeff(1, 3), 0.0, 1e-6);
    TS_ASSERT_DELTA(eye.coeff(1, 4), 1.0, 1e-6);
    TS_ASSERT_DELTA(eye.coeff(1, 5), 0.0, 1e-6);
    TS_ASSERT_DELTA(eye.coeff(1, 6), 0.0, 1e-6);
    TS_ASSERT_DELTA(eye.coeff(1, 7), 0.0, 1e-6);
    TS_ASSERT_DELTA(eye.coeff(1, 8), 1.0, 1e-6);
    TS_ASSERT_DELTA(eye.coeff(1, 9), 0.0, 1e-4);
    TS_ASSERT_DELTA(eye.coeff(1,10), 0.0, 1e-4);
    TS_ASSERT_DELTA(eye.coeff(1,11), 0.0, 1e-4);
    // Row #3
    TS_ASSERT_DELTA(eye.coeff(2, 0), 1.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(2, 1), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(2, 2), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(2, 3), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(2, 4), 1.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(2, 5), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(2, 6), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(2, 7), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(2, 8), 1.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(2, 9), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(2,10), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(2,11), 0.0, 1e-15);
  };
  
  CXXTEST_TEST(MotionTransformTrajectory)
  {
    btk::lard::Motion motion(3);
    motion.values() << 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 10.0, -43.2, 100.19,
                       0.299252, 0.947355, 0.113874, -0.361734, 0.002203, 0.932279, 0.882948, -0.320179, 0.343350, 10.0, -43.2, 100.19,
                       1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0;
    
    btk::lard::Trajectory traj(3);
    traj.values() << 33.1,-13.56, 1034.12,
                     0.23, 34.12, -98.0,
                     66.0, 333.869, 4598.234;
      
    btk::lard::Trajectory::Values result = motion.transform(traj).values();
    
    // Row #1
    TS_ASSERT_DELTA(result.coeff(0, 0),    23.56, 5e-15);
    TS_ASSERT_DELTA(result.coeff(0, 1), -1077.32, 5e-15);
    TS_ASSERT_DELTA(result.coeff(0, 2),   133.29, 5e-15);
    // Row #2
    TS_ASSERT_DELTA(result.coeff(1, 0), -88.802402038427090, 4e-5);
    TS_ASSERT_DELTA(result.coeff(1, 1), -11.529404173918849, 4e-5);
    TS_ASSERT_DELTA(result.coeff(1, 2),  98.377264095895953, 4e-5);
    // Row #3
    TS_ASSERT_DELTA(result.coeff(2, 0), 66.0, 1e-15);
    TS_ASSERT_DELTA(result.coeff(2, 1), 333.869, 1e-15);
    TS_ASSERT_DELTA(result.coeff(2, 2), 4598.234, 1e-15);
  };
  
  CXXTEST_TEST(MotionTransformPosition)
  {
    btk::lard::Motion motion(3);
    motion.values() << 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 10.0, -43.2, 100.19,
                       0.299252, 0.947355, 0.113874, -0.361734, 0.002203, 0.932279, 0.882948, -0.320179, 0.343350, 10.0, -43.2, 100.19,
                       1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0;
    motion.residuals().setZero();
    
    TS_ASSERT_EQUALS(motion.rows(),3);
    TS_ASSERT_EQUALS(motion.cols(),12);

    const auto values = Eigen::Matrix<double,1,3>(0.0,0.0,0.0);
    btk::lard::Trajectory position(values);
    TS_ASSERT_EQUALS(position.rows(),1);
    
    const auto replicate = position.replicate(motion.rows());
    TS_ASSERT_EQUALS(replicate.rows(),3);
    TS_ASSERT_EQUALS(replicate.cols(),3);
    
    const auto transform = motion.transform(position.replicate(motion.rows()));
    TS_ASSERT_EQUALS(transform.rows(),3);
    TS_ASSERT_EQUALS(transform.cols(),3);
    
    btk::lard::Trajectory t = transform;
    TS_ASSERT_EQUALS(t.rows(),3);
    TS_ASSERT_EQUALS(t.cols(),3);
    // Row #1
    TS_ASSERT_DELTA(t.values().coeff(0, 0),   10.0, 1e-15);
    TS_ASSERT_DELTA(t.values().coeff(0, 1),  -43.2, 1e-15);
    TS_ASSERT_DELTA(t.values().coeff(0, 2),  100.19, 1e-15);
    // Row #2
    TS_ASSERT_DELTA(t.values().coeff(1, 0),   10.0, 1e-15);
    TS_ASSERT_DELTA(t.values().coeff(1, 1),  -43.2, 1e-15);
    TS_ASSERT_DELTA(t.values().coeff(1, 2),  100.19, 1e-15);
    // Row #3
    TS_ASSERT_DELTA(t.values().coeff(2, 0), 0.0, 1e-15);
    TS_ASSERT_DELTA(t.values().coeff(2, 1), 0.0, 1e-15);
    TS_ASSERT_DELTA(t.values().coeff(2, 2), 0.0, 1e-15);
    
    btk::lard::Trajectory tt = motion.transform(btk::lard::Trajectory(Eigen::Matrix<double,1,3>(0.0,0.0,0.0)).replicate(motion.rows()));
    TS_ASSERT_EQUALS(tt.rows(),3);
    TS_ASSERT_EQUALS(tt.cols(),3);
    // Row #1
    TS_ASSERT_DELTA(tt.values().coeff(0, 0),   10.0, 1e-15);
    TS_ASSERT_DELTA(tt.values().coeff(0, 1),  -43.2, 1e-15);
    TS_ASSERT_DELTA(tt.values().coeff(0, 2),  100.19, 1e-15);
    // Row #2
    TS_ASSERT_DELTA(tt.values().coeff(1, 0),   10.0, 1e-15);
    TS_ASSERT_DELTA(tt.values().coeff(1, 1),  -43.2, 1e-15);
    TS_ASSERT_DELTA(tt.values().coeff(1, 2),  100.19, 1e-15);
    // Row #3
    TS_ASSERT_DELTA(tt.values().coeff(2, 0), 0.0, 1e-15);
    TS_ASSERT_DELTA(tt.values().coeff(2, 1), 0.0, 1e-15);
    TS_ASSERT_DELTA(tt.values().coeff(2, 2), 0.0, 1e-15);
  };
  
  CXXTEST_TEST(MotionEulerAngles)
  {
    btk::lard::Motion motion(3);
    motion.values() << 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 10.0, -43.2, 100.19,
                       0.299252, 0.947355, 0.113874, -0.361734, 0.002203, 0.932279, 0.882948, -0.320179, 0.343350, 10.0, -43.2, 100.19,
                       1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0;
    motion.residuals() << 0.0, 0.0, -1.0;                   
    btk::lard::Array<3> angles = motion.eulerAngles(0,1,2);
    TS_ASSERT_EQUALS(angles.values().rows(),3);
    TS_ASSERT_EQUALS(angles.values().cols(),3);
    // Row #1
    TS_ASSERT_DELTA(angles.values().coeff(0, 0), M_PI/2.0, 1e-15);
    TS_ASSERT_DELTA(angles.values().coeff(0, 1),      0.0, 1e-15);
    TS_ASSERT_DELTA(angles.values().coeff(0, 2), M_PI/2.0, 1e-15);
    // Row #2
    TS_ASSERT_DELTA(angles.values().coeff(1, 0), 0.750491578357562, 1e-5);
    TS_ASSERT_DELTA(angles.values().coeff(1, 1), 1.082104136236484, 1e-5);
    TS_ASSERT_DELTA(angles.values().coeff(1, 2), 0.879645943005142, 1e-5);
    // Row #3
    TS_ASSERT_DELTA(angles.values().coeff(2, 0), 0.0, 1e-15);
    TS_ASSERT_DELTA(angles.values().coeff(2, 1), 0.0, 1e-15);
    TS_ASSERT_DELTA(angles.values().coeff(2, 2), 0.0, 1e-15);
    
    btk::lard::Array<3> mean = motion.eulerAngles(0,1,2).mean();
    TS_ASSERT_DELTA(mean.values().coeff(0, 0), 1.160643952576229, 1e-5);
    TS_ASSERT_DELTA(mean.values().coeff(0, 1), 0.541052068118242, 1e-5);
    TS_ASSERT_DELTA(mean.values().coeff(0, 2), 1.225221134900019, 1e-5);
    
    btk::lard::Array<3> meanbis = angles.mean();
    TS_ASSERT_DELTA(meanbis.values().coeff(0, 0), 1.160643952576229, 1e-5);
    TS_ASSERT_DELTA(meanbis.values().coeff(0, 1), 0.541052068118242, 1e-5);
    TS_ASSERT_DELTA(meanbis.values().coeff(0, 2), 1.225221134900019, 1e-5);
  };

  CXXTEST_TEST(MappedScaledDifference)
  {
    double _a[20] = {0};
    double _b[20] = {0};
    double _c[20] = {0};

    btk::lard::MappedScalar a(10,_a,_a+10), b(10,_b,_b+10), c(10,_c,_c+10);
    a.values().setRandom();
    a.residuals() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    b.values().setRandom();
    b.residuals() <<  0.5, 0.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0,  0.0;

    c = 0.5 * (a - b);

    // Values
    for (int i = 1 ; i < 9 ; ++i)
      TS_ASSERT_EQUALS(_c[i], (_a[i] - _b[i]) / 2.0);
    TS_ASSERT_EQUALS(_c[0], 0.0);
    TS_ASSERT_EQUALS(_c[9], 0.0);
    // Residuals
    TS_ASSERT_EQUALS(_c[10], -1.0);
    TS_ASSERT_EQUALS(_c[11], 0.0);
    TS_ASSERT_EQUALS(_c[12], 0.0);
    TS_ASSERT_EQUALS(_c[13], 0.0);
    TS_ASSERT_EQUALS(_c[14], 0.0);
    TS_ASSERT_EQUALS(_c[15], 0.0);
    TS_ASSERT_EQUALS(_c[16], 0.0);
    TS_ASSERT_EQUALS(_c[17], 0.0);
    TS_ASSERT_EQUALS(_c[18], 0.0);
    TS_ASSERT_EQUALS(_c[19], -1.0);
  };

  CXXTEST_TEST(MappedNull)
  {
    btk::lard::MappedScalar a(0,nullptr,nullptr);
    btk::lard::MappedScalar b = a;
    TS_ASSERT_EQUALS(a.isValid(),false);
    TS_ASSERT_EQUALS(b.isValid(),false);
  };
  
  CXXTEST_TEST(Block)
  {
    btk::lard::Motion motion(3);
    motion.values().setRandom();
    motion.residuals() << -1.0, 1.0, 0.5;
    TS_ASSERT_EQUALS(((motion.block<1>(0).values() - motion.values().block(0,0,3,1)).abs() < 1e-15).all(), true);
    TS_ASSERT_EQUALS(((motion.block<1>(1).values() - motion.values().block(0,1,3,1)).abs() < 1e-15).all(), true);
    TS_ASSERT_EQUALS(((motion.block<1>(2).values() - motion.values().block(0,2,3,1)).abs() < 1e-15).all(), true);
    TS_ASSERT_EQUALS(((motion.block<1>(0).residuals() - motion.residuals()).abs() < 1e-15).all(), true);
    TS_ASSERT_EQUALS(((motion.block<1>(1).residuals() - motion.residuals()).abs() < 1e-15).all(), true);
    TS_ASSERT_EQUALS(((motion.block<1>(2).residuals() - motion.residuals()).abs() < 1e-15).all(), true);
    btk::lard::Trajectory o = motion.o();
    TS_ASSERT_EQUALS(((o.values().block(0,0,3,1) - motion.values().block(0,9,3,1)).abs() < 1e-15).all(), true);
    TS_ASSERT_EQUALS(((o.values().block(0,1,3,1) - motion.values().block(0,10,3,1)).abs() < 1e-15).all(), true);
    TS_ASSERT_EQUALS(((o.values().block(0,2,3,1) - motion.values().block(0,11,3,1)).abs() < 1e-15).all(), true);
  };
  
  CXXTEST_TEST(BlockAssignment)
  {
    btk::lard::Trajectory a(10),b(10);
    a.values().setRandom();
    a.residuals() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    b.values().setRandom();
    b.residuals() <<  0.5, 0.0, -1.0, 0.5, 0.5, 1.0, 0.65, 0.45, -1.0,  0.0;
    a.z() = b.z();
    TS_ASSERT_EQUALS(((a.values().block(0,2,3,1) - b.values().block(0,2,3,1)).abs() < 1e-15).all(), true);
    btk::lard::Trajectory::Residuals res(10);
    res << -1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, -1.0;
    TS_ASSERT_EQUALS(((a.residuals() - res).abs() < 1e-15).all(), true);
  };
  
  CXXTEST_TEST(MixOperatorsPlusAndDivide)
  {
    double _a[8] = {0};
    double _b[26] = {0};

    btk::lard::MappedVector a(2,_a,_a+6);
    btk::lard::MappedMotion b(2,_b,_b+24);
    btk::lard::Vector c(2), d(2), e(2);
    
    a.values() << 1., 2., 3., 4., 5., 6.;
    a.residuals() << 0., 0.;
    
    b.values().setZero();
    b.residuals() << 0.3, -1.;
    
    c.values() << 7., 8., 9., 10., 11., 12.;
    c.residuals() << 0., 0.;
    d.values() << 13., 14., 15., 16., 17., 18.;
    d.residuals() << 0., 0.;
    e.values().setZero();
    e.residuals() << 0., 0.;
    
    btk::lard::Vector f;
    f = (a + b.u() + c + d) / 4.0;
    TS_ASSERT_EQUALS(f.isValid(),true);
    TS_ASSERT_EQUALS(f.isOccluded(),false);
    TS_ASSERT_EQUALS(f.rows(),2);
    TS_ASSERT_EQUALS(f.cols(),3);
    const auto& fr = f.residuals();
    TS_ASSERT_EQUALS(fr.coeff(0), 0.);
    TS_ASSERT_EQUALS(fr.coeff(1), -1.);
    const auto& fv = f.values();
    TS_ASSERT_EQUALS(fv.coeff(0,0), 5.25);
    TS_ASSERT_EQUALS(fv.coeff(0,1), 6.);
    TS_ASSERT_EQUALS(fv.coeff(0,2), 6.75);
    TS_ASSERT_EQUALS(fv.coeff(1,0), 0.);
    TS_ASSERT_EQUALS(fv.coeff(1,1), 0.);
    TS_ASSERT_EQUALS(fv.coeff(1,2), 0.);
    
    btk::lard::Vector g = (a + c + d + e) / 4.0;
    TS_ASSERT_EQUALS(g.isValid(),true);
    TS_ASSERT_EQUALS(g.isOccluded(),false);
    TS_ASSERT_EQUALS(g.rows(),2);
    TS_ASSERT_EQUALS(g.cols(),3);
    const auto& gr = g.residuals();
    TS_ASSERT_EQUALS(gr.coeff(0), 0.);
    TS_ASSERT_EQUALS(gr.coeff(1), 0.);
    const auto& gv = g.values();
    TS_ASSERT_EQUALS(gv.coeff(0,0), 5.25);
    TS_ASSERT_EQUALS(gv.coeff(0,1), 6.);
    TS_ASSERT_EQUALS(gv.coeff(0,2), 6.75);
    TS_ASSERT_EQUALS(gv.coeff(1,0), 7.5);
    TS_ASSERT_EQUALS(gv.coeff(1,1), 8.25);
    TS_ASSERT_EQUALS(gv.coeff(1,2), 9);
  };
  
  CXXTEST_TEST(MixAssignment)
  {
    btk::lard::Vector a(10);
    a.values() << -1.0 , -1.0 , -1.0 , 
                   1.0 ,  1.0 ,  1.0 , 
                   0.5 ,  0.5 ,  0.5 , 
                   0.5 ,  0.5 ,  0.5 , 
                   1.0 ,  1.0 ,  1.0 , 
                   0.65,  0.65,  0.65, 
                   0.45,  0.45,  0.45, 
                   1.0 ,  1.0 ,  1.0 , 
                   0.0 ,  0.0 ,  0.0 , 
                  -0.4 , -0.4 , -0.4;
    a.residuals() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    
    double _b[40] = {0};
    btk::lard::MappedVector b(10,_b,_b+30);
    
    b = a;
    
    for (int i = 0 ; i < 30 ; ++i)
      TS_ASSERT_EQUALS(a.values().data()[i],_b[i]);
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_EQUALS(a.residuals().data()[i],_b[i+30]);
  };
};

CXXTEST_SUITE_REGISTRATION(LardXprTest)
CXXTEST_TEST_REGISTRATION(LardXprTest, ArrayAssignment)
CXXTEST_TEST_REGISTRATION(LardXprTest, ArrayNorm)
CXXTEST_TEST_REGISTRATION(LardXprTest, ArrayMean)
CXXTEST_TEST_REGISTRATION(LardXprTest, ArrayScaledDifference)
CXXTEST_TEST_REGISTRATION(LardXprTest, ArrayNormalized)
CXXTEST_TEST_REGISTRATION(LardXprTest, ArrayCross)
CXXTEST_TEST_REGISTRATION(LardXprTest, ArrayCrossBis)
CXXTEST_TEST_REGISTRATION(LardXprTest, ArrayOperatorDouble)
CXXTEST_TEST_REGISTRATION(LardXprTest, ArrayReplicate)
CXXTEST_TEST_REGISTRATION(LardXprTest, ArrayReplicateBis)
CXXTEST_TEST_REGISTRATION(LardXprTest, MotionConstructor)
CXXTEST_TEST_REGISTRATION(LardXprTest, MotionInverse)
CXXTEST_TEST_REGISTRATION(LardXprTest, MotionTransformMotion)
CXXTEST_TEST_REGISTRATION(LardXprTest, MotionTransformTrajectory)
CXXTEST_TEST_REGISTRATION(LardXprTest, MotionTransformPosition)
CXXTEST_TEST_REGISTRATION(LardXprTest, MotionEulerAngles)
CXXTEST_TEST_REGISTRATION(LardXprTest, MappedScaledDifference)
CXXTEST_TEST_REGISTRATION(LardXprTest, MappedNull)
CXXTEST_TEST_REGISTRATION(LardXprTest, Block)
CXXTEST_TEST_REGISTRATION(LardXprTest, BlockAssignment)
CXXTEST_TEST_REGISTRATION(LardXprTest, MixOperatorsPlusAndDivide)
CXXTEST_TEST_REGISTRATION(LardXprTest, MixAssignment)

#endif // LardXprTest_h