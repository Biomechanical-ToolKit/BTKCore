/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_TREE_COMPOUND_NODE_H_
#define _ORMPP_TREE_COMPOUND_NODE_H_

#include "Open3DMotion/OpenORM/TreeValue.h"
#include <string>

namespace Open3DMotion
{
	class TreeCompoundNode
	{
	public:
		TreeCompoundNode();

		TreeCompoundNode(const char* _name, TreeValue* _value);

		virtual ~TreeCompoundNode();

	public:

		const std::string& Name() const
		{ return name; }

		TreeValue* Value()
		{ return value; }

		const TreeValue* Value() const
		{ return value; }

		void ChangeValue(TreeValue* _value);

	private:

		void ClearValue();

	private:
		std::string name;
		TreeValue* value;
	};

}
#endif