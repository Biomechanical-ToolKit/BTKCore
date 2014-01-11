/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "ReadWriteXMLFloat64.h"
#include "XMLReadingMachine.h"
#include "XMLWritingMachine.h"
#include <iomanip>
#include <stdio.h>

#if defined(_MSC_VER)
  // Disable unsafe warning (use of the function 'sscanf' instead of 'sscanf_s' for portability reasons;
  #pragma warning( disable : 4996 )
#endif

namespace Open3DMotion
{
	void ReadWriteXMLFloat64::WriteValue(XMLWritingMachine& writer, const TreeValue* value) const
	{  writer.OS() << static_cast<const TreeFloat64*>( value )->Value();	}

	TreeValue* ReadWriteXMLFloat64::ReadValue(XMLReadingMachine& reader, const pugi::xml_node& element) const
	{
		std::string text;
		reader.ReadTextNode(text, element);
		TreeFloat64* value_object = new TreeFloat64;
		sscanf(text.c_str(), " %lf", &value_object->Value());
		return value_object;
	}
}
