/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"
#include "Open3DMotion/OpenORM/Leaves/MemoryHandlerBasic.h"

namespace Open3DMotion
{
	MemoryHandlerBasic* BinMemFactoryDefault::Allocate(size_t numbytes) const
	{
		return new MemoryHandlerBasic(numbytes);
	}
}
