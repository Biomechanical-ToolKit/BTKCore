#ifndef TriangleMeshTestTest_h
#define TriangleMeshTestTest_h

#include <btkTriangleMesh.h>
#include <btkPointCollection.h>

CXXTEST_SUITE(TriangleMeshTestTest)
{
  CXXTEST_TEST(Constructor_Empty)
  {
    std::vector<int> m;
    std::vector<btk::TriangleMesh::VertexLink> l;
    btk::TriangleMesh::Pointer mesh = btk::TriangleMesh::New(m,l);
    TS_ASSERT_EQUALS(mesh->GetVertexNumber(),0);
    TS_ASSERT_EQUALS(mesh->GetEdgeNumber(),0);
    TS_ASSERT_EQUALS(mesh->GetFaceNumber(),0);
  };

  CXXTEST_TEST(Constructor_NoLink)
  {
    std::vector<int> m(5);
    for (int i = 0 ; i < 5 ; ++i)
      m[i] = i;
    std::vector<btk::TriangleMesh::VertexLink> l;
    btk::TriangleMesh::Pointer mesh = btk::TriangleMesh::New(m,l);
    TS_ASSERT_EQUALS(mesh->GetVertexNumber(),5);
    TS_ASSERT_EQUALS(mesh->GetEdgeNumber(),0);
    TS_ASSERT_EQUALS(mesh->GetFaceNumber(),0);
  };

  CXXTEST_TEST(Constructor_NoMarker)
  {
    std::vector<int> m;
    std::vector<btk::TriangleMesh::VertexLink> l(5);
    l[0].SetIds(0,1);
    l[1].SetIds(0,2);
    l[2].SetIds(0,3);
    l[3].SetIds(1,2);
    l[4].SetIds(1,3);
    btk::TriangleMesh::Pointer mesh = btk::TriangleMesh::New(m,l);
    TS_ASSERT_EQUALS(mesh->GetVertexNumber(),0);
    TS_ASSERT_EQUALS(mesh->GetEdgeNumber(),0);
    TS_ASSERT_EQUALS(mesh->GetFaceNumber(),0);
  };

  CXXTEST_TEST(Constructor_WrongLink)
  {
    std::vector<int> m(5);
    for (int i = 0 ; i < 5 ; ++i)
      m[i] = i;
    std::vector<btk::TriangleMesh::VertexLink> l(10);
    l[0].SetIds(0,1);
    l[1].SetIds(0,2);
    l[2].SetIds(0,3);
    l[3].SetIds(1,2);
    l[4].SetIds(1,3);
    l[5].SetIds(1,6);
    l[6].SetIds(1,7);
    l[7].SetIds(7,8);
    l[8].SetIds(1,9);
    l[9].SetIds(10,3);
    btk::TriangleMesh::Pointer mesh = btk::TriangleMesh::New(m,l);
    TS_ASSERT_EQUALS(mesh->GetVertexNumber(),5);
    TS_ASSERT_EQUALS(mesh->GetEdgeNumber(),5);
    TS_ASSERT_EQUALS(mesh->GetFaceNumber(),2);
    btk::TriangleMesh::VertexConstIterator itM = mesh->BeginVertex();
    btk::TriangleMesh::EdgeConstIterator itE = mesh->BeginEdge();
    btk::TriangleMesh::FaceConstIterator itF = mesh->BeginFace();
    // Edge #0
    itM = mesh->BeginVertex(); std::advance(itM,0);
    TS_ASSERT(itE->GetVertex1() == &(*itM));
    itM = mesh->BeginVertex(); std::advance(itM,1);
    TS_ASSERT(itE->GetVertex2() == &(*itM));
    // Edge #1
    ++itE;
    itM = mesh->BeginVertex(); std::advance(itM,0);
    TS_ASSERT(itE->GetVertex1() == &(*itM));
    itM = mesh->BeginVertex(); std::advance(itM,2);
    TS_ASSERT(itE->GetVertex2() == &(*itM));
    // Edge #2
    ++itE;
    itM = mesh->BeginVertex(); std::advance(itM,0);
    TS_ASSERT(itE->GetVertex1() == &(*itM));
    itM = mesh->BeginVertex(); std::advance(itM,3);
    TS_ASSERT(itE->GetVertex2() == &(*itM));
    // Edge #3
    ++itE;
    itM = mesh->BeginVertex(); std::advance(itM,1);
    TS_ASSERT(itE->GetVertex1() == &(*itM));
    itM = mesh->BeginVertex(); std::advance(itM,2);
    TS_ASSERT(itE->GetVertex2() == &(*itM));
    // Edge #4
    ++itE;
    itM = mesh->BeginVertex(); std::advance(itM,1);
    TS_ASSERT(itE->GetVertex1() == &(*itM));
    itM = mesh->BeginVertex(); std::advance(itM,3);
    TS_ASSERT(itE->GetVertex2() == &(*itM));
    // Face #0
    itM = mesh->BeginVertex(); std::advance(itM,0);
    TS_ASSERT(itF->GetVertex1() == &(*itM));
    itM = mesh->BeginVertex(); std::advance(itM,1);
    TS_ASSERT(itF->GetVertex2() == &(*itM));
    itM = mesh->BeginVertex(); std::advance(itM,2);
    TS_ASSERT(itF->GetVertex3() == &(*itM));
    // Face #0
    ++itF;
    itM = mesh->BeginVertex(); std::advance(itM,0);
    TS_ASSERT(itF->GetVertex1() == &(*itM));
    itM = mesh->BeginVertex(); std::advance(itM,1);
    TS_ASSERT(itF->GetVertex2() == &(*itM));
    itM = mesh->BeginVertex(); std::advance(itM,3);
    TS_ASSERT(itF->GetVertex3() == &(*itM));
  };

  CXXTEST_TEST(ConnectPoints)
  {
    btk::PointCollection::Pointer points = btk::PointCollection::New();
    for (int i = 0 ; i < 5 ; ++i)
    {
      btk::Point::Pointer point = btk::Point::New(1);
      point->SetFrame(0,1.0,1.0,1.0); // Valid point
      points->InsertItem(point);
    }
    std::vector<int> m(5);
    for (int i = 0 ; i < 5 ; ++i)
      m[i] = i;
    std::vector<btk::TriangleMesh::VertexLink> l(10);
    l[0].SetIds(0,1);
    l[1].SetIds(0,2);
    l[2].SetIds(0,3);
    l[3].SetIds(1,2);
    l[4].SetIds(1,3);
    btk::TriangleMesh::Pointer mesh = btk::TriangleMesh::New(m,l);
    TS_ASSERT_EQUALS(mesh->GetVertexNumber(),5);
    TS_ASSERT_EQUALS(mesh->GetEdgeNumber(),5);
    TS_ASSERT_EQUALS(mesh->GetFaceNumber(),2);
    TS_ASSERT(mesh->ConnectPoints(points));
    mesh->SetCurrentFrame(0);
    for (btk::TriangleMesh::VertexConstIterator it = mesh->BeginVertex() ; it != mesh->EndVertex() ; ++it)
    {
      TS_ASSERT(it->IsValid());
      TS_ASSERT_EQUALS(it->GetCoordinateX(), 1.0);
      TS_ASSERT_EQUALS(it->GetCoordinateY(), 1.0);
      TS_ASSERT_EQUALS(it->GetCoordinateZ(), 1.0);
    }
    for (btk::TriangleMesh::EdgeConstIterator it = mesh->BeginEdge() ; it != mesh->EndEdge() ; ++it)
    {
      TS_ASSERT(it->IsValid());
    }
    for (btk::TriangleMesh::FaceConstIterator it = mesh->BeginFace() ; it != mesh->EndFace() ; ++it)
    {
      TS_ASSERT(it->IsValid());
    }
  };
};

CXXTEST_SUITE_REGISTRATION(TriangleMeshTestTest)
CXXTEST_TEST_REGISTRATION(TriangleMeshTestTest, Constructor_Empty)
CXXTEST_TEST_REGISTRATION(TriangleMeshTestTest, Constructor_NoLink)
CXXTEST_TEST_REGISTRATION(TriangleMeshTestTest, Constructor_NoMarker)
CXXTEST_TEST_REGISTRATION(TriangleMeshTestTest, Constructor_WrongLink)
CXXTEST_TEST_REGISTRATION(TriangleMeshTestTest, ConnectPoints)
#endif
