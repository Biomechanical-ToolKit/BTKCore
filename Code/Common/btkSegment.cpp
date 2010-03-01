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

#include "btkSegment.h"
#include "btkException.h"

namespace btk
{
  /**
   * @class Segment
   * @brief Object representing the modeling of a body part.
   *
   * A segment is represented by:
   *  - a label and a description
   *  - a type (a tag to indicate the body part)
   *  - a side
   *  - an anatomical frame expressed in the technical frame (also named segment coordinate system: SCS)
   *  - a tensor of inertial expressed in TF
   *  - a mass in kilogram
   *  - a center of mass expressed in TF
   *  - a map of label and Positionset expressed in TF
   *
   * An enumeration of segment's type is provided (see Segment::Type). However, if the 
   * desired type is not proposed you can easily extend the list by using the value Segment::User. 
   * For example:
   * @code
   * int Torso = Segment::User + 1;
   * enum {Patella = Segment::User + 2, Metatarsus, Tarsus}
   * @endcode
   *
   * @ingroup BTKCommon
   */
  
  /**
   * @enum Segment::Side
   * Enums used to specify the side of the segment.
   */
  /**
   * @var Segment::Side Segment::Central
   * Used for the central segment (head, trunk, HAT, ...).
   */
  /**
   * @var Segment::Side Segment::Left
   * Used for the segment on the left side.
   */
  /**
   * @var Segment::Side Segment::Right
   * Used for the segment on the right side.
   */
   
  /**
   * @enum Segment::Type
   * Enums used to indicate the body's segment.
   */
  /**
   * @var Segment::Type Segment::Head
   * Segment modeling the head.
   */
  /**
   * @var Segment::Type Segment::Trunk
   * Segment modeling the trunk.
   */
  /**
   * @var Segment::Type Segment::Arm
   * Segment modeling the arm.
   */
  /**
   * @var Segment::Type Segment::Forearm
   * Segment modeling the forearm.
   */
  /**
   * @var Segment::Type Segment::Hand
   * Segment modeling the hand.
   */
  /**
   * @var Segment::Type Segment::Thigh
   * Segment modeling the thigh.
   */
  /**
   * @var Segment::Type Segment::Shank
   * Segment modeling the shank.
   */
  /**
   * @var Segment::Type Segment::Foot
   * Segment modeling the foot.
   */
  /**
   * @var Segment::Type Segment::User
   * User defined segment .
   */
   
  /**
   * @enum Segment::Attribute
   * Attribute for the point expressed relatively in the segment coordinate system.
   */
  /**
   * @var Segment::Attribute Segment::Required
   * Used for points required in some computation.
   */
  /**
   * @var Segment::Attribute Segment::SCSBuilder
   * Used for points required in the segment coordinate system
   */
  /**
   * @var Segment::Attribute Segment::CoM
   * Used ONLY for the point representing correspopoints required in the segment coordinate system
   */
  
  /**
   * @class Segment::Node
   * @brief Point expressed in the segment coordinate system to represent extermities, CoM, muscle insertions/origins, ...
   *
   * A node is defined by its position (expressed in the SCS), a label and attributes.
   * The attributes to used are defined by the enumeration Segment::Attribute.
   * You can combine attributes if necessary. For example:
   * @code
   * // Node required to define the segment coordinate system when using least square fitting method
   * btk::Segment::Node le = btk::Segment::Node("LE")
   * le->SetAttributes(btk::Segment::SCSBuilder | btk::Segment::Required);
   * @endcode 
   * By there is no attribute (which means set to 0x00 = None);
   */
  
  /**
   * @fn Segment::Node::Node(const std::string& label = "", const Position& pos = Position::Zero(), int att = Segment::None)
   * Constructor
   */
   
  /**
   * @fn const std::string& Segment::Node::GetLabel() const
   * Returns the node's label.
   */
   
  /**
   * @fn void Segment::Node::SetLabel(const std::string& label)
   * Sets the node's label.
   */
   
  /**
   * @fn const Position& Segment::Node::GetPosition() const
   * Returns the node's position (X,Y,Z) expressed in the segment coordinate system (SCS).
   */
   
  /**
   * @fn void Segment::Node::SetPosition(double x, double y, double z)
   * Sets the position.
   */
   
