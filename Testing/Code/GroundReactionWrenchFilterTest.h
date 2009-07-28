#ifndef GroundReactionWrenchFilterTest_h
#define GroundReactionWrenchFilterTest_h

#include <btkAcquisitionFileReader.h>
#include <btkGroundReactionWrenchFilter.h>
#include <btkForcePlatformsExtractor.h>

CXXTEST_SUITE(GroundReactionWrenchFilterTest)
{ 
  CXXTEST_TEST(FileSample10Type4a)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample10/type-4a.c3d");
    btk::ForcePlatformsExtractor::Pointer pfe = btk::ForcePlatformsExtractor::New();
    btk::GroundReactionWrenchFilter::Pointer grwf = btk::GroundReactionWrenchFilter::New();
    pfe->SetInput(reader->GetOutput());
    grwf->SetInput(pfe->GetOutput());

    btk::WrenchCollection::Pointer grwc = grwf->GetOutput();
    grwc->Update();
    TS_ASSERT_EQUALS(grwc->GetItemNumber(), 2);

    btk::Wrench::Pointer grw1 = grwc->GetItem(0);
    TS_ASSERT_EQUALS(grw1->GetPosition()->GetFrameNumber(), 5760);
  };
};

CXXTEST_SUITE_REGISTRATION(GroundReactionWrenchFilterTest)
CXXTEST_TEST_REGISTRATION(GroundReactionWrenchFilterTest, FileSample10Type4a)
#endif
