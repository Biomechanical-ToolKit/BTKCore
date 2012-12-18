/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_XML_READ_WRITE_MACHINE_H_
#define _OPEN3DMOTION_XML_READ_WRITE_MACHINE_H_

#include <vector>

namespace Open3DMotion
{
	class ReadWriteXML;

	class XMLReadWriteMachine
	{
	public:
		XMLReadWriteMachine();

		virtual ~XMLReadWriteMachine();

	protected:
		std::vector<ReadWriteXML*> element;
	};

}

#endif
