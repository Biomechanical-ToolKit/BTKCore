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

#ifndef __btkModel_h
#define __btkModel_h

#include "btkDataObject.h"
#include "btkSegmentCollection.h"
#include "btkJointCollection.h"

#include <Eigen/Core>
#include <string>

namespace btk
{
  typedef enum {None, Left, Right, Both} Side;
  
  class Model;
  
  class ModelConfigurator
  {
  public:
    enum {UpperLimb, LowerLimb, WholeBody, User = 65536};
    
    typedef SharedPtr<ModelConfigurator> Pointer;
    typedef SharedPtr<const ModelConfigurator> ConstPointer;
    
    static Pointer New() {return Pointer(new ModelConfigurator());};
    // ~ModelConfigurator(); // Implicit.
    virtual void Configure(Model* model, Side s) {btkErrorMacro("Generic method doesn't do anything.")};
    
  protected:
    ModelConfigurator() {};
    
  private:
    ModelConfigurator(const ModelConfigurator& ); // Not implemented.
    ModelConfigurator& operator=(const ModelConfigurator& ); // Not implemented.
  };
  
  class Model : public DataObjectLabeled
  {
  public:
    typedef SharedPtr<Model> Pointer;
    typedef SharedPtr<const Model> ConstPointer;
    
    typedef SegmentCollection::Iterator SegmentIterator;
    typedef SegmentCollection::ConstIterator SegmentConstIterator;
    
    typedef JointCollection::Iterator JointIterator;
    typedef JointCollection::ConstIterator JointConstIterator;
    
    static Pointer New() {return Pointer(new Model());};
    
    // ~Model(); // Implicit.
    
    void Init(ModelConfigurator::Pointer mc, Side s = Both, const std::string& label = "", const std::string& desc = "");
    
    SegmentIterator BeginSegment() {return this->m_Segments->Begin();};
    SegmentConstIterator BeginSegment() const {return this->m_Segments->Begin();};
    SegmentIterator EndSegment() {return this->m_Segments->End();};
    SegmentConstIterator EndSegment() const {return this->m_Segments->End();};
    Segment::Pointer GetSegment(int idx) {return this->m_Segments->GetItem(idx);};
    Segment::ConstPointer GetSegment(int idx) const {return this->m_Segments->GetItem(idx);};
    BTK_MODELS_EXPORT void SetSegment(int idx, Segment::Pointer seg);
    SegmentCollection::Pointer GetSegments() {return this->m_Segments;};
    SegmentCollection::ConstPointer GetSegments() const {return this->m_Segments;};
    //void SetSegments(SegmentCollection::Pointer segs) ...;
    bool IsEmptySegment() const {return this->m_Segments->IsEmpty();};
    int GetSegmentNumber() const {return this->m_Segments->GetItemNumber();};
    void SetSegmentNumber(int num) {this->m_Segments->SetItemNumber(num);};
    BTK_MODELS_EXPORT SegmentIterator FindSegment(const std::string& label);
    BTK_MODELS_EXPORT SegmentConstIterator FindSegment(const std::string& label) const;
    BTK_MODELS_EXPORT void AppendSegment(Segment::Type t, Segment::Side s,const std::string& label, const std::string& description);
    BTK_MODELS_EXPORT void AppendSegment(Segment::Pointer seg);
    BTK_MODELS_EXPORT void RemoveSegment(const std::string& label);
    BTK_MODELS_EXPORT void RemoveSegment(int idx);
    BTK_MODELS_EXPORT SegmentIterator RemoveSegment(SegmentIterator loc);
    
    JointIterator BeginJoint() {return this->m_Joints->Begin();};
    JointConstIterator BeginJoint() const {return this->m_Joints->Begin();};
    JointIterator EndJoint() {return this->m_Joints->End();};
    JointConstIterator EndJoint() const {return this->m_Joints->End();};
    Joint::Pointer GetJoint(int idx) {return this->m_Joints->GetItem(idx);};
    Joint::ConstPointer GetJoint(int idx) const {return this->m_Joints->GetItem(idx);};
    BTK_MODELS_EXPORT void SetJoint(int idx, Joint::Pointer seg);
    JointCollection::Pointer GetJoints() {return this->m_Joints;};
    JointCollection::ConstPointer GetJoints() const {return this->m_Joints;};
    bool IsEmptyJoint() const {return this->m_Joints->IsEmpty();};
    int GetJointNumber() const {return this->m_Joints->GetItemNumber();};
    void SetJointNumber(int num) {this->m_Joints->SetItemNumber(num);};
    BTK_MODELS_EXPORT JointIterator FindJoint(const std::string& label);
    BTK_MODELS_EXPORT JointConstIterator FindJoint(const std::string& label) const;
    void AppendJoint(Segment::Pointer proximal, Segment::Pointer distal, const std::string& label, const std::string& description) {this->AppendJoint(Joint::New(proximal, distal, label, description));};
    BTK_MODELS_EXPORT void AppendJoint(Joint::Pointer seg);
    BTK_MODELS_EXPORT void RemoveJoint(const std::string& label);
    BTK_MODELS_EXPORT void RemoveJoint(int idx);
    BTK_MODELS_EXPORT JointIterator RemoveJoint(JointIterator loc);
    
    int GetMotionFrameNumber() const {return this->m_MotionFrameNumber;};
    BTK_MODELS_EXPORT void SetMotionFrameNumber(int num);
    BTK_MODELS_EXPORT void Clear();
    Pointer Clone() const {return Pointer(new Model(*this));};
    
  protected:
    BTK_MODELS_EXPORT Model();
    BTK_MODELS_EXPORT Model(const Model& toCopy);
    
  private:
    Model& operator=(const Model& ); // Not implemented.
    
    void RemoveJoints(Segment::Pointer seg);
    
    SegmentCollection::Pointer m_Segments;
    JointCollection::Pointer m_Joints;
    int m_MotionFrameNumber;
  };
};

#endif // __btkModel_h