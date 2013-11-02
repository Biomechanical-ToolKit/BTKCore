#ifndef ForcePlatformWrenchFilterTest_h
#define ForcePlatformWrenchFilterTest_h

#include <btkAcquisitionFileReader.h>
#include <btkForcePlatformWrenchFilter.h>
#include <btkForcePlatformsExtractor.h>

CXXTEST_SUITE(ForcePlatformWrenchFilterTest)
{ 
  CXXTEST_TEST(FileSample09PluginC3D)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample09/PlugInC3D.c3d");
    btk::ForcePlatformsExtractor::Pointer pfe = btk::ForcePlatformsExtractor::New();
    btk::ForcePlatformWrenchFilter::Pointer fpwf = btk::ForcePlatformWrenchFilter::New();
    pfe->SetInput(reader->GetOutput());
    fpwf->SetInput(pfe->GetOutput());

    btk::WrenchCollection::Pointer fpwc = fpwf->GetOutput();
    fpwc->Update();
    TS_ASSERT_EQUALS(fpwc->GetItemNumber(), 2);

    double cx[2], cy[2], cz[2];
    btk::ForcePlatform::Corners c = pfe->GetOutput()->GetItem(0)->GetCorners();
    Eigen::Matrix<double, 3, 1> t = (c.col(0) + c.col(2)) / 2;
    cx[0] = t.data()[0]; cy[0] = t.data()[1]; cz[0] = t.data()[2];
    c = pfe->GetOutput()->GetItem(1)->GetCorners();
    t = (c.col(0) + c.col(2)) / 2;
    cx[1] = t.data()[0]; cy[1] = t.data()[1]; cz[1] = t.data()[2];
    
    for (int i = 1 ; i < 50 ; ++i)
    {
      for (int j = 0 ; j < fpwc->GetItemNumber() ; ++j)
      {
        // Force
        TS_ASSERT_DELTA(fpwc->GetItem(j)->GetForce()->GetValues()(i,0), pfe->GetOutput()->GetItem(j)->GetChannel(0)->GetValues()(i), 1e-2);
        TS_ASSERT_DELTA(fpwc->GetItem(j)->GetForce()->GetValues()(i,1), -pfe->GetOutput()->GetItem(j)->GetChannel(1)->GetValues()(i), 1e-2);
        TS_ASSERT_DELTA(fpwc->GetItem(j)->GetForce()->GetValues()(i,2), -pfe->GetOutput()->GetItem(j)->GetChannel(2)->GetValues()(i), 1e-2);
        // Moment
        TS_ASSERT_DELTA(fpwc->GetItem(j)->GetMoment()->GetValues()(i,0), pfe->GetOutput()->GetItem(j)->GetChannel(3)->GetValues()(i), 1e-0);
        TS_ASSERT_DELTA(fpwc->GetItem(j)->GetMoment()->GetValues()(i,1), -pfe->GetOutput()->GetItem(j)->GetChannel(4)->GetValues()(i), 1e-0);
        TS_ASSERT_DELTA(fpwc->GetItem(j)->GetMoment()->GetValues()(i,2), -pfe->GetOutput()->GetItem(j)->GetChannel(5)->GetValues()(i), 1e-0);
        // Position
        TS_ASSERT_DELTA(fpwc->GetItem(j)->GetPosition()->GetValues()(i,0), cx[j], 1e-2);
        TS_ASSERT_DELTA(fpwc->GetItem(j)->GetPosition()->GetValues()(i,1), cy[j], 1e-2);
        TS_ASSERT_DELTA(fpwc->GetItem(j)->GetPosition()->GetValues()(i,2), cz[j], 1e-2);
      }
    }
  };
};

CXXTEST_SUITE_REGISTRATION(ForcePlatformWrenchFilterTest)
CXXTEST_TEST_REGISTRATION(ForcePlatformWrenchFilterTest, FileSample09PluginC3D)
#endif