  /**
   * @fn void Segment::Node::SetPosition(const Position& pos)
   * Sets the position.
   */
   
  /**
   * @fn int Segment::Node::GetAttributes() const
   * Returns the attributes as an integer.
   * To determine if an attribute is enabled, you can use the following code.
   * @code
   * if ((node->GetAttributes() & btk::Segment::Required) == btk::Segment::Required)
   * {
   *   // ...
   * }
   * @endcode
   */
   
  /**
   * @fn void Segment::Node::SetAttributes(int att)
   * Set the node's attributes.
   */
   
  /**
   * @typedef Segment::Pointer
   * Smart pointer associated with a Segment object.
   */
  
  /**
   * @typedef Segment::ConstPointer
   * Smart pointer associated with a const Segment object.
   */
   
  /**
   * @typedef Segment::NodeIterator;
   * Iterator for nodes contained in the segment.
   */
   
  /**
   * @typedef Segment::NodeConstIterator;
   * Const iterator for nodes contained in the segment.
   */
   
  /**
   * @typedef Segment::MotionIterator;
   * Iterator for the frames contained in the segment's motion.
   */
   
  /**
   * @typedef Segment::MotionConstIterator;
   * Const iterator for the frames contained in the segment's motion
   */
  
  /**
   * @fn Segment::Pointer Segment::New(int t, Side s, const std::string& label = "", const std::string& desc = "")
   * Creates a smart pointer associated with a Segment object.
   * Use the enumeration Segment::Type to define the type of the segment @a t.
   * If the desired type is not proposed you can easily extend the list by using the value Segment::User. For example
   * @code
   * int Torso = Segment::User + 1;
   * enum {Patella = Segment::User + 2, Metatarsus, Tarsus}
   * @endcode
   */
   
  /**
   * @fn Frame& Segment::GetRelativeAF()
   * Returns the anatomical frame (AF) expressed in the segment coordinate system.
   */
   
  /**
   * @fn const Frame& Segment::GetRelativeAF() const
   * Returns the anatomical frame (AF) expressed in the segment coordinate system.
   */ 
  
  /**
   * @fn void Segment::SetRelativeAF(const Frame& f)
   * Sets the anatomical frame (AF) expressed in the segment coordinate system.
   */
   
  /**
   * @fn NodeIterator Segment::BeginNode()
   * Returns an iterator to the beginning of the vector of nodes.
   */
   
  /**
   * @fn NodeConstIterator Segment::BeginNode() const
   * Returns a const iterator to the beginning of the vector of nodes.
   */
   
  /**
   * @fn NodeIterator Segment::EndNode()
   * Returns an iterator just past the last node.
   */
   
  /**
   * @fn NodeConstIterator Segment::EndNode() const
   * Returns a const iterator just past the last node.
   */
  
  /**
   * Clear the segment's nodes.
   */
  void Segment::ClearNode()
  {
    if (this->m_Nodes.empty())
      return;
    this->m_Nodes.clear();
    this->Modified();
  };
  
  /**
   * @fn int Segment::GetNodeNumber()
   * Returns the number of nodes.
   */
   
  /**
   * Returns the node at the index @a idx.
   */
  const Segment::Node& Segment::GetNode(int idx) const
  {
    if (idx > static_cast<int>(this->m_Nodes.size()))
      throw(OutOfRangeException("Segment::GetNode(int)"));
    return this->m_Nodes[idx];
  };
  
  /**
   * Returns the node with the label @a label.
   */
  const Segment::Node& Segment::GetNode(const std::string& label) const
  {
    std::vector<Node>::const_iterator it = this->m_Nodes.begin();
    while (it != this->m_Nodes.end())
    {
      if (it->GetLabel().compare(label) == 0)
        break;
      ++it;
    }
    if (it == this->m_Nodes.end())
      throw(OutOfRangeException("Segment::GetNode(string)"));
    return *it;
  };
  
  /**
   * Extract nodes' label and position with the corresponding attributes @a att.
   */
  void Segment::GetNodes(int att, std::map<std::string, Position>& nodes) const
  {
    nodes.clear();
    for (NodeConstIterator it = this->BeginNode() ; it != this->EndNode() ; ++it)
    {
      if ((it->GetAttributes() & att) == att)
        nodes.insert(nodes.end(), std::make_pair(it->GetLabel(), it->GetPosition()));
    }
  };
  
