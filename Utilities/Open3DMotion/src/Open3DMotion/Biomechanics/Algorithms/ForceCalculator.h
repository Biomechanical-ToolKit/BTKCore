/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_FORCE_CALCULATOR_H_
#define _OPEN3DMOTION_FORCE_CALCULATOR_H_

#include "Open3DMotion/Biomechanics/Trial/ForcePlate.h"
#include "Open3DMotion/Biomechanics/Trial/TimeSequence.h"
#include "Open3DMotion/Maths/RigidTransform3.h"

namespace Open3DMotion
{
	class BinMemHandler;
	class ForceMeasurements;
	class ForceMeasurementTimeSequence;

  // Compute forces from analog force plate inputs
  // Derived classes for AMTI, Bertec, Kistler, etc.
  class ForceCalculator
  {
		friend class ForceCalculatorFactory;
		friend class TestForceCalculator;

	protected:

		ForceCalculator();

		// Called by force calculator factory to load and verify model information
		bool SetModel(const ForcePlate& _model);

		// Override to use and verify type-specific information from model 
		virtual bool VerifyModel(const ForcePlate& model) = 0;

	public:
		virtual ~ForceCalculator();

  public:

    // Number of analog inputs required by this instance
    virtual size_t NumInputs() const = 0;

    // Compute the force for a given frame using calibrated analog data (in V) in force plate local coords
		virtual void ComputeFromCalAnalog(Vector3& force, Vector3& point, double& freemoment_z, const std::vector<double>& calanalog) const = 0;

		bool Compute(Vector3& force, Vector3& point, Vector3& freemoment, const ForceMeasurements& measurements, std::vector<double>& buffer_rawanalog, std::vector<double>& buffer_calanalog) const;

		bool Compute(TimeSequence*& force, TimeSequence*& point, TimeSequence*& freemoment, ForceMeasurementTimeSequence& mts, const BinMemFactory& memfactory) const;

		/**
		 Summary
		 Compute rigid transform from plate coords to room coords based on outline data
		**/
		static bool RigidTransform(RigidTransform3& T, const ForcePlate& model);

		const ForcePlate& Model() const
		{ return model; }

		const RigidTransform3& Transform() const
		{ return T; }

	private:
		ForcePlate model;
		RigidTransform3 T;
	};
}
#endif