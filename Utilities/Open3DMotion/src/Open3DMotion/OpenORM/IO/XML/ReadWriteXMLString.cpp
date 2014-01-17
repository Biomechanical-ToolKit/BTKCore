/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "ReadWriteXMLString.h"
#include "XMLReadingMachine.h"
#include "XMLWritingMachine.h"

namespace Open3DMotion
{
	void ReadWriteXMLString::WriteValue(XMLWritingMachine& writer, const TreeValue* value) const
	{ 
		writer.WriteTextNode( static_cast<const TreeString*>( value )->Value() );
	}

	TreeValue* ReadWriteXMLString::ReadValue(XMLReadingMachine& reader, const pugi::xml_node& element) const
	{
		std::string text;
		reader.ReadTextNode(text, element);
		if (text.size() == 0)
			return NULL;
		else
			return new TreeString(text.c_str());
	}
}
