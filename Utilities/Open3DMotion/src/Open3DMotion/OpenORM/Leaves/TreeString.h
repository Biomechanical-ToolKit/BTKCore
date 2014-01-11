/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_TREE_STRING_H_
#define _ORMPP_TREE_STRING_H_

#include "Open3DMotion/OpenORM/Leaves/TreeSimpleValue.h"
#include <string>

namespace Open3DMotion
{
	class TreeString : public TreeSimpleValue<std::string>
	{
	public:				
		DECLARE_CLASS_NAME();

		TreeString();

		TreeString(const char* _cstr);

		TreeString(const std::string& _str);

	public:

		virtual TreeString* NewBlank() const;

	};
}
#endif