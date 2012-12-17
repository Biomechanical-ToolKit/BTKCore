/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
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
	class XMLReadingMachine : public XMLReadWriteMachine
	{
	public:
		XMLReadingMachine();

	public:
		TreeValue* ReadValue(const pugi::xml_node& element) throw(XMLReadException);
	};

}

#endif
