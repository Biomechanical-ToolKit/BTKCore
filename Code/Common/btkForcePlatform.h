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

#ifndef __btkForcePlatform_h
#define __btkForcePlatform_h

#include "btkAnalogCollection.h"
#include "btkException.h"

namespace btk
{
  class ForcePlatform : public DataObject
  {
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    typedef Eigen::Matrix<double, 3, 4> Corners;
    typedef Eigen::Matrix<double, 3, 1> Corner;
    typedef Eigen::Matrix<double, 3, 1> Origin;
    typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> CalMatrix;

    typedef SharedPtr<ForcePlatform> Pointer;
    typedef SharedPtr<const ForcePlatform> ConstPointer;

    typedef AnalogCollection::Iterator Iterator;
    typedef AnalogCollection::ConstIterator ConstIterator;

    static Pointer New() {return Pointer(new ForcePlatform());};
    ~ForcePlatform() {};
    
    Iterator Begin() {return this->m_Channels->Begin();};
    ConstIterator Begin() const {return this->m_Channels->Begin();};
    Iterator End() {return this->m_Channels->End();};
    ConstIterator End() const {return this->m_Channels->End();};
    int GetChannelNumber() const {return this->m_Channels->GetItemNumber();};
    Analog::Pointer GetChannel(int idx) const {return this->m_Channels->GetItem(idx);};
    BTK_COMMON_EXPORT void SetChannel(int idx, Analog::Pointer c);
    AnalogCollection::Pointer GetChannels() const {return this->m_Channels;};

    Origin& GetOrigin() {return this->m_Origin;};
    const Origin& GetOrigin() const {return this->m_Origin;};
    BTK_COMMON_EXPORT void SetOrigin(double x, double y, double z);
    BTK_COMMON_EXPORT void SetOrigin(const Origin& o);

    BTK_COMMON_EXPORT const Corner GetCorner(int idx) const;
    BTK_COMMON_EXPORT void SetCorner(int row, int col, double v);
    BTK_COMMON_EXPORT void SetCorner(int idx, double x, double y, double z);
    BTK_COMMON_EXPORT void SetCorner(int idx, const Corner& c);
    Corners& GetCorners() {return this->m_Corners;};
    const Corners& GetCorners() const {return this->m_Corners;};
    BTK_COMMON_EXPORT void SetCorners(const Corners& c);

    CalMatrix& GetCalMatrix() {return this->m_CalMatrix;};
    const CalMatrix& GetCalMatrix() const {return this->m_CalMatrix;};
    BTK_COMMON_EXPORT void SetCalMatrix(const CalMatrix& cal);

    int GetType() const {return this->m_Type;};

  protected:
    BTK_COMMON_EXPORT ForcePlatform();

    int m_Type;
    AnalogCollection::Pointer m_Channels;
    CalMatrix m_CalMatrix;
    
  private:
    ForcePlatform(const ForcePlatform& ); // Not implemeted.
    ForcePlatform& operator=(const ForcePlatform& ); // Not implemented

    Origin m_Origin;
    Corners m_Corners;  
  };
};

#endif // __btkForcePlatform_h
