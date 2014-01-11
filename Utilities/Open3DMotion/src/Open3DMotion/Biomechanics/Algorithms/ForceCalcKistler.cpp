/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Biomechanics/Algorithms/ForceCalcKistler.h"
#include "Open3DMotion/Biomechanics/Trial/ForcePlate.h"
#include <float.h>
#include <math.h>

namespace Open3DMotion
{
  bool ForceCalc_Kistler::VerifyModel(const ForcePlate& model)
	{
		if (model.SensorSeparation.IsSet())
		{
			// use these offsets
		}
		else
		{
			// must have offsets
			return false;
		}

		// expect 8 x 1 matrix for analog scale factor
		if (model.Calibration.NumElements() == 0)
		{
			// zero elements means no additional scale factors to apply
		}
		else if (model.Calibration.NumElements() == 8)
		{
			// one-element per input means use these
		}
		else
		{
			// otherwise don't know
			return false;
		}

  	// copy calibration
		if (model.COPOptimisation.NumElements() == 0)
		{
			// no COP correction
		}
		else if (model.COPOptimisation.NumElements() == 12)
    {
			// use COP correction
    }
		else
		{
			// don't know how to use this
			return false;
		}

		// otherwise all ok
		return true;
  }

  void ForceCalc_Kistler::ComputeFromCalAnalog(Vector3& force, Vector3& point, double& freemoment_z, const std::vector<double>& calanalog) const
  {
		// rescale if additional scale factors provided
		double scaled_analog[8];
		if (Model().Calibration.NumElements() == 8)
		{
			for (int i = 0; i < 8; i++)
				scaled_analog[i] = calanalog[i] * Model().Calibration[i];
		}
		else
		{
			for (int i = 0; i < 8; i++)
				scaled_analog[i] = calanalog[i];
		}

    // force in fp co-ords
    force[0] = scaled_analog[0]+scaled_analog[1];
    force[1] = scaled_analog[2]+scaled_analog[3];
    force[2] = scaled_analog[4]+scaled_analog[5]+scaled_analog[6]+scaled_analog[7];

    // moments in fp co-ords
		double Mx = Model().SensorSeparation.X*( scaled_analog[4] + scaled_analog[5] - scaled_analog[6] - scaled_analog[7]);
		double My = Model().SensorSeparation.Y*(-scaled_analog[4] + scaled_analog[5] + scaled_analog[6] - scaled_analog[7]);
		double Mz = Model().SensorSeparation.X*(-scaled_analog[0]+scaled_analog[1]) + Model().SensorSeparation.Y*(scaled_analog[2]-scaled_analog[3]);

    // centre of pressure (un-corrected)
    double ax = -(My+Model().SensorSeparation.Z*force[0])/force[2];
    double ay =  (Mx-Model().SensorSeparation.Z*force[1])/force[2];

		// may be NaN/Inf due to divide-by-zero
		if (_finite(ax) && _finite(ay))
		{
			// 4th order corrections
			if (Model().COPOptimisation.NumElements() == 12)
			{
				// powers of ax and ay
				double ax2 = ax * ax;
				double ax3 = ax * ax2;
				double ax4 = ax * ax3;
				double ay2 = ay * ay;
				double ay3 = ay * ay2;
				double ay4 = ay * ay3;
	      
				// array reference
				const MapArrayFloat64& c = Model().COPOptimisation;

				// x-correction
				double Dax = (c[0]*ay4 + c[1]*ay2 + c[2])*ax3
									+  (c[3]*ay4 + c[4]*ay2 + c[5])*ax; 

				// y-correction
				double Day = (c[6]*ax4 + c[ 7]*ax2 + c[ 8])*ay3
									+  (c[9]*ax4 + c[10]*ax2 + c[11])*ay;

				// apply
				ax -= Dax; 
				ay -= Day;
			}

			// centre of pressure in fp co-ords
			point[0] = ax;
			point[1] = ay;
			point[2] = 0.0;
		}
		else
		{
			// just set zero when ill-defined like this
			// - must use Fz to determine how accurately the point is known
			point = 0.0;
		}

		// free moment in force plate coords (as reaction)
		freemoment_z = -(Mz - force[1]*point[0] + force[0]*point[1]);

		// change action force to reaction force
    force *= -1.0;
  }
}
