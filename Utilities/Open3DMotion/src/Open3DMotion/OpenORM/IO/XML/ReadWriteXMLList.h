/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _READ_WRITE_XML_LIST_H_
#define _READ_WRITE_XML_LIST_H_

#include "ReadWriteXML.h"
#include "Open3DMotion/OpenORM/Branches/TreeList.h"

namespace Open3DMotion
{
	class ReadWriteXMLList : public ReadWriteXML
	{
	public:

		virtual const char* SupportedValueClass() const
		{ return TreeList::classname; }

		virtual void WriteValue(XMLWritingMachine& writer, const TreeValue* value) const;

		virtual TreeValue* ReadValue(XMLReadingMachine& reader, const pugi::xml_node& element) const;
	};
}

#endif
