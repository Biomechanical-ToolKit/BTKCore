/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#pragma once

#include "Open3DMotion/OpenORM/Leaves/MemoryHandler.h"

namespace Open3DMotion
{
	class MemoryHandlerNull : public MemoryHandler
	{
	public:

		virtual void Free();

		virtual size_t SizeBytes() const;

		virtual UInt8* Data() const;

		virtual MemoryHandler* Clone() const;

	};

}
