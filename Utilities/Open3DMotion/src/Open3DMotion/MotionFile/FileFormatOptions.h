/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _FILE_FORMAT_OPTIONS_H_
#define _FILE_FORMAT_OPTIONS_H_

#include "Open3DMotion/OpenORM/Mappings/MapCompound.h"
#include "Open3DMotion/OpenORM/Mappings/MapString.h"

namespace Open3DMotion
{
	// Options for choosing format
	class FileFormatOptions : public MapCompound
	{
	public:
		FileFormatOptions();

	public:
		MapString FormatID;
	};

}

#endif
