/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Leaves/TreeString.h"

namespace Open3DMotion
{
	DEFINE_CLASS_NAME(TreeString);

	TreeString::TreeString() :
		TreeSimpleValue<std::string> ()
	{
	}

	TreeString::TreeString(const char* _cstr) :
		TreeSimpleValue<std::string> (_cstr)
	{
	}

	TreeString* TreeString::NewBlank() const
	{	
		return new TreeString();
	}

}
