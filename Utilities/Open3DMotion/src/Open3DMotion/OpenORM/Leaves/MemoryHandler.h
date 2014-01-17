/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_MEMORY_HANDLER_H_
#define _OPEN3DMOTION_MEMORY_HANDLER_H_

#include "Open3DMotion/OpenORM/Types.h"
#include "Open3DMotion/OpenORM/ClassName.h"

namespace Open3DMotion
{
	class MemoryHandler
	{
	public:
		// DISALLOW_COPY_AND_ASSIGN(MemoryHandler);

	public:

		virtual ~MemoryHandler();

		virtual size_t SizeBytes() const = 0;

		virtual UInt8* Data() const = 0;

		virtual MemoryHandler* Clone() const = 0;

		DECLARE_CLASS_NAME_ABSTRACT_BASE();

	};

}
#endif