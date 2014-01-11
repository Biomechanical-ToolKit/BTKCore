/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "XMLReadingMachine.h"
#include "ReadWriteXML.h"
#include <pugixml.hpp>

namespace Open3DMotion
{
	XMLReadingMachine::XMLReadingMachine(const BinMemFactory& _memfactory) :
		memfactory(_memfactory)
	{
	}

	XMLReadingMachine::~XMLReadingMachine()
	{
	}

	TreeValue* XMLReadingMachine::Read(const ReadWriteXML* xml_class, const pugi::xml_node& element)
	{
		// retrieve 'type' attribute if present
		pugi::xml_attribute type_node = element.attribute("type");
		std::string type_string( type_node.as_string() );

		// verify type
		if (xml_class->TypeAttribute() == NULL)
		{
			// If our kind of element expects no attribute, there should be no attribute.
			if (type_string.size())
			 return NULL;
		}
		else
		{
			// must match
			if (type_string.compare(xml_class->TypeAttribute()) != 0)
					return NULL;
		}

		// do the read
		return xml_class->ReadValue(*this, element);
	}

	TreeValue* XMLReadingMachine::ReadValue(const pugi::xml_node& xml_element) throw(XMLReadException)
	{
		for (std::vector<ReadWriteXML*>::const_iterator iter_element( element.begin() ); iter_element != element.end(); iter_element++)
		{
			TreeValue* result = Read(*iter_element, xml_element);
			if (result != NULL)
				return result;
		}

		return NULL;
	}

	void XMLReadingMachine::ReadTextNode(std::string& node_text, const pugi::xml_node& element)
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
