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

#ifndef __btkForcePlatformWrenchFilter_h
#define __btkForcePlatformWrenchFilter_h

#include "btkProcessObject.h"
#include "btkForcePlatformCollection.h"
#include "btkForcePlatformTypes.h"
#include "btkWrenchCollection.h"

#include <Eigen/Array>
#include <Eigen/Geometry>

namespace btk
{
  class ForcePlatformWrenchFilter : public ProcessObject
  {
  public:
    typedef SharedPtr<ForcePlatformWrenchFilter> Pointer;
    typedef SharedPtr<const ForcePlatformWrenchFilter> ConstPointer;

    static Pointer New() {return Pointer(new ForcePlatformWrenchFilter());};
    
    // ~ForcePlatformWrenchFilter(); // Implicit

    ForcePlatformCollection::Pointer GetInput() {return this->GetInput(0);};
    void SetInput(ForcePlatform::Pointer input)
    {
      ForcePlatformCollection::Pointer col = ForcePlatformCollection::New();
      col->InsertItem(input);
      this->SetNthInput(0, col);
    };
    void SetInput(ForcePlatformCollection::Pointer input) {this->SetNthInput(0, input);};
    WrenchCollection::Pointer GetOutput() {return this->GetOutput(0);};
    
    BTK_BASICFILTERS_EXPORT void SetTransformToGlobalFrame(bool activation = false);
    bool GetTransformToGlobalFrame() const {return this->m_GlobalTransformationActivated;};

  protected:
    BTK_BASICFILTERS_EXPORT ForcePlatformWrenchFilter();
    
    ForcePlatformCollection::Pointer GetInput(int idx) {return static_pointer_cast<ForcePlatformCollection>(this->GetNthInput(idx));};  
    WrenchCollection::Pointer GetOutput(int idx) {return static_pointer_cast<WrenchCollection>(this->GetNthOutput(idx));};
    BTK_BASICFILTERS_EXPORT virtual DataObject::Pointer MakeOutput(int idx);
    BTK_BASICFILTERS_EXPORT virtual void GenerateData();
    
  private:
    virtual void FinishTypeI(Wrench::Pointer wrh, ForcePlatform::Pointer fp, int index);
    virtual void FinishAMTI(Wrench::Pointer wrh, ForcePlatform::Pointer fp, int index);
    virtual void FinishKistler(Wrench::Pointer wrh, ForcePlatform::Pointer fp, int index);
    void TransformToGlobal(Wrench::Pointer wrh, const ForcePlatform::Corners& c) const;

    bool m_GlobalTransformationActivated;

    ForcePlatformWrenchFilter(const ForcePlatformWrenchFilter& ); // Not implemented.
    ForcePlatformWrenchFilter& operator=(const ForcePlatformWrenchFilter& ); // Not implemented.
  };
  
  inline void ForcePlatformWrenchFilter::TransformToGlobal(Wrench::Pointer wrh, const ForcePlatform::Corners& c) const
  {
    Eigen::Matrix<double, 3, 3> R;
    Eigen::Matrix<double, 3, 1> t;
    R.col(0) = c.col(0) - c.col(1);
    R.col(0).normalize();
    R.col(2) = R.col(0).cross(c.col(0) - c.col(3));
    R.col(2).normalize();
    R.col(1) = R.col(2).cross(R.col(0));
    t = (c.col(0) + c.col(2)) / 2;
    // Forces & Moments rotation
    wrh->GetForce()->GetValues() *= R.transpose();
    wrh->GetMoment()->GetValues() *= R.transpose();
    // Position rotation
    wrh->GetPosition()->GetValues() *= R.transpose();
    // Position translation
    wrh->GetPosition()->GetValues().col(0).cwise() += t.x();
    wrh->GetPosition()->GetValues().col(1).cwise() += t.y();
    wrh->GetPosition()->GetValues().col(2).cwise() += t.z();
  };
};

#endif // __btkForcePlatformWrenchFilter_h

