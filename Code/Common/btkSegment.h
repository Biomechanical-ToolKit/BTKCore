/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2010, Arnaud Barré
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

#ifndef __btkSegment_h
#define __btkSegment_h

#include "btkDataObject.h"
#include "btkMotion.h"
#include "btkGeometry.h"
#include "btkPoint.h"

#include <string>
#include <vector>
#include <map>

namespace btk
{
  class Segment : public DataObjectLabeled
  {
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  
    class Node
    {
      EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    public:
      Node(const std::string& label = "", const Position& pos = Position::Zero(), int att = Segment::None)
      : m_Label(label), m_Pos(pos)
      {
        this->m_Attributes = att;
      };
      // Node(const Node& ); // Implicit
      // ~Node(); // Implicit

      const std::string& GetLabel() const {return this->m_Label;};
      void SetLabel(const std::string& label) {this->m_Label = label;};
      const Position& GetPosition() const {return this->m_Pos;};
      void SetPosition(double x, double y, double z) {this->m_Pos << x, y, z;};
      void SetPosition(const Position& pos) {this->m_Pos = pos;};
      int GetAttributes() const {return this->m_Attributes;};
      void SetAttributes(int att) {this->m_Attributes = att;};

    private:
      std::string m_Label;
      Position m_Pos;
      int m_Attributes;
    };

    typedef enum {Head, Trunk, Arm, Forearm, Hand, Thigh, Shank, Foot, User = 65536} Type;
    typedef enum {Central, Left, Right} Side;
    typedef enum {None = 0x00, Required = 0x01, SCSBuilder = 0x02, CoM = 0x04} Attribute;
    
    typedef SharedPtr<Segment> Pointer;
    typedef SharedPtr<const Segment> ConstPointer;
    
    typedef std::vector<Node>::iterator NodeIterator;
    typedef std::vector<Node>::const_iterator NodeConstIterator;
    typedef Motion::Iterator MotionIterator;
    typedef Motion::ConstIterator MotionConstIterator;
    
    static Pointer New(int t, Side s, const std::string& label = "", const std::string& desc = "") {return Pointer(new Segment(t, s, label, desc));};
    
    Frame& GetRelativeAF() {return this->m_RelativeAF;};
    const Frame& GetRelativeAF() const {return this->m_RelativeAF;};
    void SetRelativeAF(const Frame& f) {this->m_RelativeAF = f;};
    //Tensor& GetRelativeIntertia() {return this->m_Inertia;};
    //const Tensor& GetRelativeIntertia() const {return this->m_Inertia;};
    //void SetRelativeIntertia(const Tensor& i) {this->m_Inertia = i;};
    /*
    BTK_MODELS_EXPORT Position& GetRelativeCoM();
    BTK_MODELS_EXPORT const Tensor& GetRelativeCoM();
    BTK_MODELS_EXPORT void SetRelativeCoM(const Position& t);
    */
    NodeIterator BeginNode() {return this->m_Nodes.begin();};
    NodeConstIterator BeginNode() const {return this->m_Nodes.begin();};
    NodeIterator EndNode() {return this->m_Nodes.end();};
    NodeConstIterator EndNode() const {return this->m_Nodes.end();};
    void ClearNode();
    int GetNodeNumber() {return this->m_Nodes.size();};
    BTK_MODELS_EXPORT const Node& GetNode(int idx) const;
    BTK_MODELS_EXPORT const Node& GetNode(const std::string& label) const;
    BTK_MODELS_EXPORT void GetNodes(int att, std::map<std::string, Position>& nodes) const;
    void AppendNode(const std::string& label, Position pos, int att = Segment::None) {this->AppendNode(Node(label, pos, att));};
    BTK_MODELS_EXPORT void AppendNode(const Node& node);
    BTK_MODELS_EXPORT void RemoveNode(int idx);
    BTK_MODELS_EXPORT void RemoveNode(const std::string& label);
    void SetNode(const std::string& label, const Position& pos, int att = Segment::None) {this->AppendNode(Node(label, pos, att));};
    
    MotionIterator BeginMotion();
    MotionConstIterator BeginMotion() const;
    MotionIterator EndMotion();
    MotionConstIterator EndMotion() const;
    /*
    BTK_MODELS_EXPORT void SetMotion(Motion::Pointer tf);
    */
    Motion::Pointer GetMotion() {return this->m_Motion;};
    Motion::Pointer GetAFMotion() const {return this->GetFrameMotion(this->m_RelativeAF);};
    BTK_MODELS_EXPORT Motion::Pointer GetFrameMotion(const Frame& f) const;
    BTK_MODELS_EXPORT Point::Pointer GetCoMTrajectory() const;
    BTK_MODELS_EXPORT Point::Pointer GetTrajectory(int idx) const;
    BTK_MODELS_EXPORT Point::Pointer GetTrajectory(const std::string& label) const;
    Point::Pointer GetTrajectory(double x, double y, double z) const {Position pos; pos << x, y, z; return this->GetTrajectory(pos);};
    BTK_MODELS_EXPORT Point::Pointer GetTrajectory(const Position& relative) const;
    
    int GetType() const {return this->m_Type;};
    BTK_MODELS_EXPORT void SetType(int t);
    Side GetSide() const {return this->m_Side;};
    BTK_MODELS_EXPORT void SetSide(Side s);
    
    void SetMotionFrameNumber(int num) {this->m_Motion->SetFrameNumber(num);};
    int GetMotionFrameNumber() const {return this->m_Motion->GetFrameNumber();};
    BTK_MODELS_EXPORT void ResetMotion();

    Pointer Clone() const {return Pointer(new Segment(*this));};
    
  protected:
    BTK_MODELS_EXPORT Segment(int t, Side s, const std::string& l, const std::string& d);
    
  private:
    BTK_MODELS_EXPORT Segment(const Segment& toCopy);
    Segment& operator=(const Segment& ); // Not implemented.
    
    int m_Type;
    Side m_Side;
    Frame m_RelativeAF; /**< Anatomical frame (AF) expressed in the technical frame */
    //Tensor m_RelativeInertia; /**< Segment inertia matrix expressed in the SCS */
    std::vector<Node> m_Nodes; /**< Positions expressed in the SCS */
    double m_Mass; /**< Mass in kilograms */
    Motion::Pointer m_Motion;
  };
};

#endif // __btkSegment_h