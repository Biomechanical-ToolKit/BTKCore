/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Biomechanics/Algorithms/ForceMeasurements.h"
#include "Open3DMotion/Biomechanics/Trial/ForcePlate.h"

namespace Open3DMotion
{
	void ForceMeasurements::ApplyLinearCalibration(std::vector<double>& calanalog, const std::vector<double>& rawanalog) const
	{
		calanalog.resize(rawanalog.size());
		std::vector<double>::iterator iter_output(calanalog.begin());
		std::vector<double>::const_iterator iter_input(rawanalog.begin());
		std::vector<LinearCalibration>::const_iterator iter_linear(linear.begin());
		for ( ; iter_output != calanalog.end() && iter_linear != linear.end(); iter_output++, iter_linear++, iter_input++)
		{
			iter_linear->Apply(*iter_output, *iter_input);
		}
	}

	void ForceMeasurementTimeSequence::CurrentRawAnalog(std::vector<double>& rawanalog) const
	{
		rawanalog.resize(analog_iter.size());
		std::vector<TSScalarConstIter>::const_iterator iter_iter_analog( analog_iter.begin() );
		std::vector<double>::iterator iter_raw(rawanalog.begin());
		for ( ; iter_raw != rawanalog.end(); iter_iter_analog++, iter_raw++)
		{
			*iter_raw = iter_iter_analog->Value();
		}
	}

	void ForceMeasurementTimeSequence::Begin()
	{
		for (std::vector<TSScalarConstIter>::iterator iter_iter_analog( analog_iter.begin() ); iter_iter_analog != analog_iter.end(); iter_iter_analog++)
		{
			iter_iter_analog->SeekTo(0);
		}
	}

	void ForceMeasurementTimeSequence::NextFrame()
	{
		for (std::vector<TSScalarConstIter>::iterator iter_iter_analog( analog_iter.begin() ); iter_iter_analog != analog_iter.end(); iter_iter_analog++)
		{
			iter_iter_analog->Next();
		}
	}

	bool ForceMeasurementTimeSequence::HasFrame() const
	{
		return (analog_iter.size() && analog_iter[0].HasFrame());
	}

	bool ForceMeasurementTimeSequence::Set(const ForcePlate& plate, const std::vector<const TimeSequence*> analog_all)
	{
		try
		{
			for (size_t channel_count = 0; channel_count < plate.Channels.NumElements(); channel_count++)
			{
				for (std::vector<const TimeSequence*>::const_iterator ianalog( analog_all.begin() ); ianalog != analog_all.end(); ianalog++)
				{
					if ((*ianalog)->HardwareID.IsSet() && (plate.Channels[channel_count] == (*ianalog)->HardwareID.Value()))
					{
						// store frame count and rate info from first sequence in case needed during AutoDriftOffset
						if (analog_iter.size() == 0)
						{
							numframes = (size_t) (*ianalog)->NumFrames();
							rate = (*ianalog)->Rate;
						}
						else
						{
							// all frame counts should match
							if ((size_t)(*ianalog)->NumFrames() != numframes)
								return false;
						}

						// time sequence iterator for this plate channel
						analog_iter.push_back(TSScalarConstIter(**ianalog));

						// linear calibration for this plate channel
						LinearCalibration lincal;
						if ((*ianalog)->Scale.IsSet())
							lincal.scale = (*ianalog)->Scale;
						if ((*ianalog)->Offset.IsSet())
							lincal.offset = (*ianalog)->Offset;
						linear.push_back(lincal);

						break;
					}
				}
			}
		}
		catch (const NoSuchFieldException&)
		{
			// a sequence did not have a 1D value field as required
			return false;
		}

		// true if a hardware ID found for each plate channel
		return (linear.size() == plate.Channels.NumElements());
	}

  bool ForceMeasurementTimeSequence::AutoDriftOffset(
    size_t frame_start/*=0*/, size_t min_frames/*=1*/, double min_time/*=0.2*/, double limit_duration_portion/*=0.05*/)
  {
		// use input to determine num inputs
		size_t ninputs = analog_iter.size();

		// if min_frames specified as zero, don't do an offset
		if (min_frames == 0)
			return false;

		// must have same size of calibration output array
		if (linear.size() != ninputs)
			return false;

		// must have valid time range
		if (min_time < -1E-12)
			return false;

		// convert requested min time to frames value
		size_t min_time_in_frames = 1 + (size_t)(min_time*rate);

		// use whichever is greater of requested min time and requested min frames
		if (min_time_in_frames > min_frames)
			min_frames = min_time_in_frames;

		// limit to requested file portion
		if (limit_duration_portion > 0.0)
		{
			// express limit as number of frames
			size_t limit_frames = 1 + (size_t)(limit_duration_portion*analog_iter[0].NumFrames());

			// apply limit
			if (min_frames > limit_frames)
				min_frames = limit_frames;
		}

		// find end frame
    size_t frame_end = frame_start+min_frames-1;

		// recalculate offset from data
    for (size_t j = 0; j < ninputs; j++)
    {
      if (frame_end >= analog_iter[0].NumFrames())
          return false;

      double sum(0.0);
			double sum2(0.0);
			TSScalarConstIter& iter_input = analog_iter[j];

			// form sum(x) and sum(x^2)
			iter_input.SeekTo(0);
			for (size_t i = frame_start; i <= frame_end; i++, iter_input.Next())
			{
				const double& v = iter_input.Value();
				sum += v;
				sum2 += v*v;
			}
			
			// mean & s.d. calculation
			size_t frames_used = 1+frame_end-frame_start;
			double meanoffset = sum / frames_used;
			double meanoffset2 = sum2 / frames_used;
			double offsetvar = meanoffset2 - meanoffset*meanoffset;

			// set new values in array
			linear[j].offset = meanoffset;
			linear[j].offset_confidence = sqrt(offsetvar);
    }

		// reset iterators
		Begin();

		return true;
  }
}
