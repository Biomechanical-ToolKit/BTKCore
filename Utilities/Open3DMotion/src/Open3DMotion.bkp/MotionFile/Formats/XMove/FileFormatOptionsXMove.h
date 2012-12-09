/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _FILE_FORMAT_OPTIONS_XMOVE_H_
#define _FILE_FORMAT_OPTIONS_XMOVE_H_

#include "Open3DMotion/MotionFile/FileFormatOptions.h"
#include "Open3DMotion/OpenORM/Mappings/MapBool.h"

namespace Open3DMotion
{
	// options class for XMove
	class FileFormatOptionsXMove : public FileFormatOptions
	{
	public:
		FileFormatOptionsXMove();

	public:
		MapBool Extended;
	};
}

#endif
