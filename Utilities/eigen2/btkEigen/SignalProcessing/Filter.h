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

#ifndef __btkEigenFilter_h
#define __btkEigenFilter_h

#include "btkMacro.h"

#include <Eigen/Core>

#define BTKEIGEN_FILTER_PAD_COEFFICIENTS(templatename, out, in, len) \
  Eigen::Matrix<typename templatename::Scalar, Eigen::Dynamic, 1> out; \
  out.setZero(len); \
  out.block(0,0,in.rows(),1) = in;

namespace btkEigen
{
  using namespace Eigen;
  
  /**
   * Digital filter for 1D signal.
   *
   * This implementation is based on the Direct Form II Transposed method.
   *
   * Inspired by the Matlab script of Tony Richardson: filter.m (found <a href="http://ftp.back2roots.org/back2roots/cds/fred_fish/geekgadgets_vol1_9610/ade-bin/lib/octave/1.1.1/m/signal/filter.m">here</a>)
   * and the Matlab function <a href="http://www.mathworks.com/matlabcentral/fileexchange/32261-filterm">FilterX</a>.
   */
  template<typename NumeratorFilterCoeff, typename DenominatorFilterCoeff, typename VectorType, typename OtherDerived>
  VectorType filter(const NumeratorFilterCoeff& b, const DenominatorFilterCoeff& a, const VectorType& X, const OtherDerived& si, OtherDerived& sf)
  {
    // TODO: Assert the scalar type of each input.
    // TODO: Assert the storage order.
    // TODO: Possibility to parrallelize the filter when there is several columns? OpenMP?
  
    typedef typename VectorType::Scalar Scalar;
    
    const int len = std::max(b.rows(), a.rows());
    
    ei_assert(b.cols() == 1);
    ei_assert(a.cols() == 1);
    ei_assert(si.cols() == 1);
    ei_assert(si.rows() == len-1);
    
    VectorType Y = X;
    // Copy the coefficients and pad them with zeros
    BTKEIGEN_FILTER_PAD_COEFFICIENTS(VectorType,bb,b,len)
    BTKEIGEN_FILTER_PAD_COEFFICIENTS(VectorType,aa,a,len)

    Scalar norm = aa.coeff(0);
    if (norm == 0.0)
    {
      btkErrorMacro("Impossible to filter the signal, the first element of the denominator is equal to 0.");
      return Y;
    }
    else if (ei_abs(norm - 1.0) > machine_epsilon<Scalar>())
    {
      bb /= norm;
      aa /= norm;
    }
    
    int lci = len-1; // last index for the coefficients
    int lsi = lci-1; // last index for the state vector
    const Scalar* x = X.data();
    Scalar* y = Y.data();
    sf = si;
    // General method
    if (len > 2)
    {
      for (int i = 0 ; i < Y.rows() ; ++i)
      {
        y[i] = sf.coeff(0) + bb.coeff(0) * x[i];
        // Update state vector
        // TODO: Benchmark the code to determine the fastest metod.
#if 0
        for (int j = 1; j < lci; ++j)
          sf.coeffRef(j-1) = sf.coeff(j) + bb.coeff(j) * x[i] - aa.coeff(j) * y[i];
        sf.coeffRef(lsi) = bb.coeff(lci) * x[i] - aa.coeff(lci) * y[i];
#else
        sf.segment(0,lsi) = sf.segment(1,lsi) - aa.segment(1,lsi) * y[i] + bb.segment(1,lsi) * x[i];
        sf.coeffRef(lsi) = bb.coeff(lci) * x[i] - aa.coeff(lci) * y[i];
#endif
      }
    }
    // Specialization: if the order of the filter is less than 2
    else
    {
      for (int i = 0 ; i < Y.rows() ; ++i)
      {
        y[i] = sf.coeff(0) + bb.coeff(0) * x[i];
        sf.coeffRef(0) = bb.coeff(lci) * x[i] - aa.coeff(lci) * y[i];
      }
    }
    
    return Y;
  };
  
  /**
   * Convenient function where the final state of the filter is not given.
   */
  template<typename NumeratorFilterCoeff, typename DenominatorFilterCoeff, typename VectorType, typename OtherDerived>
  VectorType filter(const NumeratorFilterCoeff& b, const DenominatorFilterCoeff& a, const VectorType& X, const OtherDerived& si)
  {
    OtherDerived sf = si;
    return filter(b,a,X,si,sf);
  }
  
  /**
   * Convenient function where the initial and final states are set to 0
   */
  template<typename NumeratorFilterCoeff, typename DenominatorFilterCoeff, typename VectorType>
  VectorType filter(const NumeratorFilterCoeff& b, const DenominatorFilterCoeff& a, const VectorType& X)
  {
    typedef typename VectorType::Scalar Scalar;
    Eigen::Matrix<Scalar, Eigen::Dynamic, 1> si = Eigen::Matrix<Scalar, Eigen::Dynamic, 1>::Zero(std::max(b.rows(), a.rows())-1);
    return filter(b,a,X,si);
  };
};
#endif // __btkEigenFilter_h