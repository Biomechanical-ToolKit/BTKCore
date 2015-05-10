#ifndef LardModelTest_h
#define LardModelTest_h

#include <btklardModel.h>

CXXTEST_SUITE(LardModelTest)
{
  CXXTEST_TEST(UtilsTransformRelativePosition)
  {
    btk::Node root("root");
    btk::Segment seg("seg",0,0,&root);
    // Position from seg
    double coords[3] = {10.,10.,10.};
    btk::RelativePosition foo("foo",coords,&seg);
    // Frame from seg
    double transfo[12] = {-1.,  0., 0.,
                           0., -1., 0.,
                           0.,  0., 1.,
                           5.,  10.,15.};
    btk::RelativeReferenceFrame bar("bar",transfo,&seg);
    // Position from "bar"
    coords[0] = 0.; coords[1] = 0.; coords[2] = 0.;
    btk::RelativePosition toto("toto",coords,&bar);
    // Frame from "bar"
    transfo[0] = 0.; transfo[1] = 0.; transfo[2] = 1.;
    transfo[3] =-1.; transfo[4] = 0.; transfo[5] = 0.;
    transfo[6] = 0.; transfo[7] =-1.; transfo[8] = 0.;
    transfo[9] = 10.; transfo[10] =-43.2; transfo[11] = 100.19;
    btk::RelativeReferenceFrame ice("ice",transfo,&bar);
    // Position from "ice"
    coords[0] = 1.; coords[1] = 2.; coords[2] = 3.;
    btk::RelativePosition milk("milk",coords,&ice);
    
    btk::lard::Motion motion(3);
    motion.values() << 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 10.0, -43.2, 100.19,
                       0.299252, 0.947355, 0.113874, -0.361734, 0.002203, 0.932279, 0.882948, -0.320179, 0.343350, 10.0, -43.2, 100.19,
                       1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0;
    
    auto _foo = btk::lard::transform_relative_position(&foo, &seg, &motion);
    auto _toto = btk::lard::transform_relative_position(&toto, &seg, &motion);
    auto _milk = btk::lard::transform_relative_position(&milk, &seg, &motion);
    
    auto _bar = btk::lard::transform_relative_frame(&bar, &seg, &motion);
    auto _ice = btk::lard::transform_relative_frame(&ice, &seg, &motion);
  };
};

CXXTEST_SUITE_REGISTRATION(LardModelTest)
CXXTEST_TEST_REGISTRATION(LardModelTest, UtilsTransformRelativePosition)

#endif // LardModelTest_h