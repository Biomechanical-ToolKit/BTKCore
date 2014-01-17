/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_LINEAR_SOLVE_3_H_
#define _OPEN3DMOTION_LINEAR_SOLVE_3_H_

#include <stdlib.h>

namespace Open3DMotion
{
	// Linear alegebra helper function: calls LAPACK routine
	// Solves linear least squares in 3D
	// i.e. finds argmin(x) { || Ax - b ||^2 }
	// @param x The LS-fit result
	// @param A Matrix of dimension N x 3
	// @param b Vector of dimension N
	// @param rms If not NULL, will be filled with the RMS best fit value
	// @param rcond Condition value: used to determine if matrix is well-conditioned
	// @return true if well-conditioned and appropriate input data, false otherwise
	bool LinearSolve3(float* x, const float* A, const float* b, int numrows, float* rms = NULL, float rcond = 0.001);
}

#endif
