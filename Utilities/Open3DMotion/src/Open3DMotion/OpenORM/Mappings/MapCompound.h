/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#pragma once

#include "Open3DMotion/OpenORM/Branches/TreeCompound.h"
#include "Open3DMotion/OpenORM/Mappings/MapElement.h"
#include "Open3DMotion/OpenORM/Mappings/MapCompoundElement.h"
#include <vector>

namespace Open3DMotion
{
	class MapCompound : public MapElement
	{
	public:

		MapCompound()
		{
		}

		MapCompound(const MapCompound& src) : MapElement(src)
		{ operator=(src); }

		virtual ~MapCompound();

		virtual TreeValue* ToTree() const;

		virtual void FromTree(const TreeValue* v);

		const MapCompound& operator=(const MapCompound& other);

	protected:

		void Register(const char* membername, MapElement* membermap);

	private:
		std::vector<MapCompoundElement*> refmap;
	};
}

// Helper to use inside constructor of compound classes to register their member variables
#define REGISTER_MEMBER(m) Register(#m, &m)
