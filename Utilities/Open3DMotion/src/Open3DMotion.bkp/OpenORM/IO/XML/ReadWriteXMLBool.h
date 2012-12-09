/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _READ_WRITE_XML_BOOL_H_
#define _READ_WRITE_XML_BOOL_H_

#include "ReadWriteXML.h"
#include "Open3DMotion/OpenORM/Leaves/TreeBool.h"

namespace Open3DMotion
{
	class ReadWriteXMLBool : public ReadWriteXML
	{
	public:
		virtual const char* TypeAttribute() const
		{ return "bool"; }

		virtual const char* SupportedValueClass() const
		{ return TreeBool::classname; }

		virtual void WriteValue(XMLWritingMachine& writer, const TreeValue* value) const;

		virtual TreeValue* ReadValue(XMLReadingMachine& reader, const pugi::xml_node& element) const;
	};
}

#endif
