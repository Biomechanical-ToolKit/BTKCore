/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "ReadWriteXMLList.h"
#include "XMLReadingMachine.h"
#include "XMLWritingMachine.h"
#include <pugixml.hpp>
#include <memory>

namespace Open3DMotion
{
	void ReadWriteXMLList::WriteValue(XMLWritingMachine& writer, const TreeValue* value) const
	{ 
		const TreeList* listobject = static_cast<const TreeList*>( value );
		const std::vector<TreeValue*>& value_list = listobject->ElementArray();
		for (std::vector<TreeValue*>::const_iterator value_iter( value_list.begin() ); value_iter != value_list.end(); value_iter++)
		{
			writer.WriteValue(listobject->ElementName(), *value_iter);
		}
	}

	TreeValue* ReadWriteXMLList::ReadValue(XMLReadingMachine& reader, const pugi::xml_node& element) const
	{
		// verify all nodes have identical name
		std::string xml_element_name;
		pugi::xml_node child;
		for (child = element.first_child(); child; child = child.next_sibling())
		{
			if (child.type() == pugi::node_element)
			{
				if (xml_element_name.empty())
				{
					// first node found - retain name
					xml_element_name = child.name();
				}
				else
				{
					// additional nodes - verify that names are all the same as the first
					if (xml_element_name.compare(child.name()) != 0)
						return NULL;
				}
			}
		}

		// must have found some nodes
		if (xml_element_name.empty())
			return NULL;

		// new list
		std::auto_ptr<TreeList> newlist( new TreeList( xml_element_name.c_str() ) );

		// read all
		for (child = element.first_child(); child; child = child.next_sibling())
		{
			if (child.type() == pugi::node_element)
			{
				TreeValue* newvalue = reader.ReadValue( child );
				if (newvalue != NULL)
				{
					newlist->Add( newvalue );
				}
				else
				{
					// silently ignore if element type is unrecognised
				}
			}
		}

		return newlist.release();
	}
}
