#include "Open3DMotion/Biomechanics/Algorithms/MOSHFIT/RigidBodyShapeCollection.h"
#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"

namespace Open3DMotion
{
	RigidBodyShapeCollection::RigidBodyShapeCollection()
	{
	}

	UInt32 RigidBodyShapeCollection::CopyFromTimeSequences(const std::vector<const TimeSequence*>& input)
	{
		try
		{
			// clear any previous
			shape.clear();

			// copy if non-empty
			size_t num_markers = input.size();
			if (num_markers)
			{
				size_t num_frames = (size_t) input[0]->NumFrames();
				shape.resize( num_frames );
				for ( std::vector<const TimeSequence*>::const_iterator iter_input( input.begin() ); iter_input != input.end(); iter_input++)
				{
					if ((size_t)(*iter_input)->NumFrames() == num_frames)
					{
						std::vector<RigidBodyShape>::iterator iter_shape( shape.begin() );
						for (TSOccVector3ConstIter iter_marker(**iter_input); iter_marker.HasFrame(); iter_marker.Next(), iter_shape++)
						{
							iter_shape->AddMarker(iter_marker.Value(), iter_marker.Occluded() ? 0 : 1);
						}
					}
					else
					{
						return RigidBodyResult::timesequence_mismatch;
					}
				}
			}

			return RigidBodyResult::success;
		}
		catch (const NoSuchFieldException&)
		{
			return RigidBodyResult::timesequence_mismatch;
		}
	}

	void RigidBodyShapeCollection::DilateVisiblity(UInt32 dilation)
	{
		if (shape.size())
		{
			size_t num_markers = shape[0].NumMarkers();

			// forward dilation
			std::vector<size_t> dilate_count_fwd(num_markers, 0);
			for (std::vector<RigidBodyShape>::iterator iter_fwd( shape.begin() ); iter_fwd != shape.end(); iter_fwd++)
			{
				for (UInt8 imarker = 0; imarker < num_markers; imarker++)
				{
					size_t& c = dilate_count_fwd[imarker];
					if (iter_fwd->Marker(imarker).visible == 0)
					{
						c = dilation;
					}
					else if (c)
					{
						--c;
						iter_fwd->Marker(imarker).visible = 0;
					}
				}
			}

			// reverse dilation
			std::vector<size_t> dilate_count_rvs(num_markers, 0);
			for (std::vector<RigidBodyShape>::reverse_iterator iter_rvs( shape.rbegin() ); iter_rvs != shape.rend(); iter_rvs++)
			{
				for (UInt8 imarker = 0; imarker < num_markers; imarker++)
				{
					size_t& c = dilate_count_rvs[imarker];
					if (iter_rvs->Marker(imarker).visible == 0)
					{
						c = dilation;
					}
					else if (c)
					{
						--c;
						iter_rvs->Marker(imarker).visible = 0;
					}
				}
			}
		}
	}

	void RigidBodyShapeCollection::ComputeVisiblityCanon(RigidBodyVisibilityCanon& canon) const
	{
		std::vector< std::vector<RigidBodyShape>::const_iterator > canon_indices;
		for (std::vector<RigidBodyShape>::const_iterator iter_shape( shape.begin() ); iter_shape != shape.end(); iter_shape++)
		{
			bool isnotsubcode = true;
			for (std::vector< std::vector<RigidBodyShape>::const_iterator >::iterator iter_canon( canon_indices.begin() ); iter_canon != canon_indices.end(); iter_canon++)
			{
				if (*iter_canon != shape.end())
				{
					if (iter_shape->IsVisibilitySubsetOf( **iter_canon ))
					{
						isnotsubcode = false;
						break;
					}

					if (iter_shape->IsVisibilitySupersetOf( **iter_canon ))
					{
						*iter_canon = shape.end();
					}
				}
			}

			if (isnotsubcode)
			{
				canon_indices.push_back( iter_shape );
			}
		}

		canon.shape.clear();
		for (std::vector< std::vector<RigidBodyShape>::const_iterator >::iterator iter_canon( canon_indices.begin() ); iter_canon != canon_indices.end(); iter_canon++)
		{
			if (*iter_canon != shape.end())
			{
				canon.shape.push_back( **iter_canon );
			}
		}
	}

