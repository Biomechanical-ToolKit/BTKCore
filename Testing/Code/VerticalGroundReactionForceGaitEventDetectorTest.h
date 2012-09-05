#ifndef VerticalGroundReactionForceGaitEventDetectorTest_h
#define VerticalGroundReactionForceGaitEventDetectorTest_h

#include <btkVerticalGroundReactionForceGaitEventDetector.h>
#include <btkAcquisitionFileReader.h>
#include <btkForcePlatformsExtractor.h>
#include <btkGroundReactionWrenchFilter.h>
#include <btkDownsampleFilter.h>

CXXTEST_SUITE(VerticalGroundReactionForceGaitEventDetectorTest)
{
  CXXTEST_TEST(NoWrench)
  {
    btk::VerticalGroundReactionForceGaitEventDetector::Pointer vgrfged = btk::VerticalGroundReactionForceGaitEventDetector::New();
    btk::EventCollection::Pointer output = vgrfged->GetOutput();
    output->Update();
    TS_ASSERT_EQUALS(output->GetItemNumber(), 0);
  };
  
  CXXTEST_TEST(Gait)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "others/Gait.c3d");
    reader->Update(); // Because this is need to give the acquisition information...
    btk::ForcePlatformsExtractor::Pointer pfe = btk::ForcePlatformsExtractor::New();
    btk::GroundReactionWrenchFilter::Pointer grwf = btk::GroundReactionWrenchFilter::New();
    btk::DownsampleFilter<btk::WrenchCollection>::Pointer dswc = btk::DownsampleFilter<btk::WrenchCollection>::New();
    dswc->SetUpDownRatio(reader->GetOutput()->GetNumberAnalogSamplePerFrame());
    pfe->SetInput(reader->GetOutput());
    grwf->SetInput(pfe->GetOutput());
    dswc->SetInput(grwf->GetOutput());
    btk::VerticalGroundReactionForceGaitEventDetector::Pointer vgrfged = btk::VerticalGroundReactionForceGaitEventDetector::New();
    vgrfged->SetInput(dswc->GetOutput());
    // vgrfged->SetThresholdValue(10); // Default value
    std::vector<std::string> mapping(2); mapping[0] = "Left"; mapping[1] = "Right";
    vgrfged->SetForceplateContextMapping(mapping);
    vgrfged->SetAcquisitionInformation(reader->GetOutput()->GetFirstFrame(), reader->GetOutput()->GetPointFrequency(), "");
    btk::EventCollection::Pointer output = vgrfged->GetOutput();
    output->Update();
    
    btk::Event::Pointer ev;
    TS_ASSERT_EQUALS(output->GetItemNumber(), 4);
    ev = output->GetItem(0);
    TS_ASSERT_EQUALS(ev->GetLabel(), "Foot Strike");
    TS_ASSERT_EQUALS(ev->GetContext(), "Left");
    TS_ASSERT_EQUALS(ev->GetDetectionFlags(), btk::Event::Automatic | btk::Event::FromForcePlatform);
    TS_ASSERT_EQUALS(ev->GetId(), 1);
    TS_ASSERT_EQUALS(ev->GetFrame(), 257);
    TS_ASSERT_EQUALS(ev->GetTime(), 2.57);
    ev = output->GetItem(1);
    TS_ASSERT_EQUALS(ev->GetLabel(), "Foot Off");
    TS_ASSERT_EQUALS(ev->GetContext(), "Left");
    TS_ASSERT_EQUALS(ev->GetDetectionFlags(), btk::Event::Automatic | btk::Event::FromForcePlatform);
    TS_ASSERT_EQUALS(ev->GetId(), 2);
    TS_ASSERT_EQUALS(ev->GetFrame(), 316);
    TS_ASSERT_EQUALS(ev->GetTime(), 3.16);
    ev = output->GetItem(2);
    TS_ASSERT_EQUALS(ev->GetLabel(), "Foot Strike");
    TS_ASSERT_EQUALS(ev->GetContext(), "Right");
    TS_ASSERT_EQUALS(ev->GetDetectionFlags(), btk::Event::Automatic | btk::Event::FromForcePlatform);
    TS_ASSERT_EQUALS(ev->GetId(), 1);
    TS_ASSERT_EQUALS(ev->GetFrame(), 209);
    TS_ASSERT_EQUALS(ev->GetTime(), 2.09);
    ev = output->GetItem(3);
    TS_ASSERT_EQUALS(ev->GetLabel(), "Foot Off");
    TS_ASSERT_EQUALS(ev->GetContext(), "Right");
    TS_ASSERT_EQUALS(ev->GetDetectionFlags(), btk::Event::Automatic | btk::Event::FromForcePlatform);
    TS_ASSERT_EQUALS(ev->GetId(), 2);
    TS_ASSERT_EQUALS(ev->GetFrame(), 267);
    TS_ASSERT_EQUALS(ev->GetTime(), 2.67);
  };
  
  CXXTEST_TEST(Gait_NoMapping)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "others/Gait.c3d");
    reader->Update(); // Because this is need to give the acquisition information...
    btk::ForcePlatformsExtractor::Pointer pfe = btk::ForcePlatformsExtractor::New();
    btk::GroundReactionWrenchFilter::Pointer grwf = btk::GroundReactionWrenchFilter::New();
    btk::DownsampleFilter<btk::WrenchCollection>::Pointer dswc = btk::DownsampleFilter<btk::WrenchCollection>::New();
    dswc->SetUpDownRatio(reader->GetOutput()->GetNumberAnalogSamplePerFrame());
    pfe->SetInput(reader->GetOutput());
    grwf->SetInput(pfe->GetOutput());
    dswc->SetInput(grwf->GetOutput());
    btk::VerticalGroundReactionForceGaitEventDetector::Pointer vgrfged = btk::VerticalGroundReactionForceGaitEventDetector::New();
    vgrfged->SetInput(dswc->GetOutput());
    // vgrfged->SetThresholdValue(10); // Default value
    vgrfged->SetAcquisitionInformation(reader->GetOutput()->GetFirstFrame(), reader->GetOutput()->GetPointFrequency(), "");
    btk::EventCollection::Pointer output = vgrfged->GetOutput();
    output->Update();
    
    btk::Event::Pointer ev;
    TS_ASSERT_EQUALS(output->GetItemNumber(), 4);
    ev = output->GetItem(0);
    TS_ASSERT_EQUALS(ev->GetLabel(), "Foot Strike");
    TS_ASSERT_EQUALS(ev->GetContext(), "General");
    TS_ASSERT_EQUALS(ev->GetDetectionFlags(), btk::Event::Automatic | btk::Event::FromForcePlatform);
    TS_ASSERT_EQUALS(ev->GetId(), 1);
    TS_ASSERT_EQUALS(ev->GetFrame(), 257);
    TS_ASSERT_EQUALS(ev->GetTime(), 2.57);
    ev = output->GetItem(1);
    TS_ASSERT_EQUALS(ev->GetLabel(), "Foot Off");
    TS_ASSERT_EQUALS(ev->GetContext(), "General");
    TS_ASSERT_EQUALS(ev->GetDetectionFlags(), btk::Event::Automatic | btk::Event::FromForcePlatform);
    TS_ASSERT_EQUALS(ev->GetId(), 2);
    TS_ASSERT_EQUALS(ev->GetFrame(), 316);
    TS_ASSERT_EQUALS(ev->GetTime(), 3.16);
    ev = output->GetItem(2);
    TS_ASSERT_EQUALS(ev->GetLabel(), "Foot Strike");
    TS_ASSERT_EQUALS(ev->GetContext(), "General");
    TS_ASSERT_EQUALS(ev->GetDetectionFlags(), btk::Event::Automatic | btk::Event::FromForcePlatform);
    TS_ASSERT_EQUALS(ev->GetId(), 1);
    TS_ASSERT_EQUALS(ev->GetFrame(), 209);
    TS_ASSERT_EQUALS(ev->GetTime(), 2.09);
    ev = output->GetItem(3);
    TS_ASSERT_EQUALS(ev->GetLabel(), "Foot Off");
    TS_ASSERT_EQUALS(ev->GetContext(), "General");
    TS_ASSERT_EQUALS(ev->GetDetectionFlags(), btk::Event::Automatic | btk::Event::FromForcePlatform);
    TS_ASSERT_EQUALS(ev->GetId(), 2);
    TS_ASSERT_EQUALS(ev->GetFrame(), 267);
    TS_ASSERT_EQUALS(ev->GetTime(), 2.67);
  };
  
  CXXTEST_TEST(PluginC3D)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample09/PlugInC3D.c3d");
    reader->Update(); // Because this is need to give the acquisition information...
    btk::ForcePlatformsExtractor::Pointer pfe = btk::ForcePlatformsExtractor::New();
    btk::GroundReactionWrenchFilter::Pointer grwf = btk::GroundReactionWrenchFilter::New();
    btk::DownsampleFilter<btk::WrenchCollection>::Pointer dswc = btk::DownsampleFilter<btk::WrenchCollection>::New();
    dswc->SetUpDownRatio(reader->GetOutput()->GetNumberAnalogSamplePerFrame());
    pfe->SetInput(reader->GetOutput());
    grwf->SetInput(pfe->GetOutput());
    dswc->SetInput(grwf->GetOutput());
    btk::VerticalGroundReactionForceGaitEventDetector::Pointer vgrfged = btk::VerticalGroundReactionForceGaitEventDetector::New();
    vgrfged->SetInput(dswc->GetOutput());
    // vgrfged->SetThresholdValue(10); // Default value
    std::vector<std::string> mapping(2); mapping[0] = "Right"; mapping[1] = "Left";
    vgrfged->SetForceplateContextMapping(mapping);
    vgrfged->SetAcquisitionInformation(reader->GetOutput()->GetFirstFrame(), reader->GetOutput()->GetPointFrequency(), "");
    btk::EventCollection::Pointer output = vgrfged->GetOutput();
    output->Update();
    
    btk::Event::Pointer ev;
    TS_ASSERT_EQUALS(output->GetItemNumber(), 4);
    ev = output->GetItem(0);
    TS_ASSERT_EQUALS(ev->GetLabel(), "Foot Strike");
    TS_ASSERT_EQUALS(ev->GetContext(), "Right");
    TS_ASSERT_EQUALS(ev->GetDetectionFlags(), btk::Event::Automatic | btk::Event::FromForcePlatform);
    TS_ASSERT_EQUALS(ev->GetId(), 1);
    TS_ASSERT_EQUALS(ev->GetFrame(), 65);
    TS_ASSERT_EQUALS(ev->GetTime(), 65.0/60.0);
    ev = output->GetItem(1);
    TS_ASSERT_EQUALS(ev->GetLabel(), "Foot Off");
    TS_ASSERT_EQUALS(ev->GetContext(), "Right");
    TS_ASSERT_EQUALS(ev->GetDetectionFlags(), btk::Event::Automatic | btk::Event::FromForcePlatform);
    TS_ASSERT_EQUALS(ev->GetId(), 2);
    TS_ASSERT_EQUALS(ev->GetFrame(), 116);
    TS_ASSERT_EQUALS(ev->GetTime(), 116.0/60.0);
    ev = output->GetItem(2);
    TS_ASSERT_EQUALS(ev->GetLabel(), "Foot Strike");
    TS_ASSERT_EQUALS(ev->GetContext(), "Left");
    TS_ASSERT_EQUALS(ev->GetDetectionFlags(), btk::Event::Automatic | btk::Event::FromForcePlatform);
    TS_ASSERT_EQUALS(ev->GetId(), 1);
    TS_ASSERT_EQUALS(ev->GetFrame(), 108);
    TS_ASSERT_EQUALS(ev->GetTime(), 108.0/60.0);
    ev = output->GetItem(3);
    TS_ASSERT_EQUALS(ev->GetLabel(), "Foot Off");
    TS_ASSERT_EQUALS(ev->GetContext(), "Left");
    TS_ASSERT_EQUALS(ev->GetDetectionFlags(), btk::Event::Automatic | btk::Event::FromForcePlatform);
    TS_ASSERT_EQUALS(ev->GetId(), 2);
    TS_ASSERT_EQUALS(ev->GetFrame(), 150);
    TS_ASSERT_EQUALS(ev->GetTime(), 150.0/60.0);
  };
  
  CXXTEST_TEST(PluginC3D_Threshold50)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample09/PlugInC3D.c3d");
    reader->Update(); // Because this is need to give the acquisition information...
    btk::ForcePlatformsExtractor::Pointer pfe = btk::ForcePlatformsExtractor::New();
    btk::GroundReactionWrenchFilter::Pointer grwf = btk::GroundReactionWrenchFilter::New();
    btk::DownsampleFilter<btk::WrenchCollection>::Pointer dswc = btk::DownsampleFilter<btk::WrenchCollection>::New();
    dswc->SetUpDownRatio(reader->GetOutput()->GetNumberAnalogSamplePerFrame());
    pfe->SetInput(reader->GetOutput());
    grwf->SetInput(pfe->GetOutput());
    dswc->SetInput(grwf->GetOutput());
    btk::VerticalGroundReactionForceGaitEventDetector::Pointer vgrfged = btk::VerticalGroundReactionForceGaitEventDetector::New();
    vgrfged->SetInput(dswc->GetOutput());
    vgrfged->SetThresholdValue(50);
    std::vector<std::string> mapping(2); mapping[0] = "Right"; mapping[1] = "Left";
    vgrfged->SetForceplateContextMapping(mapping);
    vgrfged->SetAcquisitionInformation(reader->GetOutput()->GetFirstFrame(), reader->GetOutput()->GetPointFrequency(), "");
    btk::EventCollection::Pointer output = vgrfged->GetOutput();
    output->Update();
    
    btk::Event::Pointer ev;
    TS_ASSERT_EQUALS(output->GetItemNumber(), 4);
    ev = output->GetItem(0);
    TS_ASSERT_EQUALS(ev->GetLabel(), "Foot Strike");
    TS_ASSERT_EQUALS(ev->GetContext(), "Right");
    TS_ASSERT_EQUALS(ev->GetDetectionFlags(), btk::Event::Automatic | btk::Event::FromForcePlatform);
    TS_ASSERT_EQUALS(ev->GetId(), 1);
    TS_ASSERT_EQUALS(ev->GetFrame(), 67);
    TS_ASSERT_EQUALS(ev->GetTime(), 67.0/60.0);
    ev = output->GetItem(1);
    TS_ASSERT_EQUALS(ev->GetLabel(), "Foot Off");
    TS_ASSERT_EQUALS(ev->GetContext(), "Right");
    TS_ASSERT_EQUALS(ev->GetDetectionFlags(), btk::Event::Automatic | btk::Event::FromForcePlatform);
    TS_ASSERT_EQUALS(ev->GetId(), 2);
    TS_ASSERT_EQUALS(ev->GetFrame(), 114);
    TS_ASSERT_EQUALS(ev->GetTime(), 114.0/60.0);
    ev = output->GetItem(2);
    TS_ASSERT_EQUALS(ev->GetLabel(), "Foot Strike");
    TS_ASSERT_EQUALS(ev->GetContext(), "Left");
    TS_ASSERT_EQUALS(ev->GetDetectionFlags(), btk::Event::Automatic | btk::Event::FromForcePlatform);
    TS_ASSERT_EQUALS(ev->GetId(), 1);
    TS_ASSERT_EQUALS(ev->GetFrame(), 109);
    TS_ASSERT_EQUALS(ev->GetTime(), 109.0/60.0);
    ev = output->GetItem(3);
    TS_ASSERT_EQUALS(ev->GetLabel(), "Foot Off");
    TS_ASSERT_EQUALS(ev->GetContext(), "Left");
    TS_ASSERT_EQUALS(ev->GetDetectionFlags(), btk::Event::Automatic | btk::Event::FromForcePlatform);
    TS_ASSERT_EQUALS(ev->GetId(), 2);
    TS_ASSERT_EQUALS(ev->GetFrame(), 148);
    TS_ASSERT_EQUALS(ev->GetTime(), 148.0/60.0);
  };
  
  CXXTEST_TEST(PluginC3D_ROI)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample09/PlugInC3D.c3d");
    reader->Update(); // Because this is need to give the acquisition information...
    btk::ForcePlatformsExtractor::Pointer pfe = btk::ForcePlatformsExtractor::New();
    btk::GroundReactionWrenchFilter::Pointer grwf = btk::GroundReactionWrenchFilter::New();
    btk::DownsampleFilter<btk::WrenchCollection>::Pointer dswc = btk::DownsampleFilter<btk::WrenchCollection>::New();
    dswc->SetUpDownRatio(reader->GetOutput()->GetNumberAnalogSamplePerFrame());
    pfe->SetInput(reader->GetOutput());
    grwf->SetInput(pfe->GetOutput());
    dswc->SetInput(grwf->GetOutput());
    btk::VerticalGroundReactionForceGaitEventDetector::Pointer vgrfged = btk::VerticalGroundReactionForceGaitEventDetector::New();
    vgrfged->SetInput(dswc->GetOutput());
    // vgrfged->SetThresholdValue(10); // Default value
    std::vector<std::string> mapping(2); mapping[0] = "Right"; mapping[1] = "Left";
    vgrfged->SetForceplateContextMapping(mapping);
    vgrfged->SetAcquisitionInformation(reader->GetOutput()->GetFirstFrame(), reader->GetOutput()->GetPointFrequency(), "");
    vgrfged->SetForceplateContextMapping(mapping);
    vgrfged->SetRegionOfInterest(60-reader->GetOutput()->GetFirstFrame(), 120-reader->GetOutput()->GetFirstFrame());
    btk::EventCollection::Pointer output = vgrfged->GetOutput();
    output->Update();
    
    btk::Event::Pointer ev;
    TS_ASSERT_EQUALS(output->GetItemNumber(), 3);
    ev = output->GetItem(0);
    TS_ASSERT_EQUALS(ev->GetLabel(), "Foot Strike");
    TS_ASSERT_EQUALS(ev->GetContext(), "Right");
    TS_ASSERT_EQUALS(ev->GetDetectionFlags(), btk::Event::Automatic | btk::Event::FromForcePlatform);
    TS_ASSERT_EQUALS(ev->GetId(), 1);
    TS_ASSERT_EQUALS(ev->GetFrame(), 65);
    TS_ASSERT_EQUALS(ev->GetTime(), 65.0/60.0);
    ev = output->GetItem(1);
    TS_ASSERT_EQUALS(ev->GetLabel(), "Foot Off");
    TS_ASSERT_EQUALS(ev->GetContext(), "Right");
    TS_ASSERT_EQUALS(ev->GetDetectionFlags(), btk::Event::Automatic | btk::Event::FromForcePlatform);
    TS_ASSERT_EQUALS(ev->GetId(), 2);
    TS_ASSERT_EQUALS(ev->GetFrame(), 116);
    TS_ASSERT_EQUALS(ev->GetTime(), 116.0/60.0);
    ev = output->GetItem(2);
    TS_ASSERT_EQUALS(ev->GetLabel(), "Foot Strike");
    TS_ASSERT_EQUALS(ev->GetContext(), "Left");
    TS_ASSERT_EQUALS(ev->GetDetectionFlags(), btk::Event::Automatic | btk::Event::FromForcePlatform);
    TS_ASSERT_EQUALS(ev->GetId(), 1);
    TS_ASSERT_EQUALS(ev->GetFrame(), 108);
    TS_ASSERT_EQUALS(ev->GetTime(), 108.0/60.0);
  };
};

CXXTEST_SUITE_REGISTRATION(VerticalGroundReactionForceGaitEventDetectorTest)
CXXTEST_TEST_REGISTRATION(VerticalGroundReactionForceGaitEventDetectorTest, NoWrench)
CXXTEST_TEST_REGISTRATION(VerticalGroundReactionForceGaitEventDetectorTest, Gait)
CXXTEST_TEST_REGISTRATION(VerticalGroundReactionForceGaitEventDetectorTest, Gait_NoMapping)
CXXTEST_TEST_REGISTRATION(VerticalGroundReactionForceGaitEventDetectorTest, PluginC3D)
CXXTEST_TEST_REGISTRATION(VerticalGroundReactionForceGaitEventDetectorTest, PluginC3D_Threshold50)
CXXTEST_TEST_REGISTRATION(VerticalGroundReactionForceGaitEventDetectorTest, PluginC3D_ROI)

#endif // VerticalGroundReactionForceGaitEventDetectorTest_h