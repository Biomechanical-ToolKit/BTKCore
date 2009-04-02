/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009, Arnaud Barré
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

#ifndef __btkMeasure_h
#define __btkMeasure_h

#include "btkDataObject.h"

#include <Eigen/Core>
#include <string>

namespace btk
{
  class Acquisition;
  
  template <int d>
  class Measure : public DataObject
  {
  public:
    typedef Eigen::Matrix<double, Eigen::Dynamic, d> Values;
    
    typedef SharedPtr<Measure> Pointer;
    typedef SharedPtr<const Measure> ConstPointer;
    
    static Pointer New(int frameNumber = 10) {return Pointer(new Measure("", frameNumber));};
    static Pointer New(const std::string& label, int frameNumber) {return Pointer(new Measure(label, frameNumber));};
    
    virtual ~Measure() {};
    
    const std::string GetLabel() const {return this->m_Label;};
    void SetLabel(const std::string& label);
    const std::string GetDescription() const {return this->m_Description;};
    void SetDescription(const std::string& description);
    Values& GetValues() {return this->m_Values;};
    const Values& GetValues() const {return this->m_Values;};
    void SetValues(const Values& v);
    int GetFrameNumber() const {return this->m_Values.rows();};
    void SetFrameNumber(int frameNumber);
    
  protected:
    Measure(const std::string& label, int frameNumber);
    
    std::string m_Label;
    std::string m_Description;
    Values m_Values;
    
  private:
    Measure(const Measure& ); // Not implemented.
    Measure& operator=(const Measure& ); // Not implemented.
  };
  
  template <int d>
  Measure<d>::Measure(const std::string& label, int frameNumber)
  : m_Label(label), m_Description(),
    m_Values(Values(frameNumber, d))
  {};
  
  template <int d>
  void Measure<d>::SetLabel(const std::string& label)
  {
    this->m_Label = label;
    this->Modified();
  };
  
  template <int d>
  void Measure<d>::SetDescription(const std::string& description)
  {
    this->m_Description = description;
    this->Modified();
  };
  
  template <int d>
  void Measure<d>::SetValues(const Values& v)
  {
    this->m_Values = v;
    this->Modified();
  };
  
  template <int d>
  void Measure<d>::SetFrameNumber(int frameNumber)
  {
    if (frameNumber == this->GetFrameNumber())
      return;
    else if (frameNumber >= this->GetFrameNumber())
    {
      Values tmp(frameNumber, d);
      tmp.block(0,0,this->GetFrameNumber(),d) = this->m_Values;
      this->m_Values = tmp;
    }
    else
    {
      this->m_Values = this->m_Values.block(0,0,frameNumber,3);
    }
    this->Modified();
  };
};

#endif // __btkMeasure_h