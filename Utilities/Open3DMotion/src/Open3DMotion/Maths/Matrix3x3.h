/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_MATRIX_3X3_H_
#define _OPEN3DMOTION_MATRIX_3X3_H_

#include "Open3DMotion/Maths/ValueN.h"
#include "Open3DMotion/Maths/MathsException.h"

namespace Open3DMotion
{
  // Fixed matrix of 3x3 double values
	class Matrix3x3 : public ValueN<double, 9>
  {
  public:
    // Construct uninitialised
    Matrix3x3()
    {
    }

    // Construct from array
    Matrix3x3(const double* arr)
    {
      x[0] = arr[0];
      x[1] = arr[1];
      x[2] = arr[2];
      x[3] = arr[3];
      x[4] = arr[4];
      x[5] = arr[5];
      x[6] = arr[6];
      x[7] = arr[7];
      x[8] = arr[8];
    }

    // Construct using single const for all elements
    Matrix3x3(double s)
    {
      x[0] = 
      x[1] =
      x[2] =
      x[3] =
      x[4] =
      x[5] =
      x[6] =
      x[7] =
      x[8] = s;
    }

    // Set to array
    Matrix3x3& operator=(const double* arr)
    {
      x[0] = arr[0];
      x[1] = arr[1];
      x[2] = arr[2];
      x[3] = arr[3];
      x[4] = arr[4];
      x[5] = arr[5];
      x[6] = arr[6];
      x[7] = arr[7];
      x[8] = arr[8];
      return *this;
    }

    // Set to single const
    Matrix3x3& operator=(double s)
    {
      x[0] = 
      x[1] =
      x[2] =
      x[3] =
      x[4] =
      x[5] =
      x[6] =
      x[7] =
      x[8] = s;
      return *this;
    }

    // retrieve element in row i, column j
    double& operator() (int i, int j)
    {
      CODAMOTION_MATHS_VERIFY(i >= 0);
      CODAMOTION_MATHS_VERIFY(j >= 0);
      CODAMOTION_MATHS_VERIFY(i < 3);
      CODAMOTION_MATHS_VERIFY(j < 3);
      return x[i*3+j];
    }

    // retrieve element in row i, column j (const)
    const double& operator() (int i, int j) const
    {
      CODAMOTION_MATHS_VERIFY(i >= 0);
      CODAMOTION_MATHS_VERIFY(j >= 0);
      CODAMOTION_MATHS_VERIFY(i < 3);
      CODAMOTION_MATHS_VERIFY(j < 3);
      return x[i*3+j];
    }

    // Set to identity matrix
    Matrix3x3& ID()
    {
      Matrix3x3::ID(x);
      return *this;
    }

    // Determinant
    double Det() const
    { return Matrix3x3::Det(x); }

    // Multiply by a constant scalar
    Matrix3x3& operator*=(double s)
    {
      x[0] *= s;
      x[1] *= s;
      x[2] *= s;
      x[3] *= s;
      x[4] *= s;
      x[5] *= s;
      x[6] *= s;
      x[7] *= s;
      x[8] *= s;
      return *this;
    }

  public:

    // Set A to identity
    static double* ID(double* A)
    {
      A[0] = 1.0;
      A[1] = 0.0;
      A[2] = 0.0;
      A[3] = 0.0;
      A[4] = 1.0;
      A[5] = 0.0;
      A[6] = 0.0;
      A[7] = 0.0;
      A[8] = 1.0;
      return A;
    }

    // Find determinant
    static double Det(const double* A)
    {
      return 
        A[0]*A[4]*A[8] - 
        A[0]*A[5]*A[7] + 
        A[1]*A[5]*A[6] - 
        A[1]*A[3]*A[8] +
        A[2]*A[3]*A[7] -
        A[2]*A[4]*A[6];
    }

    // Multiply A = B.C
    static double* Mul(double* A, const double* B, const double* C)
    {
      A[0] = B[0]*C[0] + B[1]*C[3] + B[2]*C[6];
      A[1] = B[0]*C[1] + B[1]*C[4] + B[2]*C[7];
      A[2] = B[0]*C[2] + B[1]*C[5] + B[2]*C[8];

      A[3] = B[3]*C[0] + B[4]*C[3] + B[5]*C[6];
      A[4] = B[3]*C[1] + B[4]*C[4] + B[5]*C[7];
      A[5] = B[3]*C[2] + B[4]*C[5] + B[5]*C[8];

      A[6] = B[6]*C[0] + B[7]*C[3] + B[8]*C[6];
      A[7] = B[6]*C[1] + B[7]*C[4] + B[8]*C[7];
      A[8] = B[6]*C[2] + B[7]*C[5] + B[8]*C[8];

      return A;
    }

