/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_TREE_INT32_H_
#define _ORMPP_TREE_INT32_H_

#include "Open3DMotion/OpenORM/Leaves/TreeSimpleValue.h"

namespace Open3DMotion
{
	class TreeInt32 : public TreeSimpleValue<Int32>
	{
	public:				
		DECLARE_CLASS_NAME();

		TreeInt32(Int32 _x = 0);
	
		virtual TreeInt32* NewBlank() const;
	};
}
#endif