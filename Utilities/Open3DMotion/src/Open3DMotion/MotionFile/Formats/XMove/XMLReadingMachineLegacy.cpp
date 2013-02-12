/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "XMLReadingMachineLegacy.h"
#include "Open3DMotion/OpenORM/IO/XML/ReadWriteXMLInt32.h"
#include "Open3DMotion/OpenORM/IO/XML/ReadWriteXMLFloat64.h"
#include "Open3DMotion/OpenORM/IO/XML/ReadWriteXMLBinary.h"
#include "Open3DMotion/OpenORM/Branches/TreeCompound.h"
#include "Open3DMotion/OpenORM/Branches/TreeList.h"
#include "Open3DMotion/Biomechanics/Trial/EventGroup.h"
#include <pugixml.hpp>
#include <cstring>

namespace Open3DMotion
{
	XMLReadingMachineLegacy::XMLReadingMachineLegacy() :
		legacy(legacy_none)
	{
	}

	TreeValue* XMLReadingMachineLegacy::ReadValue(const pugi::xml_node& xml_element) throw(XMLReadException)
	{
		// element to remap
		pugi::xml_node remapped_element = xml_element;

		// if already within compound item, remap accordingly
		if (legacy == legacy_time_sequence || legacy == legacy_event_group)
		{
			if (xml_element.type() == pugi::node_element)
			{
				// get type string
				pugi::xml_attribute type_node = xml_element.attribute("type");
				std::string type_string( type_node.as_string() );

				// override element name for Dimensions -> Dimension
				std::string name( xml_element.name() );
				if (name == "Dimensions")
				{
					name = "Dimension";
					remapped_element.set_name(name.c_str());
				}

				// override type string for known element types
				if (type_string.size() == 0)
				{		
					// known element types
					if (name.compare("Data") == 0)
						type_string = ReadWriteXMLBinary().TypeAttribute();
					else if (name.compare("Start") == 0)
						type_string = ReadWriteXMLFloat64().TypeAttribute();
					else if (name.compare("Rate") == 0)
						type_string = ReadWriteXMLFloat64().TypeAttribute();
					else if (name.compare("Frames") == 0)
						type_string = ReadWriteXMLInt32().TypeAttribute();
					else if (name.compare("Dimension") == 0)
						type_string = ReadWriteXMLInt32().TypeAttribute();
					else if (name.compare("Bytes") == 0)
						type_string = ReadWriteXMLInt32().TypeAttribute();

					// set new type
					if (type_string.size())
					{
						remapped_element.append_attribute("type") = type_string.c_str();
					}
				}
			}
		}

		// store previous compound element legacy status
		LegacyCompoundType previous_legacy = legacy;

		// look for legacy tags for compound elements and set status
		if (xml_element.type() == pugi::node_element)
		{
			if (std::string("timesequence").compare(xml_element.attribute("type").as_string()) == 0)
			{
				legacy = legacy_time_sequence;
				remapped_element.remove_attribute("type");
			}
			else if (std::string("eventflags").compare(xml_element.attribute("type").as_string()) == 0)
			{
				legacy = legacy_event_group;
				remapped_element.remove_attribute("type");

				// In legacy format, Flag ID-Name mapping is placed in root whereas in newer version must be inside EventMap
				// Also flag 'Number' should say 'ID'
				pugi::xml_node eventmap = remapped_element.child(EventGroup::EventMapName);
				if (!eventmap)
				{
					// make new child for event map and populate
					eventmap = remapped_element.append_child(EventGroup::EventMapName);
					for (pugi::xml_node child = remapped_element.first_child(); child; child = child.next_sibling())
					{
						if (child.type() == pugi::node_element && strcmp("Flag", child.name()) == 0)
						{
							pugi::xml_node flag_name = child.child("Name");
							pugi::xml_node flag_id = child.child("Number");
							if (flag_name && flag_id)
							{
								pugi::xml_node remapped_flag = eventmap.append_child(EventGroup::EventMapElementName);
								pugi::xml_node remapped_flag_name = remapped_flag.append_child(MEMBER_NAME(EventNameID::Name));
								pugi::xml_node remapped_flag_id = remapped_flag.append_child(MEMBER_NAME(EventNameID::ID));
								remapped_flag_name.text().set( flag_name.text().get() );
								remapped_flag_id.append_attribute("type") = ReadWriteXMLInt32().TypeAttribute();
								remapped_flag_id.text().set( flag_id.text().get() );
							}
						}
					}

					// remove previous items in root node
					while (remapped_element.remove_child("Flag"))
						;
				}

			}
		}

		// read data
		TreeValue* result = XMLReadingMachine::ReadValue(remapped_element);

		// flag end legacy compound item
		legacy = previous_legacy;

		return result;
	}
}
