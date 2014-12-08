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

#ifndef __btkEigenCumtrapz_h
#define __btkEigenCumtrapz_h

#include <Eigen/Dense>

namespace btkEigen
{
  using namespace Eigen;
  
  /**
   * Cumulative sum of elements.
   */
  template<typename Derived, typename OtherDerived>
  EIGEN_STRONG_INLINE void cumsum(DenseBase<OtherDerived> const & out, const DenseBase<Derived>& in)
  {
    DenseBase<OtherDerived>& out_ = const_cast< DenseBase<OtherDerived>& >(out);
    out_ = in;
    for (int i = 0 ; i < out_.cols() ; ++i)
    {
      for (int j = 1 ; j < out_.rows() ; ++j)
      {
        out_.coeffRef(j,i) += out_.coeff(j-1,i);
      }
    }
  };
  
  /**
   * Overload method to return result by value.
   */
  template<typename Derived>
  EIGEN_STRONG_INLINE Matrix<typename DenseBase<Derived>::Scalar, DenseBase<Derived>::RowsAtCompileTime, DenseBase<Derived>::ColsAtCompileTime>
  cumsum(const DenseBase<Derived>& y)
  {
    Matrix<typename DenseBase<Derived>::Scalar, DenseBase<Derived>::RowsAtCompileTime, DenseBase<Derived>::ColsAtCompileTime> z;
    cumsum(z,y);
    return z;
  };
 
  /*
   * Cumulative trapezoidal numerical integration with unit spacing.
   */
  template<typename Derived, typename OtherDerived>
  EIGEN_STRONG_INLINE void cumtrapz(DenseBase<OtherDerived> const & out, const DenseBase<Derived>& in)
  {
    DenseBase<OtherDerived>& out_ = const_cast< DenseBase<OtherDerived>& >(out);
    out_.derived().setZero(in.rows(), in.cols());
    cumsum(out_.middleRows(1,out_.rows()-1), (in.topRows(in.rows()-1) + in.bottomRows(in.rows()-1)) * 0.5);
  };
  
  /**
   * Overload method to return result by value.
   */
  template<typename Derived>
  EIGEN_STRONG_INLINE Matrix<typename DenseBase<Derived>::Scalar, DenseBase<Derived>::RowsAtCompileTime, DenseBase<Derived>::ColsAtCompileTime>
  cumtrapz(const DenseBase<Derived>& y)
  {
    Matrix<typename DenseBase<Derived>::Scalar, DenseBase<Derived>::RowsAtCompileTime, DenseBase<Derived>::ColsAtCompileTime> z;
    cumtrapz(z,y);
    return z;
  };
};


#endif // __btkEigenCumtrapz_h