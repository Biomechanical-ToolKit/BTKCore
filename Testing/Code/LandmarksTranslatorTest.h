#ifndef LandmarksTranslatorTest_h
#define LandmarksTranslatorTest_h

#include <btkLandmarksTranslator.h>

CXXTEST_SUITE(LandmarksTranslatorTest)
{
  CXXTEST_TEST(SetAndFind)
  {
    btk::LandmarksTranslator::Pointer tr = btk::LandmarksTranslator::New();
    TS_ASSERT_EQUALS(tr->GetLandmarkNumber(), 0);
    tr->SetLandmark("PX", "PROCESSUS_XYLO");
    tr->SetLandmark("", "");
    tr->SetLandmark("PX", "");
    tr->SetLandmark("", "PROCESSUS_XYLO");
    TS_ASSERT_EQUALS(tr->GetLandmarkNumber(), 1);
    tr->SetLandmark("GT", "GREATER_TRANCHOTER");
    tr->SetLandmark("ST", "STERNUM");
    tr->SetLandmark("LE", "LATERAL_EPYCONDILUS");
    TS_ASSERT_EQUALS(tr->GetLandmarkNumber(), 4);
    TS_ASSERT_EQUALS(tr->GetTranslation("GT"), "GREATER_TRANCHOTER");
    tr->SetLandmark("GT", "GRAND_TROCHANTER");
    TS_ASSERT_EQUALS(tr->GetLandmarkNumber(), 4);
    TS_ASSERT_EQUALS(tr->GetTranslation("GT"), "GRAND_TROCHANTER");
  };
};

CXXTEST_SUITE_REGISTRATION(LandmarksTranslatorTest)
CXXTEST_TEST_REGISTRATION(LandmarksTranslatorTest, SetAndFind)
#endif