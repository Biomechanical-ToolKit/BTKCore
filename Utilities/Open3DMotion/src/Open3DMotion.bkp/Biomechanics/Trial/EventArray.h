/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#pragma once

#include "Open3DMotion/OpenORM/Types.h"
#include "Open3DMotion/Biomechanics/Trial/EventIDTime.h"

#include <string>
#include <vector>
#include <map>

namespace Open3DMotion
{
	/** Manipulate an array of events.  
	
			This complements the EventGroup class.
			The EventGroup class is good for efficient binary storage of events
	    but inefficient to use for adding/removing events at runtime. */
	class EventArray
	{
	public:
		EventArray();

	public:

		void AddEvent(double time, const char* name);

		bool AddEvent(double time, Int32 id);

		void SetIDName(Int32 id, const char* name);

		void Clear();

		size_t NumEvents() const
		{ return events.size(); }

		const double& EventTime(size_t index) const
		{ return events[index].Time(); }

		Int32 EventID(size_t index) const
		{ return events[index].ID(); }

		const std::string& EventName(size_t index) const;

		const std::map<Int32, std::string>& NameMap() const
		{ return names; }

	protected:
		void AddAndSort(Int32 id, double time);

	private:
		std::vector<EventIDTime> events;
		std::map<Int32, std::string> names;
	};

};
