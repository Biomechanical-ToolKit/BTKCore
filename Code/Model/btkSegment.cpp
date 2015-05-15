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

#include "btkSegment.h"
#include "btkSegment_p.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace btk
{
  SegmentPrivate::SegmentPrivate(Segment* pint, const std::string& name, int part, int side)
  : NodePrivate(pint,name),
    Part(part), Side(side)
  {};
  
  SegmentPrivate::~SegmentPrivate() = default;
  
  Node* SegmentPrivate::makePint() const
  {
    return new Segment(this->Name, this->Part, this->Side);
  };
};

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace btk
{  
  Segment::Segment(const std::string& name, int part, int side, Node* parent)
  : Node(*new SegmentPrivate(this,name,part,side), parent)
  {};
  
  Segment::~Segment() _BTK_NOEXCEPT = default;
  
  int Segment::part() const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Part;
  };
  
  void Segment::setPart(int value) _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (optr->Part == value)
      return;
    optr->Part = value;
    this->modified();
  };
  
  int Segment::side() const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Side;
  };
  
  void Segment::setSide(int value) _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (optr->Side == value)
      return;
    optr->Side = value;
    this->modified();
  };
};