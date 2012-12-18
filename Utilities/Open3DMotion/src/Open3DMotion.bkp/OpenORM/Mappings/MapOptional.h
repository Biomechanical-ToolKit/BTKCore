/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#pragma once

#include "Open3DMotion/OpenORM/TreeValue.h"
#include "Open3DMotion/OpenORM/Mappings/MapElement.h"
#include "Open3DMotion/OpenORM/Branches/TreeCompound.h"
#include "Open3DMotion/OpenORM/Mappings/MapSimpleValue.h"

namespace Open3DMotion
{
	class MapOptionalBase
	{
	public:
		MapOptionalBase() :
			isset(false)
		{
		}

		bool IsSet() const
		{ return isset; }

		void Set()
		{
			isset = true;
		}

		void UnSet()
		{
			isset = false;
		}

	private:
		bool isset;
	};

	template<class MapClass> class MapOptional : public MapClass, public MapOptionalBase
	{
	public:

		virtual TreeValue* ToTree() const
		{
			if (IsSet())
			{
				return MapClass::ToTree();
			}
			else
			{
				return NULL;
			}
		}

		virtual void FromTree(const TreeValue* v)
		{
			MapClass::FromTree(v);
			ChildValueChanged();
		}

		virtual void ChildValueChanged()
		{
			MapOptionalBase::Set();
			MapClass::ChildValueChanged();
		}
	};
}
