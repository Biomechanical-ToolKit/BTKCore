/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_MAP_INT32_H_
#define _ORMPP_MAP_INT32_H_

#include "Open3DMotion/OpenORM/Leaves/TreeInt32.h"
#include "Open3DMotion/OpenORM/Mappings/MapSimpleValue.h"
#include "Open3DMotion/OpenORM/Mappings/MapOptional.h"

namespace Open3DMotion
{
	class MapInt32 : public MapSimpleValue<TreeInt32, Int32>
	{
	public:

		operator Int32() const
		{ return Value(); }

		const Int32& operator=(const Int32& _x)
		{ Int32& s = Value(); s = _x; return s; }

	};

	class MapOptionalInt32 : public MapOptional<MapInt32>
	{
	public:
		operator Int32() const
		{ return Value(); }

		const Int32& operator=(const Int32& _x)
		{ Int32& s = Value(); s = _x; return s; }
	};

}
#endif