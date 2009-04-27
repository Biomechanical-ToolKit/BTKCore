#ifndef MetaDataEntryTest_h
#define MetaDataEntryTest_h

#include <btkMetaDataEntry.h>

CXXTEST_SUITE(MetaDataEntryTest)
{
  CXXTEST_TEST(SimpleCtor)
  {
    btk::MetaDataEntry::Pointer test = btk::MetaDataEntry::New("POINT", "point group", false);
    TS_ASSERT_EQUALS(test->GetLabel(), "POINT");
    TS_ASSERT_EQUALS(test->GetDescription(), "point group");
    TS_ASSERT_EQUALS(test->GetUnlockState(), false);
    TS_ASSERT_EQUALS(test->HasMetaDataEntryValue(), false);
  };
  
  CXXTEST_TEST(CtorWithValue)
  {
    btk::MetaDataEntry::Pointer test = btk::MetaDataEntry::New("SCALE", (float)0.145, "marker scale");
    TS_ASSERT_EQUALS(test->GetLabel(), "SCALE");
    TS_ASSERT_EQUALS(test->GetDescription(), "marker scale");
    TS_ASSERT_EQUALS(test->GetUnlockState(), true);
    TS_ASSERT_EQUALS(test->HasMetaDataEntryValue(), true);
    btk::MetaDataEntryValue::Pointer val = test->GetMetaDataEntryValue();
    TS_ASSERT_EQUALS(val.use_count(), 2);
    TS_ASSERT_EQUALS(val->GetValues()[0], "0.145");
  };
  
  CXXTEST_TEST(CtorAndChild)
  {
    btk::MetaDataEntry::Pointer point = btk::MetaDataEntry::New("POINT", "point group", false);
    btk::MetaDataEntry::Pointer pointUsed = btk::MetaDataEntry::New("USED", (int16_t)16);
    btk::MetaDataEntry::Pointer pointScale = btk::MetaDataEntry::New("SCALE", (float)-0.0833);
    point->AppendChild(pointUsed);
    point->AppendChild(pointScale);
    TS_ASSERT_EQUALS(point->GetChildNumber(), 2);
  };
  
  CXXTEST_TEST(Clone)
  {
    btk::MetaDataEntry::Pointer point = btk::MetaDataEntry::New("POINT", "point group", false);
    btk::MetaDataEntry::Pointer pointUsed = btk::MetaDataEntry::New("USED", (int16_t)16);
    btk::MetaDataEntry::Pointer pointScale = btk::MetaDataEntry::New("SCALE", (float)-0.0833);
    point->AppendChild(pointUsed);
    point->AppendChild(pointScale);
    btk::MetaDataEntry::Pointer point2 = point->Clone();
    point2->GetChild(0)->SetLabel("FRAMES");
    TS_ASSERT_EQUALS(point->GetChild(0)->GetLabel(), "USED");
    TS_ASSERT_EQUALS(point2->GetChild(0)->GetLabel(), "FRAMES");
    point2->GetChild(0)->GetMetaDataEntryValue()->SetValues((int16_t)1300);
    TS_ASSERT_EQUALS(point->GetChild(0)->GetMetaDataEntryValue()->GetValue(0), "16");
    TS_ASSERT_EQUALS(point2->GetChild(0)->GetMetaDataEntryValue()->GetValue(0), "1300");
  };
  
  CXXTEST_TEST(Find)
  {
    btk::MetaDataEntry::Pointer point = btk::MetaDataEntry::New("POINT", "point group", false);
    btk::MetaDataEntry::Pointer pointUsed = btk::MetaDataEntry::New("USED", (int16_t)16);
    btk::MetaDataEntry::Pointer pointScale = btk::MetaDataEntry::New("SCALE", (float)-0.0833);
    point->AppendChild(pointUsed);
    point->AppendChild(pointScale);
    TS_ASSERT_EQUALS(*(point->Find("USED")), pointUsed);
  };
  
  CXXTEST_TEST(TakeChild)
  {
    btk::MetaDataEntry::Pointer point = btk::MetaDataEntry::New("POINT", "point group", false);
    btk::MetaDataEntry::Pointer pointUsed = btk::MetaDataEntry::New("USED", (int16_t)16);
    btk::MetaDataEntry::Pointer pointScale = btk::MetaDataEntry::New("SCALE", (float)-0.0833);
    point->AppendChild(pointUsed);
    point->AppendChild(pointScale);
    btk::MetaDataEntry::Pointer pointScale2 = point->TakeChild(1);
    TS_ASSERT_EQUALS(point->GetChildNumber(), 1);
    TS_ASSERT_EQUALS(point->GetChild(0), pointUsed);
    pointScale2->SetLabel("FRAMES");
    TS_ASSERT_EQUALS(pointScale->GetLabel(), "FRAMES");
  };
  
  CXXTEST_TEST(InsertAndSetChildren)
  {
    btk::MetaDataEntry::Pointer point = btk::MetaDataEntry::New("POINT", "point group", false);
    btk::MetaDataEntry::Pointer pointUsed = btk::MetaDataEntry::New("USED", (int16_t)0);
    btk::MetaDataEntry::Pointer pointScale = btk::MetaDataEntry::New("SCALE", (float)-0.0833);
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 0); dim[0] = 32;
    std::vector<int16_t> val = std::vector<int16_t>(0);
    btk::MetaDataEntry::Pointer pointLabels = btk::MetaDataEntry::New("LABELS", dim, val);
    btk::MetaDataEntry::Pointer pointFrames = btk::MetaDataEntry::New("FRAMES", (int16_t)1200);
    btk::MetaDataEntry::Pointer pointOther = btk::MetaDataEntry::New("OTHER", (int16_t)4);
    point->AppendChild(pointUsed);
    point->AppendChild(pointScale);
    
    TS_ASSERT_EQUALS(point->InsertChild(0, pointLabels), true);
    TS_ASSERT_EQUALS(point->GetChildNumber(), 3);
    TS_ASSERT_EQUALS(point->GetChild(0), pointLabels);
    TS_ASSERT_EQUALS(point->InsertChild(15, pointLabels), false);
    point->SetChild(0, pointLabels);
    point->SetChild(2, pointOther);
    TS_ASSERT_EQUALS(point->GetChild(2), pointOther);
    TS_ASSERT_EQUALS(point->GetChildNumber(), 3);
  };
};

CXXTEST_SUITE_REGISTRATION(MetaDataEntryTest)
CXXTEST_TEST_REGISTRATION(MetaDataEntryTest, SimpleCtor)
CXXTEST_TEST_REGISTRATION(MetaDataEntryTest, CtorWithValue)
CXXTEST_TEST_REGISTRATION(MetaDataEntryTest, CtorAndChild)
CXXTEST_TEST_REGISTRATION(MetaDataEntryTest, Clone)
CXXTEST_TEST_REGISTRATION(MetaDataEntryTest, Find)
CXXTEST_TEST_REGISTRATION(MetaDataEntryTest, TakeChild)
CXXTEST_TEST_REGISTRATION(MetaDataEntryTest, InsertAndSetChildren)

#endif