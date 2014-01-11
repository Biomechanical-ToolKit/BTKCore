/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Leaves/TreeBool.h"

namespace Open3DMotion
{
	DEFINE_CLASS_NAME(TreeBool);

	TreeBool::TreeBool(bool _x /*=false*/) :
		TreeSimpleValue<bool>(_x)
	{
	}

	TreeBool* TreeBool::NewBlank() const
	{	
		return new TreeBool();
	}
}
