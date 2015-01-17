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

#ifndef __btkEigenNM_h
#define __btkEigenNM_h

#include <Eigen/Core>

// Adapted Bubble sort
#define BTKEIGEN_NM_SORT() \
  for (int i = 0 ; i < n+1 ; ++i) \
    idx[i] = i; \
  bool swapped = true; \
  while (swapped) \
  { \
    swapped = false; \
    for (int i = 0; i < n; ++i) \
    { \
      if (f[idx[i]] > f[idx[i+1]]) \
      { \
        int tmp = idx[i]; \
        idx[i] = idx[i+1]; \
        idx[i+1] = tmp; \
        swapped = true; \
      } \
    } \
  } \
  iHigh = idx[n]; \
  iNextHigh = idx[n-1]; \
  iLow = idx[0];
  
#define BTKEIGEN_NM_ACCEPT(x_,f_) \
  { \
    V.row(iHigh) = x_; \
    f[iHigh] = f_; \
  }
  
#define BTKEIGEN_NM_SHRINK() \
  { \
    for (int i = 0 ; i < n+1 ; ++i) \
    { \
      if (i == iLow) \
        continue; \
      V.row(i) = V.row(i) * sigma + V.row(iLow) * (1 - sigma); \
      f[i] = (*func)(V.row(i), data); \
    } \
  }

namespace btkEigen
{
  using namespace Eigen;
  
