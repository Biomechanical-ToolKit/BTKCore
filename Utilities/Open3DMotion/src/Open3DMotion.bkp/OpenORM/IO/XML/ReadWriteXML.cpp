/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "ReadWriteXML.h"
#include "XMLWritingMachine.h"
#include <pugixml.hpp>

namespace Open3DMotion
{
	TreeValue* ReadWriteXML::Read(XMLReadingMachine& reader, const pugi::xml_node& element) const
	{
		// retrieve 'type' attribute if present
		pugi::xml_attribute type_node = element.attribute("type");
		std::string type_string( type_node.as_string() );

		// verify type
		if (TypeAttribute() == NULL)
		{
			// if our kind of element expects no attribute, there must be no attribute
			if (type_string.size())
				return NULL;
		}
		else
		{
			// special case for legacy formats where binary data has empty type string
			if (type_string.size() == 0)
			{
				if (std::string("Data").compare(element.name()) == 0)
					type_string = "base64";
			}

			// must match
			if (type_string.compare(TypeAttribute()) != 0)
					return NULL;
		}

		// do the read
		return ReadValue(reader, element);
	}

	void ReadWriteXML::Write(XMLWritingMachine& writer, const std::string& name, const TreeValue* value) const
	{
		writer.OS() << "<" << name;
		const char* type_attribute = TypeAttribute();
		if (type_attribute != NULL)
		{
			writer.OS() << " type=\"" << type_attribute << "\"";
		}
		writer.OS() << ">";
		WriteValue(writer, value);
		writer.OS() << "</" << name << ">";
	}

	void ReadWriteXML::ReadText(std::string& node_text, const pugi::xml_node& element)
	{
		for (pugi::xml_node child = element.first_child(); child; child = child.next_sibling())
		{
			if (child.type() == pugi::node_pcdata)
			{
				node_text = child.value();
				return;
			}
		}
	}
}
