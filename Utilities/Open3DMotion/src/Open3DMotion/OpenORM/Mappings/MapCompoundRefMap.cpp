/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Mappings/MapCompoundRefMap.h"
#include <stdlib.h>

namespace Open3DMotion
{
	MapCompoundRefMap::MapCompoundRefMap(const char* _name, MapElement* _refmap) :
		MapCompoundElement(_name),
		refmap(_refmap)
	{
	}

	TreeValue* MapCompoundRefMap::ToTree() const
	{
		return refmap->ToTree();
	}

	void MapCompoundRefMap::FromTree(const TreeValue* v)
	{
		refmap->FromTree(v);
	}

}
