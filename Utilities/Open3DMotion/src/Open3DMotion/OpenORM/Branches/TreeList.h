/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_TREE_LIST_H_
#define _ORMPP_TREE_LIST_H_

#include "Open3DMotion/OpenORM/TreeValue.h"
#include <vector>
#include <string>

namespace Open3DMotion
{
	/*
		Summary
		A TreeList is an array of unnamed value objects.
	*/
	class TreeList : public TreeValue
	{
	public:
		DECLARE_CLASS_NAME();

		TreeList(const char* _elementname);

		virtual ~TreeList();

	public:

		virtual TreeList* NewBlank() const;

		virtual void CopyFrom(const TreeValue* v);
	
		const std::string& ElementName() const
		{ return elementname; }

		size_t NumElements() const
		{ return elementarray.size(); }

		void Clear();

	public:

		std::vector<TreeValue*>& ElementArray()
		{ return elementarray; }

		const std::vector<TreeValue*>& ElementArray() const
		{ return elementarray; }

		void Add(TreeValue* element);

	private:
		std::string elementname;
		std::vector<TreeValue*> elementarray;
	};

}
#endif