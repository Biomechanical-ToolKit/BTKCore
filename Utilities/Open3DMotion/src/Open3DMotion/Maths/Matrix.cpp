/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Maths/Matrix.h"

extern "C"
{
#include <f2clibs/f2c.h>
#include <clapack.h>
}

namespace Open3DMotion
{
  void Matrix::Transpose(double* AT, const double* A, int rows, int cols)
  {
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
        AT[rows*j+i] = A[cols*i+j];
  }

  void Matrix::ATA(double* ATA, const double* A, int rows, int cols)
  {
    for (int i = 0; i < cols; i++)
    {
      for (int j = 0; j < cols; j++, ATA++)
      {
        *ATA = 0.0;
        const double* Ai = A + i;
        const double* Aj = A + j;
        for (int k = 0; k < rows; k++, Ai += cols, Aj += cols)
          *ATA += *Ai * *Aj;
      }
    }
  }

  void Matrix::SymmSolve(double* A, double* b, int n)
  {
    long ln(n);
    long one(1);
    long* ipiv = new long[n];
    long info(0);
    dgesv_(&ln, &one, A, &ln, ipiv, b, &ln, &info);
    delete ipiv;
  }

  void Matrix::SymmInverse(double* Ainv, double* A, int n)
  {
    long ln = n;
    long ln2 = n*n;
    memset(Ainv, 0, ln2*sizeof(double));
    for (long i = 0; i < ln2; i+=(n+1))
      Ainv[i] = 1.0;
    long* ipiv = new long[ln];
    long info(0);
    dgesv_(&ln, &ln, A, &ln, ipiv, Ainv, &ln, &info);
    delete ipiv;
  }

#ifndef OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN
  void Matrix::SVD(double* U, double* s, double* VT, const double* A, long rows, long cols)
  {
    Matrix Acpy(rows,cols);
    memcpy((double*)Acpy, A, rows*cols*sizeof(double));
    long lwork(1024);
    double work[1024];
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
      &cols, // rows
      &rows, // cols
      Acpy,   // input/output matrix
      &cols, // leading dimension of Acpy
      s,      // singular values
      VT,      // left orthonormal matrix
      &cols, // leading dimension of left
      U,      // right orthonormal matrix
      &rows, // leading dimension of right 
      work,   // workspace
      &lwork, // size of workspace
      &info);   // returned error codes
  }

  // Least squares | A.x - b |^2
  void Matrix::LS(double* A, double* b, long rows, long cols) throw(MathsException)
  {
    CODAMOTION_MATHS_VERIFY(rows >= cols); // overdetermined only
    long one(1);
    double worksize[2] = { 0.0, 0.0 };
    long lwork(-1);
    long info(0);

    Matrix AT(cols,rows);
    Matrix::Transpose(AT,A,rows,cols);

    // workspace query
    dgels_("N", &rows, &cols, &one, AT, &rows, b, &rows, worksize, &lwork, &info);
    
    // make work array
    lwork = (long)worksize[0] + 64;
    double* work = new double[lwork];

    // do calculation
    dgels_("N", &rows, &cols, &one, AT, &rows, b, &rows, work, &lwork, &info);

    // finish with array
    delete work;
  }

  // Constrained least squares | A.x - c |^2, B.x = d
  void Matrix::LS(double* x, double* A, double* c, double* B, double* d, long rowsA, long colsAandB, long rowsB)  throw(MathsException)
  {
    CODAMOTION_MATHS_VERIFY(rowsA >= colsAandB); // overdetermined only

    Matrix AT(colsAandB,rowsA);
    Matrix::Transpose(AT, A, rowsA, colsAandB);

    Matrix BT(colsAandB,rowsB);
    Matrix::Transpose(BT, B, rowsB, colsAandB);

    // workspace query
    double worksize[2] = { 0, 0 };
    long lwork(-1);
    long info(0);
    dgglse_(&rowsA, &colsAandB, &rowsB, AT, &rowsA, BT, &rowsB, c, d, x, worksize, &lwork, &info);
    
    // make work array
    lwork = (long)worksize[1] + 64;
    double* work = new double[lwork];

    // do calculation
    dgglse_(&rowsA, &colsAandB, &rowsB, AT, &rowsA, BT, &rowsB, c, d, x, work, &lwork, &info);

    // finish with array
    delete [] work;
  }
#endif
}
