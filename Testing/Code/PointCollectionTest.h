#ifndef PointCollectionTest_h
#define PointCollectionTest_h

#include <btkPointCollection.h>

// For Sleep (sleep under Un*x) function
#ifdef _MSC_VER
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif
  #include <windows.h>
#else
  #include <unistd.h>
#endif

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
  
  CXXTEST_TEST(ClearModified)
  {
    btk::PointCollection::Pointer test = btk::PointCollection::New();
    test->InsertItem(0, btk::Point::New("HEEL_R", 10));
    unsigned long int t1 = test->GetTimestamp();
#ifndef _MSC_VER
    sleep(1); // Wait 1 msec ... Because the clear action can take less than 1 usec...
#else
    ::Sleep(1);
#endif
    test->Clear();
    TS_ASSERT(test->GetTimestamp() != t1);
  };
  
  CXXTEST_TEST(ClearNotModified)
  {
    btk::PointCollection::Pointer test = btk::PointCollection::New();
    unsigned long int t1 = test->GetTimestamp();
    test->Clear();
    TS_ASSERT_EQUALS(test->GetTimestamp(), t1);
  };
};

CXXTEST_SUITE_REGISTRATION(PointCollectionTest)
CXXTEST_TEST_REGISTRATION(PointCollectionTest, Constructor)
CXXTEST_TEST_REGISTRATION(PointCollectionTest, InsertItem)
CXXTEST_TEST_REGISTRATION(PointCollectionTest, ClearModified)
CXXTEST_TEST_REGISTRATION(PointCollectionTest, ClearNotModified)
#endif
