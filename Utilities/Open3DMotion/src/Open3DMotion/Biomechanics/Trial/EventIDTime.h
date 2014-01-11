/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_EVENT_ID_TIME_H_
#define _OPEN3DMOTION_EVENT_ID_TIME_H_

#include "Open3DMotion/OpenORM/Types.h"

namespace Open3DMotion
{
	class EventIDTime
	{
	public:
		EventIDTime() :
		id(0L), time(0.0)
		{
		}

		EventIDTime(Int32 _id, double _time) :
			id(_id), time(_time)
		{
		}

	public:

		bool operator<(const EventIDTime& other) const
		{ return (Time() < other.Time()); }

	public:
		const Int32& ID() const
		{ return id; }

		const double& Time() const
		{ return time; }

	protected:
		Int32 id;
		double time;
	};

}

#endif
