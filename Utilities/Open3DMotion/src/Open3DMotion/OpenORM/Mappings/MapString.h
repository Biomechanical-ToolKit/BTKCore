/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#pragma once

#include "Open3DMotion/OpenORM/Leaves/TreeString.h"
#include "Open3DMotion/OpenORM/Mappings/MapSimpleValue.h"
#include "Open3DMotion/OpenORM/Mappings/MapOptional.h"

namespace Open3DMotion
{
	class MapString : public MapSimpleValue<TreeString, std::string>
	{
	public:

		operator const std::string&() const
		{ return Value(); }

		operator const char*() const
		{ return Value().c_str(); }

		const char* operator=(const char* _x)
		{ std::string& s = Value(); s = _x; return s.c_str(); }

		const std::string& operator=(const std::string& _x)
		{ std::string& s = Value(); s = _x; return s; }
	};

	class MapOptionalString : public MapOptional<MapString>
	{
	public:

		operator const std::string&() const
		{ return Value(); }

		operator const char*() const
		{ return Value().c_str(); }

		const char* operator=(const char* _x)
		{ std::string& s = Value(); s = _x; return s.c_str(); }

		const std::string& operator=(const std::string& _x)
		{ std::string& s = Value(); s = _x; return s; }

	};
}
