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

#ifndef __btkRelativePosition_h
#define __btkRelativePosition_h

#include "btkModelExport.h"
#include "btkNode.h"
#include "btkMacros.h" // _BTK_NOEXCEPT

namespace btk
{
  class RelativePositionPrivate;
  
  class BTK_MODEL_EXPORT RelativePosition : public Node
  {
    BTK_DECLARE_PIMPL_ACCESSOR(RelativePosition)
    
  public:
    RelativePosition(const std::string& name, const double coordinates[3], Node* parent = nullptr);
    ~RelativePosition() _BTK_NOEXCEPT;
    
    RelativePosition(const RelativePosition& ) = delete;
    RelativePosition(RelativePosition&& ) _BTK_NOEXCEPT = delete;
    RelativePosition& operator=(const RelativePosition& ) = delete;
    RelativePosition& operator=(RelativePosition&& ) _BTK_NOEXCEPT = delete;
    
    const double* data() const _BTK_NOEXCEPT;
    double* data() _BTK_NOEXCEPT;
    
  private:
    RelativePosition(RelativePositionPrivate& pimpl, Node* parent) _BTK_NOEXCEPT;
  };
};

#endif // __btkRelativePosition_h