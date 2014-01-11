/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_FORCE_CHANNEL_CALIBRATION_H_
#define _OPEN3DMOTION_FORCE_CHANNEL_CALIBRATION_H_

#include <vector>
#include "Open3DMotion/OpenORM/Types.h"
#include "Open3DMotion/Biomechanics/Algorithms/LinearCalibration.h"
#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"

namespace Open3DMotion
{
	class ForcePlate;
	class TimeSequence;
	class ForceCalculator;
	class BinMemHandler;

	/** Base class to hold linear calibration information for force plate channels and apply it */
	class ForceMeasurements
	{
	public:
    virtual ~ForceMeasurements()
    {
    }

		size_t NumChannels() const
		{ return linear.size(); }

		virtual void CurrentRawAnalog(std::vector<double>& rawanalog) const = 0;

		void ApplyLinearCalibration(std::vector<double>& calanalog, const std::vector<double>& rawanalog) const;

	protected:
		std::vector<LinearCalibration> linear;
	};

	class ForceMeasurementTimeSequence : public ForceMeasurements
	{
	public:

		/** Initialise linear calibration using time sequences.
				@param plate Force plate structure.
				@param analog_all Available analog sequences from a trial.
				@return true if suitable analog time sequences available for this plate, false otherwise. */
		bool Set(const ForcePlate& plate, const std::vector<const TimeSequence*> analog_all);

		/** Estimate drift using a portion of the given data (assumed to have no force on plate) */
		bool AutoDriftOffset(
			size_t frame_start = 0, 
			size_t min_frames = 1, 
			double min_time = 0.2,
			double limit_duration_portion = 0.05);

		/** Implementation to get current frame from iterators */
		void CurrentRawAnalog(std::vector<double>& rawanalog) const;

		/** Go to beginning of iterators */
		void Begin();

		/** Increment all iterators */
		void NextFrame();

		/** Check iterators have next */
		bool HasFrame() const;

		/** Sample rate of analog in Hz (assumed the same across all channels) */
		double Rate() const
		{ return rate; }

		/** Number of frames */
		size_t NumFrames() const
		{ return numframes; }

	protected:
		std::vector<TSScalarConstIter> analog_iter;
		size_t numframes;
		double rate;
	};
}

#endif