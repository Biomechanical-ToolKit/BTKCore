/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_MAP_ARRAY_INT32_H_
#define _ORMPP_MAP_ARRAY_INT32_H_

#include "Open3DMotion/OpenORM/Mappings/MapArraySimpleValue.h"
#include "Open3DMotion/OpenORM/Leaves/TreeInt32.h"

namespace Open3DMotion
{
	class MapArrayInt32 : public MapArraySimpleValue<TreeInt32, Int32>
	{
	public:
		MapArrayInt32(const char* _elementname) :
			MapArraySimpleValue<TreeInt32, Int32>(_elementname)
		{
		}
	};
}
#endif
