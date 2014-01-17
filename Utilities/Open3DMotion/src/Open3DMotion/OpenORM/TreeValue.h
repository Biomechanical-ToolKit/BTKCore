/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_TREE_VALUE_H_
#define _ORMPP_TREE_VALUE_H_

#include "Open3DMotion/OpenORM/Types.h"
#include "Open3DMotion/OpenORM/ClassName.h"
#include <stdlib.h>

namespace Open3DMotion
{
	class TreeValue
	{
	public:
		TreeValue();

		virtual ~TreeValue();

		virtual TreeValue* NewBlank() const = 0;

		virtual void CopyFrom(const TreeValue* src) = 0;

		DECLARE_CLASS_NAME_ABSTRACT_BASE();
	};

	template<class ValueClass> const ValueClass* TreeValueCast(const TreeValue* v)
	{	return NamedClassCast<TreeValue, ValueClass> (v);	}

	template<class ValueClass> ValueClass* TreeValueCast(TreeValue* v)
	{	return NamedClassCast<TreeValue, ValueClass> (v);	}

}

#endif
