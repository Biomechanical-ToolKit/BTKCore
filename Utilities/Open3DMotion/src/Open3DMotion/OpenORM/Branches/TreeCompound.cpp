/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Branches/TreeCompound.h"

namespace Open3DMotion
{
	DEFINE_CLASS_NAME(TreeCompound);

	TreeCompound::TreeCompound()
	{
	}

	TreeCompound::~TreeCompound()
	{
		Clear();
	}

	TreeCompound* TreeCompound::NewBlank() const
	{
		return new TreeCompound;
	}


	void TreeCompound::Clear()
	{
		for (std::vector<TreeCompoundNode*>::iterator i(contents.begin()); i != contents.end(); i++)
		{
			delete (*i);
		}
		contents.clear();
	}

	const TreeCompoundNode* TreeCompound::Node(size_t index) const
	{
		return contents[index];
	}

	TreeCompoundNode* TreeCompound::Node(size_t index)
	{
		return contents[index];
	}

	void TreeCompound::Set(const char* name, TreeValue* value)
	{
		for (std::vector<TreeCompoundNode*>::iterator i(contents.begin()); i != contents.end(); i++)
		{
			if ((*i)->Name().compare(name) == 0)
			{
				(*i)->ChangeValue(value);
				return;
			}
		}

		contents.push_back(new TreeCompoundNode(name, value));
	}

	void TreeCompound::Remove(const char* name)
	{
		for (std::vector<TreeCompoundNode*>::iterator i(contents.begin()); i != contents.end(); i++)
		{
			TreeCompoundNode* node = (*i);
			if (node->Name().compare(name) == 0)
			{
				contents.erase(i);
				delete node;
				return;
			}
		}
	}

	const TreeValue* TreeCompound::Get(const char* name) const
	{
		for (std::vector<TreeCompoundNode*>::const_iterator i(contents.begin()); i != contents.end(); i++)
		{
			if ((*i)->Name().compare(name) == 0)
			{
				return (*i)->Value();
			}
		}

		return NULL;
	}

	TreeValue* TreeCompound::Get(const char* name)
	{
		for (std::vector<TreeCompoundNode*>::iterator i(contents.begin()); i != contents.end(); i++)
		{
			if ((*i)->Name().compare(name) == 0)
			{
				return (*i)->Value();
			}
		}

		return NULL;
	}

	void TreeCompound::CopyFrom(const TreeValue* v)
	{
		const TreeCompound* src = TreeValueCast<TreeCompound> (v);
		if (src != NULL)
		{
			Clear();
			for (std::vector<TreeCompoundNode*>::const_iterator i(src->contents.begin()); i != src->contents.end(); i++)
			{
				TreeValue* newvalue = (*i)->Value()->NewBlank(); 
				newvalue->CopyFrom( (*i)->Value() );
				Set((*i)->Name().c_str(), newvalue);
			}
		}

	}
}
