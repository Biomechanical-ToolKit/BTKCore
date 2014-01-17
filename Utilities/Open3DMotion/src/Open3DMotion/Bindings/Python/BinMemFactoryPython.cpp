/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Bindings/Python/BinMemFactoryPython.h"

namespace Open3DMotion
{
	MemoryHandlerPython* BinMemFactoryPython::Allocate(size_t numbytes) const
	{
		return new MemoryHandlerPython(numbytes);
	}
}
