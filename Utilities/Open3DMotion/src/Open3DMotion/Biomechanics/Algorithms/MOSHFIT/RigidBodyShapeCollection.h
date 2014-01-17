/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_RIGID_BODY_SHAPE_COLLECTION_H_
#define _OPEN3DMOTION_RIGID_BODY_SHAPE_COLLECTION_H_

#include "Open3DMotion/OpenORM/Types.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"
#include "Open3DMotion/Biomechanics/Algorithms/MOSHFIT/RigidBodyShape.h"
#include <limits.h>

namespace Open3DMotion
{
	// forward decl
	class BinMemFactory;
	class TimeSequence;
	class RigidBodyVisibilityCanon;
	class RigidBodyConnectedVisibilityCanon;

	/** Collection of shapes of 3D points with visiblity information. */
	class RigidBodyShapeCollection
	{
	public:
		
		/** Construct empty collection. */
		RigidBodyShapeCollection();

		/** Use multiple time sequences of point data and add one shape for every frame.

				@return  RigidBodyResult::success if successful, 
				         RigidBodyResult::timesequence_mismatch if sequences contain
								 different numbers of frames or are not point data. 
		*/
		UInt32 CopyFromTimeSequences(const std::vector<const TimeSequence*>& input);

		/** @return Number of shapes */
		size_t NumShapes() const
		{ return shape.size(); }

		/** Retrieve a shape
				@param index zero-based index of shape.
				@return The shape at the given index.
			*/
		const RigidBodyShape& Shape(size_t index) const
		{ return shape[index]; }

		/** Determine a collection of shapes which have canonical codes as defined by the MOSHFIT algorithm.
				@param canon To be filled with the resulting visibility codes.
			*/
		void ComputeVisiblityCanon(RigidBodyVisibilityCanon& canon) const;

		/** Apply the portion of the MOSHFIT algorithm which computes mean shape.
				@param distribution_tolerance Required tolerance on accuracy of input data.
				@param convergence_accuracy Numerical precision to which conversion is required.
				@return RigidBodyResult::success if successful, error code otherwise.
		 */
		UInt32 ComputeMeanShape(RigidBodyShape& mean, double distribution_tolerance, double convergence_accuracy) const;

		/** Apply the MOSHFIT algorithm to compute mean shape and fit it to each shape in the collection.
			  @param output Collection of time seequences for reconstructed outputs.
				@param rateHz The sample rate to specify in output time sequences.
				@param distribution_tolerance Required tolerance on accuracy of input data.
				@param convergence_accuracy Numerical precision to which conversion is required.
				@param memfactory Optional selection of alternative memory allocation factory for output.
				@return RigidBodyResult::success if successful, error code otherwise.
			*/
		UInt32 MOSHFIT(std::vector<TimeSequence*>& output, double rateHz, double distribution_tolerance, double convergence_accuracy, const BinMemFactory& memfactory = BinMemFactoryDefault()) const;

		/** Utility method to increase duration of occluded regions 
		    @param dilation The number of frames to expand occluded region. */
		void DilateVisiblity(UInt32 dilation);

	private:
		RigidBodyShapeCollection(const RigidBodyShapeCollection& src);
		const RigidBodyShapeCollection& operator=(const RigidBodyShapeCollection& src);

	protected:
		std::vector<RigidBodyShape> shape;
	};

	/** Collection of 3D shapes corresponding to canonical visiblity codes. */
	class RigidBodyVisibilityCanon : public RigidBodyShapeCollection
	{
	public:
		/** Construct empty.  Use RigidBodyShapeCollection::ComputeVisiblityCanon to construct non-empty version. */
		RigidBodyVisibilityCanon();

		/** Compute the set of codes which are shape-connected and order them according to MOSHFIT.
				@param connected Filled with the resulting set of codes.
				@param distribution_tolerance Required tolerance on accuracy of input data.
			*/
		UInt32 ComputeConnectedVisibilityCanon(RigidBodyConnectedVisibilityCanon& connected, double distribution_tolerance) const;

	private:
		RigidBodyVisibilityCanon(const RigidBodyVisibilityCanon& src);
		const RigidBodyVisibilityCanon& operator=(const RigidBodyVisibilityCanon& src);
	
	private:
		static const size_t invalid_index /*=SIZE_MAX*/;
	};

	/** Ordered collection of shape-connected 3D shapes as required by the MOSHFIT algorithm. */
	class RigidBodyConnectedVisibilityCanon : public RigidBodyVisibilityCanon
	{
	public:
		/** Construct empty.  Use RigidBodyVisibilityCanon::ComputeConnectedVisibilityCanon to construct non-empty version. */
		RigidBodyConnectedVisibilityCanon();

		/** Use adapted generalised procrustes to compute unique mean shape.
			  @param mean Filled with resulting shape. 
			*/
		void ComputeApproxMeanShape(RigidBodyShape& mean) const;

	private:
		RigidBodyConnectedVisibilityCanon(const RigidBodyConnectedVisibilityCanon& src);
		const RigidBodyConnectedVisibilityCanon& operator=(const RigidBodyConnectedVisibilityCanon& src);
	};
}

#endif
