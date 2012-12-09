/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/TreeValue.h"
#include <string>

namespace Open3DMotion
{
	TreeValue::TreeValue()
	{
	}

	TreeValue::~TreeValue()
	{
	}

	bool TreeValue::ClassNameMatches(const char* name) const
	{
		return (strcmp(ClassName(), name) == 0);
	}
}
