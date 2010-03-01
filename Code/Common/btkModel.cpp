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

#include "btkModel.h"

namespace btk
{
  /**
   * @typedef Side
   * Enumeration to configure the side in the model
   */
  /**
   * @var Side None
   * No side.
   */
  /**
   * @var Side Left
   * Left side
   */
  /**
   * @var Side Right
   * Right side.
   */
  /**
   * @var Side Both
   * Left and right side in this order.
   */
   
  /**
   * @class ModelConfigurator
   * @brief Generic model configurator. Used to facilate the creation of models.
   *  
   * @ingroup BTKModels
   */
   
  /**
   * @var ModelConfigurator ModelConfigurator::UpperLimb
   * Used to specialize the configuration of the upper limb model.
   */
  /**
   * @var ModelConfigurator ModelConfigurator::LowerLimb
   * Used to specialize the configuration of the lower limb model.
   */
  /**
   * @var ModelConfigurator ModelConfigurator::WholeBody
   * Used to specialize the configuration of the whole body model.
   */
  /**
   * @var ModelConfigurator ModelConfigurator::User
   * User defined specialization.
   */
  
  /**
   * @typedef ModelConfigurator::Pointer
   * Smart pointer associated with a ModelConfigurator object.
   */
  
  /**
   * @typedef ModelConfigurator::ConstPointer
   * Smart pointer associated with a const ModelConfigurator object.
   */
   
  /**
   * @fn static ModelConfigurator::Pointer ModelConfigurator::New()
   * Creates a smart pointer associated with a ModelConfigurator object.
   */
   
  /**
   * @fn virtual void ModelConfigurator::Configure(Model* model, Side s)
   * Generic configuration method. Do nothing.
   */
   
  /**
   * @fn ModelConfigurator::ModelConfigurator()
   * Constructor.
   */
   
  /**
   * @class Model
   * @brief Collection of segments and joints used to compute kinematics, kinetics, etc.
   *
   * @ingroup BTKCommon
   */
  
  /**
   * @typedef Model::Pointer
   * Smart pointer associated with a Model object.
   */
  
  /**
   * @typedef Model::ConstPointer
   * Smart pointer associated with a const Model object.
   */
   
  /**
   * @typedef Model::SegmentIterator
   * Iterator for segments contained in the Model object.
   */
  
  /**
   * @typedef Model::SegmentConstIterator
   * Const iterator for segments contained in the Model object.
   */
   
  /**
   * @typedef Model::JointIterator
   * Iterator for joints contained in the Model object.
   */
  
  /**
   * @typedef Model::JointConstIterator
   * Const iterator for joints contained in the Model object.
   */
   
  /**
   * @fn Model::Pointer Model::New()
   * Creates a smart pointer associated with a Model object.
   */
  
  /**
   * Create a model based on the configurator @a mc and the side @a s. A model has also a label and a description.
   */
  void Model::Init(ModelConfigurator::Pointer mc, Side s, const std::string& label, const std::string& desc)
  {
    this->SetLabel(label);
    this->SetDescription(desc);
    if (!mc.get())
    {  
      this->m_Segments->Clear();
      this->m_Joints->Clear();
    }
    else
      mc->Configure(this, s);
    this->m_MotionFrameNumber = 1;
    this->Modified();
  };
  
  /**
   * @fn Model::SegmentIterator Model::BeginSegment()
   * Returns an iterator to the beginning of the list of segments.
   */
  
  /**
   * @fn Model::SegmentConstIterator Model::BeginSegment() const
   * Returns a const iterator to the beginning of the list of segments.
   */
  
  /**
   * @fn Model::SegmentIterator Model::EndSegment()
   * Returns an iterator just past the last segment.
   */
  
  /**
   * @fn Model::SegmentConstIterator Model::EndSegment() const
   * Returns a const iterator just past the last segment.
   */
  
  /**
   * @fn Segment::Pointer Model::GetSegment(int idx)
   * Gets the segment at the index @a idx as a smart pointer.
   */
  
  /**
   * @fn Segment::ConstPointer Model::GetSegment(int idx) const
   * Gets the segment at the index @a idx as a const smart pointer.
   */
  
  /**
   * Sets the segment @a seg at the index @ idx and resize its motion to the number of frames  in this model if necessary.
   */
  void Model::SetSegment(int idx, Segment::Pointer seg)
  {
    if (this->m_Segments->SetItem(idx, seg))
    {
      if (seg->GetMotionFrameNumber() != this->m_MotionFrameNumber)
      {
        btkErrorMacro("Frame number is different than model motion's frame number. The segment's motion is resized.");
        seg->SetMotionFrameNumber(this->m_MotionFrameNumber);
      }
      this->Modified();
    }
  };
  
  /**
   * @fn SegmentCollection::Pointer Model::GetSegments()
   * Returns the list of segments.
   */
  
  /**
   * @fn SegmentCollection::ConstPointer Model::GetSegments() const
   * Returns the list of segments.
   */
  
  /**
   * @fn bool Model::IsEmptySegment() const
   * Returns true if the collection is empty.
   */
  
  /**
   * @fn int Model::GetSegmentNumber() const
   * Returns the number of segments in the list.
   */
  
