/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include <math.h>
#include <stdlib.h>
#include "Open3DMotion/Maths/LinearSolve3.h"

#ifndef OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN
extern "C"
{
#include <f2clibs/f2c.h>
#include <clapack.h>
}
#else
#include <Eigen/Dense>
#endif

namespace Open3DMotion
{

	// Linear alegebra helper function: calls LAPACK routine
	// Solves linear least squares in 3D
	bool LinearSolve3(float* x, const float* A, const float* b, int numrows, float* rms /*=NULL*/, float rcond /*=0.001*/)
	{	
#ifndef OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN

		int i, j;

		// transpose as required for lapack
		float* AT = new float[3*numrows];
		for (i = 0; i < 3; i++)
			for (j = 0; j < numrows; j++)
				AT[i*numrows+j] = A[3*j + i];

		// copy input vector
		float* y = new float[numrows];
		for (i = 0; i < numrows; i++)
			y[i] = b[i];

		// params required for lapack
		long mrows = numrows;
		long ncols = 3;
		long nrhs = 1;
		long lda = numrows;
		long ldb = numrows;
		long jpvt[3] = { 0, 0, 0 };
		long rank = 0;
		long info = 0;

		// call lapack function to query work size
		long lwork_query = -1;
		float opt_work_size = 1.0f;
		sgelsy_(&mrows, &ncols, &nrhs, AT, &lda, y, &ldb, jpvt, &rcond, &rank, &opt_work_size, &lwork_query, &info);

		// return if any probs with params
		if (info != 0)
			return false;

		// get opt work size and call for real
		long lwork_opt = (long)opt_work_size;
		float* work = new float[lwork_opt];
		sgelsy_(&mrows, &ncols, &nrhs, AT, &lda, y, &ldb, jpvt, &rcond, &rank, work, &lwork_opt, &info);

		// copy result
		for (i = 0; i < 3; i++)
			x[i] = y[i];

		// done with temp data
		delete [] work;
		delete [] AT;
		delete [] y;

		// must have params ok (info = 0) and full rank (3)
		if (info != 0 || rank != 3)
			return false;

		// if requested, compute RMS
		if (rms)
		{
			float sumsq(0.0f);
			for (i = 0; i < numrows; i++)
			{
				float ax(0.0f);
				for (j = 0; j < 3; j++)
					ax += A[3*i+j] * x[j];
				float diff = ax - b[i];
				sumsq += diff*diff;
			}
			*rms = sqrt(sumsq / numrows);
		}

		return true;

#else

		// Map to Eigen objects
		Eigen::Map< const Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor> > C(A, numrows, 3);
    Eigen::Map< const Eigen::Matrix<float, Eigen::Dynamic, 1> > d(b, numrows, 1);
    Eigen::Map< Eigen::Matrix<float, 3, 1> > y(x, 3, 1);

		// compute CTC ( = ATA )
		Eigen::MatrixXf CT = C.transpose();
		Eigen::Matrix3f CTC = CT * C;

		// perform check on reciprocal condition number	to mimic LAPACK behaviour
		Eigen::Vector3f e = Eigen::EigenSolver<Eigen::Matrix3f> (CTC).eigenvalues().real();
		float CTC_rcond = e.minCoeff() / e.maxCoeff();
		if (CTC_rcond < rcond)
			return false;		

		// compute CTd = ATb
		Eigen::Vector3f CTd = CT * d;

		// solve
#if EIGEN_VERSION_AT_LEAST(3,0,0)
		y = CTC.ldlt().solve(CTd);
#else
    CTC.ldlt().solve(CTd, &y);
#endif

		// compute rms error if requested
		if (rms)
			*rms = sqrt((C*y - d).squaredNorm() / numrows);

		return true;

#endif
	}

}
