/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2012, Arnaud Barré
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name(s) of the copyright holders nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
 
#include "btkTriangleMesh.h"
#include "btkConvert.h"

namespace btk
{
  /**
   * @class TriangleMesh btkTriangleMesh.h
   * Read-only class representing a triangle mesh based on markers's IDs and links between them.
   */
    
  /**
   * @typedef TriangleMesh::Pointer
   * Smart pointer associated with a TriangleMesh object.
   */

  /**
   * @typedef TriangleMesh::ConstPointer
   * Smart pointer associated with a const TriangleMesh object.
   */

  /**
   * @typedef TriangleMesh::VertexIterator
   * Iterator related to the list of vertices stored in the mesh.
   */
  
  /**
   * @typedef TriangleMesh::VertexConstIterator
   * Const Iterator related to the list of vertices stored in the mesh.
   */

  /**
   * @typedef TriangleMesh::EdgeIterator
   * Iterator related to the list of edges stored in the mesh.
   */
  
  /**
   * @typedef TriangleMesh::EdgeConstIterator
   * Const Iterator related to the list of edges stored in the mesh.
   */
   
   /**
    * @typedef TriangleMesh::FaceIterator
    * Iterator related to the list of faces stored in the mesh.
    */

   /**
    * @typedef TriangleMesh::FaceConstIterator
    * Const Iterator related to the list of faces stored in the mesh.
    */
    
  /**
   * @fn TriangleMesh::Pointer TriangleMesh::New(const std::list<int>& m, const std::list<TriangleMesh::VertexLink>& l)
   * Creates a smart pointer to a TriangleMesh object.
   */
   
  /**
   * @fn TriangleMesh::Pointer TriangleMesh::New(const std::list<int>& m, const std::list<TriangleMesh::VertexLink>& l, const std::list<TriangleMesh::VertexFace>& f)
   * Creates a smart pointer to a TriangleMesh object.
   */
    
  /**
   * @fn TriangleMesh::VertexIterator TriangleMesh::BeginVertex()
   * Returns an iterator to the beginning of the list of vertices.
   */
  
  /**
   * @fn TriangleMesh::VertexConstIterator TriangleMesh::BeginVertex() const
   * Returns a const iterator to the beginning of the list of vertices.
   */
  
  /**
   * @fn TriangleMesh::VertexIterator TriangleMesh::EndVertex()
   * Returns an iterator just past the last vertex.
   */
  
  /**
   * @fn TriangleMesh::VertexConstIterator TriangleMesh::EndVertex() const
   * Returns a const iterator just past the last vertex.
   */
   
  /**
   * @fn TriangleMesh::EdgeIterator TriangleMesh::BeginEdge()
   * Returns an iterator to the beginning of the list of edges.
   */
  
  /**
   * @fn TriangleMesh::EdgeConstIterator TriangleMesh::BeginEdge() const
   * Returns a const iterator to the beginning of the list of edges.
   */
  
  /**
   * @fn TriangleMesh::EdgeIterator TriangleMesh::EndEdge()
   * Returns an iterator just past the last edge.
   */
  
  /**
   * @fn TriangleMesh::EdgeConstIterator TriangleMesh::EndEdge() const
   * Returns a const iterator just past the last edge.
   */
   
  /**
   * @fn TriangleMesh::FaceIterator TriangleMesh::BeginFace()
   * Returns an iterator to the beginning of the list of faces.
   */
  
  /**
   * @fn TriangleMesh::FaceConstIterator TriangleMesh::BeginFace() const
   * Returns a const iterator to the beginning of the list of faces.
   */
  
  /**
   * @fn TriangleMesh::FaceIterator TriangleMesh::EndFace()
   * Returns an iterator just past the last face.
   */
  
  /**
   * @fn TriangleMesh::FaceConstIterator TriangleMesh::EndFace() const
   * Returns a const iterator just past the last face.
   */
  
  /**
   * @fn int TriangleMesh::GetVertexNumber() const
   * Returns the number of vertives.
   */
  
  /**
   * @fn int TriangleMesh::GetEdgeNumber() const
   * Returns the number of edges.
   */
   
  /**
   * @fn int TriangleMesh::GetFaceNumber() const
   * Returns the number of faces.
   */
  
  /**
   * Return the greatest ID for the vertices stored in the mesh.
   */
  int TriangleMesh::GetMaxVertexId() const
  {
    int maxId = -1;
    for (size_t i = 0 ; i < this->m_Vertices.size() ; ++i)
    {
      if (this->m_Vertices[i].m_Id > maxId)
        maxId = this->m_Vertices[i].m_Id;
    };
    return maxId;
  };
  
