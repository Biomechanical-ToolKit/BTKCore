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

#ifndef __btkEigenDenseBaseAddons_h
#define __btkEigenDenseBaseAddons_h

/// \internal helper struct to form the return type of the percentile
template<typename OtherDerived> struct precentile_return_type {
  typedef typename internal::scalar_product_traits<typename internal::traits<Derived>::Scalar,typename internal::traits<OtherDerived>::Scalar>::ReturnType Scalar;
  typedef Array<typename DenseBase<OtherDerived>::Scalar, DenseBase<OtherDerived>::RowsAtCompileTime, DenseBase<OtherDerived>::ColsAtCompileTime> type;
};

/// \internal Macro used to compute the median in the percentile method
#define EIGEN_INTERNAL_MEDIAN_COMPUTATION(out) \
  Index idx = num / 2; \
  double out = sorted.coeff(idx); \
  if (num % 2 == 0) \
    out = (sorted.coeff(idx-1) + out) / 2.0;

/**
 * Compute the percentile of the colum/row vector data for the given percentage(s).
 * The method is similar to the one proposed by Matlab/Scilab/Octave but not NumPy
 * The general method to compute the perecentiles uses the linear interpolation between closest ranks [1] for percentages between 100*(0.5/N) and 100*((N-0.5)/N) (where N is the number of elements in the data).
 * There are also specific case:
 *  - when the number of element in the data is equal to 1;
 *  - when the percentage(s) is(are) lower than 100*(0.5/N);
 *  - when the percentage(s) is(are) greater than 100*((N-0.5)/N);
 *  - when the percentage(s) is(are) equal to 50 (median).
 * [1] http://en.wikipedia.org/wiki/Percentile#Linear_interpolation_between_closest_ranks
 * @return A vector with the same dimension than the input @a other.
 * @note The percentage(s) must be between 0.0 and 100.0
 * @warning This method use a temporary to internally sort the data.
 */
template<typename OtherDerived>
typename precentile_return_type<OtherDerived>::type
percentile(const DenseBase<OtherDerived>& other) const
{
  typedef typename DenseBase<Derived>::Scalar Scalar;
  typedef typename DenseBase<Derived>::Index Index;
  typedef typename DenseBase<OtherDerived>::Scalar OtherScalar;
  
  EIGEN_STATIC_ASSERT_VECTOR_ONLY(Derived)
  EIGEN_STATIC_ASSERT_VECTOR_ONLY(OtherDerived)
  eigen_assert((other.minCoeff() >= Scalar(0)) && (other.maxCoeff() <= Scalar(100)) && "Error in the given percentageThe given percentage(s). The value must be between 0.0 and 100.0");
  
  Index num = std::max(this->rows(), this->cols());
  
  // Special case: Only one element
  if (num == 1)
  {
    return precentile_return_type<OtherDerived>::type::Constant(other.rows(), other.cols(), this->coeff(0));
  }
  else
  {
    const Scalar snum = Scalar(num);
    const Scalar min_ = 100.0 * 0.5 / snum;
    const Scalar max_ = 100.0 * (0.5 + snum - 1.0) / snum;
    Array<Scalar, DenseBase<Derived>::RowsAtCompileTime, DenseBase<Derived>::ColsAtCompileTime> sorted = this->derived();
    _btk_eigen_quicksort<Scalar,Index>(sorted.data(), 0, num-1);
    // Special case: 50% (median)
    if (other.isConstant(Scalar(50)))
    {
      EIGEN_INTERNAL_MEDIAN_COMPUTATION(m);
      return precentile_return_type<OtherDerived>::type::Constant(other.rows(), other.cols(), m);
    }
    // Special case: 0% and before the next increment (included)
    else if ((other.derived().array() <= min_).all())
    {
      return precentile_return_type<OtherDerived>::type::Constant(other.rows(), other.cols(), sorted.coeff(0));
    }
    // Special case: 100% and before the previous increment (included)
    else if ((other.derived().array() >= max_).all())
    {
      return precentile_return_type<OtherDerived>::type::Constant(other.rows(), other.cols(), sorted.coeff(num-1));
    }
    else
    // General case using a linear interpolation between closest ranks
    {
      // NOTE: A method without a loop should be better... But is it possible?
      typename precentile_return_type<OtherDerived>::type p(other.rows(), other.cols());
      Index num_other = std::max(other.rows(), other.cols());
      for (Index i = 0 ; i < num_other ; ++i)
      {
        // Median
        if (fabs(other.coeff(i) - Scalar(50)) < NumTraits<OtherScalar>::dummy_precision())
        {
          EIGEN_INTERNAL_MEDIAN_COMPUTATION(m);
          p.coeffRef(i) = m;
        }
        // 0% and before the next increment (included)
        else if (other.coeff(i) <= min_)
        {
          p.coeffRef(i) = sorted.coeff(0);
        }
        // 100% and before the previous increment (included)
        else if (other.coeff(i) >= max_)
        {
          p.coeffRef(i) = sorted.coeff(num-1);
        }
        else
        {
          Scalar idx = other.coeff(i) / Scalar(100) * snum - 0.5;
          Scalar idx_ = floor(idx < 0.0 ? 0.0 : idx);
          Scalar v = sorted.coeff(Index(idx_));
          if (fabs(idx - idx_) >= NumTraits<OtherScalar>::dummy_precision())
          {
            Scalar pidx_ = 100.0 * (0.5 + idx_) / snum;
            v += snum / 100.0 * (other.coeff(i) - pidx_) * (sorted.coeff(Index(idx_)+1) - v);
          }
          p.coeffRef(i) = v;
        }
      }
      return p;
    }
  }
  // NOTE: THE NEXT RETURN SHOULD NEVER BE USED!
  eigen_assert(0);
  return typename precentile_return_type<OtherDerived>::type();
};

