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

#include "btkRelativeReferenceFrame.h"
#include "btkRelativeReferenceFrame_p.h"

#include <cassert>
#include <algorithm> // std::copy_n

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace btk
{
  // Note: The member Proximal and Distal are used to simplify the mangement of the segment in case one of them is set to null (to represent the global frame).  
  RelativeReferenceFramePrivate::RelativeReferenceFramePrivate(RelativeReferenceFrame* pint, const std::string& name)
  : NodePrivate(pint,name)
  {};
  
  RelativeReferenceFramePrivate::~RelativeReferenceFramePrivate() = default;
  
  Node* RelativeReferenceFramePrivate::makePint() const
  {
    return new RelativeReferenceFrame(this->Name, this->Data);
  };
};

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace btk
{
  RelativeReferenceFrame::RelativeReferenceFrame(const std::string& name, Node* parent)
  : Node(*new RelativeReferenceFramePrivate(this,name), parent)
  {
    auto optr = this->pimpl();
    optr->Data[0] = 1.; optr->Data[1] = 0.; optr->Data[2] = 0.; // u
    optr->Data[3] = 0.; optr->Data[4] = 1.; optr->Data[5] = 0.; // v
    optr->Data[6] = 0.; optr->Data[7] = 0.; optr->Data[8] = 1.; // w
    optr->Data[9] = 0.; optr->Data[10] = 0.; optr->Data[11] = 0.; // o
  };
  
  RelativeReferenceFrame::RelativeReferenceFrame(const std::string& name, const double data[12], Node* parent)
  : Node(*new RelativeReferenceFramePrivate(this,name), parent)
  {
    auto optr = this->pimpl();
    std::copy_n(data, 12, optr->Data);
  };
  
  RelativeReferenceFrame::RelativeReferenceFrame(const std::string& name, const double orientation[9], const double position[3], Node* parent)
  : Node(*new RelativeReferenceFramePrivate(this,name), parent)
  {
    auto optr = this->pimpl();
    std::copy_n(orientation, 9, optr->Data);
    std::copy_n(position, 3, optr->Data+9);
  };
  
  RelativeReferenceFrame::~RelativeReferenceFrame() = default;
  
  const double* RelativeReferenceFrame::u() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Data;
  };
  
  double* RelativeReferenceFrame::u() noexcept
  {
    auto optr = this->pimpl();
    return optr->Data;
  };
  
  const double* RelativeReferenceFrame::v() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Data+3;
  };
  
  double* RelativeReferenceFrame::v() noexcept
  {
    auto optr = this->pimpl();
    return optr->Data+3;
  };
  
  const double* RelativeReferenceFrame::w() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Data+6;
  };
  
  double* RelativeReferenceFrame::w() noexcept
  {
    auto optr = this->pimpl();
    return optr->Data+6;
  };
  
  const double* RelativeReferenceFrame::o() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Data+9;
  };
  
  double* RelativeReferenceFrame::o() noexcept
  {
    auto optr = this->pimpl();
    return optr->Data+9;
  };
  
  const double* RelativeReferenceFrame::data() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Data;
  };
  
  double* RelativeReferenceFrame::data() noexcept
  {
    auto optr = this->pimpl();
    return optr->Data;
  };
};