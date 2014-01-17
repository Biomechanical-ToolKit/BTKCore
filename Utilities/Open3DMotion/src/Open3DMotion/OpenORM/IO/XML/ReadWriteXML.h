/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _READ_WRITE_XML_H_
#define _READ_WRITE_XML_H_

#include "Open3DMotion/OpenORM/TreeValue.h"
#include <string>

// forward declaration
namespace pugi
{
	class xml_node;
}

namespace Open3DMotion
{
	class XMLReadingMachine;
	class XMLWritingMachine;

	/** Defines the conversion between a supported value type from OpenORM++ and XML.
	    An array of object derived from this is held by each instance of XMLReadWriteMachine and used
			to perform XML conversion on whole OpenORM trees. */
	class ReadWriteXML
	{
	public:
		virtual ~ReadWriteXML()
		{
		}

	public:

		/** Get attribute to insert on XML node to indicate that this class should be used 
		    @return Attribute name. */
		virtual const char* TypeAttribute() const
		{ return NULL; }

		/** The value class which the derived class supports.
		    @return Supported class name as used by TreeValue::ClassNameMatches . */
		virtual const char* SupportedValueClass() const = 0;

		/** Derived reader from XML to OpenORM value */
		virtual TreeValue* ReadValue(XMLReadingMachine& reader, const pugi::xml_node& element) const = 0;

		/** Derived writer from OpenORM value to XML */
		virtual void WriteValue(XMLWritingMachine& writer, const TreeValue* value) const = 0;

	};

}

#endif
