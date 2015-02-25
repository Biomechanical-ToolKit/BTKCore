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

#include <cassert>

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace btk
{
  TimeSequencePrivate::TimeSequencePrivate(TimeSequence* pint, const std::vector<unsigned>& dimensions, unsigned samples, const std::string& name, double rate, const std::string& unit, int type, double startTime, double scale, double offset, const std::array<double,2>& range)
  : NodePrivate(pint,name),
    Dimensions(dimensions), AccumulatedDimensions(), Samples(samples), SampleRate(rate), Type(type), Unit(unit), StartTime(startTime), Scale(scale), Offset(offset), Range(range), Data(nullptr)
  {
    assert(!dimensions.empty());
    // Allocate data memory;
    if (samples != 0)
    {
      size_t num = 1;
      for(const unsigned& cpt: dimensions)
        num *= cpt;
      assert(num != 0);
      this->Data = new double[samples * num];
    }
    // Compute accumulated dimensions (used for the method data(sample, indices))
    this->AccumulatedDimensions.resize(dimensions.size()-1,dimensions[0]);
    for (size_t i = this->AccumulatedDimensions.size() ; i > 0  ; --i)
      this->AccumulatedDimensions[i-1] = this->AccumulatedDimensions[i] * this->Dimensions[this->AccumulatedDimensions.size()-i];
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
  constexpr std::array<double,2> TimeSequence::InfinityRange;
  
  TimeSequence::TimeSequence(unsigned component, unsigned samples, const std::string& name, double rate, const std::string& unit, int type, double startTime, double scale, double offset, const std::array<double,2>& range, Node* parent)
  : TimeSequence(std::vector<unsigned>({component}),samples,name,rate,unit,type,startTime,scale,offset,range,parent)
  {};
  
  TimeSequence::TimeSequence(const std::vector<unsigned>& dimensions, unsigned samples, const std::string& name, double rate, const std::string& unit, int type, double startTime, double scale, double offset, const std::array<double,2>& range, Node* parent)
  : Node(*new TimeSequencePrivate(this,dimensions,samples,name,rate,unit,type,startTime,scale,offset,range),parent)
  {};
  
  /*
   * @fn TimeSequence::~TimeSequence()
   * Destructor (default)
   */
  TimeSequence::~TimeSequence() noexcept = default;
  
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
  
  int TimeSequence::type() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Type;
  };
  
  void TimeSequence::setType(int value) noexcept
  {
    auto optr = this->pimpl();
    if (optr->Type == value)
      return;
    optr->Type = value;
    this->modified();
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
  
  double TimeSequence::scale() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Scale;
  };
  
  void TimeSequence::setScale(double value) noexcept
  {
    auto optr = this->pimpl();
    if (std::fabs(value - optr->Scale) < std::numeric_limits<double>::epsilon())
      return;
    optr->Scale = value;
    this->modified();
  };
  
  double TimeSequence::offset() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Offset;
  };
  
  void TimeSequence::setOffset(double value) noexcept
  {
    auto optr = this->pimpl();
    if (std::fabs(value - optr->Offset) < std::numeric_limits<double>::epsilon())
      return;
    optr->Offset = value;
    this->modified();
  };
  
  const std::array<double,2>& TimeSequence::range() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Range;
  };
  
  void TimeSequence::setRange(const std::array<double,2>& value) noexcept
  {
    auto optr = this->pimpl();
    if ((std::fabs(value[0] - optr->Range[0]) < std::numeric_limits<double>::epsilon())
     && (std::fabs(value[1] - optr->Range[1]) < std::numeric_limits<double>::epsilon()))
      return;
    optr->Range = value;
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
  
  double& TimeSequence::data(unsigned sample, std::vector<unsigned>&& indices) noexcept
  {
    auto optr = this->pimpl();
    assert(sample < optr->Samples);
    assert(indices.size() <= optr->Dimensions.size());
    size_t col = indices.empty() ? 0 : indices.back();
    for (size_t i = 0 ; i < std::min(indices.size(),optr->AccumulatedDimensions.size()) ; ++i)
      col += optr->AccumulatedDimensions[i] * indices[i];
    return optr->Data[col*optr->Samples+sample];
  };
};