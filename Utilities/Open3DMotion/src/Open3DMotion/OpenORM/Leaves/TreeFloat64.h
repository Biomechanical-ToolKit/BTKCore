/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_TREE_FLOAT64_H_
#define _ORMPP_TREE_FLOAT64_H_

#include "Open3DMotion/OpenORM/Leaves/TreeSimpleValue.h"

namespace Open3DMotion
{
	class TreeFloat64 : public TreeSimpleValue<double>
	{
	public:				
		DECLARE_CLASS_NAME();

		TreeFloat64(double _x = 0.0);
	
		virtual TreeFloat64* NewBlank() const;
	};
}
#endif