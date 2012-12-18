/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "XMLWritingMachine.h"
#include "ReadWriteXML.h"
#include <pugixml.hpp>

namespace Open3DMotion
{
	XMLWritingMachine::XMLWritingMachine(std::ostream& _os) :
		os(_os)
			// ,
		// writer(_os)
	{
	}

	void XMLWritingMachine::WriteTextNode(const std::string& value)
	{
		pugi::xml_document d;
		pugi::xml_node n = d.append_child(pugi::node_pcdata);
		n.set_value(value.c_str());
		d.save(os, "", pugi::format_no_declaration | pugi::format_raw, pugi::encoding_utf8);
	}
	
	void XMLWritingMachine::WriteValue(const std::string& name, const TreeValue* value)
	{
		for (std::vector<ReadWriteXML*>::const_iterator iter_element( element.begin() ); iter_element != element.end(); iter_element++)
		{
			if (value->ClassNameMatches((*iter_element)->SupportedValueClass()))
			{
				(*iter_element)->Write(*this, name, value);
				break;
			}
		}
	}
}
