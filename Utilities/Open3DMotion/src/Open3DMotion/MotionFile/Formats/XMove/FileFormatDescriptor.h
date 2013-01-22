/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/


#include "Open3DMotion/OpenORM/Mappings/MapCompound.h"
#include "Open3DMotion/OpenORM/Mappings/MapString.h"

namespace Open3DMotion
{
	class FileFormatDescriptor : public MapCompound
	{
	public:
		FileFormatDescriptor();

	public:
		MapOptionalString FormatID;
		MapOptionalString Company;
		MapOptionalString ProgramName;
		MapOptionalString ProgramVersion;
		MapOptionalString LibraryName;
		MapOptionalString LibraryVersion;
	};
}