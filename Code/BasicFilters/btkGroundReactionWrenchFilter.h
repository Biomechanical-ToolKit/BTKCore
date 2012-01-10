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

#ifndef __btkGroundReactionWrenchFilter_h
#define __btkGroundReactionWrenchFilter_h

#include "btkForcePlatformWrenchFilter.h"
#include "btkForcePlatformCollection.h"
#include "btkForcePlatformTypes.h"
#include "btkWrenchCollection.h"

#include <Eigen/Array>
#include <Eigen/Geometry>

namespace btk
{
  class GroundReactionWrenchFilter : public ForcePlatformWrenchFilter
  {
  public:
    typedef SharedPtr<GroundReactionWrenchFilter> Pointer;
    typedef SharedPtr<const GroundReactionWrenchFilter> ConstPointer;

    static Pointer New() {return Pointer(new GroundReactionWrenchFilter());};
    
    // ~GroundReactionWrenchFilter(); // Implicit

    bool GetThresholdState() const {return this->m_ThresholdActivated;};
    BTK_BASICFILTERS_EXPORT void SetThresholdState(bool activated = false);
    double GetThresholdValue() const {return this->m_ThresholdValue;};
    BTK_BASICFILTERS_EXPORT void SetThresholdValue(double v);
    
  protected:
    BTK_BASICFILTERS_EXPORT GroundReactionWrenchFilter();
    
  private:
    virtual void FinishTypeI(Wrench::Pointer wrh, ForcePlatform::Pointer fp, int index);
    virtual void FinishAMTI(Wrench::Pointer wrh, ForcePlatform::Pointer fp, int index);
    virtual void FinishKistler(Wrench::Pointer wrh, ForcePlatform::Pointer fp, int index);
    void FinishGRWComputation(Wrench::Pointer grw, const ForcePlatform::Origin& o) const;
    
    GroundReactionWrenchFilter(const GroundReactionWrenchFilter& ); // Not implemented.
    GroundReactionWrenchFilter& operator=(const GroundReactionWrenchFilter& ); // Not implemented.

    bool m_ThresholdActivated;
    double m_ThresholdValue;
  };

  inline void GroundReactionWrenchFilter::FinishGRWComputation(Wrench::Pointer grw, const ForcePlatform::Origin& o) const
  { 
    typedef Eigen::Matrix<double, Eigen::Dynamic, 1> Component;
    Component Fx = grw->GetForce()->GetValues().col(0);
    Component Fy = grw->GetForce()->GetValues().col(1);
    Component Fz = grw->GetForce()->GetValues().col(2);
    Component Mx = grw->GetMoment()->GetValues().col(0);
    Component My = grw->GetMoment()->GetValues().col(1);
    Component Mz = grw->GetMoment()->GetValues().col(2);
    Component Px = grw->GetPosition()->GetValues().col(0);
    Component Py = grw->GetPosition()->GetValues().col(1);
    Component Pz = grw->GetPosition()->GetValues().col(2);

    // Square norm of the forces.
    Component sNF =  grw->GetForce()->GetValues().rowwise().squaredNorm();
    // M_s = M_o + F x OS
    Mx += Fy * o.z() - o.y() * Fz;
    My += Fz * o.x() - o.z() * Fx;
    Mz += Fx * o.y() - o.x() * Fy;
    // PWA
    // For explanations of the PWA calculation, see Shimba T. (1984), 
    // "An estimation of center of gravity from force platform data", 
    // Journal of Biomechanics 17(1), 53–60.
    Px = (Fy.cwise() * Mz - Fz.cwise() * My).cwise() / sNF
         - (Fx.cwise().square().cwise() * My - Fx.cwise() * (Fy.cwise() * Mx)).cwise() / (sNF.cwise() * Fz);
    Py = (Fz.cwise() * Mx - Fx.cwise() * Mz).cwise() / sNF
         - (Fx.cwise() * (Fy.cwise() * My) - Fy.cwise().square().cwise() * Mx).cwise() / (sNF.cwise() * Fz);
    Pz.setZero();
    // Suppress false PWA
    for (int i = 0 ; i < Fz.rows() ; ++i)
    {
      if ((sNF.coeff(i) == 0.0) || (this->m_ThresholdActivated && (fabs(Fz.coeff(i)) <= this->m_ThresholdValue)))
      {
        Px.coeffRef(i) = 0.0;
        Py.coeffRef(i) = 0.0;
        // Pz.coeffRef(i) = 0.0; // Already set to 0
        grw->GetPosition()->GetResiduals().coeffRef(i) = -1.0;
        grw->GetPosition()->GetMasks().coeffRef(i) = -1.0;
      }
    }
    // M_pwa = M_s + F_s x PWA
    Mx += Fy.cwise() * Pz - Py.cwise() * Fz;
    My += Fz.cwise() * Px - Pz.cwise() * Fx;
    Mz += Fx.cwise() * Py - Px.cwise() * Fy;

    grw->GetForce()->GetValues().col(0) = Fx;
    grw->GetForce()->GetValues().col(1) = Fy;
    grw->GetForce()->GetValues().col(2) = Fz;
    grw->GetMoment()->GetValues().col(0) = Mx;
    grw->GetMoment()->GetValues().col(1) = My;
    grw->GetMoment()->GetValues().col(2) = Mz;
    grw->GetPosition()->GetValues().col(0) = Px;
    grw->GetPosition()->GetValues().col(1) = Py;
    grw->GetPosition()->GetValues().col(2) = Pz;
  };
};

#endif // __btkGroundReactionWrenchFilter_h

