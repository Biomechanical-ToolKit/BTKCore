/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _TREE_VALUE_H_
#define _TREE_VALUE_H_

#include "Open3DMotion/OpenORM/Types.h"
#include <stdlib.h>

/** Macro to declare a name for introspection */
#define DECLARE_CLASS_NAME() \
	static const char classname[]; \
	virtual const char* ClassName() const { return classname; } \
	 static bool ClassNameMatches(const TreeValue* value) \
	 { return (value == 0) ? false : value->ClassNameMatches(classname); }

/** Macro to implement a text string name for a class */
#define DEFINE_CLASS_NAME(x) \
	const char x::classname[] = #x

namespace Open3DMotion
{
	class TreeValue
	{
	public:
		// DISALLOW_COPY_AND_ASSIGN(TreeValue);

	public:
		TreeValue();

		virtual ~TreeValue();

		bool ClassNameMatches(const char* name) const;

		virtual const char* ClassName() const = 0;

		virtual TreeValue* NewBlank() const = 0;

		virtual void CopyFrom(const TreeValue* src) = 0;
	};

	template<class ValueClass> const ValueClass* TreeValueCast(const TreeValue* v)
	{	return ValueClass::ClassNameMatches(v) ? static_cast<const ValueClass*> (v) : NULL;	}

	template<class ValueClass> ValueClass* TreeValueCast(TreeValue* v)
	{	return ValueClass::ClassNameMatches(v) ? static_cast<ValueClass*> (v) : NULL;	}
}

#endif
