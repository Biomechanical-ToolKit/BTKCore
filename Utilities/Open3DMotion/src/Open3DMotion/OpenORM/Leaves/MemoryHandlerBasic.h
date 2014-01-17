/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_MEMORY_HANDLER_BASIC_H_
#define _OPEN3DMOTION_MEMORY_HANDLER_BASIC_H_

#include "Open3DMotion/OpenORM/Leaves/MemoryHandler.h"

namespace Open3DMotion
{
	/**
	
	A simple reference-counted memory handler.
	Cloning creates a reference, not a deep copy.
	Memory is freed when the last object pointing to the data is destroyed.

	*/
	class MemoryHandlerBasic : public MemoryHandler
	{
	public:
		DECLARE_CLASS_NAME();

		MemoryHandlerBasic(size_t _sizebytes);

		MemoryHandlerBasic(size_t _sizebytes, UInt8* acquire_pointer);

		MemoryHandlerBasic(const MemoryHandlerBasic& ref);

		virtual ~MemoryHandlerBasic();

		virtual MemoryHandlerBasic* Clone() const;

		virtual size_t SizeBytes() const
		{ return sizebytes; }

		virtual UInt8* Data() const
		{ return data; }

		virtual size_t RefCount() const;

	private:
		size_t sizebytes;
		size_t* refcount;
		UInt8* data;
	};

}
#endif