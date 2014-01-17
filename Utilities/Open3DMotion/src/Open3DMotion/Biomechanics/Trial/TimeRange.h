/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_TIME_RANGE_H_
#define _OPEN3DMOTION_TIME_RANGE_H_

#include "Open3DMotion/OpenORM/Mappings/MapCompound.h"
#include "Open3DMotion/OpenORM/Mappings/MapInt32.h"
#include "Open3DMotion/OpenORM/Mappings/MapFloat64.h"

namespace Open3DMotion
{
	class TimeRange : public MapCompound
	{
	public:
		TimeRange();

	public:
		MapFloat64 Rate;
		MapFloat64 Start;
		MapInt32 Frames;
	};

}
#endif