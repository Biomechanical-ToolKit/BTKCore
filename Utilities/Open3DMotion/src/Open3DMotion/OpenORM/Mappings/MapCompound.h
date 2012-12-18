/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_MAP_COMPOUND_H_
#define _ORMPP_MAP_COMPOUND_H_

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

	const char* MemberName(const char* qualified_name);

}

// Helper to use inside constructor of compound classes to register their member variables
#define REGISTER_MEMBER(m) Register(#m, &m)

// Helper to express a member variable name as a string
#define MEMBER_NAME(qualified_name) Open3DMotion::MemberName(#qualified_name)

#endif