	UInt32 RigidBodyShapeCollection::ComputeMeanShape(RigidBodyShape& mean, double distribution_tolerance, double convergence_accuracy) const
	{
		// minimal set of visiblity codes
		RigidBodyVisibilityCanon canon;
		ComputeVisiblityCanon(canon);

		//  re-order codes to ensure maximal ones go first
		RigidBodyConnectedVisibilityCanon connected_canon;
		UInt32 canon_result = canon.ComputeConnectedVisibilityCanon(connected_canon, distribution_tolerance);
		
		// must be a complete covering of all points to work
		if (canon_result != RigidBodyResult::success)
			return canon_result;

		// initialise shape
		connected_canon.ComputeApproxMeanShape(mean);

		// number of points
		size_t num_markers = connected_canon.Shape(0).NumMarkers();

		// zero vector to init markers
		Vector3 zero(0.0);

		// transform every shape to this one and update mean
		// double previous_residual_rms = DBL_MAX;
		for (size_t opt_count = 0; opt_count < 100; opt_count++)
		{
			// transform every shape and sum
			std::vector<Vector3> point_sum(num_markers, zero);
			std::vector<size_t> point_count(num_markers, 0);
			// double dx2_sum = 0.0;
			// size_t dx2_count = 0;
			for (std::vector<RigidBodyShape>::const_iterator iter_shape( shape.begin() ); iter_shape != shape.end(); iter_shape++)
			{
				RigidTransform3 T;
				iter_shape->ComputeFitTo(T, mean);
				for (size_t marker_index = 0; marker_index < num_markers; marker_index++)
				{
					const RigidBodyMarker& x = iter_shape->Marker(marker_index);
					if (x.visible)
					{
						// transform point to be nearest to mean as we can
						Vector3 Tx;
						RigidTransform3::MulVec(Tx, T, x.position);
						point_sum[marker_index] += Tx;
						point_count[marker_index]++;

						// form sum residuals
						// Vector3 dx;
						// Vector3::Sub(dx, Tx, x.position);
						// dx2_sum += dx.Modulus2();
						// dx2_count++;
					}
				}
			}

			// update mean and compute max difference with previous
			double max_difference = 0.0;
			for (size_t marker_index = 0; marker_index < num_markers; marker_index++)
			{
				size_t n = point_count[marker_index];
				if (n)
				{
					// compute updated marker position
					Vector3& new_position = point_sum[marker_index];
					new_position /= n;

					// compute maximum coordinate difference with previous
					RigidBodyMarker& m = mean.Marker(marker_index);
					for (size_t dimension = 0; dimension < 3; dimension++)
					{
						double d = fabs(new_position[dimension] - m.position[dimension]);
						if (d > max_difference)
							max_difference = d;
					}

					// set new values
					m.position = new_position;
				}
			}

			// verify residuals as stopping criteria
			// double residual_rms = sqrt( dx2_sum / dx2_count );
			if (max_difference < convergence_accuracy)
			{
				// fprintf(stderr, "Iterations used: %u\n", opt_count);
				return RigidBodyResult::success;
			}
			// previous_residual_rms = residual_rms;
		}

		return RigidBodyResult::did_not_converge;
	}

  const size_t RigidBodyVisibilityCanon::invalid_index = SIZE_MAX;
  
	RigidBodyVisibilityCanon::RigidBodyVisibilityCanon()
	{
	}

