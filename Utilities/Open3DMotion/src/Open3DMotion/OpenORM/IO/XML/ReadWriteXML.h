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

		/** Convert from XML element.
				@param reader Associated XML reader class.
		    @param element The element to read. It's assumed this is is correctly formatted - will produce a default or empty value if not.
	   */
		virtual TreeValue* Read(XMLReadingMachine& reader, const pugi::xml_node& element) const;

		/** Write a value to XML.
		    @param writer Associated XML writer class.
				@param element The element to write.  It's assumed this has already been checked and matches the supported value class */
		virtual void Write(XMLWritingMachine& writer, const std::string& name, const TreeValue* value) const;

		/** Get attribute to insert on XML node to indicate that this class should be used 
		    @return Attribute name. */
		virtual const char* TypeAttribute() const
		{ return NULL; }

		/** The value class which the derived class supports.
		    @return Supported class name as used by TreeValue::ClassNameMatches . */
		virtual const char* SupportedValueClass() const = 0;

	protected:

		/** Helper for reading text data from XML */
		static void ReadText(std::string& node_text, const pugi::xml_node& element);

		/** Derived reader from XML to OpenORM value */
		virtual TreeValue* ReadValue(XMLReadingMachine& reader, const pugi::xml_node& element) const = 0;

		/** Derived writer from OpenORM value to XML */
		virtual void WriteValue(XMLWritingMachine& writer, const TreeValue* value) const = 0;

	};

}

#endif
