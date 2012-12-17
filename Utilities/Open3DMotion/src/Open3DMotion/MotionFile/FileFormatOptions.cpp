/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/


#include "Open3DMotion/MotionFile/FileFormatOptions.h"

namespace Open3DMotion
{
	FileFormatOptions::FileFormatOptions()
	{
		REGISTER_MEMBER(FormatID);
		REGISTER_MEMBER(PathName);
	}
}
