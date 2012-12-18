/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_EVENT_GROUP_H_
#define _OPEN3DMOTION_EVENT_GROUP_H_

#include "Open3DMotion/OpenORM/Mappings/RichBinary/RichBinary.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinIter.h"
#include "Open3DMotion/Biomechanics/Trial/EventNameID.h"
#include "Open3DMotion/Biomechanics/Trial/EventArray.h"

namespace Open3DMotion
{
	class EventGroup : public RichBinary
	{
	public:

		EventGroup();

	public:

		void SetEvents(const EventArray& events, BinMemFactory& memfactory);

		void GetEvents(EventArray& events) const;

	public:

		/** Name to give data structure on the tree ("EventStructure") */
		static const char StructureName[];

		/** name of the id field ("flag") */
		static const char IDFieldName[];

		/** name of the time field ("time") */
		static const char TimeFieldName[];

	public:		

		MapString Name;

	protected:

		MapArrayCompound<EventNameID> EventMap;

	};

	class EventGroupConstIter : public BinConstIter2<Int32, double>
	{
	public:
		EventGroupConstIter(const EventGroup& eg) :
				BinConstIter2<Int32, double> (eg, EventGroup::IDFieldName, 1, EventGroup::TimeFieldName, 1)
		{
		}

	public:
		const Int32& ID() const
		{ return *ValuePtr0(); }

		const double& Time() const
		{ return *ValuePtr1(); }
	};

	class EventGroupIter : public BinIter2<Int32, double>
	{
	public:
		EventGroupIter(const EventGroup& eg) :
				BinIter2<Int32, double> (eg, EventGroup::IDFieldName, 1, EventGroup::TimeFieldName, 1)
		{
		}

	public:
		Int32& ID() const
		{ return *ValuePtr0(); }

		double& Time() const
		{ return *ValuePtr1(); }
	};
}
#endif