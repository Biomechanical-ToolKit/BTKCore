/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Leaves/TreeBinary.h"
#include "Open3DMotion/OpenORM/Leaves/MemoryHandlerNull.h"
#include <stdlib.h>

namespace Open3DMotion
{
	DEFINE_CLASS_NAME(TreeBinary);

	TreeBinary::TreeBinary() :
		binmemory(new MemoryHandlerNull())
	{
	}
		
	TreeBinary::TreeBinary(const MemoryHandler* _binmemory) :
		binmemory(_binmemory->Clone())
	{
	}

	TreeBinary::~TreeBinary()
	{
		delete binmemory;
	}

	TreeBinary* TreeBinary::NewBlank() const
	{
		return new TreeBinary();
	}

	void TreeBinary::CopyFrom(const TreeValue* v)
	{
		const TreeBinary* src = TreeValueCast<TreeBinary> (v);
		if (src != NULL)
		{
			delete binmemory;
			binmemory = src->binmemory->Clone();
		}
	}

	void TreeBinary::SetData(const MemoryHandler* _binmemory)
	{
		delete binmemory;
		binmemory = _binmemory->Clone();
	}

}

