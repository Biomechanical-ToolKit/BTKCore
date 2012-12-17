/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_LINEAR_CALIBRATION_H_
#define _OPEN3DMOTION_LINEAR_CALIBRATION_H_

#include <math.h>

namespace Open3DMotion
{
	// A linear transform to be applied to a 1-dimensional channel of data.
	// Allows confidence bounds on the offset value.
	class LinearCalibration
	{
	public:
		
		// Linear scale
		double scale;

		// Offset value
		double offset;
		
		// Confidence limits (standard deviation) on confidence value
		double offset_confidence;

	public:

		// Construct with scale = 1, offset = 0, offset standard deviation (confidence) = 0
		LinearCalibration() :
				scale(1.0),
				offset(0.0),
				offset_confidence(0.0)
		{
		}

		// Construct with given scale and offset, zero standard deviation
		LinearCalibration(double nscale, double noffset) :
				scale(nscale),
				offset(noffset),
				offset_confidence(0.0)
		{
		}

		// Construct with given scale, offset, and offset standard deviation
		LinearCalibration(double nscale, double noffset, double noffset_confidence) :
				scale(nscale),
				offset(noffset),
				offset_confidence(noffset_confidence)
		{
		}

		void Apply(double& a, const double& b) const
		{ a = scale * (b - offset); 	}

		void ApplyInverse(double& a, const double& b) const
		{ a = (b / scale) + offset; 	}

		void Apply(double& a, double& a_confidence, const double& b, const double& b_confidence) const
		{ 
			Apply(a, b);
			a_confidence = scale * sqrt( (b_confidence*b_confidence + offset_confidence*offset_confidence) );
		}

		void ApplyInverse(double& a, double& a_confidence, const double& b, const double& b_confidence) const
		{ 
			ApplyInverse(a, b);
			double s_confidence = b_confidence / scale;
			a_confidence = sqrt(s_confidence*s_confidence - offset_confidence*offset_confidence);
		}

	};

}

#endif