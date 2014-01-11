/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Mappings/MapCompound.h"
#include "Open3DMotion/OpenORM/Mappings/MapCompoundRefMap.h"

namespace Open3DMotion
{
	void MapCompound::Register(const char* membername, MapElement* membermap)
	{
		refmap.push_back(new MapCompoundRefMap(membername, membermap));
		membermap->SetParent(this);
	}

	const MapCompound& MapCompound::operator=(const MapCompound& other)
	{
		// don't copy ref map
		MapElement::operator=(other);
		return *this;
	}

	MapCompound::~MapCompound()
	{
		for (std::vector<MapCompoundElement*>::iterator i( refmap.begin() ); i != refmap.end(); i++)
		{
			delete ( *i );
		}
		refmap.clear();
	}

	TreeValue* MapCompound::ToTree() const
	{
		// clone the current map
		TreeCompound* cpy = new TreeCompound();
		for (std::vector<MapCompoundElement*>::const_iterator i( refmap.begin() ); i != refmap.end(); i++)
		{
			TreeValue* cpyvalue = (*i)->ToTree();
			if (cpyvalue != NULL)
			{
				cpy->Set((*i)->Name(), cpyvalue);
			}
		}
		return cpy;
	}

	void MapCompound::FromTree(const TreeValue* v)
	{
		const TreeCompound* src = TreeValueCast<TreeCompound> (v);
		if (src)
		{
			// copy all items named in our current map
			for (std::vector<MapCompoundElement*>::iterator i( refmap.begin() ); i != refmap.end(); i++)
			{
				const TreeValue* srcvalue = src->Get((*i)->Name());
				if (srcvalue != NULL)
				{
					(*i)->FromTree(srcvalue);
				}
			}
		}
	}

	const char* MemberName(const char* qualified_name)
	{
		const char* last_colon = NULL;
		for (const char* c = qualified_name; *c; c++)
		{
			if (*c == ':')
				last_colon = c;
		}
		return (last_colon != NULL) ? (last_colon+1) : qualified_name;
	}
}
