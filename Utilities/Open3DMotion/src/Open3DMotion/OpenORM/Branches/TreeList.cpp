/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Branches/TreeList.h"
#include <string>

namespace Open3DMotion
{
	DEFINE_CLASS_NAME(TreeList);

	TreeList::TreeList(const char* _elementname) : 
		elementname(_elementname)
	{
	}

	TreeList::~TreeList()
	{
		Clear();
	}

	void TreeList::Add(TreeValue* element)
	{
		elementarray.push_back(element);
	}

	void TreeList::Clear()
	{
		for (std::vector<TreeValue*>::iterator i(elementarray.begin()); i != elementarray.end(); i++)
			delete (*i);
		elementarray.clear();
	}

	TreeList* TreeList::NewBlank() const
	{
		return new TreeList(ElementName().c_str());
	}

	void TreeList::CopyFrom(const TreeValue* v)
	{
		const TreeList* src = TreeValueCast<TreeList> (v);
		if (src != NULL)
		{
			Clear();
			elementname = src->ElementName();
			for (std::vector<TreeValue*>::const_iterator i( src->ElementArray().begin() ); i != src->ElementArray().end(); i++)
			{
				if ( (*i) == NULL )
				{
					Add(NULL);
				}
				else
				{
					TreeValue* element = ( *i )->NewBlank();
					element->CopyFrom( *i );
					Add(element);
				}
			}
		}
	}
}
