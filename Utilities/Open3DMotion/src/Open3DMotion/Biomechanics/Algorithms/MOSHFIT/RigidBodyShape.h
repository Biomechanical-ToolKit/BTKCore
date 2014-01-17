/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_RIGID_BODY_SHAPE_H_
#define _OPEN3DMOTION_RIGID_BODY_SHAPE_H_

#include "Open3DMotion/Maths/Vector3.h"
#include "Open3DMotion/Maths/RigidTransform3.h"
#include <vector>

class TestRigidBodyShape;

namespace Open3DMotion
{
	/** Constants for possible return values from shape computations and MOSHFIT algorithm. */
	class RigidBodyResult
	{
	public:

		/** Computation successful. */
		static const UInt32 success /*=0*/;

		/** Problem with input time sequences. */
		static const UInt32 timesequence_mismatch /*=1*/;

		/** Visiblity graph was not fully connected. */
		static const UInt32 visibility_disconnected /*=2*/;

		/** Not enough points were used as input. */
		static const UInt32 insufficient_points /*=3*/;
		
		/** Convergence failure. */
		static const UInt32 did_not_converge /*=4*/;
	};

	/** 3D point with associated visibility information. */
	class RigidBodyMarker
	{
	public:
		/** 3D location. */
		Vector3 position;

		/** Visibility flag (should be set to 1 if visible, 0 otherwise). */
		UInt8 visible;
	};

	/** Shape defined by 3D points, each with associated visibility information. */
	class RigidBodyShape
	{
		friend class ::TestRigidBodyShape;

	public:
		
		/** Construct with no points. */
		RigidBodyShape();
		
		/** Copy constructor.
				@param src Shape to copy.
		 */
		RigidBodyShape(const RigidBodyShape& src);

		/** Assignment operator.
				@param src Shape to copy.
			*/
		const RigidBodyShape& operator=(const RigidBodyShape& src);

		/** Destructor */
		~RigidBodyShape();

		/** Add a 3D point.
				@param position a 3-element array corresponding to 3D coordinate.
				@param visible set to true to flag as visible, false to flag as occluded. */
		void AddMarker(const double* position, bool visible);

		/** @return Number of points in this shape. */
		size_t NumMarkers() const
		{ return marker.size(); }

		/** Retrieve one point of shape (immutable).
				@param index Zero-based index of point.
				@return Point at specified index.
			*/
		const RigidBodyMarker& Marker(size_t index) const
		{ return marker[index]; }

		/** Retrieve one point of shape.
				@param index Zero-based index of point.
				@return Point at specified index.
			*/
		RigidBodyMarker& Marker(size_t index)
		{ return marker[index]; }

		/** Determine whether visibility flags are a superset of another point collection.
				@param other Shape to compare.
				@return true if a superset, false otherwise.
			*/
		bool IsVisibilitySupersetOf(const RigidBodyShape& other) const;

		/** Determine whether visibility flags are a subset of another point collection.
				@param other Shape to compare.
				@return true if a subset, false otherwise.
			*/
		bool IsVisibilitySubsetOf(const RigidBodyShape& other) const;

		/** Count the number of visible points in common with another shape.
				@param other Shape to compare.
				@return Number of points in common. */
		size_t NumberOfVisibleMarkersInCommonWith(const RigidBodyShape& other) const;

		/** Determine whether there is a unique Procrustean fit to another shape, according to MOSHFIT criteria.
				@param other Shape to compare.
				@param distribution_tolerance Required tolerance on accuracy of input data.
				@return true if a unique fit is guaranteed by MOSHFIT critera, false otherwise.
			*/
		bool HasUniqueFitWith(const RigidBodyShape& other, double tolerance) const;

		/** Compute the Procrustes fit using visible points.
				@param T to be filled with the computed transformation from this shape to base shape.
				@param base Base shape to fit.
				@return RigidBodyResult::success if successful, error code otherwise.
			*/
		UInt32 ComputeFitTo(RigidTransform3& T, const RigidBodyShape& base) const;

	protected:
		static void EvaluateNonsingularity3D(std::vector<double>& s, std::vector<double>& coords);

	protected:
		std::vector<RigidBodyMarker> marker;
	};

}

#endif
