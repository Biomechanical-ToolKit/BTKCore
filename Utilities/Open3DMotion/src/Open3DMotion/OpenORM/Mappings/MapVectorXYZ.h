/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_MAP_VECTOR_XYZ_H_
#define _ORMPP_MAP_VECTOR_XYZ_H_

#include "Open3DMotion/OpenORM/Mappings/MapCompound.h"
#include "Open3DMotion/OpenORM/Mappings/MapFloat64.h"

namespace Open3DMotion
{
	class MapVectorXYZ : public MapCompound
	{
	public:
		MapVectorXYZ();

	public:
		MapFloat64 X;
		MapFloat64 Y;
		MapFloat64 Z;

	public:
		void GetVector(double* xyz) const;

		void SetVector(const double* xyz);
	};
}
#endif