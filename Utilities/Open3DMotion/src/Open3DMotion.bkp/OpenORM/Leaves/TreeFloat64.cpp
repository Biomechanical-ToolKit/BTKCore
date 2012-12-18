/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Leaves/TreeFloat64.h"

namespace Open3DMotion
{
	DEFINE_CLASS_NAME(TreeFloat64);

	TreeFloat64::TreeFloat64(double _x /*= 0.0*/) :
		TreeSimpleValue<double>(_x)
	{
	}

	TreeFloat64* TreeFloat64::NewBlank() const
	{	
		return new TreeFloat64();
	}
}
