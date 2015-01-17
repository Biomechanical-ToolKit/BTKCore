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

#ifndef __btkEigenInterp1PCHIP_h
#define __btkEigenInterp1PCHIP_h

#include "interp1_base.h"

#include "Utilities/maths/sign.h"

namespace btkEigen
{
  using namespace Eigen;
  
  /**
   * PCHIP: Piecewise Cubic Hermite Interpolating Polynomial.
   *
   * The computation of the slopes is inspired by the SciPy internal function '_find_derivatives' use by the function 'pchip'.
   * The computation of the slopes at the endpoints is based on Matlab non-centerd, shape-preserving three-point formulae.
   * The cubic interpolation method is the same as proposed on Wikipedia: http://en.wikipedia.org/wiki/Monotone_cubic_interpolation#Cubic_interpolation
   *
   * WARNING: The value for the horizontal axis (variable x) MUST be monotone increasing
   *
   * @par References
   * F. N. Fritsch and R. E. Carlson@n
   * <em>Monotone Piecewise Cubic Interpolation</em>@n
   * SIAM J. Numerical Analysis, <b>1980</b>, 17, 238-246.
   *
   */
  
  template <typename VectorType>
  struct PCHIP_interp : Base_interp<VectorType>
  {
    typedef typename VectorType::Scalar Scalar;
    typedef typename VectorType::Index Index;
    
    VectorType dk;
    
  	PCHIP_interp(const VectorType* x, const VectorType* y)
  	: Base_interp<VectorType>(x,y,2), dk()
    {
      this->setinternals(x,y);
    };
	
    Scalar rawinterp(Index j, Scalar x)
    {
      Index klo = j, khi=j+1;
    	Scalar h = this->xx->coeff(khi) - this->xx->coeff(klo);
      eigen_assert(h > 0.0);
      Scalar t = (x - this->xx->coeff(klo)) / h;
      const Scalar c1 = Scalar(1);
      const Scalar c2 = Scalar(2);
      const Scalar c3 = Scalar(3);
      Scalar h00 = c2*t*t*t - c3*t*t + c1;
      Scalar h10 = t*t*t - c2*t*t + t;
      Scalar h01 = -c2*t*t*t + c3*t*t;
      Scalar h11 = t*t*t - t*t;
      double val = this->yy->coeff(klo)*h00 + h*this->dk.coeff(klo)*h10 + this->yy->coeff(khi)*h01 + h*this->dk.coeff(khi)*h11;
      return val;
  	};
    
    void setinternals(const VectorType* x, const VectorType* y)
    {
      Index num = x->rows() * x->cols();
      VectorType hk = x->segment(1,num-1) - x->segment(0,num-1);
      VectorType mk = (y->segment(1,num-1) - y->segment(0,num-1)).cwiseQuotient(hk);
      VectorType smk(num-1); smk.setZero(); smk = (mk.array() > 0.0).select(1.0, smk) - (mk.array() < 0.0).select(1.0, smk);
#if 1
      // Matlab method
      VectorType condition(num-2); condition.setZero(); condition = ((smk.segment(0,num-2).cwiseProduct(smk.segment(1,num-2))).array() > 0).select(1.0,condition);
      VectorType hs = hk.segment(1,num-2) + hk.segment(0,num-2);
      VectorType w1 = (hk.segment(0,num-2) + hs).cwiseQuotient(3.0 * hs);
      VectorType w2 = (hk.segment(1,num-2) + hs).cwiseQuotient(3.0 * hs);
      VectorType mk1 = mk.segment(0,num-2).cwiseAbs();
      VectorType mk2 = mk.segment(1,num-2).cwiseAbs();
      VectorType dmax = (mk1.array() > mk2.array()).select(mk1,mk2);
      VectorType dmin = (mk1.array() < mk2.array()).select(mk1,mk2);
      this->dk.resize(num);
      this->dk.segment(1,num-2) = (condition).select(dmin.cwiseQuotient((w1.cwiseProduct(mk.segment(0,num-2))+w2.cwiseProduct(mk.segment(1,num-2))).cwiseQuotient(dmax)),0.0);
      // dk(0)
      this->dk.coeffRef(0) = ((2.0*hk.coeff(0)+hk.coeff(1))*mk.coeff(0) - hk.coeff(0)*mk.coeff(1)) / (hk.coeff(0)+hk.coeff(1));
      if (sign(dk.coeff(0)) != sign(mk.coeff(0)))
        this->dk.coeffRef(0) = 0.0;
      else if ((sign(mk.coeff(0)) != sign(mk.coeff(1))) && (std::fabs(dk.coeff(0)) > std::fabs(3.0*mk.coeff(0))))
        this->dk.coeffRef(0) = 3.0*mk.coeff(0);
      // dk(-1)
      this->dk.coeffRef(num-1) = ((2.0*hk.coeff(num-2)+hk.coeff(num-3))*mk.coeff(num-2) - hk.coeff(num-2)*mk.coeff(num-3)) / (hk.coeff(num-2)+hk.coeff(num-3));
      if (sign(dk.coeff(num-1)) != sign(mk.coeff(num-2)))
        this->dk.coeffRef(num-1) = 0.0;
      else if ((sign(mk.coeff(num-2)) != sign(mk.coeff(num-3))) && (std::fabs(dk.coeff(num-1)) > std::fabs(3.0*mk.coeff(num-2))))
        this->dk.coeffRef(num-1) = 3.0*mk.coeff(num-2);
#else
      // ScyPy method
      VectorType w1 = 2.0 * hk.segment(1,num-2) + hk.segment(0,num-2);
      VectorType w2 = hk.segment(1,num-2) + 2.0 * hk.segment(0,num-2);
      VectorType whmean = (w1 + w2).cwiseInverse().cwiseProduct((w1.cwiseQuotient(mk.segment(1,num-2)) + w2.cwiseQuotient(mk.segment(0,num-2))));
      VectorType condition(num-2); condition.setZero(); condition = (smk.segment(0,num-2).array() != smk.segment(1,num-2).array()).select(1.0,condition) + (smk.segment(0,num-2).array() == 0).select(1.0,condition) + (smk.segment(1,num-2).array() == 0).select(1.0,condition);
      this->dk.resize(num);
      this->dk.segment(1,num-2) = condition.select(0.0, whmean.cwiseInverse());
      dk.coeffRef(0) = ((mk.coeff(0) == 0) || (dk.coeff(1) == 0)) ? 0.0 : 1.0/(1.0/mk.coeff(0)+1.0/dk.coeff(1));
      dk.coeffRef(num-1) = ((mk.coeff(num-2) == 0) || (dk.coeff(num-2) == 0)) ? 0.0 : 1.0/(1.0/mk.coeff(num-2)+1.0/dk.coeff(num-2));
#endif
    };
  };
};

#endif // __btkEigenInterp1PCHIP_h