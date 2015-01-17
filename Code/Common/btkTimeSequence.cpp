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

#include "btkTimeSequence.h"
#include "btkTimeSequence_p.h"

#include <numeric>

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace btk
{
  TimeSequencePrivate::TimeSequencePrivate(TimeSequence* pint, const std::vector<unsigned>& dimensions, unsigned samples, const std::string& name, double rate, const std::string& unit, double startTime)
  : NodePrivate(pint,name),
    Dimensions(dimensions), Samples(samples), SampleRate(rate), Unit(unit), StartTime(startTime), Data(nullptr)
  {
    // Allocate data memory;
    if (samples != 0)
    {
      size_t num = 1;
      for(const unsigned& cpt: dimensions)
        num *= cpt;
      if (num != 0)
        this->Data = new double[samples * num];
    }
  };
  
  TimeSequencePrivate::~TimeSequencePrivate()
  {
    delete[] this->Data;
  }
};

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace btk
{
  TimeSequence::TimeSequence(unsigned component, unsigned samples, const std::string& name, double rate, const std::string& unit, double startTime, Node* parent)
  : TimeSequence(std::vector<unsigned>({component}),samples,name,rate,unit,startTime,parent)
  {};
  
  TimeSequence::TimeSequence(const std::vector<unsigned>& dimensions, unsigned samples, const std::string& name, double rate, const std::string& unit, double startTime, Node* parent)
  : Node(*new TimeSequencePrivate(this,dimensions,samples,name,rate,unit,startTime),parent)
  {};
  
  /*
   * @fn TimeSequence::~TimeSequence()
   * Destructor (default)
   */
  
  double TimeSequence::sampleRate() const noexcept
  {
    auto optr = this->pimpl();
    return optr->SampleRate;
  };
  
  void TimeSequence::setSampleRate(double value)noexcept
  {
    auto optr = this->pimpl();
    if (std::fabs(value - optr->SampleRate) < std::numeric_limits<double>::epsilon())
      return;
    optr->SampleRate = value;
    this->modified();
  };

  const std::vector<unsigned>& TimeSequence::dimensions() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Dimensions;
  };
  
  unsigned TimeSequence::samples() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Samples;
  };

  unsigned TimeSequence::components() const noexcept
  {
    auto optr = this->pimpl();
    return std::accumulate(optr->Dimensions.cbegin(), optr->Dimensions.cend(), 1, [](unsigned x, unsigned y)->unsigned {return x*y;});
  };
      
  size_t TimeSequence::elements() const noexcept
  {
    return static_cast<size_t>(this->samples()) * static_cast<size_t>(this->components());
  };
  
  double TimeSequence::duration() const noexcept
  {
    auto optr = this->pimpl();
    if (optr->SampleRate <= 0.0)
      return 0.0;
    return static_cast<double>(optr->Samples) / optr->SampleRate;
  };
  
  const std::string& TimeSequence::unit() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Unit;
  };
  
  void TimeSequence::setUnit(const std::string& value) noexcept
  {
    auto optr = this->pimpl();
    if (value == optr->Unit)
      return;
    optr->Unit = value;
    this->modified();
  };
  
  double TimeSequence::startTime() const noexcept
  {
    auto optr = this->pimpl();
    return optr->StartTime;
  };
  
  void TimeSequence::setStartTime(double value) noexcept
  {
    auto optr = this->pimpl();
    if (std::fabs(value - optr->StartTime) < std::numeric_limits<double>::epsilon())
      return;
    optr->StartTime = value;
    this->modified();
  };
  
  const double* TimeSequence::data() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Data;
  };
  
  double* TimeSequence::data() noexcept
  {
    auto optr = this->pimpl();
    return optr->Data;
  };
};