/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_MATRIX_H_
#define _OPEN3DMOTION_MATRIX_H_

#include "Open3DMotion/OpenORM/Types.h"
#include "Open3DMotion/Maths/MathsException.h"
#include <math.h>
#include <memory.h>

namespace Open3DMotion
{
  // generic matrix
  class Matrix
  {
  public:
    // construct empty (null) matrix
    Matrix() :
      x(0), m(0), n(0)
    {
    }

    // construct with m rows and n columns
    Matrix(int _m, int _n) :
      x(new double[_m*_n]), m(_m), n(_n)
    {
    }

    // construct with m rows and n columns
    // initialised to given value
    Matrix(int _m, int _n, double init) :
      x(new double[_m*_n]), m(_m), n(_n)
    {
      *this = init;
    }

		// copy constructor
		Matrix(const Matrix& src) :
      x(0), m(0), n(0)
		{
			// use overridden assignment operator
			*this = src;
		}

		// destructor
		~Matrix()
		{
			if (x)
				delete x;
		}

  public:

    // retrieve element in row i, column j
    double& operator() (int i, int j) throw(MathsException)
    {
      CODAMOTION_MATHS_VERIFY(i >= 0);
      CODAMOTION_MATHS_VERIFY(j >= 0);
      CODAMOTION_MATHS_VERIFY(i < m);
      CODAMOTION_MATHS_VERIFY(j < n);
      return x[i*n+j];
    }

    // retrieve element in row i, column j (const)
    const double& operator() (int i, int j) const throw(MathsException)
    {
      CODAMOTION_MATHS_VERIFY(i >= 0);
      CODAMOTION_MATHS_VERIFY(j >= 0);
      CODAMOTION_MATHS_VERIFY(i < m);
      CODAMOTION_MATHS_VERIFY(j < n);
      return x[i*n+j];
    }

    // cast to array
    operator double * ()
    { return x; }

    // cast to array (const)
    operator const double * () const
    { return x; }

    // rows
    int M() const
    { return m; }

    // columns
    int N() const
    { return n; }

    // set identity (square matrices only)
    Matrix& ID() throw(MathsException)
    {
      CODAMOTION_MATHS_VERIFY(m == n);
      if (m != n)
        return *this;
      memset(x, 0, n*n*sizeof(double));
      for (int j = 0; j < n; j++)
        (*this)(j,j) = 1.0;
      return *this;
    }

    // set all values the same
    Matrix& operator=(double s)
    {
      int size = m*n;
      for (int i = 0; i < size; i++)
        x[i] = s;
      return *this;
    }

    // assignment: resize and copy source matrix
    Matrix& operator=(const Matrix& src)
    {
			// delete if exists
			if (x)
			{
				delete x;
				x = NULL;
				m = 0;
				n = 0;
			}

			// copy if source is non-empty
			if (src.x)
			{
				m = src.m;
				n = src.n;
				x = new double[m*n];
				memcpy(x, src.x, m*n*sizeof(double));
			}

			return *this;
    }

		// set to transpose(A).A
    void ATA(const Matrix& A) throw(MathsException)
    {
      if (x)
        delete x;
      CODAMOTION_MATHS_VERIFY(A.M() != 0);
      CODAMOTION_MATHS_VERIFY(A.N() != 0);
      x = new double[A.N()*A.N()];
      m = n = A.N();
      ATA(x, A.x, m, n);
    }

    // set to transpose(A)
    void Transpose(const Matrix& A) throw(MathsException)
    {
      if (x)
        delete x;
      CODAMOTION_MATHS_VERIFY(A.M() != 0);
      CODAMOTION_MATHS_VERIFY(A.N() != 0);
      x = new double[A.N()*A.M()];
      m = A.N();
      n = A.M();
      Transpose(x, A.x, m, n);
    }


    // transpose
    static void Transpose(double* AT, const double* A, int rows, int cols);
 
    // transpose(A).A
    static void ATA(double* ATA, const double* A, int rows, int cols);

    // Solve symmetric system
    static void SymmSolve(double* A, double* b, int n);

    // Inverse of symmetric matrix
    static void SymmInverse(double* Ainv, double* A, int n);

    // Singular value decomposition U.diag(s).VT = A
    static void SVD(double* U, double* s, double* VT, const double* A, long rows, long cols);

    // Least squares | A.x - b |^2
    static void LS(double* A, double* b, long rows, long cols)  throw(MathsException);

    // Constrained least squares | A.x - c |^2, B.x = d
    static void LS(double* x, double* A, double* c, double* B, double* d, long rowsA, long colsAandB, long rowsB)  throw(MathsException);

  protected:
    double* x;
    int m;
    int n;
  };
}
#endif