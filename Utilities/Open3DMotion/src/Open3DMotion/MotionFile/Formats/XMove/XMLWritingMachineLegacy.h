/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_XML_WRITING_MACHINE_LEGACY_H_
#define _OPEN3DMOTION_XML_WRITING_MACHINE_LEGACY_H_

#include "Open3DMotion/OpenORM/IO/XML/XMLWritingMachine.h"

namespace Open3DMotion
{
	class XMLWritingMachineLegacy : public XMLWritingMachine
	{
	public:
		XMLWritingMachineLegacy(std::ostream& _os);

		/** Write a value to XML.
				@param element The element to write.  It's assumed this has already been checked and matches the supported value class */
		virtual void Write(const ReadWriteXML* xml_writer, const std::string& name, const TreeValue* value);
	};
}

#endif