  /**
   * @fn void Model::SetSegmentNumber(int num)
   * Sets the number of segments in the list.
   */
  
  /**
   * Finds the segment with the proposed @a label and returns the iterator associed
   * with it. If no segment has @a label as label, an iterator pointing to the 
   * end of the collection is returned.
   */
  Model::SegmentIterator Model::FindSegment(const std::string& label)
  {
    SegmentIterator it = this->BeginSegment();
    while (it != this->EndSegment())
    {
      if ((*it)->GetLabel().compare(label) == 0)
        break;
      ++it;
    }
    return it;
  };
  
  /**
   * Finds the segment with the proposed @a label and returns the iterator associed
   * with it. If no segment has @a label as label, an iterator pointing to the 
   * end of the collection is returned.
   */
  Model::SegmentConstIterator Model::FindSegment(const std::string& label) const
  {
    SegmentConstIterator it = this->BeginSegment();
    while (it != this->EndSegment())
    {
      if ((*it)->GetLabel().compare(label) == 0)
        break;
      ++it;
    }
    return it;
  };
  
  /**
   * Append a segment at the end of the list.
   */
  void Model::AppendSegment(Segment::Type t, Segment::Side s, const std::string& label, const std::string& description)
  {
    Segment::Pointer seg = Segment::New(t, s, label, description);
    seg->SetMotionFrameNumber(this->m_MotionFrameNumber);
    this->AppendSegment(seg);
  }
  
  /**
   * Append a segment at the end of the list a and resize its motion to fit the model's motion size if necessary.
   */
  void Model::AppendSegment(Segment::Pointer seg)
  {
    if (this->m_Segments->InsertItem(seg))
    {
      if (seg->GetMotionFrameNumber() != this->m_MotionFrameNumber)
      {
        btkErrorMacro("Frame number is different than model motion's frame number. The segment's motion is resized.");
        seg->SetMotionFrameNumber(this->m_MotionFrameNumber);
      }
      this->Modified();
    }
  }
  
  /**
   * Remove the segment wih the label @a label. Removes also the joints which contain this segment.
   * No action is taken if none segment has the given label.
   */
  void Model::RemoveSegment(const std::string& label)
  {
    SegmentIterator it = this->FindSegment(label);
    this->RemoveSegment(it);
  };
  
  /**
   * Remove the segment wih the index @a idx. Removes also the joints which contain this segment.
   */
  void Model::RemoveSegment(int idx)
  {
    if (idx >= this->GetSegmentNumber())
    {
      btkErrorMacro("Out of range.");
      return;
    }
    SegmentIterator it = this->BeginSegment();
    std::advance(it, idx);
    this->RemoveSegment(it);
  }
  
  /**
   * Remove the segment associated wih the iterator @a loc and update the iterator. Removes also the joints which contain this segment.
   */
  Model::SegmentIterator Model::RemoveSegment(Model::SegmentIterator loc)
  {
    if (loc == this->EndSegment())
    {
      btkErrorMacro("Out of range.");
      return loc;
    }
    Segment::Pointer seg = *loc;
    this->RemoveJoints(seg);
    SegmentIterator it = this->m_Segments->RemoveItem(loc);
    if (*it != seg) // Must use this inequality instead of (it != loc) due to an assertion error in MSVC
      this->Modified();
    return it;
  };
   
  /**
   * @fn Model::JointIterator Model::BeginJoint()
   * Returns an iterator to the beginning of the list of segments.
   */
  
  /**
   * @fn Model::JointConstIterator Model::BeginJoint() const
   * Returns a const iterator to the beginning of the list of segments.
   */
  
  /**
   * @fn Model::JointIterator Model::EndJoint()
   * Returns an iterator just past the last segment.
   */
  
  /**
   * @fn Model::JointConstIterator Model::EndJoint() const
   * Returns a const iterator just past the last segment.
   */
  
  /**
   * @fn Joint::Pointer Model::GetJoint(int idx)
   * Gets the joint at the index @a idx as a smart pointer.
   */
  
  /**
   * @fn Joint::ConstPointer Model::GetJoint(int idx) const
   * Gets the joint at the index @a idx as a const smart pointer.
   */
  
  /**
   * Sets the joint @a joint at the index @ idx.
   */
  void Model::SetJoint(int idx, Joint::Pointer joint)
  { 
     if (this->m_Joints->SetItem(idx, joint))
       this->Modified();
  };
  
  /**
   * @fn JointCollection::Pointer Model::GetJoints()
   * Returns the list of joints.
   */
  
  /**
   * @fn JointCollection::ConstPointer Model::GetJoints() const
   * Returns the list of joints.
   */
  
  /**
   * @fn bool Model::IsEmptyJoint() const
   * Returns true if the collection is empty.
   */
  
  /**
   * @fn int Model::GetJointNumber() const
   * Returns the number of joints in the list.
   */
  
  /**
   * @fn void Model::SetJointNumber(int num)
   * Sets the number of joints in the list.
   */
  