  /**
   * @fn void Segment::AppendNode(const std::string& label, Position pos, int att = Segment::None)
   * Appends a new relative position.
   * If the node already exists (same label), then its position and attributes are set by @a pos and @a att respectively.
   * Use the enumeration Segment::Attribute to set node's attributes.
   */
  
  /**
   * Appends a new relative position.
   * If the node already exists (same label), then its position and attributes are set by @a pos and @a att respectively.
   * Use the enumeration Segment::Attribute to set node's attributes.
   */
  void Segment::AppendNode(const Node& node)
  {
    std::vector<Node>::iterator it = this->m_Nodes.begin();
    while (it != this->m_Nodes.end())
    {
      if (it->GetLabel().compare(node.GetLabel()) == 0)
        break;
      ++it;
    }
    if (it == this->m_Nodes.end())
      this->m_Nodes.push_back(node);
    else
    {
      it->SetPosition(node.GetPosition());
      it->SetAttributes(node.GetAttributes());
    }
    this->Modified();
  };
  
  /**
   * Removes the node at the index @a idx.
   */
  void Segment::RemoveNode(int idx)
  {
    if (idx > static_cast<int>(this->m_Nodes.size()))
      return;
    NodeIterator it = this->BeginNode();
    std::advance(it, idx);
    this->m_Nodes.erase(it);
    this->Modified();
  };
  
  /**
   * Removes the node with the label @a label.
   */
  void Segment::RemoveNode(const std::string& label)
  {
    std::vector<Node>::iterator it = this->m_Nodes.begin();
    while (it != this->m_Nodes.end())
    {
      if (it->GetLabel().compare(label) == 0)
        break;
      ++it;
    }
    if (it == this->m_Nodes.end())
      return;
    this->m_Nodes.erase(it);
    this->Modified();
  };
  
  /**
   * @fn void Segment::SetNode(const std::string& label, const Position& pos, int att = 0)
   * Sets a node.
   * If the node doesn't exist, it will be appended.
   * Use the enumeration Segment::Attribute to set node's attributes.
   */
   
  /**
   * @fn MotionIterator Segment::BeginMotion()
   * Returns an iterator to the beginning of the list of frames.
   */
   
  /**
   * @fn MotionConstIterator Segment::BeginMotion() const
   * Returns a const iterator to the beginning of the list of frames.
   */
   
  /**
   * @fn MotionIterator Segment::EndMotion()
   * Returns an iterator just past the last frames.
   */
   
  /**
   * @fn MotionConstIterator Segment::EndMotion() const
   * Returns a const iterator just past the last frames.
   */
  
  /**
   * @fn Motion::Pointer Segment::GetMotion()
   * Returns the global motion of the segment.
   */
  
  /**
   * @fn Motion::Pointer Segment::GetAFMotion() const
   * Computes and returns the global motion of the anatomical frame associated with the segment.
   */
  
  /**
   * Computes and returns the global motion of the relative frame @a f expressed in the segment coordinate system.
   */
  Motion::Pointer Segment::GetFrameMotion(const Frame& f) const
  {
    Motion::Pointer frame = Motion::New();
    frame->SetFrameNumber(this->m_Motion->GetFrameNumber());
    for (int i = 0 ; i < this->m_Motion->GetFrameNumber() ; ++i)
      frame->operator[](i).SetMatrix(this->m_Motion->operator[](i).GetMatrix() * f.GetMatrix());
    return frame;
  };
  
  /**
   * Convenient method to compute the trajectory of the segment's CoM (Center of Mass).
   */
  Point::Pointer Segment::GetCoMTrajectory() const
  {
    std::vector<Node>::const_iterator it = this->m_Nodes.begin();
    while (it != this->m_Nodes.end())
    {
      if ((it->GetAttributes() & CoM) == CoM)
        break;
      ++it;
    }
    if (it == this->m_Nodes.end())
    {
      btkErrorMacro("No Segment's CoM. Computed values correspond to the segment's origin trajectory.");
      return this->GetTrajectory(Eigen::Matrix<double, 3,1>::Zero());
    }
    return this->GetTrajectory(it->GetPosition());
  };
  
