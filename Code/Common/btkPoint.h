/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2012, Arnaud Barré
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
  class Point : public Measure<3>
  {
  public:
    typedef Eigen::Matrix<double, Eigen::Dynamic, 1> Residuals;
    typedef Eigen::Matrix<double, Eigen::Dynamic, 1> Masks;
    typedef enum {Marker = 0, Angle, Force, Moment, Power, Scalar, Reaction} Type;
    
    typedef SharedPtr<Point> Pointer;
    typedef SharedPtr<const Point> ConstPointer;
    
    static Pointer New(const std::string& label = "", Type t = Marker, const std::string& desc = "") {return Pointer(new Point(label, t, desc));};
    static Pointer New(int frameNumber) {return Pointer(new Point("", frameNumber, Marker, ""));};
    static Pointer New(const std::string& label, int frameNumber, Type t = Marker, const std::string& desc = "") {return Pointer(new Point(label, frameNumber, t, desc));};
    
    virtual ~Point() {};
    
    void SetFrame(int frame, double x, double y, double z, double res = 0.0, double mask = 0.0);
    
    Residuals& GetResiduals() {return this->m_Residuals;};
    const Residuals& GetResiduals() const {return this->m_Residuals;};
    BTK_COMMON_EXPORT void SetResiduals(const Residuals& r);
    Masks& GetMasks() {return this->m_Masks;};
    const Masks& GetMasks() const {return this->m_Masks;};
    BTK_COMMON_EXPORT void SetMasks(const Masks& m);
    BTK_COMMON_EXPORT void SetFrameNumber(int frameNumber);
    Type GetType() const {return this->m_Type;};
    BTK_COMMON_EXPORT void SetType(Point::Type t);
    Pointer Clone() const {return Pointer(new Point(*this));};
    
  protected:
    BTK_COMMON_EXPORT Point(const std::string& label, Type t, const std::string& desc);
    BTK_COMMON_EXPORT Point(const std::string& label, int frameNumber, Type t, const std::string& desc);
    
  private:
    BTK_COMMON_EXPORT Point(const Point& toCopy);
    Point& operator=(const Point& ); // Not implemented.
    
    Residuals m_Residuals;
    Masks m_Masks;
    Type m_Type;
  };
  
  inline void Point::SetFrame(int frame, double x, double y, double z, double res, double mask)
  {
    this->m_Values.coeffRef(frame,0) = x;
    this->m_Values.coeffRef(frame,1) = y;
    this->m_Values.coeffRef(frame,2) = z; 
    this->m_Residuals.coeffRef(frame) = res;
    this->m_Masks.coeffRef(frame) = mask;
  };
};

#endif // __btkPoint_h
