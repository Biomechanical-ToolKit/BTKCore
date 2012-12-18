/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_MAP_COMPOUND_REF_MAP_H_
#define _ORMPP_MAP_COMPOUND_REF_MAP_H_

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
#endif