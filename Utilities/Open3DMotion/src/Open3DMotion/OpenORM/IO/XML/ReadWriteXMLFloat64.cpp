/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "ReadWriteXMLFloat64.h"
#include "XMLReadingMachine.h"
#include "XMLWritingMachine.h"
#include <iomanip>

namespace Open3DMotion
{
	void ReadWriteXMLFloat64::WriteValue(XMLWritingMachine& writer, const TreeValue* value) const
	{  writer.OS() << static_cast<const TreeFloat64*>( value )->Value();	}

	TreeValue* ReadWriteXMLFloat64::ReadValue(XMLReadingMachine& /*reader*/, const pugi::xml_node& element) const
	{
		std::string text;
		ReadText(text, element);
		TreeFloat64* value_object = new TreeFloat64;
		sscanf(text.c_str(), " %lf", &value_object->Value());
		return value_object;
	}
}
