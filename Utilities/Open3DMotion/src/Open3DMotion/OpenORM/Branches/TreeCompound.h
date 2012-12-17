/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_TREE_COMPOUND_H_
#define _ORMPP_TREE_COMPOUND_H_

#include "Open3DMotion/OpenORM/Branches/TreeCompoundNode.h"
#include <vector>

namespace Open3DMotion
{
	/*

	  Summary
		A TreeCompound is an array of named value objects, each of which may be of a different type.

	*/
	class TreeCompound : public TreeValue
	{
	public:
		DECLARE_CLASS_NAME();

		TreeCompound();

		virtual ~TreeCompound();

	public:

		virtual TreeCompound* NewBlank() const;

		virtual void CopyFrom(const TreeValue* src);

		void Clear();

		size_t NumElements() const
		{ return contents.size(); }

		const TreeCompoundNode* Node(size_t index) const;

		TreeCompoundNode* Node(size_t index);

		void Set(const char* name, TreeValue* value);

		void Remove(const char* name);

		const TreeValue* Get(const char* name) const;

		TreeValue* Get(const char* name);

		template<class ValueType> const ValueType* GetType(const char* name) const
		{
			const TreeValue* t = Get(name);
			if (t != NULL)
			{
				if (ValueType::ClassNameMatches(t))
				{
					return static_cast<const ValueType*>( t );
				}
			}
			return NULL;
		}

	private:
		std::vector<TreeCompoundNode*> contents;
	};
}
#endif