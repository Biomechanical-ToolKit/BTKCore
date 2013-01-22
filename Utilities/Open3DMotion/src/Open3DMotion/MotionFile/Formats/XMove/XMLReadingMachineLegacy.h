/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_XML_READING_MACHINE_LEGACY_H_
#define _OPEN3DMOTION_XML_READING_MACHINE_LEGACY_H_

#include "Open3DMotion/OpenORM/IO/XML/XMLReadingMachine.h"

namespace Open3DMotion
{
	class TreeList;

	class XMLReadingMachineLegacy : public XMLReadingMachine
	{
	public:
		XMLReadingMachineLegacy();

	public:
		virtual TreeValue* ReadValue(const pugi::xml_node& element) throw(XMLReadException);

	public:

		enum LegacyCompoundType
		{
			legacy_none,
			legacy_time_sequence,
			legacy_event_group
		};

	protected:
		LegacyCompoundType legacy;
	};

}

#endif
