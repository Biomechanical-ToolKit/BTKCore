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

#ifndef __btkPoint_h
#define __btkPoint_h

#include "btkMeasure.h"

namespace btk
{
  class Point;
  
  template <>
  struct MeasureTraits<Point>
  {
    typedef Eigen::Matrix<double, Eigen::Dynamic, 3> Values; ///< Point' values along the time with 3 components (3 columns).
    typedef Eigen::Matrix<double, Eigen::Dynamic, 1> Residuals; ///< Vector of double representing the residuals associated with each frames (if applicable).
    
    /**
     * @class Data
     * @brief Class storing the measures for a point (3D values and residuals along the time).
     */
    class Data : public MeasureData<Point>
    {
    public:
      typedef typename MeasureTraits<Point>::Residuals Residuals; ///< Vector of double representing the residuals associated with each frames (if applicable).
      
      typedef SharedPtr<Data> Pointer;
      typedef SharedPtr<const Data> ConstPointer;
      
      static Pointer Null() {return Pointer();};
      static Pointer New(int frameNumber) {return Pointer(new Data(frameNumber));};
      
      void Resize(int frameNumber);
      
      Residuals& GetResiduals() {return this->m_Residuals;};
      const Residuals& GetResiduals() const {return this->m_Residuals;};
      void SetResiduals(const Residuals& r) {this->m_Residuals = r; this->Modified();};
      
      Pointer Clone() const {return Pointer(new Data(*this));}
      
    private:
      Data(int frameNumber) : MeasureData<Point>(frameNumber), m_Residuals(Residuals::Zero(frameNumber,MeasureTraits<Point>::Residuals::ColsAtCompileTime)) {};
      Data(const Data& toCopy) : MeasureData<Point>(toCopy), m_Residuals(toCopy.m_Residuals) {};
      Data& operator=(const Data& ); // Not implemented.
      
      Residuals m_Residuals;
    };
  };

  class Point : public Measure<Point>
  {
  public:
    typedef MeasureTraits<Point>::Residuals Residuals;
    
    typedef enum {Marker = 0, Angle, Force, Moment, Power, Scalar, Reaction} Type;
    
    typedef SharedPtr<Point> Pointer;
    typedef SharedPtr<const Point> ConstPointer;
    
    static Pointer New(const std::string& label = "", Type t = Marker, const std::string& desc = "") {return Pointer(new Point(label, t, desc));};
    static Pointer New(int frameNumber) {return Pointer(new Point("", frameNumber, Marker, ""));};
    static Pointer New(const std::string& label, int frameNumber, Type t = Marker, const std::string& desc = "") {return Pointer(new Point(label, frameNumber, t, desc));};
    
    virtual ~Point() {};
    
    void SetDataSlice(int idx, double x, double y, double z, double res = 0.0);
    
    BTK_COMMON_EXPORT Residuals& GetResiduals();
    BTK_COMMON_EXPORT const Residuals& GetResiduals() const;
    BTK_COMMON_EXPORT void SetResiduals(const Residuals& r);
    
    Type GetType() const {return this->m_Type;};
    BTK_COMMON_EXPORT void SetType(Point::Type t);
    
    Pointer Clone() const {return Pointer(new Point(*this));};
    
  protected:
    BTK_COMMON_EXPORT Point(const std::string& label, Type t, const std::string& desc);
    BTK_COMMON_EXPORT Point(const std::string& label, int frameNumber, Type t, const std::string& desc);
    
  private:
    BTK_COMMON_EXPORT Point(const Point& toCopy);
    Point& operator=(const Point& ); // Not implemented.
    
    Type m_Type;
  };
  
  inline void Point::SetDataSlice(int idx, double x, double y, double z, double res)
  {
    this->GetValues().coeffRef(idx,0) = x;
    this->GetValues().coeffRef(idx,1) = y;
    this->GetValues().coeffRef(idx,2) = z; 
    this->GetResiduals().coeffRef(idx) = res;
  };
  
  // ----------------------------------------------------------------------- //
  
  inline void MeasureTraits<Point>::Data::Resize(int frameNumber)
  {
    // Values
    if (frameNumber > this->m_Values.rows())
    {
      Values v = Values::Zero(frameNumber,Values::ColsAtCompileTime);
      if (this->m_Values.data() != 0)
        v.block(0,0,this->m_Values.rows(),Values::ColsAtCompileTime) = this->m_Values;
      this->m_Values = v;
    }
    else
      this->m_Values.conservativeResize(frameNumber,Values::ColsAtCompileTime);
    // Residuals
    if (frameNumber > this->m_Residuals.rows())
    {
      Residuals r = Residuals::Zero(frameNumber, Residuals::ColsAtCompileTime);
      if (this->m_Residuals.data() != 0)
        r.block(0,0,this->m_Residuals.rows(),Residuals::ColsAtCompileTime) = this->m_Residuals;
      this->m_Residuals = r;
    }
    else
      this->m_Residuals.conservativeResize(frameNumber);
  };
};

#endif // __btkPoint_h
