/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Biomechanics/Trial/EventGroup.h"
#include <algorithm>

namespace Open3DMotion
{
	static const std::string EVENT_ARRAY_INVALID_INDEX_STRING("");

	EventArray::EventArray()
	{
	}

	void EventArray::AddEvent(double time, const char* name)
	{
		// find max id and also see if we have this name
		Int32 useid = -1;
		Int32 maxid = 0;
		for (std::map<Int32, std::string>::const_iterator i( names.begin() ); i != names.end(); i++)
		{
			if (i->first > maxid)
				maxid = i->first;

			if ((useid == -1) && (i->second.compare(name) == 0))
				useid = i->first;
		}

		// name not in list - auto-generate new one using max id + 1
		if (useid == -1)
		{
			useid = maxid + 1;
			SetIDName(useid, name);
		}

		// add it
		AddAndSort(useid, time);
	}

	void EventArray::AddAndSort(Int32 id, double time)
	{
		// set event
		events.push_back(EventIDTime(id, time));

		// sort by time, uses EventIDTime::operator< to compare times
		std::stable_sort(events.begin(), events.end());
	}

	bool EventArray::AddEvent(double time, Int32 id)
	{
		// verify that id is in list
		std::map<Int32, std::string>::const_iterator i = names.find(id);
		
		// abort if not there
		if (i == names.end())
			return false;
		
		// insert and resort
		AddAndSort(id, time);

		// done
		return true;
	}

	void EventArray::SetIDName(Int32 id, const char* name)
	{
		names[id] = name;
	}

	void EventArray::Clear()
	{
		events.clear();
		names.clear();
	}

	const std::string& EventArray::EventName(size_t index) const
	{
		Int32 id = EventID(index);
		return names.find(id)->second;
	}

}
