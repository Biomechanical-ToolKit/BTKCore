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

#ifndef __btkTimeSequence_h
#define __btkTimeSequence_h

#include "btkTrialExport.h"
#include "btkNode.h"
#include "btkMacros.h" // _BTK_CONSTEXPR, _BTK_NOEXCEPT

#include <vector>
#include <array>
#include <numeric>
#include <initializer_list>

namespace btk
{
  class TimeSequencePrivate;
  
  class BTK_TRIAL_EXPORT TimeSequence : public Node
  {
    BTK_DECLARE_PIMPL_ACCESSOR(TimeSequence)
  
  public:
    enum : int {Unknown = 0, Marker, Angle, Force, Moment, Power, Scalar, Analog, Pose};
    static _BTK_CONSTEXPR std::array<double,2> InfinityRange{{-std::numeric_limits<double>::infinity(),std::numeric_limits<double>::infinity()}};
    
    TimeSequence(const std::string& name, unsigned components, unsigned samples, double rate, double start, int type, const std::string& unit, double scale, double offset, const std::array<double,2>& range, Node* parent = nullptr);
    TimeSequence(const std::string& name, unsigned components, unsigned samples, double rate, double start, int type, const std::string& unit, Node* parent = nullptr);
    TimeSequence(const std::string& name, const std::vector<unsigned>& dimensions, unsigned samples, double rate, double start, int type, const std::string& unit, double scale, double offset, const std::array<double,2>& range, Node* parent = nullptr);
    TimeSequence(const std::string& name, const std::vector<unsigned>& dimensions, unsigned samples, double rate, double start, int type, const std::string& unit, Node* parent = nullptr);
    ~TimeSequence() _BTK_NOEXCEPT;
    
    TimeSequence(const TimeSequence& ) = delete;
    TimeSequence(TimeSequence&& ) _BTK_NOEXCEPT = delete;
    TimeSequence& operator=(const TimeSequence& ) = delete;
    TimeSequence& operator=(TimeSequence&& ) _BTK_NOEXCEPT = delete;
    
    double sampleRate() const _BTK_NOEXCEPT;
    void setSampleRate(double value) _BTK_NOEXCEPT;
    
    const std::vector<unsigned>& dimensions() const _BTK_NOEXCEPT;
    unsigned samples() const _BTK_NOEXCEPT;
    unsigned components() const _BTK_NOEXCEPT;
    size_t elements() const _BTK_NOEXCEPT;
    
    double duration() const _BTK_NOEXCEPT;
    
    int type() const _BTK_NOEXCEPT;
    void setType(int value) _BTK_NOEXCEPT;
    
    const std::string& unit() const _BTK_NOEXCEPT;
    void setUnit(const std::string& value) _BTK_NOEXCEPT;
    
    double startTime() const _BTK_NOEXCEPT;
    void setStartTime(double value) _BTK_NOEXCEPT;
    
    double scale() const _BTK_NOEXCEPT;
    void setScale(double value) _BTK_NOEXCEPT;
    
    double offset() const _BTK_NOEXCEPT;
    void setOffset(double value) _BTK_NOEXCEPT;
    
    const std::array<double,2>& range() const _BTK_NOEXCEPT;
    void setRange(const std::array<double,2>& value) _BTK_NOEXCEPT;
    
    const double* data() const _BTK_NOEXCEPT;
    double* data() _BTK_NOEXCEPT;
    
    template <typename... Is> double data(unsigned sample, Is... indices) const _BTK_NOEXCEPT;
    template <typename... Is> double& data(unsigned sample, Is... indices) _BTK_NOEXCEPT;
    
    void resize(unsigned samples);
  
  private:
    double& data(unsigned sample, std::initializer_list<unsigned>&& indices) const _BTK_NOEXCEPT;
  };
  
  template <typename... Is>
  inline double TimeSequence::data(unsigned sample, Is... indices) const _BTK_NOEXCEPT
  {
    return this->data(sample, {static_cast<unsigned>(indices)...});
  };
  
  template <typename... Is>
  inline double& TimeSequence::data(unsigned sample, Is... indices) _BTK_NOEXCEPT
  {
    return this->data(sample, {static_cast<unsigned>(indices)...});
  };
};

#endif // __btkTimeSequence_h