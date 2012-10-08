#ifndef ForcePlatformsExtractorTest_h
#define ForcePlatformsExtractorTest_h

#include <btkAcquisitionFileReader.h>
#include <btkForcePlatformsExtractor.h>

CXXTEST_SUITE(ForcePlatformsExtractorTest)
{
  CXXTEST_TEST(FileSample01Eb015pi)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pi.c3d");
    //reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::ForcePlatformsExtractor::Pointer pfe = btk::ForcePlatformsExtractor::New();
    pfe->SetInput(reader->GetOutput());
    btk::ForcePlatformCollection::Pointer pfc = pfe->GetOutput();
    pfe->Update();
    TS_ASSERT_EQUALS(pfc->GetItemNumber(), 2);

    btk::ForcePlatform::Pointer pf1 = pfc->GetItem(0);
    TS_ASSERT_EQUALS(pf1->GetType(), 2);
    TS_ASSERT_EQUALS(pf1->GetChannelNumber(), 6);
    TS_ASSERT_EQUALS(pf1->GetChannel(0)->GetLabel(), "FX1");
    TS_ASSERT_EQUALS(pf1->GetChannel(1)->GetLabel(), "FY1");
    TS_ASSERT_EQUALS(pf1->GetChannel(2)->GetLabel(), "FZ1");
    TS_ASSERT_EQUALS(pf1->GetChannel(3)->GetLabel(), "MX1");
    TS_ASSERT_EQUALS(pf1->GetChannel(4)->GetLabel(), "MY1");
    TS_ASSERT_EQUALS(pf1->GetChannel(5)->GetLabel(), "MZ1");
    btk::ForcePlatform::Origin o1; o1 << -4.4 , 1.9, -21.6;
    TS_ASSERT_EQUALS(pf1->GetOrigin().isApprox(o1, 0.00001), true);
    btk::ForcePlatform::Corner c11; c11 << 520.0451 , 1242.1694, 0.6219;
    btk::ForcePlatform::Corner c21; c21 << 57.0463 , 1243.1996, 0.6211;
    btk::ForcePlatform::Corner c31; c31 << 58.1765 , 1751.1663, 2.0812;
    btk::ForcePlatform::Corner c41; c41 << 521.1754 , 1750.1661, 2.0820;
    TS_ASSERT_EQUALS(pf1->GetCorner(0).isApprox(c11, 0.0001), true);
    TS_ASSERT_EQUALS(pf1->GetCorner(1).isApprox(c21, 0.0001), true);
    TS_ASSERT_EQUALS(pf1->GetCorner(2).isApprox(c31, 0.0001), true);
    TS_ASSERT_EQUALS(pf1->GetCorner(3).isApprox(c41, 0.0001), true);
    btk::ForcePlatform::Corners cs1; cs1 << c11, c21, c31, c41;
    TS_ASSERT_EQUALS(pf1->GetCorners().isApprox(cs1, 0.0001), true);

    btk::ForcePlatform::Pointer pf2 = pfc->GetItem(1);
    TS_ASSERT_EQUALS(pf2->GetType(), 2);
    TS_ASSERT_EQUALS(pf2->GetChannelNumber(), 6);
    TS_ASSERT_EQUALS(pf2->GetChannel(0)->GetLabel(), "FX2");
    TS_ASSERT_EQUALS(pf2->GetChannel(1)->GetLabel(), "FY2");
    TS_ASSERT_EQUALS(pf2->GetChannel(2)->GetLabel(), "FZ2");
    TS_ASSERT_EQUALS(pf2->GetChannel(3)->GetLabel(), "MX2");
    TS_ASSERT_EQUALS(pf2->GetChannel(4)->GetLabel(), "MY2");
    TS_ASSERT_EQUALS(pf2->GetChannel(5)->GetLabel(), "MZ2");
    btk::ForcePlatform::Origin o2; o2 << -4.06 , 3.81, -20.06;
    TS_ASSERT_EQUALS(pf2->GetOrigin().isApprox(o2, 0.00001), true);
    btk::ForcePlatform::Corner c12; c12 << 53.6655  , 1139.9977, 1.9204;
    btk::ForcePlatform::Corner c22; c22 << 516.6432 , 1143.3159, 1.2880;
    btk::ForcePlatform::Corner c32; c32 << 520.2825 , 635.3301, 0.1814;
    btk::ForcePlatform::Corner c42; c42 << 57.2948 , 632.0118, 0.8138;
    TS_ASSERT_EQUALS(pf2->GetCorner(0).isApprox(c12, 0.0001), true);
    TS_ASSERT_EQUALS(pf2->GetCorner(1).isApprox(c22, 0.0001), true);
    TS_ASSERT_EQUALS(pf2->GetCorner(2).isApprox(c32, 0.0001), true);
    TS_ASSERT_EQUALS(pf2->GetCorner(3).isApprox(c42, 0.0001), true);
    btk::ForcePlatform::Corners cs2; cs2 << c12, c22, c32, c42;
    TS_ASSERT_EQUALS(pf2->GetCorners().isApprox(cs2, 0.0001), true);
  };

  CXXTEST_TEST(FileSample10Type2)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample10/TYPE-2.C3D");
    //btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::ForcePlatformsExtractor::Pointer pfe = btk::ForcePlatformsExtractor::New();
    pfe->SetInput(reader->GetOutput());
    btk::ForcePlatformCollection::Pointer pfc = pfe->GetOutput();
    pfe->Update();
    TS_ASSERT_EQUALS(pfc->GetItemNumber(), 1);

    btk::ForcePlatform::Pointer pf1 = pfc->GetItem(0);
    TS_ASSERT_EQUALS(pf1->GetType(), 2);
    TS_ASSERT_EQUALS(pf1->GetChannelNumber(), 6);
    TS_ASSERT_EQUALS(pf1->GetChannel(0)->GetLabel(), "FX1");
    TS_ASSERT_EQUALS(pf1->GetChannel(1)->GetLabel(), "FY1");
    TS_ASSERT_EQUALS(pf1->GetChannel(2)->GetLabel(), "FZ1");
    TS_ASSERT_EQUALS(pf1->GetChannel(3)->GetLabel(), "MX1");
    TS_ASSERT_EQUALS(pf1->GetChannel(4)->GetLabel(), "MY1");
    TS_ASSERT_EQUALS(pf1->GetChannel(5)->GetLabel(), "MZ1");
    btk::ForcePlatform::Origin o1; o1 << -1.6 , 0.7, 37.5;
    TS_ASSERT_EQUALS(pf1->GetOrigin().isApprox(o1, 0.00001), true);
    TS_ASSERT_DELTA(pf1->GetChannel(0)->GetValues()(1), 0.08843, 0.00001);
    TS_ASSERT_DELTA(pf1->GetChannel(0)->GetValues()(3), -0.08843, 0.00001);
    TS_ASSERT_DELTA(pf1->GetChannel(0)->GetValues()(1020), -69.59441, 0.00001);
    TS_ASSERT_DELTA(pf1->GetChannel(0)->GetValues()(1021), -72.24731, 0.00001);
    TS_ASSERT_DELTA(pf1->GetChannel(1)->GetValues()(0), 0.17762, 0.00001);
    TS_ASSERT_DELTA(pf1->GetChannel(3)->GetValues()(2), 36.83129, 0.00001);
    TS_ASSERT_DELTA(pf1->GetChannel(3)->GetValues()(560), 73.66259, 0.00001);
    TS_ASSERT_DELTA(pf1->GetChannel(3)->GetValues()(561), 0.0, 0.00001);
  };

  CXXTEST_TEST(FileSample10Type4)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample10/TYPE-4.C3D");
    btk::ForcePlatformsExtractor::Pointer pfe = btk::ForcePlatformsExtractor::New();
    pfe->SetInput(reader->GetOutput());
    btk::ForcePlatformCollection::Pointer pfc = pfe->GetOutput();
    pfe->Update();
    TS_ASSERT_EQUALS(pfc->GetItemNumber(), 1);

    btk::ForcePlatformType4::Pointer pf1 = static_pointer_cast<btk::ForcePlatformType4>(pfc->GetItem(0));
    TS_ASSERT_EQUALS(pf1->GetType(), 4);
    TS_ASSERT_EQUALS(pf1->GetChannelNumber(), 6);
    TS_ASSERT_EQUALS(pf1->GetChannel(0)->GetFrameNumber(), 3980);
    TS_ASSERT_EQUALS(pf1->GetChannel(0)->GetLabel(), "FX1");
    TS_ASSERT_EQUALS(pf1->GetChannel(1)->GetLabel(), "FY1");
    TS_ASSERT_EQUALS(pf1->GetChannel(2)->GetLabel(), "FZ1");
    TS_ASSERT_EQUALS(pf1->GetChannel(3)->GetLabel(), "MX1");
    TS_ASSERT_EQUALS(pf1->GetChannel(4)->GetLabel(), "MY1");
    TS_ASSERT_EQUALS(pf1->GetChannel(5)->GetLabel(), "MZ1");
    btk::ForcePlatformType4::CalMatrix cal = btk::ForcePlatformType4::CalMatrix(6,6);
    cal << 1.5270, 0.0140, 0.0460,-4.2840,  -5.4940,  2.8560,
          -0.0050, 1.5390,-0.0190, 17.1600,  5.6680, -9.6390,
           0.0000,-0.0090, 5.9880, 13.2330, 13.2500,  4.6090,
           0.0000, 0.0020, 0.0000,741.8710, -0.5390, -0.5970,
           0.0020,-0.0010, 0.000,  -1.6820,739.6310,  2.0420,
          -0.0020,-0.0050,-0.0020, -3.2500, -0.5940,391.8790;
    TS_ASSERT_EQUALS(pf1->GetCalMatrix().isApprox(cal.transpose(), 0.0001), true);
    btk::ForcePlatform::Origin o1; o1 << -1.6 , 0.7, 37.5;
    TS_ASSERT_EQUALS(pf1->GetOrigin().isApprox(o1, 0.00001), true);
    
  };

  CXXTEST_TEST(FileSample19Sample19)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample19/sample19.c3d");
    btk::ForcePlatformsExtractor::Pointer pfe = btk::ForcePlatformsExtractor::New();
    pfe->SetInput(reader->GetOutput());
    btk::ForcePlatformCollection::Pointer pfc = pfe->GetOutput();
    pfe->Update();
    TS_ASSERT_EQUALS(pfc->GetItemNumber(), 4);
    btk::ForcePlatform::Pointer pf1 = pfc->GetItem(0);
    TS_ASSERT_EQUALS(pf1->GetChannelNumber(), 6);
    TS_ASSERT_EQUALS(pf1->GetChannel(0)->GetFrameNumber(), 34672 * 18);
    TS_ASSERT_EQUALS(pf1->GetChannel(0)->GetLabel(), "FP1C1");
    TS_ASSERT_EQUALS(pf1->GetChannel(1)->GetLabel(), "FP1C2");
    TS_ASSERT_EQUALS(pf1->GetChannel(2)->GetLabel(), "FP1C3");
    TS_ASSERT_EQUALS(pf1->GetChannel(3)->GetLabel(), "FP1C4");
    TS_ASSERT_EQUALS(pf1->GetChannel(4)->GetLabel(), "FP1C5");
    TS_ASSERT_EQUALS(pf1->GetChannel(5)->GetLabel(), "FP1C6");

    TS_ASSERT_DELTA(pf1->GetChannel(3)->GetValues()(561), 0.0, 0.00001);
  };
  
  CXXTEST_TEST(FPAnalogModified)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample09/PluginC3D.c3d");
    btk::ForcePlatformsExtractor::Pointer pfe = btk::ForcePlatformsExtractor::New();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    pfe->SetInput(acq);
    btk::ForcePlatformCollection::Pointer pfc = pfe->GetOutput();
    pfe->Update();
    TS_ASSERT_EQUALS(pfc->GetItemNumber(), 2);
    unsigned long int ts = pfc->GetTimestamp();
    reader.reset(); // Destroy the parent Filter
    acq->Modified();
    acq->GetAnalog(0)->Modified();
    pfc->Update();
    TS_ASSERT_EQUALS(pfc->GetItemNumber(), 2);
    TS_ASSERT(ts != pfc->GetTimestamp());
  };
  
  CXXTEST_TEST(FPAnalogNotModified)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample09/PluginC3D.c3d");
    btk::ForcePlatformsExtractor::Pointer pfe = btk::ForcePlatformsExtractor::New();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    pfe->SetInput(acq);
    btk::ForcePlatformCollection::Pointer pfc = pfe->GetOutput();
    pfe->Update();
    TS_ASSERT_EQUALS(pfc->GetItemNumber(), 2);
    unsigned long int ts = pfc->GetTimestamp();
    reader.reset(); // Destroy the parent Filter
    unsigned long int ts_ = acq->GetAnalogs()->GetTimestamp();
    acq->Modified();
    TS_ASSERT(ts_ == acq->GetAnalogs()->GetTimestamp());
    pfc->Update();
    TS_ASSERT_EQUALS(pfc->GetItemNumber(), 2);
    TS_ASSERT(ts == pfc->GetTimestamp());
  };
};

CXXTEST_SUITE_REGISTRATION(ForcePlatformsExtractorTest)
CXXTEST_TEST_REGISTRATION(ForcePlatformsExtractorTest, FileSample01Eb015pi)
CXXTEST_TEST_REGISTRATION(ForcePlatformsExtractorTest, FileSample10Type2)
CXXTEST_TEST_REGISTRATION(ForcePlatformsExtractorTest, FileSample10Type4)
CXXTEST_TEST_REGISTRATION(ForcePlatformsExtractorTest, FileSample19Sample19)
CXXTEST_TEST_REGISTRATION(ForcePlatformsExtractorTest, FPAnalogModified)
CXXTEST_TEST_REGISTRATION(ForcePlatformsExtractorTest, FPAnalogNotModified)

#endif
