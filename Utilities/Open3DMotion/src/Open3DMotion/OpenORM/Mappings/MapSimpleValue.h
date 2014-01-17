/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_MAP_SIMPLE_VALUE_H_
#define _ORMPP_MAP_SIMPLE_VALUE_H_

#include "Open3DMotion/OpenORM/Mappings/MapElement.h"

namespace Open3DMotion
{
	template<class ValueClass, class DataType> class MapSimpleValue : public ValueClass, public MapElement
	{
	public:
		MapSimpleValue()
		{ }

		virtual TreeValue* ToTree() const
		{ TreeValue* v = ValueClass::NewBlank(); v->CopyFrom(this); return v; }

		virtual void FromTree(const TreeValue* v)
		{ ValueClass::CopyFrom(v); }

		DataType& Value()
		{ ChildValueChanged(); return ValueClass::Value(); }

		const DataType& Value() const
		{ return ValueClass::Value(); }
	};
}
#endif