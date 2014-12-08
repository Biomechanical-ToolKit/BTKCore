/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2014, Arnaud Barré
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

#ifndef __btkEigenInterp1Base_h
#define __btkEigenInterp1Base_h

#include <Eigen/Core>

/*
 * Basis for the 1D interpolation 
 * This code is based on the implementation proposed in the book "Numeric Recipes in C" (3rd ed.)
 */

namespace btkEigen
{
  using namespace Eigen;
  
  template <typename VectorType>
  struct Base_interp
  {
    typedef typename VectorType::Scalar Scalar;
    typedef typename VectorType::Index Index;
    
  	Index n, mm, jsav, cor, dj;
  	const VectorType *xx, *yy;
  	
    Base_interp(const VectorType* x, const VectorType* y, int m)
    {
      this->n = x->rows() * x->cols();
      this->mm = m;
      this->jsav = Index(0);
      this->cor = Index(0);
      this->xx = x;
      this->yy = y;
  		this->dj = std::min(Index(1), static_cast<Index>(std::pow(static_cast<Scalar>(n),0.25)));
  	};

  	Scalar interp(Scalar x)
    {
  		Index jlo = this->cor ? this->hunt(x) : this->locate(x);
  		return this->rawinterp(jlo, x);
  	};

  	Index locate(const Scalar x);
  	Index hunt(const Scalar x);
	
  	virtual Scalar rawinterp(Index jlo, Scalar x) = 0;
  };

  template <typename VectorType>
  typename Base_interp<VectorType>::Index Base_interp<VectorType>::locate(const Scalar x)
  {
  	eigen_assert((this->n >= 2) && (this->mm >= 2) && (this->mm <= this->n) && "locate size error");
    bool ascnd = (this->xx->coeff(n-1) >= this->xx->coeff(0));
  	Index ju = n-1, jl = Index(0);
  	while (ju-jl > 1)
    {
  		Index jm = (ju + jl) >> 1;
  		if ((x >= this->xx->coeff(jm)) == ascnd)
  			jl=jm;
  		else
  			ju=jm;
  	}
  	this->cor = abs(jl - this->jsav) > this->dj ? 0 : 1;
  	this->jsav = jl; 
  	return std::max(Index(0), std::min(this->n - this->mm, jl - ((this->mm - 2) >> 1)));
  };
  
  template <typename VectorType>
  typename Base_interp<VectorType>::Index Base_interp<VectorType>::hunt(const Scalar x)
  {
  	eigen_assert((this->n >= 2) && (this->mm >= 2) && (this->mm <= this->n) && "locate size error");
  	bool ascnd = (this->xx->coeff(n-1) >= this->xx->coeff(0));
  	Index jl = this->jsav, jm = Index(0), ju = Index(0), inc = Index(1);
  	if (jl < 0 || jl > n-1)
    {
  		jl = 0;
  		ju = n-1;
  	}
    else
    {
  		if ((x >= this->xx->coeff(jl)) == ascnd)
      {
  			for (;;)
        {
  				ju = jl + inc;
  				if (ju >= this->n-1)
          {
            ju = this->n-1;
            break;
          }
  				else if ((x < this->xx->coeff(ju)) == ascnd)
            break;
  				else
          {
  					jl = ju;
  					inc += inc;
  				}
  			}
  		}
      else
      {
  			ju = jl;
  			for (;;)
        {
  				jl = jl - inc;
  				if (jl <= 0)
          {
            jl = 0;
            break;
          }
  				else if ((x >= this->xx->coeff(jl)) == ascnd)
            break;
  				else
          {
  					ju = jl;
  					inc += inc;
  				}
  			}
  		}
  	}
  	while (ju-jl > 1)
    {
  		jm = (ju+jl) >> 1;
  		if ((x >= this->xx->coeff(jm)) == ascnd)
  			jl = jm;
  		else
  			ju = jm;
  	}
  	this->cor = abs(jl - this->jsav) > this->dj ? 0 : 1;
  	this->jsav = jl;
  	return std::max(Index(0), std::min(this->n - this->mm, jl - ((this->mm - 2) >> 1)));
  };
};

#endif // __btkEigenInterp1Base_h