/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _READ_WRITE_XML_STRING_H_
#define _READ_WRITE_XML_STRING_H_

#include "ReadWriteXML.h"
#include "Open3DMotion/OpenORM/Leaves/TreeString.h"

namespace Open3DMotion
{
	class ReadWriteXMLString : public ReadWriteXML
	{
	public:

		virtual const char* SupportedValueClass() const
		{ return TreeString::classname; }

		virtual void WriteValue(XMLWritingMachine& writer, const TreeValue* value) const;

		virtual TreeValue* ReadValue(XMLReadingMachine& reader, const pugi::xml_node& element) const;
	};
}

#endif
