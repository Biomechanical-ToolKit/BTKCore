/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Biomechanics/Trial/EventGroup.h"

namespace Open3DMotion
{
	const char EventGroup::StructureName[] = "EventStructure";

	const char EventGroup::IDFieldName[] = "flag";

	const char EventGroup::TimeFieldName[] = "time";

	const char EventGroup::EventMapName[] = "EventMap";

	const char EventGroup::EventMapElementName[] = "Event";

	EventGroup::EventGroup() :
		RichBinary(StructureName),
		EventMap(EventMapElementName)
	{
		REGISTER_MEMBER(Name);
		Register(EventMapName, &EventMap);
	}

	void EventGroup::SetEvents(const EventArray& events, const BinMemFactory& memfactory)
	{
		// put all names in list
		// size_t num_names = events.NameMap().size();
		for (std::map<Int32, std::string>::const_iterator iname( events.NameMap().begin() ); iname != events.NameMap().end(); iname++)
		{
			EventNameID nameid;
			nameid.ID = iname->first;
			nameid.Name = iname->second;
			EventMap.Add(nameid);
		}

		// copy events
		size_t num_events = events.NumEvents();
		std::vector<BinaryFieldSpec> layout;
		layout.push_back(BinaryFieldSpec::FromType<Int32>(EventGroup::IDFieldName, 1));
		layout.push_back(BinaryFieldSpec::FromType<double>(EventGroup::TimeFieldName, 1));
		Allocate(layout, num_events, memfactory);
		for (EventGroupIter i(*this); i.HasFrame(); i.Next())
		{
			i.Time() = events.EventTime(i.FrameIndex());
			i.ID() = events.EventID(i.FrameIndex());
		}
	}

	void EventGroup::GetEvents(EventArray& events) const
	{
		// put all names in list
		size_t num_names = EventMap.NumElements();
		for (size_t iname = 0; iname < num_names; iname++)
		{
			const EventNameID& nameid = EventMap[iname];
			events.SetIDName(nameid.ID, nameid.Name);
		}

		// put all event times
		for (EventGroupConstIter i(*this); i.HasFrame(); i.Next())
		{
			events.AddEvent(i.Time(), i.ID());
		}

	}

}
