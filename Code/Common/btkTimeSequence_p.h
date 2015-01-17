/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2014, Arnaud Barré
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
 * WARNING: This file and its content is not included in the public API and 
 * can change drastically from one release to another.
 */

#include "btkNode_p.h"
#include "btkProperty.h"

namespace btk
{
  class TimeSequence;
  
  class TimeSequencePrivate : public NodePrivate
  {
    BTK_DECLARE_PINT_ACCESSOR(TimeSequence)
    
    BTK_DECLARE_NODEID(TimeSequence, Node)
    BTK_DECLARE_STATIC_PROPERTIES(TimeSequence, Node,
      Property<TimeSequence,unsigned>("samples",&TimeSequence::samples),
      Property<TimeSequence,double>("sampleRate",&TimeSequence::sampleRate,&TimeSequence::setSampleRate),
      Property<TimeSequence,double>("startTime",&TimeSequence::startTime,&TimeSequence::setStartTime),
      Property<TimeSequence,const std::string&>("unit",&TimeSequence::unit,&TimeSequence::setUnit),
      Property<TimeSequence,double>("duration",&TimeSequence::duration)
        // Scale ?
        // Offset ?
    )
    
  public:
    TimeSequencePrivate(TimeSequence* pint, const std::vector<unsigned>& dimensions, unsigned samples, const std::string& name, double rate, const std::string& unit, double startTime);
    ~TimeSequencePrivate();
    
    std::vector<unsigned> Dimensions;
    unsigned Samples;
    double SampleRate;
    std::string Unit;
    double StartTime;
    double* Data;
  };
};

#endif // __btkObject_p_h
