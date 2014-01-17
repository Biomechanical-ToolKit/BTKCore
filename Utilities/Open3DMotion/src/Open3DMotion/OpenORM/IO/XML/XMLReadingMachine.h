/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_XML_READING_MACHINE_H_
#define _OPEN3DMOTION_XML_READING_MACHINE_H_

#include "Open3DMotion/OpenORM/TreeValue.h"
#include "Open3DMotion/OpenORM/IO/XML/XMLReadWriteMachine.h"
#include "Open3DMotion/OpenORM/IO/XML/XMLReadException.h"

namespace pugi
{
	class xml_node;
}

namespace Open3DMotion
{
	class BinMemFactory;

	class XMLReadingMachine : public XMLReadWriteMachine
	{
	public:
		XMLReadingMachine(const BinMemFactory& _memfactory);

		virtual ~XMLReadingMachine();

	public:
		/** Convert from XML element.
		    @param element The element to read. It's assumed this is is correctly formatted - will produce a default or empty value if not.
	   */
		virtual TreeValue* Read(const ReadWriteXML* xml_reader, const pugi::xml_node& element);

		virtual TreeValue* ReadValue(const pugi::xml_node& element) throw(XMLReadException);

		virtual void ReadTextNode(std::string& node_text, const pugi::xml_node& element);

		const BinMemFactory& MemFactory() const
		{ return memfactory; }

	private:
		const BinMemFactory& memfactory;
	};

}

#endif
