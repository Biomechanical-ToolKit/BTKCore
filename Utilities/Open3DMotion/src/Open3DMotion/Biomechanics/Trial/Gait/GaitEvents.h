/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_GAIT_EVENTS_H_
#define _OPEN3DMOTION_GAIT_EVENTS_H_

#include "Open3DMotion/Biomechanics/Trial/EventArray.h"

namespace Open3DMotion
{
	class GaitEvents
	{
	private:
		GaitEvents();

	public:
		static const char GroupName[];
		static const char RepresentativeGroupName[];
		static const char LStart[];
		static const char LToeOff[];
		static const char LEnd[];
		static const char RStart[];
		static const char RToeOff[];
		static const char REnd[];
	};

}

#endif