  /**
   * Computes and returns the global trajectory of the position at the index @a idx expressed in the segment coordinate system.
   */
  Point::Pointer Segment::GetTrajectory(int idx) const
  {
    if (idx > static_cast<int>(this->m_Nodes.size()))
      throw(OutOfRangeException("Segment::GetTrajectory(int)"));
    return this->GetTrajectory(this->m_Nodes[idx].GetPosition());
  };
  
  /**
   * Computes and returns the global trajectory of the position with the label @a label expressed in the segment coordinate system.
   */
  Point::Pointer Segment::GetTrajectory(const std::string& label) const
  {
    std::vector<Node>::const_iterator it = this->m_Nodes.begin();
    while (it != this->m_Nodes.end())
    {
      if (it->GetLabel().compare(label) == 0)
        break;
      ++it;
    }
    if (it == this->m_Nodes.end())
      throw(OutOfRangeException("Segment::GetTrajectory(string)"));
    return this->GetTrajectory(it->GetPosition());
  };
  
  /**
   * @fn Point::Pointer Segment::GetTrajectory(double x, double y, double z) const
   * Computes and returns the global trajectory of the position with the coordinates (@a x, @a y, @a z) expressed in the segment coordinate system.
   */
  
  /**
   * Computes and returns the global trajectory of the position @a relative expressed in the segment coordinate system.
   */
  Point::Pointer Segment::GetTrajectory(const Position& relative) const
  {
    Point::Pointer point = Point::New();
    point->SetFrameNumber(this->m_Motion->GetFrameNumber());
    Point::Values& values = point->GetValues();
    Eigen::Matrix<double,4,1> pos;
    pos << relative, 1.0;
    for (int i = 0 ; i < this->m_Motion->GetFrameNumber() ; ++i)
    {
      Eigen::Matrix<double, 4,1> foo = this->m_Motion->operator[](i).GetMatrix() * pos;
      values.coeffRef(i,0) = foo.coeff(0);
      values.coeffRef(i,1) = foo.coeff(1);
      values.coeffRef(i,2) = foo.coeff(2);
    }  
    return point;
  };
  
  /**
   * @fn int Segment::GetType() const
   * Gets the segment's type.
   */
  
  /**
   * Sets the segment's type.
   */
  void Segment::SetType(int t)
  {
    if (this->m_Type == t)
      return;
    this->m_Type = t;
    this->Modified();
  };
  
  /**
   * @fn int Segment::GetSide() const
   * Gets the segment's side.
   */
  
  /**
   * Sets the segment's side.
   */
  void Segment::SetSide(Side s)
  {
    if (this->m_Side == s)
      return;
    this->m_Side = s;
    this->Modified();
  };
  
  /**
   * @fn int Segment::GetMotionFrameNumber() const
   * Returns number of frames for the segment's motion.
   */
   
  /**
   * @fn void Segment::SetMotionFrameNumber(int num)
   * Set the number of frames for the segment's motion.
   */
  
  /**
   * Clear the segment's motion and set the number of frames to 1.
   */
  void Segment::ResetMotion()
  {
    this->m_Motion->SetFrameNumber(1);
    this->m_Motion->operator[](0).SetMatrix(Frame::Matrix::Identity());
    this->Modified();
  };
  
  /**
   * @fn Pointer Segment::Clone() const
   * Returns a deep copy of the object as a smart pointer.
   */
  
  /**
   * Constructor.
   * Use the enumeration Segment::Type to define the type @a t of the segment.
   */
  Segment::Segment(int t, Side s, const std::string& l, const std::string& d)
  : DataObjectLabeled(l,d),
    m_RelativeAF(),
    //m_Inertia(Inertia::Zero()),
    m_Nodes()
  {
    this->m_Type = t;
    this->m_Side = s;
    this->m_Mass = 0.0;
    this->m_Motion = Motion::New();
    this->m_Motion->SetFrameNumber(1);
  };
  
  Segment::Segment(const Segment& toCopy)
  :   DataObjectLabeled(toCopy),
      m_RelativeAF(toCopy.m_RelativeAF),
      //m_Inertia(toCopy.m_Inertia),
      m_Nodes(toCopy.m_Nodes)
  {
    this->m_Type = toCopy.m_Type;
    this->m_Side = toCopy.m_Side;
    this->m_Mass = toCopy.m_Mass;
    this->m_Motion = toCopy.m_Motion->Clone();
  };
  
};