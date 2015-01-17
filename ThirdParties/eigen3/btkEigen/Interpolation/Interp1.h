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

#ifndef __btkEigenInterp1_h
#define __btkEigenInterp1_h

#include "interp1_linear.h"
#include "interp1_pchip.h"

namespace btkEigen
{
  using namespace Eigen;
  
  struct interp1
  {
    interp1()
    {};
    
    template <typename VectorType, typename OtherVectorType>
    void linear(OtherVectorType* yi, const VectorType& x, const VectorType& y, const OtherVectorType& xi);
    
    template <typename VectorType, typename OtherVectorType>
    void pchip(OtherVectorType* yi, const VectorType& x, const VectorType& y, const OtherVectorType& xi);
  };
  
  template <typename VectorType, typename OtherVectorType>
  inline void interp1::linear(OtherVectorType* yi, const VectorType& x, const VectorType& y, const OtherVectorType& xi)
  {
    typedef typename VectorType::Index Index;
    Linear_interp<VectorType> li(&x, &y);
    yi->resize(xi.rows(), xi.cols());
    for (Index i = 0 ; i < (xi.rows() * xi.cols()) ; ++i)
      yi->coeffRef(i) = li.interp(xi.coeff(i));
  };
  
  template <typename VectorType, typename OtherVectorType>
  inline void interp1::pchip(OtherVectorType* yi, const VectorType& x, const VectorType& y, const OtherVectorType& xi)
  {
    typedef typename VectorType::Index Index;
    PCHIP_interp<VectorType> pi(&x, &y);
    yi->resize(xi.rows(), xi.cols());
    for (Index i = 0 ; i < (xi.rows() * xi.cols()) ; ++i)
      yi->coeffRef(i) = pi.interp(xi.coeff(i));
  };
};

#endif // __btkEigenInterp1_h