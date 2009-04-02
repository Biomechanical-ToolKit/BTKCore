#ifndef VTKMarkerSourceTest_h
#define VTKMarkerSourceTest_h

#include <btkVTKMarkerSource.h>

CXXTEST_SUITE(VTKMarkerSourceTest)
{
  CXXTEST_TEST(Constructor)
  {
    btk::VTKMarkerSource* test = btk::VTKMarkerSource::New();
    TS_ASSERT_EQUALS(test->GetNumberOfInputPorts(), 1);
    TS_ASSERT_EQUALS(test->GetNumberOfOutputPorts(), 1);
    test->Delete();
  };
  
  CXXTEST_TEST(SetInput)
  {
    btk::VTKMarkerSource* test = btk::VTKMarkerSource::New();
    btk::Marker::Pointer marker = btk::Marker::New("TEST", 10);
    test->SetInput(marker);
    TS_ASSERT_EQUALS(marker.use_count(), 2);
    test->Delete();
    TS_ASSERT_EQUALS(marker.use_count(), 1);
  };
  
  CXXTEST_TEST(Update)
  {
    btk::VTKMarkerSource* test = btk::VTKMarkerSource::New();
    btk::Marker::Pointer marker = btk::Marker::New("FRAME#1", 10);
    double* coord = marker->GetValues().data();
    coord[0] = 0; coord[10] = 0; coord[20] = 0;
    coord[1] = 1; coord[11] = 1; coord[21] = 1;
    coord[2] = 2; coord[12] = 2; coord[22] = 2;
    coord[3] = 3; coord[13] = 3; coord[23] = 3;
    coord[4] = 4; coord[14] = 4; coord[24] = 4;
    coord[5] = 5; coord[15] = 5; coord[25] = 5;
    coord[6] = 6; coord[16] = 6; coord[26] = 6;
    coord[7] = 7; coord[17] = 7; coord[27] = 7;
    coord[8] = 8; coord[18] = 8; coord[28] = 8;
    coord[9] = 9; coord[19] = 9; coord[29] = 9;
    test->SetInput(marker);
    vtkPolyData* poly = test->GetOutput();
    test->Update();
    vtkPoints* points = poly->GetPoints();
    TS_ASSERT_EQUALS(points->GetNumberOfPoints(), 10);
    double* point = points->GetPoint(3);
    TS_ASSERT_EQUALS(point[0], 3);
    TS_ASSERT_EQUALS(point[1], 3);
    TS_ASSERT_EQUALS(point[2], 3);
    point = points->GetPoint(9);
    TS_ASSERT_EQUALS(point[0], 9);
    TS_ASSERT_EQUALS(point[1], 9);
    TS_ASSERT_EQUALS(point[2], 9);
    test->Delete();
  };
};

CXXTEST_SUITE_REGISTRATION(VTKMarkerSourceTest)
CXXTEST_TEST_REGISTRATION(VTKMarkerSourceTest, Constructor)
CXXTEST_TEST_REGISTRATION(VTKMarkerSourceTest, SetInput)
CXXTEST_TEST_REGISTRATION(VTKMarkerSourceTest, Update)
#endif