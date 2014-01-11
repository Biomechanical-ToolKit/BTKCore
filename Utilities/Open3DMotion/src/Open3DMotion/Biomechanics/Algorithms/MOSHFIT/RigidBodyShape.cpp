/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Biomechanics/Algorithms/MOSHFIT/RigidBodyShape.h"

#ifndef OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN
extern "C"
{
#include <f2clibs/f2c.h>
#include <clapack.h>
}
#else
#include <Eigen/Dense>
#include <Eigen/SVD>
#endif

namespace Open3DMotion
{  
  const UInt32 RigidBodyResult::success = 0;
  const UInt32 RigidBodyResult::timesequence_mismatch = 1;
  const UInt32 RigidBodyResult::visibility_disconnected = 2;
  const UInt32 RigidBodyResult::insufficient_points = 3;
  const UInt32 RigidBodyResult::did_not_converge = 4;
  
	RigidBodyShape::RigidBodyShape()
	{
	}
		
	RigidBodyShape::RigidBodyShape(const RigidBodyShape& src)
	{
		*this = src;
	}

	const RigidBodyShape& RigidBodyShape::operator=(const RigidBodyShape& src)
	{
		marker.assign( src.marker.begin(), src.marker.end() );
		return *this;
	}

	RigidBodyShape::~RigidBodyShape()
	{
	}

	void RigidBodyShape::AddMarker(const double* position, bool visible)
	{
		marker.resize(NumMarkers()+1);
		marker.back().position = position;
		marker.back().visible = visible ? 1 : 0;
	}