    // Multiply A = transpose(B).C
    static double* TransMul(double* A, const double* B, const double* C)
    {
      A[0] = B[0]*C[0] + B[3]*C[3] + B[6]*C[6];
      A[1] = B[0]*C[1] + B[3]*C[4] + B[6]*C[7];
      A[2] = B[0]*C[2] + B[3]*C[5] + B[6]*C[8];

      A[3] = B[1]*C[0] + B[4]*C[3] + B[7]*C[6];
      A[4] = B[1]*C[1] + B[4]*C[4] + B[7]*C[7];
      A[5] = B[1]*C[2] + B[4]*C[5] + B[7]*C[8];

      A[6] = B[2]*C[0] + B[5]*C[3] + B[8]*C[6];
      A[7] = B[2]*C[1] + B[5]*C[4] + B[8]*C[7];
      A[8] = B[2]*C[2] + B[5]*C[5] + B[8]*C[8];

      return A;
    }

    // Multiply A = B.transpose(C)
    static double* MulTrans(double* A, const double* B, const double* C)
    {
      A[0] = B[0]*C[0] + B[1]*C[1] + B[2]*C[2];
      A[1] = B[0]*C[3] + B[1]*C[4] + B[2]*C[5];
      A[2] = B[0]*C[6] + B[1]*C[7] + B[2]*C[8];

      A[3] = B[3]*C[0] + B[4]*C[1] + B[5]*C[2];
      A[4] = B[3]*C[3] + B[4]*C[4] + B[5]*C[5];
      A[5] = B[3]*C[6] + B[4]*C[7] + B[5]*C[8];

      A[6] = B[6]*C[0] + B[7]*C[1] + B[8]*C[2];
      A[7] = B[6]*C[3] + B[7]*C[4] + B[8]*C[5];
      A[8] = B[6]*C[6] + B[7]*C[7] + B[8]*C[8];

      return A;
    }

    // Multiply A = transpose(B).transpose(C)
    static double* TransMulTrans(double* A, const double* B, const double* C)
    {
      A[0] = B[0]*C[0] + B[3]*C[1] + B[6]*C[2];
      A[1] = B[0]*C[3] + B[3]*C[4] + B[6]*C[5];
      A[2] = B[0]*C[6] + B[3]*C[7] + B[6]*C[8];

      A[3] = B[1]*C[0] + B[4]*C[1] + B[7]*C[2];
      A[4] = B[1]*C[3] + B[4]*C[4] + B[7]*C[5];
      A[5] = B[1]*C[6] + B[4]*C[7] + B[7]*C[8];

      A[6] = B[2]*C[0] + B[5]*C[1] + B[8]*C[2];
      A[7] = B[2]*C[3] + B[5]*C[4] + B[8]*C[5];
      A[8] = B[2]*C[6] + B[5]*C[7] + B[8]*C[8];

      return A;
    }

    // Multiply y = R.x where x is Vector3
    static double* MulVec(double* y, const double* R, const double* x)
    {
      y[0] = R[0]*x[0] + R[1]*x[1] + R[2]*x[2];
      y[1] = R[3]*x[0] + R[4]*x[1] + R[5]*x[2];
      y[2] = R[6]*x[0] + R[7]*x[1] + R[8]*x[2];
      return y;
    }

    // Multiply y = transpose(R).x where x is Vector3
    static double* TransMulVec(double* y, const double* R, const double* x)
    {
      y[0] = R[0]*x[0] + R[3]*x[1] + R[6]*x[2];
      y[1] = R[1]*x[0] + R[4]*x[1] + R[7]*x[2];
      y[2] = R[2]*x[0] + R[5]*x[1] + R[8]*x[2];
      return y;
    }

    // Singular value decomposition U.diag(s).transpose(V) = A
    static void SVD(double* U, double* s, double* VT, const double* A);

		// combine B and C in ratio A = (1-s)*B + s*C
		static void Ratio(double* A, double s, const double* B, const double* C)
		{
			double ns = 1.0 - s;
			A[0] = ns*B[0] + s*C[0];
			A[1] = ns*B[1] + s*C[1];
			A[2] = ns*B[2] + s*C[2];
			A[3] = ns*B[3] + s*C[3];
			A[4] = ns*B[4] + s*C[4];
			A[5] = ns*B[5] + s*C[5];
			A[6] = ns*B[6] + s*C[6];
			A[7] = ns*B[7] + s*C[7];
			A[8] = ns*B[8] + s*C[8];
		}
  };
}
#endif
