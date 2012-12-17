/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Branches/TreeCompoundNode.h"

namespace Open3DMotion
{
	TreeCompoundNode::TreeCompoundNode() :
		value(NULL)
	{
	}

	TreeCompoundNode::TreeCompoundNode(const char* _name, TreeValue* _value) :
		name(_name),
		value(_value)
	{
	}

	TreeCompoundNode::~TreeCompoundNode()
	{
		ClearValue();
	}

	void TreeCompoundNode::ClearValue()
	{
		if (value != NULL)
		{
			delete value;
			value = NULL;
		}
	}

	void TreeCompoundNode::ChangeValue(TreeValue* _value)
	{
		ClearValue();
		value = _value;
	}

}
