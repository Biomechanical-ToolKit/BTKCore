/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_FORCE_CALC_AMTI_H_
#define _OPEN3DMOTION_FORCE_CALC_AMTI_H_

#include "Open3DMotion/Biomechanics/Algorithms/ForceCalculator.h"
#include "Open3DMotion/Maths/Vector3.h"

namespace Open3DMotion
{
  // Force calculation: analog -> F & pt of application
  // for AMTI plates
  class ForceCalc_AMTI : public ForceCalculator
  {
	public:
		
		static void ApplyScaling(std::vector<double>& corrected_analog, const MapArrayFloat64& calibration, const std::vector<double>& input_analog);

		static void ApplyMatrix(std::vector<double>& corrected_analog, const MapArrayFloat64& calibration, const std::vector<double>& input_analog);

	public:
   
		// Verify AMTI-specific model params
		virtual bool VerifyModel(const ForcePlate& model);

    // Either 6 inputs (when no calibration matrix or 6 x 1 calibration matrix)
		// Or num inputs equivalent to num calibration elements / 6
		virtual size_t NumInputs() const
    { return numinputs; }

    // Compute the force
    virtual void ComputeFromCalAnalog(Vector3& force, Vector3& point, double& freemoment_z, const std::vector<double>& calanalog) const;  

		void ComputeFromCalPlate(Vector3& force, Vector3& point, double& freemoment_z, const std::vector<double>& calanalog) const;

  protected:
		size_t numinputs;
  };
}

#endif