  /**
   * Connect the given @a points to the mesh by finding the corresponding IDs. Returns true if all the vertices are connected.
   * If one of the point are not found (vertex ID not recognised), then the connections are reseted and the method return false.
   */
  bool TriangleMesh::ConnectPoints(PointCollection::Pointer points)
  {
    if (this->mp_Points == points)
      return true;
      
    int pointNumber = points->GetItemNumber();
    
    bool found = true;
    for (size_t i = 0 ; i < this->m_Vertices.size() ; ++i)
    {
      if ((this->m_Vertices[i].m_Id < 0) || (this->m_Vertices[i].m_Id >= pointNumber))
      {
        found = false;
        break;
      }
      else
      {
        PointCollection::ConstIterator it = points->Begin();
        std::advance(it, this->m_Vertices[i].m_Id);
        this->m_Vertices[i].mp_Point = *it;
      }
    }
    if (!found)
    {
      for (size_t i = 0 ; i < this->m_Vertices.size() ; ++i)
        this->m_Vertices[i].mp_Point.reset();
      this->mp_Points.reset();
    }
    if (!points)
      found = true;
    return found;
  };
  
  /**
   * @fn int TriangleMesh::GetCurrentFrameIndex() const
   * Returns the index of the current frame used by the vertices to get their coordinates. The index starts from 0.
   */
   
  /**
   * @fn void TriangleMesh::SetCurrentFrameIndex(int frame)
   * Sets the index of the current frame used by the vertices to get their coordinates. The index for the first frame is 0.
   */
  
  /**
   * Set the mesh based only on the given marker's id @a m and the links between them @a l.
   * If one of the given link contains a wrong ID (out of range), then, it is not added to the list of edges.
   * @warning This method can create non-manifold mesh!
   */
  void TriangleMesh::SetDefinition(const std::vector<int>& m, const std::vector<VertexLink>& l)
  {
    this->m_Vertices.resize(m.size());
    this->m_Edges.resize(l.size());
    this->m_Faces.clear();
    
    this->SetGeometryPartially(m,l);
    
    // Build faces
    // - List the connectivity of each vertex
    std::vector< std::list<int> > connectivity(this->m_Vertices.size());
    for (int i = 0 ; i < static_cast<int>(connectivity.size()) ; ++i)
    {
      for (size_t j = 0 ; j < this->m_Edges.size() ; ++j)
      {
        if ((this->m_Edges[j].mp_Vertices[0]->m_RelativeId == i) && (this->m_Edges[j].mp_Vertices[1]->m_RelativeId > i))
          connectivity[i].push_back(this->m_Edges[j].mp_Vertices[1]->m_RelativeId);
        if ((this->m_Edges[j].mp_Vertices[1]->m_RelativeId == i) && (this->m_Edges[j].mp_Vertices[0]->m_RelativeId > i))
          connectivity[i].push_back(this->m_Edges[j].mp_Vertices[0]->m_RelativeId);
      }
      // Sort the markers connected
      connectivity[i].sort();
    }
    // - Look for connections between points
    for (int i = 0 ; i < static_cast<int>(connectivity.size()) ; ++i)
    {
      for (std::list<int>::const_iterator itP = connectivity[i].begin() ; itP != connectivity[i].end() ; ++itP)
      {
        if (*itP <= i)
          continue;
        for (std::list<int>::const_iterator itN = connectivity[*itP].begin() ; itN != connectivity[*itP].end() ; ++itN)
        {
          std::list<int>::const_iterator itPP = itP; ++itPP;
          for ( ; itPP != connectivity[i].end() ; ++itPP)
          {
            if (*itPP == *itN)
            {
              // Face found
              // TODO: Sort the vertex in a counter clockwise order (surface normal outside).
              Face face;
              face.mp_Vertices[0] = &this->m_Vertices[i];
              face.mp_Vertices[1] = &this->m_Vertices[*itP];
              face.mp_Vertices[2] = &this->m_Vertices[*itN];
              this->m_Faces.push_back(face);
              break;
            }
          }
        }
      }
    }
  };
   
