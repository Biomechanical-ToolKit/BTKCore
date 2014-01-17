/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Biomechanics/Trial/TimeRange.h"

namespace Open3DMotion
{
	TimeRange::TimeRange()
	{
		REGISTER_MEMBER(Rate);
		REGISTER_MEMBER(Start);
		REGISTER_MEMBER(Frames);
	}
}
