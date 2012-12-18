/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Maths/Matrix3x3.h"

#ifndef OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN
extern "C"
{
#include <f2clibs/f2c.h>
#include <clapack.h>
}
#endif

namespace Open3DMotion
{
#ifndef OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN
	void Matrix3x3::SVD(double* U, double* s, double* VT, const double* A)
  {
    long three(3);
    Matrix3x3 Acpy(A);
    long lwork(256);
    double work[256];
    long info(0);

    // use lapack routine
    // - note U and VT are swapped
    // - this is because of the fortran column-major
    //   ordering for matrices - it turns out that
    //   using a row major matrix here corresponds to
    //   swapping U and VT
    dgesvd_(
      "A",  // all of U
      "A",  // all of VT
      &three, // rows
      &three, // cols
      Acpy,   // input/output matrix
      &three, // leading dimension of Acpy
      s,      // singular values
      VT,      // left orthonormal matrix
      &three, // leading dimension of left
      U,      // right orthonormal matrix
      &three, // leading dimension of right 
      work,   // workspace
      &lwork, // size of workspace
      &info);   // returned error codes
  }
#endif
}

