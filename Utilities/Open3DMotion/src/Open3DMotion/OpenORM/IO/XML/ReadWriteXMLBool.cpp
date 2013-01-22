/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "ReadWriteXMLBool.h"
#include "XMLReadingMachine.h"
#include "XMLWritingMachine.h"
#include <iomanip>
#include <cstdio>

#if defined(_MSC_VER)
  // Disable unsafe warning (use of the function 'sscanf' instead of 'sscanf_s' for portability reasons;
  #pragma warning( disable : 4996 )
#endif

namespace Open3DMotion
{
	void ReadWriteXMLBool::WriteValue(XMLWritingMachine& writer, const TreeValue* value) const
	{  writer.OS() << (static_cast<const TreeBool*>( value )->Value() ? "1" : "0");	}

	TreeValue* ReadWriteXMLBool::ReadValue(XMLReadingMachine& reader, const pugi::xml_node& element) const
	{
		std::string text;
		reader.ReadTextNode(text, element);
		int number;
		sscanf(text.c_str(), " %d", &number);
		return new TreeBool(number ? true : false);
	}
}
