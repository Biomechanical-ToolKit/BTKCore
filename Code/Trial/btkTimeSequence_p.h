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

#ifndef __btkTimeSequence_p_h
#define __btkTimeSequence_p_h

/*
 * WARNING: This file and its content are not included in the public API and 
 * can change drastically from one release to another.
 */

#include "btkNode_p.h"
#include "btkProperty.h"

#include <vector>
#include <array>
#include <string>

namespace btk
{
  class TimeSequence;
  
  class TimeSequencePrivate : public NodePrivate
  {
    BTK_DECLARE_PINT_ACCESSOR(TimeSequence)
    
    BTK_DECLARE_STATIC_PROPERTIES_DERIVED(TimeSequence, Node,
      Property<TimeSequence,int,&TimeSequence::type,&TimeSequence::setType>{"type"},
      Property<TimeSequence,unsigned,&TimeSequence::samples>{"samples"},
      Property<TimeSequence,double,&TimeSequence::sampleRate,&TimeSequence::setSampleRate>{"sampleRate"},
      Property<TimeSequence,double,&TimeSequence::startTime,&TimeSequence::setStartTime>{"startTime"},
      Property<TimeSequence,const std::string&,&TimeSequence::unit,&TimeSequence::setUnit>{"unit"},
      Property<TimeSequence,double,&TimeSequence::scale,&TimeSequence::setScale>{"scale"},
      Property<TimeSequence,double,&TimeSequence::offset,&TimeSequence::setOffset>{"offset"},
      Property<TimeSequence,const std::array<double,2>&,&TimeSequence::range,&TimeSequence::setRange>{"range"},
      Property<TimeSequence,unsigned,&TimeSequence::components>{"components"}
    )
    
  public:
    TimeSequencePrivate(TimeSequence* pint, const std::string& name, const std::vector<unsigned>& dimensions, unsigned samples, double rate, double start, int type, const std::string& unit, double scale, double offset, const std::array<double,2>& range);
    ~TimeSequencePrivate();
    
    virtual Node* makePint() const override;
    
    std::vector<unsigned> Dimensions, AccumulatedDimensions;
    unsigned Samples;
    double SampleRate;
    double StartTime;
    int Type;
    std::string Unit;
    double Scale;
    double Offset;
    std::array<double,2> Range;
    double* Data;
  };
};

#endif // __btkObject_p_h
