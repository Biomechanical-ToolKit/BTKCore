/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Leaves/TreeInt32.h"

namespace Open3DMotion
{
	DEFINE_CLASS_NAME(TreeInt32);

	TreeInt32::TreeInt32(Int32 _x /*= 0*/) :
		TreeSimpleValue<Int32>(_x)
	{
	}

	TreeInt32* TreeInt32::NewBlank() const
	{
		return new TreeInt32();
	}
}
