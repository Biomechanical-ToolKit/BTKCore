/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "XMLWritingMachineLegacy.h"
#include "Open3DMotion/OpenORM/IO/XML/ReadWriteXML.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinaryFieldSpec.h"
#include "Open3DMotion/Biomechanics//Trial/Trial.h"
#include <pugixml.hpp>
#include <memory>

namespace Open3DMotion
{
	XMLWritingMachineLegacy::XMLWritingMachineLegacy(std::ostream& _os) :
		XMLWritingMachine(_os)
	{
	}

	void XMLWritingMachineLegacy::Write(const ReadWriteXML* xml_class, const std::string& name, const TreeValue* value)
	{
		const char* name_to_write(NULL);
		if (name.compare(MEMBER_NAME(BinaryFieldSpec::Dimension)) == 0)
		{
			// remap Dimension -> Dimensions
			name_to_write = "Dimensions";
		}
		else
		{
			// usual name
			name_to_write = name.c_str();
		}

		// type attribute (or NULL for compound/list/string)
		const char* type_attribute = xml_class->TypeAttribute();

		// override for known structure names
		std::auto_ptr<TreeCompound> remapped_group;
		std::auto_ptr<TreeString> remapped_group_name;
		MapArrayCompound<EventNameID> remapped_event_ids(EventGroup::EventMapElementName);
		if (type_attribute == NULL && value->ClassNameMatches(TreeCompound::classname))
		{
			// put type attribute onto sequences and event groups
			if (name.compare("Sequence") == 0)
			{
				type_attribute = "timesequence";
			}
			else if (name.compare("EventGroup") == 0)
			{
				type_attribute = "eventflags";

				// remap event names
				const TreeValue* id_list = TreeValueCast<const TreeCompound>(value)->Get( EventGroup::EventMapName );
				if (id_list)
				{
					// parse the event map for later use
					remapped_event_ids.FromTree( id_list );

					// re-assign the value pointer to a mutable copy of the event group
					remapped_group = std::auto_ptr<TreeCompound> ( new TreeCompound );
					remapped_group->CopyFrom( value );
					value = remapped_group.get();

					// remove event map from it
					remapped_group->Remove( EventGroup::EventMapName );

					// extract group name so can use it just before events to preserve same order as legacy
					const TreeString* name = remapped_group->GetType<TreeString>( MEMBER_NAME(EventGroup::Name) );
					if (name)
					{
						// copy name
						remapped_group_name = std::auto_ptr<TreeString>( new TreeString(*name) );
						
						// remove from map
						remapped_group->Remove( MEMBER_NAME(EventGroup::Name) );
					}
				}
			}
		}

		// start of element
		os << "<" << name_to_write;

		// add optional type
		if (type_attribute != NULL)
		{
			os << " type=\"" << type_attribute << "\"";
		}
		os << ">";

		// remapped name to insert at start
		if (remapped_group_name.get() != NULL)
		{
			WriteValue("Name", remapped_group_name.get());
		}
		
		// remapped flags to insert after name 
		for (size_t iname = 0; iname < remapped_event_ids.NumElements(); iname++)
		{
			const EventNameID& name_id = remapped_event_ids[iname];
			std::auto_ptr<TreeCompound> remapped_name_id( new TreeCompound );
			remapped_name_id->Set("Number", new TreeInt32(name_id.ID));
			remapped_name_id->Set("Name", new TreeString(name_id.Name));
			WriteValue("Flag", remapped_name_id.get());
		}

		// the usual enclosed value
		xml_class->WriteValue(*this, value);
		
		// end of element
		os << "</" << name_to_write << ">";
	}
}
