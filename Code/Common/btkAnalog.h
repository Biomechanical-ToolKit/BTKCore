/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2013, Arnaud Barré
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

#ifndef __btkAnalog_h
#define __btkAnalog_h

#include "btkMeasure.h"

namespace btk
{
  class Analog;
  
  template <>
  struct MeasureTraits<Analog>
  {
    typedef Eigen::Matrix<double, Eigen::Dynamic, 1> Values; ///< Analog's  values along the time with 1 components (1 column).
    
   /**
    * @class Data
    * @brief Class storing the measures for an analog channel.
    */
   class Data : public MeasureData<Analog>
    {
    public:
      typedef SharedPtr<Data> Pointer;
      typedef SharedPtr<const Data> ConstPointer;
      
      static Pointer Null() {return Pointer();};
      static Pointer New(int frameNumber) {return Pointer(new Data(frameNumber));};
      
      void Resize(int frameNumber);
      
      Pointer Clone() const {return Pointer(new Data(*this));}
      
    private:
      Data(int frameNumber) : MeasureData<Analog>(frameNumber) {};
      Data(const Data& toCopy) : MeasureData<Analog>(toCopy) {};
      Data& operator=(const Data& ); // Not implemented.
    };
  };
  
  class Analog : public Measure<Analog>
  {
  public:
    typedef MeasureTraits<Analog>::Values Values;
    
    typedef enum {Unknown = 0, 
                  PlusMinus10 = 10000, PlusMinus5 = 5000, PlusMinus2Dot5 = 2500, 
                  PlusMinus1Dot25 = 1250, PlusMinus1 = 1000, PlusMinus0Dot5 = 500, 
                  PlusMinus0Dot25 = 250, PlusMinus0Dot1 = 100, PlusMinus0Dot05 = 50} Gain;

    typedef SharedPtr<Analog> Pointer;
    typedef SharedPtr<const Analog> ConstPointer;
    
    static Pointer New(const std::string& label = "", const std::string& desc = "") {return Pointer(new Analog(label, desc));};
    static Pointer New(int frameNumber) {return Pointer(new Analog("", frameNumber));};
    static Pointer New(const std::string& label, int frameNumber) {return Pointer(new Analog(label, frameNumber));};
    
    virtual ~Analog() {};
    
    const std::string& GetUnit() const {return this->m_Unit;};
    BTK_COMMON_EXPORT void SetUnit(const std::string& u);
    Gain GetGain() const {return this->m_Gain;};
    BTK_COMMON_EXPORT void SetGain(Gain g);
    int GetOffset() const {return this->m_Offset;};
    BTK_COMMON_EXPORT void SetOffset(int o);
    double GetScale() const {return this->m_Scale;};
    BTK_COMMON_EXPORT void SetScale(double s);

    void SetDataSlice(int frame, double val);
    
    Pointer Clone() const {return Pointer(new Analog(*this));}
    
  protected:
    BTK_COMMON_EXPORT Analog(const std::string& label, const std::string& desc);
    BTK_COMMON_EXPORT Analog(const std::string& label, int frameNumber, Gain g = Unknown);
    
  private:
    Analog(const Analog& toCopy);
    Analog& operator=(const Analog& ); // Not implemented.
    
    std::string m_Unit;
    Gain m_Gain;
    int m_Offset;
    double m_Scale;
  };
  
  inline void Analog::SetDataSlice(int frame, double val)
  {
    this->GetValues().coeffRef(frame, 0) = val;
  };
  
  // ----------------------------------------------------------------------- //
  
  inline void MeasureTraits<Analog>::Data::Resize(int frameNumber)
  {
    if (frameNumber > this->m_Values.rows())
    {
      Values v = Values::Zero(frameNumber,Values::ColsAtCompileTime);
      if (this->m_Values.data() != 0)
        v.block(0,0,this->m_Values.rows(),Values::ColsAtCompileTime) = this->m_Values;
      this->m_Values = v;
    }
    else
      this->m_Values = this->m_Values.block(0,0,frameNumber,Values::ColsAtCompileTime);
  };
};

#endif // __btkAnalog_h
