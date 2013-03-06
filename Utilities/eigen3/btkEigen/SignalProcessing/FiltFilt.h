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

#ifndef __btkEigenFiltFilt_h
#define __btkEigenFiltFilt_h

#include "Filter.h"

#include <Eigen/LU>

#define BTKEIGEN_FILFILT_REVERSE_INPLACE(m) \
  { \
    int dlen =  m.rows() * m.cols(); \
    for (int k = 0 ; k < dlen / 2 ; ++k) \
    { \
      Scalar s = m.data()[k]; \
      m.data()[k] = m.data()[dlen-k-1]; \
      m.data()[dlen-k-1] = s; \
    } \
  }

namespace btkEigen
{
  using namespace Eigen;
  
  /**
   * A forward-backward digital filter without phase delay (zero phase distorsion). 
   * Compared to a simple forward filter, the order of this filter is twice of the original order and the cutoff frequency is reduced. 
   * To have a more stable filter, the intial state of the filter is computed using the method proposed by Gustafsson (1996).
   *
   * Inspired from the filtfilt function provided in SciPy.
   *
   * @par References
   * Gustafsson, F.@n
   * <em>Determining the Initial States in Forward-Backward Filtering</em>@n
   * IEEE transactions on signal processing, <b>1996</b>, 44 (4), 988-992
   */
  template<typename NumeratorFilterCoeff, typename DenominatorFilterCoeff, typename MatrixType>
  MatrixType filtfilt(const NumeratorFilterCoeff& b, const DenominatorFilterCoeff& a, const MatrixType& X)
  {
    typedef typename MatrixType::Scalar Scalar;
    typedef Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> FFMatrix;
    typedef Eigen::Matrix<Scalar, Eigen::Dynamic, 1> FFVector;
  
    const int slen = X.rows();
    const int order = std::max(b.rows(), a.rows());
    const int elen = 3 * (order - 1); // Number of element used in the reflections
    
    eigen_assert((order > 1) && "The order of the filter must be greater than 1.");
    eigen_assert((slen > elen) && "The signal to filter must have a length 3 times greater than the order of the filter.");
  
    // Copy the coefficients and pad them with zeros 
    BTKEIGEN_FILTER_PAD_COEFFICIENTS(MatrixType,bb,b,order)
    BTKEIGEN_FILTER_PAD_COEFFICIENTS(MatrixType,aa,a,order)
    
    // Compute the initial state of the filter
    FFVector zi;
    if (order == 2)
    {
      zi.resize(1,1);
      zi.coeffRef(0) = (1.0 + aa.coeff(1)) / (bb.coeff(1) - bb.coeff(0)*aa.coeff(1));
    }
    else
    {
      FFMatrix temp(order-1,order-2);
      temp.block(0,0,order-2,order-2) = -FFMatrix::Identity(order-2,order-2);
      temp.block(order-2,0,1,order-2) = FFMatrix::Zero(1,order-2);
      FFMatrix temp1(order-1,order-1);
      temp1 << aa.block(1,0,order-1,1), temp;
      temp1 += FFMatrix::Identity(order-1,order-1);
      FFMatrix temp2 =  bb.block(1,0,order-1,1) - (bb.coeff(0) * aa.block(1,0,order-1,1));
      zi = temp1.lu().solve(temp2);
    }
    
    MatrixType Y = X;
    for (int i = 0 ; i < Y.cols() ; ++i)
    {
      // Reflection at the beginning
      FFVector pre = -1.0 * Y.block(1,i,elen,1);
      BTKEIGEN_FILFILT_REVERSE_INPLACE(pre)
      pre.array() += 2.0 * Y.coeff(0,i);
      // Reflection at the end
      FFVector post = -1.0 * Y.block(Y.rows()-elen-1,i,elen,1);
      BTKEIGEN_FILFILT_REVERSE_INPLACE(post)
      post.array() += 2.0 * Y.coeff(Y.rows()-1,i);
      // Signal to filter
      FFVector y(Y.rows() + 2*elen,1);
      y << pre, Y.col(i), post;
      // Forward filter
      FFVector zif = zi*y.coeff(0);
      y = filter(bb,aa,y,zif);
      // Backward filter
      BTKEIGEN_FILFILT_REVERSE_INPLACE(y)
      FFVector zib = zi*y.coeff(0);
      y = filter(bb,aa,y,zib);
      // Final filtered signal
      BTKEIGEN_FILFILT_REVERSE_INPLACE(y)
      Y.col(i) = y.block(elen,0,Y.rows(),1);
    }
    
    return Y;
  };
};
#endif // __btkEigenFiltFilt_h