	UInt32 RigidBodyVisibilityCanon::ComputeConnectedVisibilityCanon(RigidBodyConnectedVisibilityCanon& connected, double distribution_tolerance) const
	{
		// init to empty
		connected.shape.clear();

		// num codes in canon
		const size_t canon_size = shape.size();

		// must have some codes
		if (canon_size)
		{
			// initialise connectivity
			std::vector< size_t > canon_sequence(canon_size, invalid_index);

			// default start node is zero
			UInt32 init_canon_index = 0;

			// only the start node is on level 0
			canon_sequence[init_canon_index] = 0;
			connected.shape.push_back(shape[init_canon_index]);
			bool foundnewconnections = true;

			// find how many connect directly, how many connect 1-removed, etc
			for (UInt32 ilevel = 1; foundnewconnections; ilevel++)
			{
				foundnewconnections = false;
				for (UInt32 icode = 0; icode < canon_size; icode++)
				{
					if (canon_sequence[icode] == (ilevel-1))
					{
						for (UInt32 jcode = 0; jcode < canon_size; jcode++)
						{
							if (canon_sequence[jcode] == invalid_index)
							{
								// see if icode and jcode are connected
								//      - if so store the level they are on
								//      - and set foundnewconnections = true
							
								// codes overlap - store level and set foundnewconnections
								if (shape[icode].HasUniqueFitWith(shape[jcode], distribution_tolerance))
								{
									canon_sequence[jcode] = ilevel;
									connected.shape.push_back(shape[jcode]);
									foundnewconnections = true;
								}
							}
						}
					}
				}
			}
		}

		if (connected.shape.size() != shape.size()) 
			return RigidBodyResult::timesequence_mismatch;

		return RigidBodyResult::success;
	}

	RigidBodyConnectedVisibilityCanon::RigidBodyConnectedVisibilityCanon()
	{
	}

	void RigidBodyConnectedVisibilityCanon::ComputeApproxMeanShape(RigidBodyShape& mean) const
	{
		if (shape.size())
		{
			// iterator at start of list
			std::vector<RigidBodyShape>::const_iterator iter_shape( shape.begin() ); 
			
			// initialise mean to first shape
			mean = *iter_shape;

			// add all others
			while ((++iter_shape) != shape.end())
			{
				// fit this shape to mean
				RigidTransform3 T;
				UInt32 fit_result = iter_shape->ComputeFitTo(T, mean);
				
				// this if statement is just defensive code
				// - in fact the nature of the canon should guarantee sufficient points for a unique fit
				if (fit_result == RigidBodyResult::success)
				{
					// loop over markers
					for (size_t marker_index = 0; marker_index < mean.NumMarkers(); marker_index++)
					{
						const RigidBodyMarker& sample_marker = iter_shape->Marker(marker_index);
						RigidBodyMarker& mean_marker = mean.Marker(marker_index);

						if (!mean_marker.visible && sample_marker.visible)
						{
							// transform marker from this shape to mean
							Vector3 Tx;
							RigidTransform3::MulVec(Tx, T, sample_marker.position);

							// fill in mean
							mean_marker.position = Tx;
							mean_marker.visible = true;
						}
					}
				}
			}
		}
	}

	UInt32 RigidBodyShapeCollection::MOSHFIT(std::vector<TimeSequence*>& output, double rateHz, double distribution_tolerance, double convergence_accuracy, const BinMemFactory& memfactory /*=BinMemFactoryDefault()*/) const
	{
		// compute mean shape
		RigidBodyShape mean;
		UInt32 status = ComputeMeanShape(mean, distribution_tolerance, convergence_accuracy);
		if (status != RigidBodyResult::success)
			return status;

		// build output arrays and iterators
		std::vector<TSOccVector3Iter> outputiter;
		size_t num_shapes = NumShapes();
		size_t num_markers = shape[0].NumMarkers();
		TimeRange tr;
		tr.Frames = num_shapes;
		tr.Start = 0.0;
		tr.Rate = rateHz;
		size_t imarker; 
		for (imarker = 0; imarker < num_markers; imarker++)
		{
			TimeSequence* ts = TSFactoryOccValue(3).New(tr, memfactory);
			output.push_back(ts);
			outputiter.push_back( TSOccVector3Iter(*ts) );
		}

		// iterate over all shapes
		for (std::vector<RigidBodyShape>::const_iterator ishape( shape.begin() ); ishape != shape.end(); ishape++)
		{
			// compute fit of mean to this shape
			RigidTransform3 T;
			mean.ComputeFitTo(T, *ishape);

			// transform mean and use result
			for (imarker = 0; imarker < num_markers; imarker++)
			{
				RigidTransform3::MulVec(outputiter[imarker].Value(), T, mean.Marker(imarker).position);
				outputiter[imarker].Occluded() = 0;
				outputiter[imarker].Next();
			}
		}

		return RigidBodyResult::success;
	}
}
