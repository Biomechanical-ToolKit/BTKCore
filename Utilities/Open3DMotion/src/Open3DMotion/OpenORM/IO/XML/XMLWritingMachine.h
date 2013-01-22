/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_XML_WRITING_MACHINE_H_
#define _OPEN3DMOTION_XML_WRITING_MACHINE_H_

#include "XMLReadWriteMachine.h"
#include "Open3DMotion/OpenORM/TreeValue.h"

#include <ostream>

namespace Open3DMotion
{
	class XMLWritingMachine : public XMLReadWriteMachine
	{
	public:
		XMLWritingMachine(std::ostream& _os);

		virtual ~XMLWritingMachine();

	public:
		std::ostream& OS()
		{ return os; }

		/** Write a value to XML.
				@param element The element to write.  It's assumed this has already been checked and matches the supported value class */
		virtual void Write(const ReadWriteXML* xml_writer, const std::string& name, const TreeValue* value);

		virtual void WriteTextNode(const std::string& value);

		virtual void WriteValue(const std::string& name, const TreeValue* value);

	protected:
		std::ostream& os;
	};
}

#endif
