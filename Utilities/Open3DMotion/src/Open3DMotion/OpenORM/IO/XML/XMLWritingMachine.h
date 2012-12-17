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

namespace Open3DMotion
{
	class XMLWritingMachine : public XMLReadWriteMachine
	{
	public:
		XMLWritingMachine(std::ostream& _os);

	public:
		std::ostream& OS()
		{ return os; }

		void WriteTextNode(const std::string& value);

		void WriteValue(const std::string& name, const TreeValue* value);

	protected:
		std::ostream& os;
	};
}

#endif
