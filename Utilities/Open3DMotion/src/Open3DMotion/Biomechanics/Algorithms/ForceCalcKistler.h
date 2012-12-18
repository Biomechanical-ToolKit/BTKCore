/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_FORCE_CALC_KISTLER_H_
#define _OPEN3DMOTION_FORCE_CALC_KISTLER_H_

#include "Open3DMotion/Biomechanics/Algorithms/ForceCalculator.h"
#include "Open3DMotion/Maths/Vector3.h"

namespace Open3DMotion
{
  class ForceCalc_Kistler : public ForceCalculator
  {
  public:

		// Verify Kistler-specific model params
		virtual bool VerifyModel(const ForcePlate& model);

    // 8 Inputs
		virtual size_t NumInputs() const
    { return 8; }

    // Compute the force
    virtual void ComputeFromCalAnalog(Vector3& force, Vector3& point, double& freemoment_z, const std::vector<double>& calanalog) const;
		
  };
}

#endif
