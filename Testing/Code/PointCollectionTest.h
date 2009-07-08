#ifndef PointCollectionTest_h
#define PointCollectionTest_h

#include <btkPointCollection.h>

CXXTEST_SUITE(PointCollectionTest)
{
  CXXTEST_TEST(Constructor)
  {
    btk::PointCollection::Pointer test = btk::PointCollection::New();
    TS_ASSERT_EQUALS(test->GetItemNumber(), 0);
  };
  
  CXXTEST_TEST(InsertItem)
  {
    btk::PointCollection::Pointer test = btk::PointCollection::New();
    btk::Point::Pointer m = btk::Point::New("HEEL_R", 10);
    TS_ASSERT_EQUALS(test->InsertItem(0, m), true);
    TS_ASSERT_EQUALS(test->GetItemNumber(), 1);
    m->GetValues().data()[0] = 1;
    TS_ASSERT_EQUALS(test->GetItem(0)->GetValues().data()[0], 1);
  };
};

CXXTEST_SUITE_REGISTRATION(PointCollectionTest)
CXXTEST_TEST_REGISTRATION(PointCollectionTest, Constructor)
CXXTEST_TEST_REGISTRATION(PointCollectionTest, InsertItem)
#endif
