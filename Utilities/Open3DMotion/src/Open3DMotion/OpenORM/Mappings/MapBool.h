/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_MAP_BOOL_H_
#define _ORMPP_MAP_BOOL_H_

#include "Open3DMotion/OpenORM/Leaves/TreeBool.h"
#include "Open3DMotion/OpenORM/Mappings/MapSimpleValue.h"
#include "Open3DMotion/OpenORM/Mappings/MapOptional.h"

namespace Open3DMotion
{
	class MapBool : public MapSimpleValue<TreeBool, bool>
	{
	public:

		operator bool()
		{ return Value(); }

		const bool& operator=(const bool& _x)
		{ bool& s = Value(); s = _x; return s; }

	};

	class MapOptionalBool : public MapOptional<MapBool>
	{
	public:
		operator bool()
		{ return Value(); }

		const bool& operator=(const bool& _x)
		{ bool& s = Value(); s = _x; return s; }
	};
}
#endif