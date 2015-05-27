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

#ifndef __btkRelativeReferenceFrame_h
#define __btkRelativeReferenceFrame_h

#include "btkModelExport.h"
#include "btkNode.h"
#include "btkMacros.h" // _BTK_NOEXCEPT

namespace btk
{
  class RelativeReferenceFramePrivate;
  
  class BTK_NODE(BTK_MODEL_EXPORT, RelativeReferenceFrame) : public Node
  {
    BTK_DECLARE_PIMPL_ACCESSOR(RelativeReferenceFrame)
    
  public:
    RelativeReferenceFrame(const std::string& name, Node* parent = nullptr);
    RelativeReferenceFrame(const std::string& name, const double data[12], Node* parent = nullptr);
    RelativeReferenceFrame(const std::string& name, const double orientation[9], const double position[3], Node* parent = nullptr);
    ~RelativeReferenceFrame() _BTK_NOEXCEPT;
    
    RelativeReferenceFrame(const RelativeReferenceFrame& ) = delete;
    RelativeReferenceFrame(RelativeReferenceFrame&& ) _BTK_NOEXCEPT = delete;
    RelativeReferenceFrame& operator=(const RelativeReferenceFrame& ) = delete;
    RelativeReferenceFrame& operator=(RelativeReferenceFrame&& ) _BTK_NOEXCEPT = delete;
    
    const double* u() const _BTK_NOEXCEPT;
    double* u() _BTK_NOEXCEPT;
    
    const double* v() const _BTK_NOEXCEPT;
    double* v() _BTK_NOEXCEPT;
    
    const double* w() const _BTK_NOEXCEPT;
    double* w() _BTK_NOEXCEPT;
    
    const double* o() const _BTK_NOEXCEPT;
    double* o() _BTK_NOEXCEPT;    
    
    const double* data() const _BTK_NOEXCEPT;
    double* data() _BTK_NOEXCEPT;
    
  private:
    RelativeReferenceFrame(RelativeReferenceFramePrivate& pimpl, Node* parent) _BTK_NOEXCEPT;
  };
};

#endif // __btkRelativeReferenceFrame_h