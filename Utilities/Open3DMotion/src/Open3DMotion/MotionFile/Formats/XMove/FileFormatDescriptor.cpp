/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "FileFormatDescriptor.h"

namespace Open3DMotion
{
	FileFormatDescriptor::FileFormatDescriptor()
	{
		REGISTER_MEMBER(FormatID);
		REGISTER_MEMBER(Company);
		REGISTER_MEMBER(ProgramName);
		REGISTER_MEMBER(ProgramVersion);
		REGISTER_MEMBER(LibraryName);
		REGISTER_MEMBER(LibraryVersion);
	}
}