/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#pragma once

#include "Open3DMotion/OpenORM/Mappings/MapCompoundElement.h"

namespace Open3DMotion
{
	class MapCompoundRefMap : public MapCompoundElement
	{
	public:
		MapCompoundRefMap(const char* _name, MapElement* _refmap);

		const MapElement* RefMap() const
		{ return refmap; }

		MapElement* RefMap()
		{ return refmap; }

		virtual TreeValue* ToTree() const;

		virtual void FromTree(const TreeValue* v);

	private:
		MapElement* refmap;
	};
}
