/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "ReadWriteXMLInt32.h"
#include "XMLReadingMachine.h"
#include "XMLWritingMachine.h"
#include <iomanip>
#include <cstdio>

namespace Open3DMotion
{
	TreeValue* ReadWriteXMLInt32::ReadValue(XMLReadingMachine& /*reader*/, const pugi::xml_node& element) const
	{
		std::string text;
		ReadText(text, element);
		TreeInt32* value_object = new TreeInt32;
		sscanf(text.c_str(), " %d", &value_object->Value());
		return value_object;
	}
	
	void ReadWriteXMLInt32::WriteValue(XMLWritingMachine& writer, const TreeValue* value) const
		{ writer.OS() << static_cast<const TreeInt32*>( value )->Value();	}
}