  /**
   * Finds the joint with the proposed @a label and returns the iterator associed
   * with it. If no segment has @a label as label, an iterator pointing to the 
   * end of the collection is returned.
   */
  Model::JointIterator Model::FindJoint(const std::string& label)
  {
    JointIterator it = this->BeginJoint();
    while (it != this->EndJoint())
    {
      if ((*it)->GetLabel().compare(label) == 0)
        break;
      ++it;
    }
    return it;
  };
  
  /**
   * Finds the joint with the proposed @a label and returns the iterator associed
   * with it. If no segment has @a label as label, an iterator pointing to the 
   * end of the collection is returned.
   */
  Model::JointConstIterator Model::FindJoint(const std::string& label) const
  {
    JointConstIterator it = this->BeginJoint();
    while (it != this->EndJoint())
    {
      if ((*it)->GetLabel().compare(label) == 0)
        break;
      ++it;
    }
    return it;
  };
  
  /**
   * @fn void Model::AppendJoint(Segment::Pointer proximal, Segment::Pointer distal, const std::string& label, const std::string& description)
   * Append a joint at the end of the list.
   */
  
  /**
   * Append a joint at the end of the list.
   */
  void Model::AppendJoint(Joint::Pointer joint)
  {
    if (this->m_Joints->InsertItem(joint))
      this->Modified();
  }
  
  /**
   * Remove the joint wih the label @a label.
   * No action is taken if none segment has the given label.
   */
  void Model::RemoveJoint(const std::string& label)
  {
    
    JointIterator it = this->FindJoint(label);
    if (it == this->EndJoint())
      return;
    this->m_Joints->RemoveItem(it);
    this->Modified();
  };
  
  /**
   * Remove the joint wih the index @a idx.
   */
  void Model::RemoveJoint(int idx)
  {
    int before = this->GetJointNumber();
    this->m_Joints->RemoveItem(idx);
    int after = this->GetJointNumber();
    if (before != after)
      this->Modified();
  };
  
  /**
   * Remove the joint associated wih the iterator @a loc and update the iterator.
   */
  Model::JointIterator Model::RemoveJoint(Model::JointIterator loc)
  {
    if (loc == this->EndJoint())
    {
      btkErrorMacro("Out of range.");
      return loc;
    }
    Joint::Pointer jnt = *loc;
    JointIterator it = this->m_Joints->RemoveItem(loc);
    if (*it != jnt) // Must use this inequality instead of (it != loc) due to an assertion error in MSVC
      this->Modified();
    return it;
  };
  
  /**
   * Clear the contents of the model (label, description, segments, joints) and reset its motion frame number to 1.
   */
  void Model::Clear()
  {
    this->SetLabel("");
    this->SetDescription("");
    this->m_Segments->Clear();
    this->m_Joints->Clear();
    this->m_MotionFrameNumber = 1;
  };
   
  /**
   * @fn int Model::GetMotionFrameNumber() const
   * Returns number of frames for the model's motion.
   */
   
  /**
   * Set the number of frames for the model's motion (and then these segments).
   */
  void Model::SetMotionFrameNumber(int num)
  {
    if (this->m_MotionFrameNumber == num)
      return;
    this->m_MotionFrameNumber = num;
    for (SegmentIterator it = this->BeginSegment() ; it != this->EndSegment() ; ++it)
      (*it)->SetMotionFrameNumber(num);
    this->Modified();
  };
  
  /**
   * @fn Pointer Model::Clone() const
   * Deep copy.
   */
  
  /**
   * Constructor.
   */
  Model::Model()
  : DataObjectLabeled("", "")
  {
    this->m_Segments = SegmentCollection::New();
    this->m_Joints = JointCollection::New();
    this->m_MotionFrameNumber = 1;
  };
  
  /**
   * Copy constructor.
   * The segments used in the cloned joints are replaced by the cloned segments in the model.
   * If a segment is used in a joint but not included in the model, it won't be replaced.
   */
  Model::Model(const Model& toCopy)
  : DataObjectLabeled(toCopy)
  {
    this->m_Segments = toCopy.m_Segments->Clone();
    this->m_Joints = toCopy.m_Joints->Clone();
    
    SegmentConstIterator itST = this->BeginSegment();
    for (SegmentConstIterator itS = toCopy.BeginSegment() ; itS != toCopy.EndSegment() ; ++itS)
    {
      for (JointIterator itJ = this->BeginJoint() ; itJ != this->EndJoint() ; ++itJ)
      {
        if ((*itJ)->GetProximalSegment().get() && (*itS == (*itJ)->GetProximalSegment()))
          (*itJ)->m_Proximal = *itST;
        else if ((*itJ)->GetDistalSegment().get() && (*itS == (*itJ)->GetDistalSegment()))
          (*itJ)->m_Distal = *itST;
      }
      ++itST;
    }
    
    this->m_MotionFrameNumber = toCopy.m_MotionFrameNumber;
  };
  
  void Model::RemoveJoints(Segment::Pointer seg)
  {
    JointIterator it = this->BeginJoint();
    while (it != this->EndJoint())
    {
      if (((*it)->GetProximalSegment() == seg) || ((*it)->GetDistalSegment() == seg))
        it = this->RemoveJoint(it);
      else
        ++it;
    }
  };
};