  /**
   * Set the mesh based on the given marker's id @a m, the links between them (@a l) and the faces defined as a set of three markers. (@a f).
   * If one of the given link/face contains a wrong ID (out of range), then, it is not added to the list of edges/faces.
   */
  void TriangleMesh::SetDefinition(const std::vector<int>& m, const std::vector<VertexLink>& l, const std::vector<VertexFace>& f)
  {
    this->m_Vertices.resize(m.size());
    this->m_Edges.resize(l.size());
    this->m_Faces.resize(f.size());
    
    this->SetGeometryPartially(m,l);
    
    // Build faces
    int validFace = 0;
    for (size_t i = 0 ; i < f.size() ; ++i)
    {
      int pos1 = this->FindVertex(f[i].GetIds()[0]);
      if (pos1 == -1)
      {
        btkErrorMacro("The face #" + ToString(i+1) + " uses a first point which is not listed (unknown ID). The face is removed.");
        continue;
      }
      int pos2 = this->FindVertex(f[i].GetIds()[1]);
      if (pos2 == -1)
      {
        btkErrorMacro("The face #" + ToString(i+1) + " uses a second point which is not listed (unknown ID). The face is removed.");
        continue;
      }
      int pos3 = this->FindVertex(f[i].GetIds()[2]);
      if (pos3 == -1)
      {
        btkErrorMacro("The face #" + ToString(i+1) + " uses a third point which is not listed (unknown ID). The face is removed.");
        continue;
      }
      Face face;
      face.mp_Vertices[0] = &this->m_Vertices[pos1];
      face.mp_Vertices[1] = &this->m_Vertices[pos2];
      face.mp_Vertices[2] = &this->m_Vertices[pos3];
      this->m_Faces[validFace] = face;
      ++validFace;
    }
    this->m_Faces.resize(validFace);
  };
   
  /**
   * Constructor. Build the vertices, edges and faces, from the set of IDs @a m and the set of links @a l.
   * If one of the given link contains a wrong ID (out of range), then, it is not added to the list of edges.
   *
   * @warning This method can create non-manifold mesh!
   */
  TriangleMesh::TriangleMesh(const std::vector<int>& m, const std::vector<VertexLink>& l)
  : m_Vertices(m.size()), m_Edges(l.size()), m_Faces(), mp_Points()
  {
    this->m_CurrentFrame = -1;
    this->SetDefinition(m,l);
  };
  
  /**
   * Constructor. Build the vertices, edges and faces, from the set of IDs @a m, the set of links @a l and the set of faces @a f.
   * If one of the given link or face contains a wrong ID (out of range), then, it is not added.
   */
  TriangleMesh::TriangleMesh(const std::vector<int>& m, const std::vector<VertexLink>& l, const std::vector<VertexFace>& f)
  : m_Vertices(m.size()), m_Edges(l.size()), m_Faces(f.size()), mp_Points()
  {
    this->m_CurrentFrame = -1;
    this->SetDefinition(m,l,f);
  }
  
  void TriangleMesh::SetGeometryPartially(const std::vector<int>& m, const std::vector<VertexLink>& l)
  {
    // Build vertices
    for (size_t i = 0 ; i < this->m_Vertices.size() ; ++i)
    {
      this->m_Vertices[i].m_Id = m[i];
      this->m_Vertices[i].m_RelativeId = static_cast<int>(i);
      this->m_Vertices[i].mp_CurrentFrame = &(this->m_CurrentFrame);
    }
    
    // Build edges
    int validEdge = 0;
    for (size_t i = 0 ; i < l.size() ; ++i)
    {
      int pos1 = this->FindVertex(l[i].GetIds()[0]);
      if (pos1 == -1)
      {
        btkErrorMacro("The link #" + ToString(i+1) + " uses a first point which is not listed (unknown ID). The link is removed.");
        continue;
      }
      int pos2 = this->FindVertex(l[i].GetIds()[1]);
      if (pos2 == -1)
      {
        btkErrorMacro("The link #" + ToString(i+1) + " uses a second point which is not listed (unknown ID). The link is removed.");
        continue;
      }
      Edge edge;
      edge.mp_Vertices[0] = &this->m_Vertices[pos1];
      edge.mp_Vertices[1] = &this->m_Vertices[pos2];
      this->m_Edges[validEdge] = edge;
      ++validEdge;
    }
    this->m_Edges.resize(validEdge);
  };
  
  int TriangleMesh::FindVertex(int vid)
  {
    int pos = -1;
    for (size_t i = 0 ; i < this->m_Vertices.size() ; ++i)
    {
      if (vid == this->m_Vertices[i].m_Id)
      {
        pos = this->m_Vertices[i].m_RelativeId;
        break;
      }
    }
    return pos;
  };
  
  // ----------------------------- VertexLink ------------------------------ //
  
  /**
   * @class TriangleMesh::VertexLink btkTriangleMesh.h
   * Store two marker's IDs to create a link between them (Only to create mesh's edge).
   */
  
  /**
   * @fn TriangleMesh::VertexLink::VertexLink(int id1 = -1, int id2 = -1)
   * Constructor. The given IDs correspond to markers' IDs which will be used as vertices.
   */
  
  /**
   * @fn const int* TriangleMesh::VertexLink::GetIds() const
   * Returns the IDs as an array of two integers.
   */
  
  /**
   * @fn void TriangleMesh::VertexLink::SetIds(int id1, int id2)
   * Sets the IDs.
   */
  
  // ----------------------------- VertexFace ------------------------------ //
  
  /**
   * @class TriangleMesh::VertexFace btkTriangleMesh.h
   * Store three marker's IDs to create a face.
   */
  
