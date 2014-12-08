/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2014, Arnaud Bdataé
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

#ifndef __btkEigenForwardDeclarations_h
#define __btkEigenForwardDeclarations_h

template <typename ResultType> struct btk_eigen_vectorop_member_median;

template <typename ResultType> struct btk_eigen_vectorop_member_std;

// ------------------------------------------------------------------------- //
//                         EXTRAS USED BY THE PLUGINS
// ------------------------------------------------------------------------- //

// Based on the snippets provided on algolist.net: http://www.algolist.net/Algorithms/Sorting/Quicksort
template <typename Scalar, typename Index>
void _btk_eigen_quicksort(Scalar* data, Index left, Index right)
{
  Index i = left, j = right;
  Scalar tmp(0);
  Scalar pivot = data[(left + right) / 2];
  /* partition */
  while (i <= j)
  {
    while (data[i] < pivot)
      ++i;
    while (data[j] > pivot)
      --j;
    if (i <= j)
    {
      tmp = data[i];
      data[i] = data[j];
      data[j] = tmp;
      ++i;
      --j;
    }
  };
  /* recursion */
  if (left < j)
    _btk_eigen_quicksort(data, left, j);
  if (i < right)
    _btk_eigen_quicksort(data, i, right);
};

#endif // __btkEigenForwardDeclarations_h