/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#pragma once

#include "Open3DMotion/OpenORM/Leaves/TreeFloat64.h"
#include "Open3DMotion/OpenORM/Mappings/MapSimpleValue.h"
#include "Open3DMotion/OpenORM/Mappings/MapOptional.h"

namespace Open3DMotion
{
	class MapFloat64 : public MapSimpleValue<TreeFloat64, double>
	{
	public:

		operator double() const
		{ return Value(); }

		const double& operator=(const double& _x)
		{ double& s = Value(); s = _x; return s; }

	};

	class MapOptionalFloat64 : public MapOptional<MapFloat64>
	{
	public:

		operator double() const
		{ return Value(); }

		const double& operator=(const double& _x)
		{ double& s = Value(); s = _x; return s; }

	};
}