  /**
   * @brief Computes the unconstrained minimimum of given function with the Nelder-Mead (NM) algorithm
   *
   * The downhill simplex algorithm (also named Nelder-Mead and Amoeba) performs an optimization search
   * to find the minimum of a function with mutli-independent variable.
   * The method only requires function evaluations, it doesn't use derivatives.
   *
   * @param func Function pointer used for the objective function. The function must return a single value.
   * @param x Vector used as input and ouput.
   * @param data Optional input containing extra parameter for the objective function.
   * @param fTol Tolerance for the function values used in the stop condition criteria (default 1.0e-4).
   * @param xTol Tolerance for the vertices values used in the stop condition criteria (default 1.0e-4).
   * @param maxIter Maximum number of iteration (default: 250).
   *
   * This implementation doesn't (yet) take into account the restart of the algorithm 
   * if one of the following condition is not reached:
   *  - O'Neill factorial test ;
   *  - Kelley's stagnation condition. 
   *
   * The initial simplex is computed using the method of L. Pfeffer (Baudin, 2010 ; Fan, 2002).
   *
   * The stopping conition criteria is the same than used by Matlab and Scilab: "-tolssizedeltafvmethod".
   * 
   * For further information, see:
   * Nelder, J.A.; Mead, R.@n
   * <em>A simplex Method for Function Minimization</em>@n
   * Computer Journal, <b>1965</b> 7, 308-313. 
   * @n
   * Baudin, M.@n
   * <em>Nelder-Mead User's Manual</em>@n
   * <b>2010</b>
   * @n
   * Fan, E.@n
   * <em>Global Optimization of Lennard-Jones Atomic Clusters</em>@n
   * M.Sc thesis, McMaster University, <b>2002</b>.
   */
  template<typename ResultType>
  bool NM(typename ResultType::Scalar (*func)(const ResultType& , void* ), 
          ResultType* x, void* data = NULL,
          double fTol = 1.0e-4, // precision<typename ResultType::Scalar>(), // Should be used for the other stop condition
          double xTol = 1.0e-4,
          int maxIter = 250)
  {
    typedef typename ResultType::Scalar Scalar;
    typedef typename Eigen::Matrix<Scalar, Eigen::Dynamic, 1> NMVectorX;
    typedef typename Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> NMMatrixX;
    
    // Check the dimensions of the input/output
    // The variable 'x' must be a row vector.
    eigen_assert(x->rows() == 1);
    const int n = x->cols();
    
    // Constant coefficient used in the algorithm
    // Values used here are standard values found in the litterature.
    const double rho = 1.0;   // reflection (rho > 0)
    const double chi = 2.0;   // expansion (chi > rho)
    const double gamma = 0.5; // contraction (0 < gam < 1)
    const double sigma = 0.5; // shrinkage (0 < sig < 1)
    
    // Initialize the simplex (n+1,n) where 'n' is the number of parameters to optimize
    // Intial simplex based on Pfefferís method. Several other methods exist... which one is the best.
    const double usualDelta = 0.05;
    const double zeroTermDelta = 0.00025; // Most common value used compared to 0.0075 as proposed in Fan (2002). Maybe this value is adapted for float type instead of double?
    NMMatrixX V(n+1,n);
    V.row(0) = *x;
    for (int i = 1 ; i < n+1 ; ++i)
    {
      V.row(i) = *x;
      if (V.coeff(i,i-1) == 0.0)
        V.coeffRef(i,i-1) = zeroTermDelta;
      else
        V.coeffRef(i,i-1) = (1.0 + usualDelta) * V.coeff(i,i-1);
    }
      
    // Compute the value for each vertex in the simplex using the given function
    NMVectorX vf(n+1,1);
    for (int i = 0 ; i < n+1 ; ++i)
      vf.coeffRef(i) = (*func)(V.row(i), data);
    double* f = vf.data();
    
    int iHigh, iNextHigh, iLow;
    // Used to sort the vertices.
    Eigen::Matrix<int, Eigen::Dynamic, 1> vidx = Eigen::Matrix<int, Eigen::Dynamic, 1>(n+1);
    int* idx = vidx.data();
    // Used for the stop condition criteria
    Eigen::Matrix<double, Eigen::Dynamic, 1> ones = Eigen::Matrix<double, Eigen::Dynamic, 1>::Ones(n+1);
    
    int iter = 0;
    while (iter < maxIter)
    {
      // Extract the highest (worst vertex), next highest and lowest (best vertex) values.
      // (i.e. sort f from the lowest value to the highest. Adapt v in consequence)
      BTKEIGEN_NM_SORT()
      
      // Check the close condition
#if 1
      // Same condition than in Matlab and Scilab: "tolssizedeltafvmethod"
      // Based on the size of the simplex and the difference of function value in the simplex
      if (((vf.array() - f[iLow]).abs().maxCoeff() <= fTol) && ((V - (ones * V.row(iLow))).array().abs().maxCoeff() <= xTol))
        break;
#else
      if (fabs(f[iHigh]-f[iLow]) < fTol)
        break;
#endif
        
      // Compute the centroid where the worst vertex is excluded
      ResultType c = ResultType::Zero();
      for (int i = 0 ; i < n+1 ; ++i)
      {
        if (i == iHigh)
          continue;
        c += V.row(i);
      }
      c /= static_cast<double>(n);
      // Compute the reflection point with respect to the worst vertex
      ResultType xr = (1 + rho) * c - rho * V.row(iHigh);
      Scalar fr = (*func)(xr, data);
      
      // Check the possibilities to replace the worst vertex by a better one:
      // - Try to expand the point
      if (fr < f[iLow])
      {
        ResultType xe = (1 + rho * chi) * c - rho * chi * V.row(iHigh);
        Scalar fe = (*func)(xe, data);
        if (fe < fr)
          BTKEIGEN_NM_ACCEPT(xe,fe)
        else
          BTKEIGEN_NM_ACCEPT(xr,fr)
      }
      // - Accept the point
      else if ((fr >= f[iLow]) && (fr < f[iNextHigh]))
        BTKEIGEN_NM_ACCEPT(xr,fr)
      // - Outside contraction
      else if ((fr >= f[iNextHigh]) && (fr < f[iHigh]))
      {
        ResultType xc = (1 + rho * gamma) * c - rho * gamma * V.row(iHigh);
        Scalar fc = (*func)(xc, data);
        if (fc < fr)
          BTKEIGEN_NM_ACCEPT(xc,fc)
        else
          BTKEIGEN_NM_SHRINK()
      }
      // - Inside contraction
      else
      {
        ResultType xc = (1 - gamma) * c + gamma * V.row(iHigh);
        Scalar fc = (*func)(xc, data);
        if (fc < f[iHigh])
          BTKEIGEN_NM_ACCEPT(xc,fc)
        else
          BTKEIGEN_NM_SHRINK()
      }
      ++iter;
    }
    
    *x = V.row(iLow);
    
    if (iter >= maxIter)
      return false;
      
    return true;
  };
};

#endif // __btkEigenNM_h