/**
 * Overload method to compute the percentile for only one given percentage
 */
EIGEN_STRONG_INLINE typename internal::traits<Derived>::Scalar
percentile(typename DenseBase<Derived>::RealScalar p) const
{
  typedef DenseBase<Derived>::Scalar Scalar;
  return this->percentile(Eigen::Array<Scalar,1,1>::Constant(p)).coeff(0);
};

/**
 * Overload method to compute the percentile for only two given percentages
 * @return A row/colum vector depending of the size of the current data.
 */
EIGEN_STRONG_INLINE typename DenseBase<Derived>::template precentile_return_type< Array<typename DenseBase<Derived>::Scalar, (DenseBase<Derived>::RowsAtCompileTime == 1 ? 1 : 2), (DenseBase<Derived>::ColsAtCompileTime == 1 ? 1 : 2)> >::type
percentile(typename DenseBase<Derived>::RealScalar p1, typename DenseBase<Derived>::RealScalar p2) const
{
  typedef DenseBase<Derived>::Scalar Scalar;
  return this->percentile(Eigen::Array< Scalar,(DenseBase<Derived>::RowsAtCompileTime == 1 ? 1 : 2),(DenseBase<Derived>::ColsAtCompileTime == 1 ? 1 : 2)>(p1,p2));
};

/**
 * Overload method to compute the percentile for only three given percentages
 * @return A row/colum vector depending of the size of the current data.
 */
EIGEN_STRONG_INLINE typename DenseBase<Derived>::template precentile_return_type< Array<typename DenseBase<Derived>::Scalar, (DenseBase<Derived>::RowsAtCompileTime == 1 ? 1 : 3), (DenseBase<Derived>::ColsAtCompileTime == 1 ? 1 : 3)> >::type
percentile(typename DenseBase<Derived>::RealScalar p1, typename DenseBase<Derived>::RealScalar p2, typename DenseBase<Derived>::RealScalar p3) const
{
  typedef DenseBase<Derived>::Scalar Scalar;
  return this->percentile(Eigen::Array< Scalar,(DenseBase<Derived>::RowsAtCompileTime == 1 ? 1 : 3), (DenseBase<Derived>::ColsAtCompileTime == 1 ? 1 : 3)>(p1,p2,p3));
};

/**
 * Compute the median for the column/row vector data.
 * @note This is a conveniant method which replace the call of the method percentile(50.0).
 */
EIGEN_STRONG_INLINE typename internal::traits<Derived>::Scalar
median() const
{
  return this->percentile(50.0);
};

/**
 * Compute the standard deviation for the given column/row vector.
 * NOTE: (Extract from NumPy documentation)
 * "In standard statistical practice, 'ddof=1' provides an unbiased estimator 
 *  of the variance of the infinite population. 'ddof=0' provides a maximum 
 *  likelihood estimate of the variance for normally distributed variables. The
 *  standard deviation computed in this function is the square root of
 *  the estimated variance, so even with 'ddof=1', it will not be an
 *  unbiased estimate of the standard deviation per se"
 */
EIGEN_STRONG_INLINE typename internal::traits<Derived>::Scalar
std(typename DenseBase<Derived>::RealScalar ddof = DenseBase<Derived>::RealScalar(1)) const
{
  EIGEN_STATIC_ASSERT_VECTOR_ONLY(Derived)
  Index num = std::max(this->rows(), this->cols());
  if (num == 1)
    return DenseBase<Derived>::Scalar(0);
  return std::sqrt((this->derived().array() - this->mean()).cwiseAbs2().sum() / (static_cast<double>(num) - ddof));
};

#endif // __btkEigenDenseBaseAddons_h