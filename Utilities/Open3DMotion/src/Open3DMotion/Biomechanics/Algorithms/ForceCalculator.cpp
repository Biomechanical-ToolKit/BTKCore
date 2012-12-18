/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Biomechanics/Algorithms/ForceCalculator.h"
#include "Open3DMotion/Biomechanics/Algorithms/LinearCalibration.h"
#include "Open3DMotion/Biomechanics/Algorithms/ForceMeasurements.h"
#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"

namespace Open3DMotion
{
	bool ForceCalculator::RigidTransform(RigidTransform3& T, const ForcePlate& model)
	{
		if (model.Outline.NumElements() == 4)
		{
			// form outline from model
			Vector3 outline[4];
			for (UInt32 i = 0; i < 4; i++)
			{
				model.Outline[i].GetVector(outline[i]);
			}

			// mean central point
			Vector3 centre(0,0,0);
			centre += outline[0];
			centre += outline[1];
			centre += outline[2];
			centre += outline[3];
			centre *= 0.25;

			// mean fp X-direction
			Vector3 meanX(0,0,0);
			meanX -= outline[0];
			meanX += outline[1];
			meanX += outline[2];
			meanX -= outline[3];

			// mean fp Y-direction
			Vector3 meanY(0,0,0);
			meanY -= outline[0];
			meanY -= outline[1];
			meanY += outline[2];
			meanY += outline[3];

			// transform fp co-ords ---> global co-ords
			RigidTransform3::FromXYVec(T, centre, meanX, meanY);	
		}
		else
		{
			// do not have 4 corners as needed
			return false;
		}

		return true;
	}

	ForceCalculator::ForceCalculator()
	{
	}

	ForceCalculator::~ForceCalculator()
	{
	}

	bool ForceCalculator::SetModel(const ForcePlate& _model)
	{
		// store model
		model = _model;

		// compute rigid transform
		if (!RigidTransform(T, model))
			return false;

		// plate-specific configuration
		if (!VerifyModel(model))
			return false;

		// done
		return true;
	}

	bool ForceCalculator::Compute(Vector3& force, Vector3& point, Vector3& freemoment, const ForceMeasurements& m, std::vector<double>& buffer_rawanalog, std::vector<double>& buffer_calanalog) const
	{
		if (m.NumChannels() != NumInputs())
			return false;

		// retrieve current set of raw analog signals (for this TS frame or live input)
		m.CurrentRawAnalog(buffer_rawanalog);

		// apply scales and offsets
		m.ApplyLinearCalibration(buffer_calanalog, buffer_rawanalog);

		// do the computation
		Vector3 f_local, p_local;
		double freemoment_local_z(0.0);
		ComputeFromCalAnalog(f_local, p_local, freemoment_local_z, buffer_calanalog);

		// 3D version of free moment
		Vector3 freemoment_local(0.0, 0.0, freemoment_local_z);

		// transform orientation of force
		Matrix3x3::MulVec(force, T.R, f_local);

		// transform orientation and location of centre-of-pressure
		RigidTransform3::MulVec(point, T, p_local);

		// transform orientation of free moment
		Matrix3x3::MulVec(freemoment, T.R, freemoment_local);

		return true;
	}

	bool ForceCalculator::Compute(TimeSequence*& force, TimeSequence*& point, TimeSequence*& freemoment, ForceMeasurementTimeSequence& mts, BinMemFactory& memfactory) const
	{
		// initialise to NULL
		force = NULL;
		point = NULL;

    // must have enough input channels
    if (mts.NumChannels() != NumInputs())
      return false;

		// use input time range for output
		TimeRange tr;
		tr.Start = 0.0;
		tr.Frames = mts.NumFrames();
		tr.Rate = mts.Rate();

		// allocate time sequences
		force = TSFactoryValue(3).New(tr, memfactory);
		point = TSFactoryValue(3).New(tr, memfactory);
		freemoment = TSFactoryValue(3).New(tr, memfactory);

		// per-frame buffers
		std::vector<double> buffer_rawanalog;
		std::vector<double> buffer_calanalog;

		// do all calcs
		TSVector3Iter iter_force(*force);
		TSVector3Iter iter_point(*point);
		TSVector3Iter iter_freemoment(*freemoment);
		mts.Begin();

		for (; iter_force.HasFrame(); iter_force.Next(), iter_point.Next(), iter_freemoment.Next(), mts.NextFrame())
    {
			// compute for this frame, local to plate
			Vector3 f, p, freemoment;
			Compute(f, p, freemoment, mts, buffer_rawanalog, buffer_calanalog);

			// copy to ts
			Vector3::Copy(iter_force.Value(), f);
			Vector3::Copy(iter_point.Value(), p);
			Vector3::Copy(iter_freemoment.Value(), freemoment);
    }

		return true;
  }

}
