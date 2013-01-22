/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _READ_WRITE_XML_INT32_H_
#define _READ_WRITE_XML_INT32_H_

#include "ReadWriteXML.h"
#include "Open3DMotion/OpenORM/Leaves/TreeInt32.h"

namespace Open3DMotion
{
	class ReadWriteXMLInt32 : public ReadWriteXML
	{
	public:
		virtual const char* TypeAttribute() const
		{ return "long"; }

		virtual const char* SupportedValueClass() const
		{ return TreeInt32::classname; }

		virtual TreeValue* ReadValue(XMLReadingMachine& reader, const pugi::xml_node& element) const;
	
		virtual void WriteValue(XMLWritingMachine& writer, const TreeValue* value) const;
	};

}

#endif
