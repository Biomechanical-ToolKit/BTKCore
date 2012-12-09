/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#pragma once

#include "Open3DMotion/OpenORM/TreeValue.h"
#include <stdlib.h>

namespace Open3DMotion
{

	template<class DataType> class TreeSimpleValue : public TreeValue
	{
	public:
		TreeSimpleValue()
		{
		}

		TreeSimpleValue(const DataType& _x) :
			x(_x)
		{
		}

		virtual ~TreeSimpleValue()
		{
		}

	public:

		virtual void CopyFrom(const TreeValue* v)
		{
			if (v->ClassNameMatches(ClassName()))
			{
				const TreeSimpleValue<DataType>* src = static_cast<const TreeSimpleValue<DataType>* > (v);
				x = src->x;
			}
		}

		const DataType& Value() const
		{ return x; }

		DataType& Value()
		{ return x; }

	protected:
		DataType x;
	};

}
