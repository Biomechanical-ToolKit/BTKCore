/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Mappings/MapElement.h"
#include <stdlib.h>

namespace Open3DMotion
{
	MapElement::MapElement() :
		parent(NULL)
	{
	}

	MapElement::~MapElement()
	{
	}

	void MapElement::SetParent(MapElement* _parent)
	{
		parent = _parent;
	}

	void MapElement::ChildValueChanged()
	{
		if (parent != NULL)
			parent->ChildValueChanged();
	}

}
