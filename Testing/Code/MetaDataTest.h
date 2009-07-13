#ifndef MetaDataTest_h
#define MetaDataTest_h

#include <btkMetaData.h>
#include <btkMetaDataUtils.h>

CXXTEST_SUITE(MetaDataTest)
{
  CXXTEST_TEST(SimpleCtor)
  {
    btk::MetaData::Pointer test = btk::MetaData::New("POINT", "point group", false);
    TS_ASSERT_EQUALS(test->GetLabel(), "POINT");
    TS_ASSERT_EQUALS(test->GetDescription(), "point group");
    TS_ASSERT_EQUALS(test->GetUnlockState(), false);
    TS_ASSERT_EQUALS(test->HasInfo(), false);
  };
  
  CXXTEST_TEST(CtorWithValue)
  {
    btk::MetaData::Pointer test = btk::MetaData::New("SCALE", (float)0.145, "point scale");
    TS_ASSERT_EQUALS(test->GetLabel(), "SCALE");
    TS_ASSERT_EQUALS(test->GetDescription(), "point scale");
    TS_ASSERT_EQUALS(test->GetUnlockState(), true);
    TS_ASSERT_EQUALS(test->HasInfo(), true);
    btk::MetaDataInfo::Pointer val = test->GetInfo();
    TS_ASSERT_EQUALS(val.use_count(), 2);
    std::vector<std::string> values = val->ToString();
    TS_ASSERT_EQUALS(values[0], "0.145");
  };
  
  CXXTEST_TEST(CtorAndChild)
  {
    btk::MetaData::Pointer point = btk::MetaData::New("POINT", "point group", false);
    btk::MetaData::Pointer pointUsed = btk::MetaData::New("USED", (int16_t)16);
    btk::MetaData::Pointer pointScale = btk::MetaData::New("SCALE", (float)-0.0833);
    point->AppendChild(pointUsed);
    point->AppendChild(pointScale);
    TS_ASSERT_EQUALS(point->GetChildNumber(), 2);
  };
  
  CXXTEST_TEST(Clone)
  {
    btk::MetaData::Pointer point = btk::MetaData::New("POINT", "point group", false);
    btk::MetaData::Pointer pointUsed = btk::MetaData::New("USED", (int16_t)16);
    btk::MetaData::Pointer pointScale = btk::MetaData::New("SCALE", (float)-0.0833);
    point->AppendChild(pointUsed);
    point->AppendChild(pointScale);
    btk::MetaData::Pointer point2 = point->Clone();
    point2->GetChild(0)->SetLabel("FRAMES");
    TS_ASSERT_EQUALS(point->GetChild(0)->GetLabel(), "USED");
    TS_ASSERT_EQUALS(point2->GetChild(0)->GetLabel(), "FRAMES");
    point2->GetChild(0)->GetInfo()->SetValues((int16_t)1300);
    std::vector<std::string> values = point->GetChild(0)->GetInfo()->ToString();
    TS_ASSERT_EQUALS(values[0], "16");
    values = point2->GetChild(0)->GetInfo()->ToString();
    TS_ASSERT_EQUALS(values[0], "1300");
  };
  
  CXXTEST_TEST(Find)
  {
    btk::MetaData::Pointer point = btk::MetaData::New("POINT", "point group", false);
    btk::MetaData::Pointer pointUsed = btk::MetaData::New("USED", (int16_t)16);
    btk::MetaData::Pointer pointScale = btk::MetaData::New("SCALE", (float)-0.0833);
    point->AppendChild(pointUsed);
    point->AppendChild(pointScale);
    TS_ASSERT_EQUALS(*(point->FindChild("USED")), pointUsed);
  };
  
  CXXTEST_TEST(TakeChild)
  {
    btk::MetaData::Pointer point = btk::MetaData::New("POINT", "point group", false);
    btk::MetaData::Pointer pointUsed = btk::MetaData::New("USED", (int16_t)16);
    btk::MetaData::Pointer pointScale = btk::MetaData::New("SCALE", (float)-0.0833);
    point->AppendChild(pointUsed);
    point->AppendChild(pointScale);
    btk::MetaData::Pointer pointScale2 = point->TakeChild(1);
    TS_ASSERT_EQUALS(point->GetChildNumber(), 1);
    TS_ASSERT_EQUALS(point->GetChild(0), pointUsed);
    pointScale2->SetLabel("FRAMES");
    TS_ASSERT_EQUALS(pointScale->GetLabel(), "FRAMES");
  };
  
  CXXTEST_TEST(InsertAndSetChildren)
  {
    btk::MetaData::Pointer point = btk::MetaData::New("POINT", "point group", false);
    btk::MetaData::Pointer pointUsed = btk::MetaData::New("USED", (int16_t)0);
    btk::MetaData::Pointer pointScale = btk::MetaData::New("SCALE", (float)-0.0833);
    std::vector<uint8_t> dim = std::vector<uint8_t>(2, 0); dim[0] = 32;
    std::vector<int16_t> val = std::vector<int16_t>(0);
    btk::MetaData::Pointer pointLabels = btk::MetaData::New("LABELS", dim, val);
    btk::MetaData::Pointer pointFrames = btk::MetaData::New("FRAMES", (int16_t)1200);
    btk::MetaData::Pointer pointOther = btk::MetaData::New("OTHER", (int16_t)4);
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

  CXXTEST_TEST(UtilsCreateChild)
  {
    btk::MetaData::Pointer root = btk::MetaData::New("ROOT");
    btk::MetaData::Pointer point = btk::MetaData::New("POINT", "point group", false);
    root->AppendChild(point);
    TS_ASSERT_EQUALS(root->GetChildNumber(), 1);
    TS_ASSERT_EQUALS(point->GetLabel(), "POINT");
    TS_ASSERT_EQUALS(point->GetDescription().length(), 11);
    TS_ASSERT_EQUALS(point->GetUnlockState(), false);
    btk::MetaData::Pointer point2 = btk::MetaDataCreateChild(root, "POINT");
    TS_ASSERT_EQUALS(root->GetChildNumber(), 1);
    TS_ASSERT_EQUALS(point->GetLabel(), "POINT");
    TS_ASSERT_EQUALS(point->GetDescription().length(), 0);
    TS_ASSERT_EQUALS(point2->GetUnlockState(), true);
    TS_ASSERT_EQUALS(point2->GetLabel(), "POINT");
    TS_ASSERT_EQUALS(point2->GetDescription().length(), 0);
    TS_ASSERT_EQUALS(point2->GetUnlockState(), true);
  };

  CXXTEST_TEST(UtilsCreateChildFloat1Value)
  {
    btk::MetaData::Pointer analog = btk::MetaData::New("ANALOG");
    
    btk::MetaDataCreateChild(analog, "SCALE", (float)0.08333);
    btk::MetaData::Pointer scale = analog->GetChild("SCALE");
    TS_ASSERT_EQUALS(analog->GetChildNumber(), 1);
    TS_ASSERT_EQUALS(scale->GetLabel(), "SCALE");
    TS_ASSERT_EQUALS(scale->GetDescription().length(), 0);
    TS_ASSERT_DELTA(scale->GetInfo()->ToFloat()[0], 0.08333, 0.0001);
  };

  CXXTEST_TEST(UtilsCreateChildFloat12Values)
  {
    btk::MetaData::Pointer analog = btk::MetaData::New("ANALOG");
    btk::MetaDataCreateChild(analog, "SCALE", std::vector<float>(12, (float)0.08333));
    btk::MetaData::Pointer scale = analog->GetChild("SCALE");    
    TS_ASSERT_EQUALS(analog->GetChildNumber(), 1);
    TS_ASSERT_EQUALS(scale->GetLabel(), "SCALE");
    TS_ASSERT_EQUALS(scale->GetDescription().length(), 0);
    TS_ASSERT_EQUALS(scale->GetInfo()->GetValues().size(), 12);
  };

  CXXTEST_TEST(UtilsCreateChildFloat300Values)
  {
    btk::MetaData::Pointer analog = btk::MetaData::New("ANALOG");
    btk::MetaDataCreateChild(analog, "SCALE", std::vector<float>(300, (float)0.08333));
    btk::MetaData::Pointer scale = analog->GetChild("SCALE");    
    TS_ASSERT_EQUALS(analog->GetChildNumber(), 2);
    TS_ASSERT_EQUALS(scale->GetLabel(), "SCALE");
    TS_ASSERT_EQUALS(scale->GetDescription().length(), 0);
    TS_ASSERT_EQUALS(scale->GetInfo()->GetValues().size(), 255);
    btk::MetaData::Pointer scale2 = analog->GetChild("SCALE2");
    TS_ASSERT_EQUALS(scale2->GetLabel(), "SCALE2");
    TS_ASSERT_EQUALS(scale2->GetDescription().length(), 0);
    TS_ASSERT_EQUALS(scale2->GetInfo()->GetValues().size(), 45);
  };

  CXXTEST_TEST(UtilsMetaDataCollapseString)
  {
    btk::MetaData::Pointer point = btk::MetaData::New("POINT");
    btk::MetaDataCreateChild(point, "LABELS", std::vector<std::string>(1120, "TESTS"));
    TS_ASSERT_EQUALS(point->GetChildNumber(), 5);    
    btk::MetaData::Pointer labels = point->GetChild("LABELS");    
    TS_ASSERT_EQUALS(labels->GetInfo()->GetValues().size(), 255);
    btk::MetaData::Pointer labels2 = point->GetChild("LABELS2");
    TS_ASSERT_EQUALS(labels2->GetInfo()->GetValues().size(), 255);
    btk::MetaData::Pointer labels3 = point->GetChild("LABELS3");
    TS_ASSERT_EQUALS(labels3->GetInfo()->GetValues().size(), 255);
    btk::MetaData::Pointer labels4 = point->GetChild("LABELS4");
    TS_ASSERT_EQUALS(labels4->GetInfo()->GetValues().size(), 255);
    btk::MetaData::Pointer labels5 = point->GetChild("LABELS5");
    TS_ASSERT_EQUALS(labels5->GetInfo()->GetValues().size(), 100);
    std::vector<std::string> values;
    btk::MetaDataCollapseChildrenValues(values, point, "LABELS");
    TS_ASSERT_EQUALS(values.size(), 1120);
  };

  CXXTEST_TEST(UtilsMetaDataCollapseStringAndBlank)
  {
    btk::MetaData::Pointer point = btk::MetaData::New("POINT");
    btk::MetaDataCreateChild(point, "LABELS", std::vector<std::string>(125, "TESTS"));
    std::vector<std::string> values;
    btk::MetaDataCollapseChildrenValues<std::string>(values, point, "LABELS", 150, "uname*");
    TS_ASSERT_EQUALS(values.size(), 150);
    for (int i = 125 ; i < 150 ; ++i)
      TS_ASSERT_EQUALS(values.at(i), "uname*" + btk::ToString(i+1));
  };

  CXXTEST_TEST(UtilsMetaDataCollapseInteger)
  {
    btk::MetaData::Pointer point = btk::MetaData::New("POINT");
    btk::MetaDataCreateChild(point, "LABELS", std::vector<int16_t>(1120, 54));
    TS_ASSERT_EQUALS(point->GetChildNumber(), 5);    
    btk::MetaData::Pointer labels = point->GetChild("LABELS");    
    TS_ASSERT_EQUALS(labels->GetInfo()->GetValues().size(), 255);
    btk::MetaData::Pointer labels2 = point->GetChild("LABELS2");
    TS_ASSERT_EQUALS(labels2->GetInfo()->GetValues().size(), 255);
    btk::MetaData::Pointer labels3 = point->GetChild("LABELS3");
    TS_ASSERT_EQUALS(labels3->GetInfo()->GetValues().size(), 255);
    btk::MetaData::Pointer labels4 = point->GetChild("LABELS4");
    TS_ASSERT_EQUALS(labels4->GetInfo()->GetValues().size(), 255);
    btk::MetaData::Pointer labels5 = point->GetChild("LABELS5");
    TS_ASSERT_EQUALS(labels5->GetInfo()->GetValues().size(), 100);
    std::vector<int16_t> values;
    btk::MetaDataCollapseChildrenValues(values, point, "LABELS");
    TS_ASSERT_EQUALS(values.size(), 1120);
  };

  CXXTEST_TEST(UtilsMetaDataCollapseIntegerAndBlank)
  {
    btk::MetaData::Pointer point = btk::MetaData::New("POINT");
    btk::MetaDataCreateChild(point, "LABELS", std::vector<int16_t>(125, 54));
    std::vector<int16_t> values;
    btk::MetaDataCollapseChildrenValues<int16_t>(values, point, "LABELS", 150);
    TS_ASSERT_EQUALS(values.size(), 150);
    for (int i = 125 ; i < 150 ; ++i)
      TS_ASSERT_EQUALS(values.at(i), 0);
  };
};

CXXTEST_SUITE_REGISTRATION(MetaDataTest)
CXXTEST_TEST_REGISTRATION(MetaDataTest, SimpleCtor)
CXXTEST_TEST_REGISTRATION(MetaDataTest, CtorWithValue)
CXXTEST_TEST_REGISTRATION(MetaDataTest, CtorAndChild)
CXXTEST_TEST_REGISTRATION(MetaDataTest, Clone)
CXXTEST_TEST_REGISTRATION(MetaDataTest, Find)
CXXTEST_TEST_REGISTRATION(MetaDataTest, TakeChild)
CXXTEST_TEST_REGISTRATION(MetaDataTest, InsertAndSetChildren)
CXXTEST_TEST_REGISTRATION(MetaDataTest, UtilsCreateChild)
CXXTEST_TEST_REGISTRATION(MetaDataTest, UtilsCreateChildFloat1Value)
CXXTEST_TEST_REGISTRATION(MetaDataTest, UtilsCreateChildFloat12Values)
CXXTEST_TEST_REGISTRATION(MetaDataTest, UtilsCreateChildFloat300Values)
CXXTEST_TEST_REGISTRATION(MetaDataTest, UtilsMetaDataCollapseString)
CXXTEST_TEST_REGISTRATION(MetaDataTest, UtilsMetaDataCollapseStringAndBlank)
CXXTEST_TEST_REGISTRATION(MetaDataTest, UtilsMetaDataCollapseInteger)
CXXTEST_TEST_REGISTRATION(MetaDataTest, UtilsMetaDataCollapseIntegerAndBlank)
#endif
