/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Leaves/MemoryHandlerBasic.h"
#include <stdlib.h>
#include <memory.h>

namespace Open3DMotion
{
	DEFINE_CLASS_NAME(MemoryHandlerBasic);

	MemoryHandlerBasic::MemoryHandlerBasic(size_t _sizebytes) :
		sizebytes(_sizebytes),
    refcount(new size_t),
		data(new UInt8[_sizebytes])
	{
		*refcount = 1;
	}

	MemoryHandlerBasic::MemoryHandlerBasic(size_t _sizebytes, UInt8* acquire_pointer) :
		sizebytes(_sizebytes),
    refcount(new size_t),
		data(acquire_pointer)
	{
		*refcount = 1;
	}

	MemoryHandlerBasic::MemoryHandlerBasic(const MemoryHandlerBasic& src) :
    MemoryHandler(src),
		sizebytes(src.sizebytes),
		refcount(src.refcount),
    data(src.data)
	{
		(*refcount)++;
	}
		
	MemoryHandlerBasic::~MemoryHandlerBasic()
	{
		if ((--(*refcount) == 0) && (data != NULL))
		{
			delete [] data;
			delete refcount;
			data = NULL;
			refcount = NULL;
		}
	}

	MemoryHandlerBasic* MemoryHandlerBasic::Clone() const
	{
		return new MemoryHandlerBasic(*this);
	}

	size_t MemoryHandlerBasic::RefCount() const
	{ 
		return (refcount != NULL) ? *refcount : 0; 
	}

}
