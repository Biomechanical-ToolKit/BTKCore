/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#pragma once

#include "Open3DMotion/OpenORM/Leaves/TreeBinary.h"
#include "Open3DMotion/OpenORM/Mappings/MapElement.h"

namespace Open3DMotion
{
	/**
	    Object mapping for binary blob
	*/
	class MapBinary : public TreeBinary, public MapElement
	{
	public:
		MapBinary()
		{
		}

		virtual void SetData(const MemoryHandler* _binmemory)
		{ TreeBinary::SetData(_binmemory); ChildValueChanged(); }

		UInt8* Data()
		{ ChildValueChanged(); return TreeBinary::Data(); }

		const UInt8* Data() const
		{ return TreeBinary::Data(); }

		virtual TreeValue* ToTree() const
		{
			TreeBinary* t = this->NewBlank();
			t->CopyFrom(this);
			return t;
		}

		virtual void FromTree(const TreeValue* src)
		{
			CopyFrom(src);
		}

	};
}
