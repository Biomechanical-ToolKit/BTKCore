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

namespace Open3DMotion
{
	void ReadWriteXMLBool::WriteValue(XMLWritingMachine& writer, const TreeValue* value) const
	{  writer.OS() << (static_cast<const TreeBool*>( value )->Value() ? "1" : "0");	}

	TreeValue* ReadWriteXMLBool::ReadValue(XMLReadingMachine& /*reader*/, const pugi::xml_node& element) const
	{
		std::string text;
		ReadText(text, element);
		int number;
		sscanf(text.c_str(), " %d", &number);
		return new TreeBool(number ? true : false);
	}
}
