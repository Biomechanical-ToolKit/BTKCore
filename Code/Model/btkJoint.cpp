/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2015, Arnaud Barré
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

#include "btkJoint.h"
#include "btkJoint_p.h"
#include "btkSegment.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace btk
{
  // Note: The member Proximal and Distal are used to simplify the mangement of the segment in case one of them is set to null (to represent the global frame).
  
  JointPrivate::JointPrivate(Joint* pint, const std::string& name, Segment* proximal, Segment* distal)
  : NodePrivate(pint,name), Proximal(proximal), Distal(distal)
  {};
  
  JointPrivate::~JointPrivate() = default;
  
  Node* JointPrivate::makePint() const
  {
    return new Joint(this->Name, nullptr, nullptr);
  };
  
  void JointPrivate::finalizePint(Node* pint) const
  {
    auto node = node_cast<Joint*>(pint);
    if (node != nullptr)
    {
      if (this->Proximal != nullptr)
        node->setProximalSegment(node->findChild<Segment*>(this->Proximal->name()));
      if (this->Distal != nullptr)
        node->setDistalSegment(node->findChild<Segment*>(this->Distal->name()));
    }
  };
};

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace btk
{
  Joint::Joint(const std::string& name, Segment* proximal, Segment* distal, Node* parent)
  : Node(*new JointPrivate(this,name,proximal,distal), parent)
  {
    this->appendChild(proximal);
    this->appendChild(distal);
  };
  
  Joint::~Joint() = default;
  
  Segment* Joint::proximalSegment() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Proximal;
  };
  
  void Joint::setProximalSegment(Segment* value) noexcept
  {
    auto optr = this->pimpl();
    if (optr->Proximal != value)
    {
      this->replaceChild(optr->Proximal,value);
      optr->Proximal = value;
      this->modified();
    }
  };
  
  Segment* Joint::distalSegment() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Distal;
  };
  
  void Joint::setDistalSegment(Segment* value) noexcept
  {
    auto optr = this->pimpl();
    if (optr->Distal != value)
    {
      this->replaceChild(optr->Distal,value);
      optr->Distal = value;
      this->modified();
    }
  };
};