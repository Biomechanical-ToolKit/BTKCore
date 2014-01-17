/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_TREE_BINARY_H_
#define _ORMPP_TREE_BINARY_H_

#include "Open3DMotion/OpenORM/TreeValue.h"
#include "Open3DMotion/OpenORM/Leaves/MemoryHandler.h"

namespace Open3DMotion
{
	/**
	    Wrap a binary blob of data for use on a tree.
	*/
	class TreeBinary : public TreeValue
	{
	public:
		DECLARE_CLASS_NAME();

		TreeBinary();

		TreeBinary(const MemoryHandler* _binmemory);

		virtual ~TreeBinary();

		virtual TreeBinary* NewBlank() const;

		virtual void CopyFrom(const TreeValue* v);

		virtual void SetData(const MemoryHandler* _binmemory);

		const UInt8* Data() const
		{ return binmemory->Data(); }

		UInt8* Data()
		{ return binmemory->Data(); }

		size_t SizeBytes() const
		{ return binmemory->SizeBytes(); }

		const MemoryHandler* BinMemory() const
		{ return binmemory; }

	private:
		MemoryHandler* binmemory;
	};
}
#endif
