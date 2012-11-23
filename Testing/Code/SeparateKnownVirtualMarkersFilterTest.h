#ifndef SeparateKnownVirtualMarkersFilterTest_h
#define SeparateKnownVirtualMarkersFilterTest_h

#include <btkPoint.h>
#include <btkAcquisition.h>
#include  <btkPointCollection.h>
#include <btkAcquisitionFileReader.h>
#include <btkSeparateKnownVirtualMarkersFilter.h>

CXXTEST_SUITE(SeparateKnownVirtualMarkersFilterTest)
{ 
  CXXTEST_TEST(Constructor)
  {
    btk::SeparateKnownVirtualMarkersFilter::Pointer skvm = btk::SeparateKnownVirtualMarkersFilter::New();
    std::list<btk::SeparateKnownVirtualMarkersFilter::StringAxes> labels = skvm->GetVirtualReferenceFrames();
    size_t num = 19;
    TS_ASSERT_EQUALS(labels.size(), num);
    std::list<btk::SeparateKnownVirtualMarkersFilter::StringAxes>::const_iterator it = labels.begin();
    if (labels.size() >= num)
    {
      // HED
      TS_ASSERT_EQUALS(it->Origin, std::string("HEDO"));
      TS_ASSERT_EQUALS(it->Axis1, std::string("HEDA"));
      TS_ASSERT_EQUALS(it->Axis2, std::string("HEDL"));
      TS_ASSERT_EQUALS(it->Axis3, std::string("HEDP"));
      ++it;
      // LCL
      TS_ASSERT_EQUALS(it->Origin, std::string("LCLO"));
      TS_ASSERT_EQUALS(it->Axis1, std::string("LCLA"));
      TS_ASSERT_EQUALS(it->Axis2, std::string("LCLL"));
      TS_ASSERT_EQUALS(it->Axis3, std::string("LCLP"));
      ++it;
      // LFE
      TS_ASSERT_EQUALS(it->Origin, std::string("LFEO"));
      TS_ASSERT_EQUALS(it->Axis1, std::string("LFEA"));
      TS_ASSERT_EQUALS(it->Axis2, std::string("LFEL"));
      TS_ASSERT_EQUALS(it->Axis3, std::string("LFEP"));
      ++it;
      // LFO
      TS_ASSERT_EQUALS(it->Origin, std::string("LFOO"));
      TS_ASSERT_EQUALS(it->Axis1, std::string("LFOA"));
      TS_ASSERT_EQUALS(it->Axis2, std::string("LFOL"));
      TS_ASSERT_EQUALS(it->Axis3, std::string("LFOP"));
      ++it;
      // LHN
      TS_ASSERT_EQUALS(it->Origin, std::string("LHNO"));
      TS_ASSERT_EQUALS(it->Axis1, std::string("LHNA"));
      TS_ASSERT_EQUALS(it->Axis2, std::string("LHNL"));
      TS_ASSERT_EQUALS(it->Axis3, std::string("LHNP"));
      ++it;
      // LHU
      TS_ASSERT_EQUALS(it->Origin, std::string("LHUO"));
      TS_ASSERT_EQUALS(it->Axis1, std::string("LHUA"));
      TS_ASSERT_EQUALS(it->Axis2, std::string("LHUL"));
      TS_ASSERT_EQUALS(it->Axis3, std::string("LHUP"));
      ++it;
      // LRA
      TS_ASSERT_EQUALS(it->Origin, std::string("LRAO"));
      TS_ASSERT_EQUALS(it->Axis1, std::string("LRAA"));
      TS_ASSERT_EQUALS(it->Axis2, std::string("LRAL"));
      TS_ASSERT_EQUALS(it->Axis3, std::string("LRAP"));
      ++it;
      // LTI
      TS_ASSERT_EQUALS(it->Origin, std::string("LTIO"));
      TS_ASSERT_EQUALS(it->Axis1, std::string("LTIA"));
      TS_ASSERT_EQUALS(it->Axis2, std::string("LTIL"));
      TS_ASSERT_EQUALS(it->Axis3, std::string("LTIP"));
      ++it;
      // LTO
      TS_ASSERT_EQUALS(it->Origin, std::string("LTOO"));
      TS_ASSERT_EQUALS(it->Axis1, std::string("LTOA"));
      TS_ASSERT_EQUALS(it->Axis2, std::string("LTOL"));
      TS_ASSERT_EQUALS(it->Axis3, std::string("LTOP"));
      ++it;
      // PEL
      TS_ASSERT_EQUALS(it->Origin, std::string("PELO"));
      TS_ASSERT_EQUALS(it->Axis1, std::string("PELA"));
      TS_ASSERT_EQUALS(it->Axis2, std::string("PELL"));
      TS_ASSERT_EQUALS(it->Axis3, std::string("PELP"));
      ++it;
      // RCL
      TS_ASSERT_EQUALS(it->Origin, std::string("RCLO"));
      TS_ASSERT_EQUALS(it->Axis1, std::string("RCLA"));
      TS_ASSERT_EQUALS(it->Axis2, std::string("RCLL"));
      TS_ASSERT_EQUALS(it->Axis3, std::string("RCLP"));
      ++it;
      // RFE
      TS_ASSERT_EQUALS(it->Origin, std::string("RFEO"));
      TS_ASSERT_EQUALS(it->Axis1, std::string("RFEA"));
      TS_ASSERT_EQUALS(it->Axis2, std::string("RFEL"));
      TS_ASSERT_EQUALS(it->Axis3, std::string("RFEP"));
      ++it;
      // RFO
      TS_ASSERT_EQUALS(it->Origin, std::string("RFOO"));
      TS_ASSERT_EQUALS(it->Axis1, std::string("RFOA"));
      TS_ASSERT_EQUALS(it->Axis2, std::string("RFOL"));
      TS_ASSERT_EQUALS(it->Axis3, std::string("RFOP"));
      ++it;
      // RHN
      TS_ASSERT_EQUALS(it->Origin, std::string("RHNO"));
      TS_ASSERT_EQUALS(it->Axis1, std::string("RHNA"));
      TS_ASSERT_EQUALS(it->Axis2, std::string("RHNL"));
      TS_ASSERT_EQUALS(it->Axis3, std::string("RHNP"));
      ++it;
      // RHU
      TS_ASSERT_EQUALS(it->Origin, std::string("RHUO"));
      TS_ASSERT_EQUALS(it->Axis1, std::string("RHUA"));
      TS_ASSERT_EQUALS(it->Axis2, std::string("RHUL"));
      TS_ASSERT_EQUALS(it->Axis3, std::string("RHUP"));
      ++it;
      // RRA
      TS_ASSERT_EQUALS(it->Origin, std::string("RRAO"));
      TS_ASSERT_EQUALS(it->Axis1, std::string("RRAA"));
      TS_ASSERT_EQUALS(it->Axis2, std::string("RRAL"));
      TS_ASSERT_EQUALS(it->Axis3, std::string("RRAP"));
      ++it;
      // RTI
      TS_ASSERT_EQUALS(it->Origin, std::string("RTIO"));
      TS_ASSERT_EQUALS(it->Axis1, std::string("RTIA"));
      TS_ASSERT_EQUALS(it->Axis2, std::string("RTIL"));
      TS_ASSERT_EQUALS(it->Axis3, std::string("RTIP"));
      ++it;
      // RTO
      TS_ASSERT_EQUALS(it->Origin, std::string("RTOO"));
      TS_ASSERT_EQUALS(it->Axis1, std::string("RTOA"));
      TS_ASSERT_EQUALS(it->Axis2, std::string("RTOL"));
      TS_ASSERT_EQUALS(it->Axis3, std::string("RTOP"));
      ++it;
      // TRX
      TS_ASSERT_EQUALS(it->Origin, std::string("TRXO"));
      TS_ASSERT_EQUALS(it->Axis1, std::string("TRXA"));
      TS_ASSERT_EQUALS(it->Axis2, std::string("TRXL"));
      TS_ASSERT_EQUALS(it->Axis3, std::string("TRXP"));
      ++it;
    }
    std::list<std::string> labels2 = skvm->GetVirtualMarkers();
    num = 2;
    TS_ASSERT_EQUALS(labels2.size(), num);
    std::list<std::string>::const_iterator it2 = labels2.begin();
    if (labels2.size() >= num)
    {
      TS_ASSERT_EQUALS(*it2, std::string("CentreOfMass")); ++it2;
      TS_ASSERT_EQUALS(*it2, std::string("CentreOfMassFloor")); ++it2;
    }
  };
  
  CXXTEST_TEST(DefaultLabels)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample09/PlugInC3D.c3d");
    reader->Update();
    
    btk::SeparateKnownVirtualMarkersFilter::Pointer skvm = btk::SeparateKnownVirtualMarkersFilter::New();
    skvm->SetInput(reader->GetOutput()->GetPoints());
    skvm->Update();
    
    int inc;
     // markers
    btk::PointCollection::Pointer points = skvm->GetOutput(0);
    TS_ASSERT_EQUALS(points->GetItemNumber(), 18);
    inc = 0;
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RKNE"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RTOE"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RTIB"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RASI"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RTHI"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RHEE"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LKNE"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RANK"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RCLA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LTHI"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LASI"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("C7"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LTOE"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LANK"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("SACR"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LHEE"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LCLA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LTIB"));
    // virtual used for axes
    points = skvm->GetOutput(1);
    TS_ASSERT_EQUALS(points->GetItemNumber(), 36);
    inc = 0;
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LFEO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LFEA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LFEL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LFEP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LFOO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LFOA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LFOL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LFOP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LTIO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LTIA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LTIL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LTIP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LTOO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LTOA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LTOL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("LTOP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("PELO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("PELA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("PELL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("PELP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RFEO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RFEA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RFEL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RFEP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RFOO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RFOA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RFOL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RFOP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RTIO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RTIA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RTIL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RTIP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RTOO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RTOA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RTOL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("RTOP"));
    // other virtual markers
    points = skvm->GetOutput(2);
    TS_ASSERT_EQUALS(points->GetItemNumber(), 0);
    // other type of points
    points = skvm->GetOutput(3);
    TS_ASSERT_EQUALS(points->GetItemNumber(), 32);
  };
  
  CXXTEST_TEST(DefaultLabelsAndPrefix)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample04/sub_labels.c3d");
    reader->Update();
    
    btk::SeparateKnownVirtualMarkersFilter::Pointer skvm = btk::SeparateKnownVirtualMarkersFilter::New();
    skvm->SetInput(reader->GetOutput()->GetPoints());
    skvm->SetLabelPrefix("Matt:");
    skvm->Update();
    
    int inc;
     // markers
    btk::PointCollection::Pointer points = skvm->GetOutput(0);
    TS_ASSERT_EQUALS(points->GetItemNumber(), 50);
    inc = 0;
    // virtual used for axes
    points = skvm->GetOutput(1);
    TS_ASSERT_EQUALS(points->GetItemNumber(), 36);
    inc = 0;
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LFEO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LFEA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LFEL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LFEP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LFOO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LFOA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LFOL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LFOP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LTIO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LTIA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LTIL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LTIP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LTOO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LTOA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LTOL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LTOP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:PELO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:PELA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:PELL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:PELP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RFEO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RFEA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RFEL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RFEP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RFOO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RFOA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RFOL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RFOP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RTIO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RTIA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RTIL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RTIP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RTOO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RTOA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RTOL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RTOP"));
    // other virtual markers
    points = skvm->GetOutput(2);
    TS_ASSERT_EQUALS(points->GetItemNumber(), 0);
    // other type of points
    points = skvm->GetOutput(3);
    TS_ASSERT_EQUALS(points->GetItemNumber(), 28);
  };
  
  CXXTEST_TEST(FromLabelsLists)
  {
    // virtual markers for frame axes
    const char* labels[] = {"LFE", "LFO", "LTI", "LTO", "RFE", "RFO", "RTI", "RTO"};
    size_t ln = sizeof(labels) / sizeof(char*);
    std::list<btk::SeparateKnownVirtualMarkersFilter::StringAxes> virtualMarkerLabelsAxes;
    for (size_t i = 0 ; i < ln ; ++i)
    {
      std::string label(labels[i]);
      virtualMarkerLabelsAxes.push_back(btk::SeparateKnownVirtualMarkersFilter::StringAxes(
                                        label + "O", label + "A", label + "L", label + "P"));
    }
  
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample04/sub_labels.c3d");
    reader->Update();
    
    btk::SeparateKnownVirtualMarkersFilter::Pointer skvm = btk::SeparateKnownVirtualMarkersFilter::New();
    skvm->SetInput(reader->GetOutput()->GetPoints());
    skvm->SetLabelPrefix("Matt:");
    skvm->AppendVirtualMarker("LKNE");
    skvm->AppendVirtualMarker("RKNE");
    skvm->SetVirtualReferenceFrames(virtualMarkerLabelsAxes);
    skvm->Update();
    
    int inc;
     // markers
    btk::PointCollection::Pointer points = skvm->GetOutput(0);
    TS_ASSERT_EQUALS(points->GetItemNumber(), 52);
    inc = 0;
    // virtual used for axes
    points = skvm->GetOutput(1);
    TS_ASSERT_EQUALS(points->GetItemNumber(), 32);
    inc = 0;
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LFEO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LFEA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LFEL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LFEP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LFOO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LFOA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LFOL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LFOP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LTIO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LTIA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LTIL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LTIP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LTOO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LTOA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LTOL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LTOP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RFEO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RFEA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RFEL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RFEP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RFOO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RFOA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RFOL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RFOP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RTIO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RTIA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RTIL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RTIP"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RTOO"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RTOA"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RTOL"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RTOP"));
    // other virtual markers
    points = skvm->GetOutput(2);
    TS_ASSERT_EQUALS(points->GetItemNumber(), 2);
    inc = 0;
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:LKNE"));
    TS_ASSERT_EQUALS(points->GetItem(inc++)->GetLabel(), std::string("Matt:RKNE"));
    // other type of points
    points = skvm->GetOutput(3);
    TS_ASSERT_EQUALS(points->GetItemNumber(), 28);
  };
};

CXXTEST_SUITE_REGISTRATION(SeparateKnownVirtualMarkersFilterTest)
CXXTEST_TEST_REGISTRATION(SeparateKnownVirtualMarkersFilterTest, Constructor)
CXXTEST_TEST_REGISTRATION(SeparateKnownVirtualMarkersFilterTest, DefaultLabels)
CXXTEST_TEST_REGISTRATION(SeparateKnownVirtualMarkersFilterTest, DefaultLabelsAndPrefix)
CXXTEST_TEST_REGISTRATION(SeparateKnownVirtualMarkersFilterTest, FromLabelsLists)

#endif