  /**
   * @fn TriangleMesh::VertexFace::VertexFace(int id1 = -1, int id2 = -1, int id3 = -1)
   * Constructor. The given IDs correspond to markers' IDs which will be used as vertices.
   */
  
  /**
   * @fn const int* TriangleMesh::VertexFace::GetIds() const
   * Returns the IDs as an array of three integers.
   */
  
  /**
   * @fn void TriangleMesh::VertexFace::SetIds(int id1, int id2, int id3)
   * Sets the IDs.
   */
   
  // ------------------------------- Vertex -------------------------------- //
  
  /**
   * @class TriangleMesh::Vertex btkTriangleMesh.h
   * Read-only class used to bridge the content of a Point into a vertex.
   *
   * The bridge is only effective when the method TriangleMesh::ConnectPoints is used an valided.
   *
   * This class should be used only by using the method TriangleMesh::BeginVertex().
   */
  
  /**
   * Constructor.
   */
  TriangleMesh::Vertex::Vertex()
  : mp_Point()
  {
    this->m_Id = -1;
    this->m_RelativeId = -1;
    this->mp_CurrentFrame = 0;
  };
  
  /**
   * @fn int TriangleMesh::Vertex::GetId() const
   * Returns the ID of the corresponding marker.
   */
  
  
  /**
   * @fn int TriangleMesh::Vertex::GetRelativeId() const
   * Returns the relative ID of the corresponding marker. The value of the relative ID corresponds to the order of the given markers in the TriangleMesh constructor.
   */
   
  
  /**
   * @fn double TriangleMesh::Vertex::GetCoordinateX() const
   * Returns the value of the X coordinate for the frame set.
   * @warning This method must be used only after connecting of collection of points using the method TriangleMesh::ConnectPoints() and setting the current frame (see TriangleMesh::SetCurrentFrame()).
   */
   
  
  /**
   * @fn double TriangleMesh::Vertex::GetCoordinateY() const
   * Returns the value of the Y coordinate for the frame set.
   * @warning This method must be used only after connecting of collection of points using the method TriangleMesh::ConnectPoints() and setting the current frame (see TriangleMesh::SetCurrentFrame()).
   */
   
  
  /**
   * @fn double TriangleMesh::Vertex::GetCoordinateZ() const
   * Returns the value of the Z coordinate for the frame set.
   * @warning This method must be used only after connecting of collection of points using the method TriangleMesh::ConnectPoints() and setting the current frame (see TriangleMesh::SetCurrentFrame()).
   */
  
  /**
   * @fn bool TriangleMesh::Vertex::IsValid() const
   * Return true if the residual for the current frame is greater or equal to 0. Otherwise, returns false.
   */
  
  // -------------------------------- Edge --------------------------------- //
  
  /**
   * @class TriangleMesh::Edge btkTriangleMesh.h
   * Read-only class used to represent an edge as a set of two vertices.
   *
   * This class should be used only by using the method TriangleMesh::BeginEdge().
   */
   
  /**
   * @fn TriangleMesh::Edge::Edge()
   * Constructor. By default, the pointers to the vertices are NULL.
   */
  
  /**
   * @fn bool TriangleMesh::Edge::IsValid()
   * Returns true if the vertices defining the edge are valids.
   * @warning This method is not thread-safe and must be used only after the connection of a set o points (see method TriangleMesh::ConnectPoints()).
   */
  
  /**
   * @fn const TriangleMesh::Vertex* TriangleMesh::Edge::GetVertex1()
   * Returns the first vertex defining the edge.
   */
   
  /**
   * @fn const TriangleMesh::Vertex* TriangleMesh::Edge::GetVertex2()
   * Returns the second vertex defining the edge.
   */
   
  // -------------------------------- Face --------------------------------- //
  
  /**
   * @class TriangleMesh::Face btkTriangleMesh.h
   * Read-only class used to represent a face as a set of three vertices.
   *
   * This class should be used only by using the method TriangleMesh::BeginFace().
   */
  
  /**
   * @fn TriangleMesh::Face::Face()
   * Constructor. By default, the pointers to the vertices are NULL.
   */
  
  /**
   * @fn bool TriangleMesh::Face::IsValid()
   * Returns true if the vertices defining the edge are valids.
   * @warning This method is not thread-safe and must be used only after the connection of a set o points (see method TriangleMesh::ConnectPoints()).
   */
  
  /**
   * @fn const TriangleMesh::Vertex* TriangleMesh::Face::GetVertex1()
   * Returns the first vertex defining the face.
   */
   
  /**
   * @fn const TriangleMesh::Vertex* TriangleMesh::Face::GetVertex2()
   * Returns the second vertex defining the face.
   */
   
  /**
   * @fn const TriangleMesh::Vertex* TriangleMesh::Face::GetVertex3()
   * Returns the third vertex defining the face.
   */
};