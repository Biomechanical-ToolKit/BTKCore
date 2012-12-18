/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_EVENT_NAME_ID_H_
#define _OPEN3DMOTION_EVENT_NAME_ID_H_


#include "Open3DMotion/OpenORM/Mappings/MapCompound.h"
#include "Open3DMotion/OpenORM/Mappings/MapInt32.h"
#include "Open3DMotion/OpenORM/Mappings/MapString.h"

namespace Open3DMotion
{
	class EventNameID : public MapCompound
	{
	public:
		EventNameID();

	public:
		MapString Name;
		MapInt32 ID;
	};
}
#endif