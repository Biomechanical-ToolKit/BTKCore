/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_FORCE_PLATE_H_
#define _OPEN3DMOTION_FORCE_PLATE_H_

#include "Open3DMotion/OpenORM/Branches/TreeCompound.h"
#include "Open3DMotion/OpenORM/Mappings/MapSimpleValue.h"
#include "Open3DMotion/OpenORM/Mappings/MapVectorXYZ.h"
#include "Open3DMotion/OpenORM/Mappings/MapArrayFloat64.h"
#include "Open3DMotion/OpenORM/Mappings/MapArrayInt32.h"
#include "Open3DMotion/OpenORM/Mappings/MapArrayCompound.h"
#include "Open3DMotion/OpenORM/Mappings/MapString.h"

namespace Open3DMotion
{
	class ForcePlate : public MapCompound
	{
	public:
		static const char TypeAMTI[];
		static const char TypeKistler[];

	public:
		ForcePlate();

	public:
		MapString Type;
		MapString Model;
    MapArrayCompound<MapVectorXYZ> Outline;
		MapArrayFloat64 Calibration;   
		MapArrayInt32 Channels;

		// Kistler-specific
    MapOptional<MapVectorXYZ> SensorSeparation;
		MapArrayFloat64 COPOptimisation;

		// strain-gauge-specific
		MapOptional<MapVectorXYZ> CentreOffset;

		// optional pre-computed data read from legacy MDF files
		MapArrayInt32 PreCompCoPChannels;
	};

}
#endif