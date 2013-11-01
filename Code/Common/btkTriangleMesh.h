/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2013, Arnaud Barré
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

#ifndef __btkTriangleMesh_h
#define __btkTriangleMesh_h

#include "btkDataObject.h"
#include "btkPoint.h"
#include "btkPointCollection.h"

#include <vector>

namespace btk
{
  class TriangleMesh : public DataObject
  {
  public:    
    typedef btkSharedPtr<TriangleMesh> Pointer;
    typedef btkSharedPtr<const TriangleMesh> ConstPointer;
    
    class VertexLink
    {
    public:
      VertexLink(int id1 = -1, int id2 = -1) {this->m_Ids[0] = id1; this->m_Ids[1] = id2;};
      const int* GetIds() const {return this->m_Ids;};
      void SetIds(int id1, int id2) {this->m_Ids[0] = id1; this->m_Ids[1] = id2;};
      friend bool operator== (const VertexLink& lhs, const VertexLink& rhs) {return (lhs.m_Ids[0] == rhs.m_Ids[0]) && (lhs.m_Ids[1] == rhs.m_Ids[1]);};
    private:
      int m_Ids[2];
    };
    
    class VertexFace
    {
    public:
      VertexFace(int id1 = -1, int id2 = -1, int id3 = -1) {this->m_Ids[0] = id1; this->m_Ids[1] = id2; this->m_Ids[2] = id3;};
      const int* GetIds() const {return this->m_Ids;};
      void SetIds(int id1, int id2, int id3) {this->m_Ids[0] = id1; this->m_Ids[1] = id2; this->m_Ids[2] = id3;};
      friend bool operator== (const VertexFace& lhs, const VertexFace& rhs) {return (lhs.m_Ids[0] == rhs.m_Ids[0]) && (lhs.m_Ids[1] == rhs.m_Ids[1]) && (lhs.m_Ids[2] == rhs.m_Ids[2]);};
    private:
      int m_Ids[3];
    };
    
    class Vertex
    {
    public:
      BTK_COMMON_EXPORT Vertex();
      int GetId() const {return this->m_Id;};
      int GetRelativeId() const {return this->m_RelativeId;};
      double GetCoordinateX() const {return this->mp_Point->GetValues().coeff(*this->mp_CurrentFrame,0);};
      double GetCoordinateY() const {return this->mp_Point->GetValues().coeff(*this->mp_CurrentFrame,1);};
      double GetCoordinateZ() const {return this->mp_Point->GetValues().coeff(*this->mp_CurrentFrame,2);};
      bool IsValid() const {return this->mp_Point->GetResiduals().coeff(*this->mp_CurrentFrame) >= 0.0;};
    private:
      friend class TriangleMesh;
      int m_Id;
      int m_RelativeId;
      Point::Pointer mp_Point;
      int* mp_CurrentFrame;
    };
    
    class Edge
    {
    public:
      Edge() {this->mp_Vertices[0] = 0; this->mp_Vertices[1] = 0;};
      bool IsValid() const {return (this->mp_Vertices[0]->IsValid() && this->mp_Vertices[1]->IsValid());}
      const Vertex* GetVertex1() const {return this->mp_Vertices[0];};
      const Vertex* GetVertex2() const {return this->mp_Vertices[1];};
    private:
      friend class TriangleMesh;
      Vertex* mp_Vertices[2];
    };
    
    class Face
    {
    public:
      Face() {this->mp_Vertices[0] = 0; this->mp_Vertices[1] = 0; this->mp_Vertices[2] = 0;};
      bool IsValid() const {return (this->mp_Vertices[0]->IsValid() && this->mp_Vertices[1]->IsValid() && this->mp_Vertices[2]->IsValid());}
      const Vertex* GetVertex1() const {return this->mp_Vertices[0];};
      const Vertex* GetVertex2() const {return this->mp_Vertices[1];};
      const Vertex* GetVertex3() const {return this->mp_Vertices[2];};
    private:
      friend class TriangleMesh;
      Vertex* mp_Vertices[3];
    };
    
    typedef std::vector<Vertex>::iterator VertexIterator;
    typedef std::vector<Vertex>::const_iterator VertexConstIterator;
    typedef std::vector<Edge>::iterator EdgeIterator;
    typedef std::vector<Edge>::const_iterator EdgeConstIterator;
    typedef std::vector<Face>::iterator FaceIterator;
    typedef std::vector<Face>::const_iterator FaceConstIterator;
    
    static Pointer New(const std::vector<int>& m, const std::vector<VertexLink>& l) {return Pointer(new TriangleMesh(m,l));};
    static Pointer New(const std::vector<int>& m, const std::vector<VertexLink>& l, const std::vector<VertexFace>& f) {return Pointer(new TriangleMesh(m,l,f));};
    
    // ~TriangleMesh(); // Implicit.
    
    VertexIterator BeginVertex() {return this->m_Vertices.begin();};
    VertexConstIterator BeginVertex() const {return this->m_Vertices.begin();};
    EdgeIterator BeginEdge() {return this->m_Edges.begin();};
    EdgeConstIterator BeginEdge() const {return this->m_Edges.begin();};
    FaceIterator BeginFace() {return this->m_Faces.begin();};
    FaceConstIterator BeginFace() const {return this->m_Faces.begin();};
    
    VertexIterator EndVertex() {return this->m_Vertices.end();};
    VertexConstIterator EndVertex() const {return this->m_Vertices.end();};
    EdgeIterator EndEdge() {return this->m_Edges.end();};
    EdgeConstIterator EndEdge() const {return this->m_Edges.end();};
    FaceIterator EndFace() {return this->m_Faces.end();};
    FaceConstIterator EndFace() const {return this->m_Faces.end();};
    
    int GetVertexNumber() const {return static_cast<int>(this->m_Vertices.size());};
    int GetEdgeNumber() const {return static_cast<int>(this->m_Edges.size());}
    int GetFaceNumber() const {return static_cast<int>(this->m_Faces.size());};
    
    BTK_COMMON_EXPORT int GetMaxVertexId() const;
    
    BTK_COMMON_EXPORT bool ConnectPoints(PointCollection::Pointer points);
    
    int GetCurrentFrameIndex() const {return this->m_CurrentFrame;};
    void SetCurrentFrameIndex(int frame) {this->m_CurrentFrame = frame;};
    
    BTK_COMMON_EXPORT void SetDefinition(const std::vector<int>& m, const std::vector<VertexLink>& l);
    BTK_COMMON_EXPORT void SetDefinition(const std::vector<int>& m, const std::vector<VertexLink>& l, const std::vector<VertexFace>& f);
    
  protected:
    BTK_COMMON_EXPORT TriangleMesh(const std::vector<int>& m, const std::vector<VertexLink>& l);
    BTK_COMMON_EXPORT TriangleMesh(const std::vector<int>& m, const std::vector<VertexLink>& l, const std::vector<VertexFace>& f);
    
  private:
    void SetGeometryPartially(const std::vector<int>& m, const std::vector<VertexLink>& l);
    int FindVertex(int vid);
    
    TriangleMesh(const TriangleMesh& ); // Not implemented.
    TriangleMesh& operator=(const TriangleMesh& ); // Not implemented.
    
    std::vector<Vertex> m_Vertices;
    std::vector<Edge> m_Edges;
    std::vector<Face> m_Faces;
    PointCollection::Pointer mp_Points;
    int m_CurrentFrame;
  };
};

#endif // __btkTriangleMesh_h
