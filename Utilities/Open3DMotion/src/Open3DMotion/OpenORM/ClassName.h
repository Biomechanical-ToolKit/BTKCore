/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_CLASS_NAME_H_
#define _ORMPP_CLASS_NAME_H_

/** Declare abstract base class as nameable (derived class instances will contain names) */
#define DECLARE_CLASS_NAME_ABSTRACT_BASE() \
		bool ClassNameMatches(const char* name) const; \
		virtual const char* ClassName() const = 0;

/** Implementation required for nameable abstract base class */
#define DEFINE_CLASS_NAME_ABSTRACT_BASE(x) \
		bool x::ClassNameMatches(const char* name) const \
	  { return (strcmp(ClassName(), name) == 0); }

/** Macro to declare a name for introspection */
#define DECLARE_CLASS_NAME() \
	static const char classname[]; \
	virtual const char* ClassName() const { return classname; }

/** Macro to implement a text string name for a class */
#define DEFINE_CLASS_NAME(x) \
	const char x::classname[] = #x

namespace Open3DMotion
{
	template<class BaseClass, class ValueClass> const ValueClass* NamedClassCast(const BaseClass* v)
	{	return (v && v->ClassNameMatches(ValueClass::classname)) ? static_cast<const ValueClass*> (v) : 0;	}

	template<class BaseClass, class ValueClass> ValueClass* NamedClassCast(BaseClass* v)
	{	return (v && v->ClassNameMatches(ValueClass::classname)) ? static_cast<ValueClass*> (v) : 0;	}
}

#endif