	bool RigidBodyShape::IsVisibilitySupersetOf(const RigidBodyShape& other) const
	{
		if (marker.size() == other.marker.size())
		{
			for (std::vector<RigidBodyMarker>::const_iterator iter_this( marker.begin() ), iter_other( other.marker.begin() );
					 iter_this != marker.end(); iter_this++, iter_other++)
			{
				if (iter_other->visible && !iter_this->visible)
					return false;
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	bool RigidBodyShape::IsVisibilitySubsetOf(const RigidBodyShape& other) const
	{
		if (marker.size() == other.marker.size())
		{
			for (std::vector<RigidBodyMarker>::const_iterator iter_this( marker.begin() ), iter_other( other.marker.begin() );
					 iter_this != marker.end(); iter_this++, iter_other++)
			{
				if (iter_this->visible && !iter_other->visible)
					return false;
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	size_t RigidBodyShape::NumberOfVisibleMarkersInCommonWith(const RigidBodyShape& other) const
	{
		size_t count = 0;
		if (marker.size() == other.marker.size())
		{
			for (std::vector<RigidBodyMarker>::const_iterator iter_this( marker.begin() ), iter_other( other.marker.begin() );
					 iter_this != marker.end(); iter_this++, iter_other++)
			{
				if (iter_this->visible && iter_other->visible)
					++count;
			}
		}
		return count;
	}

	void RigidBodyShape::EvaluateNonsingularity3D(std::vector<double>& s, std::vector<double>& coords)
	{
		long num_points(coords.size() / 3);
    s.resize(3);
    
#ifndef OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN
    long three(3);
		long lwork(256);
		double work[256];
		long info(0);
				
		std::vector<double> U(9);
		std::vector<double> VT(num_points*num_points);

		// use lapack routine
		// note coords must be column-major so first 3 elements correspond to first coord
		dgesvd_(
			"N",  // don't actually need U
			"N",  // don't actually need VT
			&three, // rows
			&num_points,      // cols
			&coords[0],   // input/output matrix
			&three, // leading dimension of Acpy
			&s[0],      // singular values
			&U[0],      // left orthonormal matrix
			&three, // leading dimension of left
			&VT[0],      // right orthonormal matrix
			&num_points, // leading dimension of right 
			work,   // workspace
			&lwork, // size of workspace
			&info);   // returned error codes
#else    
		Eigen::Map< Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor> >
      _coords(&coords[0], (int)num_points, 3);
    Eigen::Map< Eigen::Matrix<double, 3, 1> > _s(&s[0], 3, 1);
    Eigen::JacobiSVD< Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor> > svd(_coords);
    _s = svd.singularValues();
#endif // OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN
	}

  
	bool RigidBodyShape::HasUniqueFitWith(const RigidBodyShape& other, double tolerance) const
	{
		if (marker.size() == other.marker.size())
		{
			size_t num_in_common(0);
			Vector3 common_centroid(0.0);
			std::vector<double> common_coords;
			common_coords.reserve(3*NumMarkers());
			for (std::vector<RigidBodyMarker>::const_iterator iter_this( marker.begin() ), iter_other( other.marker.begin() );
					 iter_this != marker.end(); iter_this++, iter_other++)
			{
				if (iter_this->visible && iter_other->visible)
				{
					const Vector3& x = iter_this->position;
					for (size_t j = 0; j < 3; j++)
						common_coords.push_back(x[j]);
					num_in_common++;
					common_centroid += x;
				}
			}

			if (num_in_common >= 3)
			{
				// subract centroid
				common_centroid /= num_in_common;
				double* x = &common_coords[0];
				for (size_t j = 0; j < num_in_common; j++, x+=3)
				{
					Vector3::Sub(x, x, common_centroid);
				}

				// find singular values to estimate how non-colinear these points are
				std::vector<double> s;
				RigidBodyShape::EvaluateNonsingularity3D(s, common_coords);

				// Simplified expression to test points sufficiently non-colinear
				// (condition is sufficient but not always necessary)
				double min_colinearity_allowed = 3.56*sqrt((double)num_in_common)*tolerance;
				if (s[1] > min_colinearity_allowed)
					return true;
			}
		}

		return false;
	}

	UInt32 RigidBodyShape::ComputeFitTo(RigidTransform3& T, const RigidBodyShape& base) const
	{
		size_t num_points = NumMarkers();
		if (num_points == base.NumMarkers())
		{
			// get centroids
			Vector3 centroidA(0,0,0);
			Vector3 centroidB(0,0,0);
			const RigidBodyMarker* iter_A = &base.marker[0];
			const RigidBodyMarker* iter_B = &marker[0];
			size_t ipoint(0);
			size_t num_in_common(0);
			for (ipoint = 0; ipoint < num_points; ipoint++, iter_A++,iter_B++)
			{
				if (iter_A->visible && iter_B->visible)
				{
					Vector3::Add(centroidA.x, centroidA.x, iter_A->position);
					Vector3::Add(centroidB.x, centroidB.x, iter_B->position);
					++num_in_common;
				}
			}

			if (num_in_common >= 3)
			{
				// normalise
				centroidA /= num_in_common;
				centroidB /= num_in_common;

				// correlation matrix
				double sum_correl[9] =
				{ 0.0, 0.0, 0.0,
					0.0, 0.0, 0.0,
					0.0, 0.0, 0.0 
				};

				// find correlation matrix
				iter_A = &base.marker[0];
				iter_B = &marker[0];
				for (ipoint = 0; ipoint < num_points; ipoint++, iter_A++, iter_B++)
				{
					if (iter_A->visible && iter_B->visible)
					{
						// input minus centroid
						Vector3 normA;
						Vector3::Sub(normA, iter_A->position, centroidA);

						// cal minus centroid
						Vector3 normB;
						Vector3::Sub(normB, iter_B->position, centroidB);

						// correlation (outer product)
						for (int i = 0; i < 3; i++)
							for (int j = 0; j < 3; j++)
							sum_correl[3*i+j] += normA[i]*normB[j];
					}
				}

				// SVD of correlation to get rotation
				// from mean coords to this frame
				double s[3];
				double U[9], VT[9];
				Matrix3x3::SVD(U, s, VT, sum_correl);

				// force right-handed coord system
				double detU = Matrix3x3::Det(U);
				double detV = Matrix3x3::Det(VT);
				if (detU*detV < 0.0)
				{
					VT[6] *= -1.0;
					VT[7] *= -1.0;
					VT[8] *= -1.0;
				}

				// this is the correct way round
				Matrix3x3::Mul(T.R, U, VT);

				// do post-subtraction of centroid B to get translation vector
				double RcentroidB[3];
				Matrix3x3::MulVec(RcentroidB, T.R, centroidB);
				Vector3::Sub(T.t, centroidA, RcentroidB);

		#if 0
				iter_A = ptA;
				iter_B = ptB;
				for (ipoint = 0; ipoint < npoints; ipoint++, iter_A+=3, iter_B+=3)
				{
					Vector3 Tb;
					RigidTransform3::MulVec(Tb, R, t, iter_B);
					cerr << "x: " << iter_A[0] << " " << Tb[0] << endl;
					cerr << "y: " << iter_A[1] << " " << Tb[1] << endl;
					cerr << "z: " << iter_A[2] << " " << Tb[2] << endl;
					Vector3::Sub(Tb, Tb, iter_A);
					cerr << "Mod: " << Tb.Modulus() << endl;
				}
		#endif

				return RigidBodyResult::success;
			}
		}

		return RigidBodyResult::insufficient_points;
	}

}
