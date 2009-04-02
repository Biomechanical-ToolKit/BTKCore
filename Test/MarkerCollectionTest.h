#ifndef MarkerCollectionTest_h
#define MarkerCollectionTest_h

#include <btkMarkerCollection.h>

CXXTEST_SUITE(MarkerCollectionTest)
{
  CXXTEST_TEST(Constructor)
  {
    btk::MarkerCollection::Pointer test = btk::MarkerCollection::New();
    TS_ASSERT_EQUALS(test->GetItemNumber(), 0);
  };
  
  CXXTEST_TEST(InsertItem)
  {
    btk::MarkerCollection::Pointer test = btk::MarkerCollection::New();
    btk::Marker::Pointer m = btk::Marker::New("HEEL_R", 10);
    TS_ASSERT_EQUALS(test->InsertItem(0, m), true);
    TS_ASSERT_EQUALS(test->GetItemNumber(), 1);
    m->GetValues().data()[0] = 1;
    TS_ASSERT_EQUALS(test->GetItem(0)->GetValues().data()[0], 1);
  };
};

CXXTEST_SUITE_REGISTRATION(MarkerCollectionTest)
CXXTEST_TEST_REGISTRATION(MarkerCollectionTest, Constructor)
CXXTEST_TEST_REGISTRATION(MarkerCollectionTest, InsertItem)
#endif