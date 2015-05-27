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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS
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

#ifndef __btkJoint_h
#define __btkJoint_h

#include "btkModelExport.h"
#include "btkNode.h"
#include "btkMacros.h" // _BTK_NOEXCEPT

namespace btk
{
  class Segment;
  
  class JointPrivate;
  
  class BTK_NODE(BTK_MODEL_EXPORT, Joint) : public Node
  {
    BTK_DECLARE_PIMPL_ACCESSOR(Joint)
    
  public:
    Joint(const std::string& name, Segment* proximal, Segment* distal, Node* parent = nullptr);
    ~Joint() _BTK_NOEXCEPT;
    
    Joint(const Joint& ) = delete;
    Joint(Joint&& ) _BTK_NOEXCEPT = delete;
    Joint& operator=(const Joint& ) = delete;
    Joint& operator=(Joint&& ) _BTK_NOEXCEPT = delete;
    
    Segment* proximalSegment() const _BTK_NOEXCEPT;
    void setProximalSegment(Segment* value) _BTK_NOEXCEPT;
    
    Segment* distalSegment() const _BTK_NOEXCEPT;
    void setDistalSegment(Segment* value) _BTK_NOEXCEPT;
    
  private:
    Joint(JointPrivate& pimpl, Node* parent) _BTK_NOEXCEPT;
  };
};

#endif // __btkJoint_h