/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "XMLReadingMachine.h"
#include "ReadWriteXML.h"

namespace Open3DMotion
{
	XMLReadingMachine::XMLReadingMachine()
	{
	}

	TreeValue* XMLReadingMachine::ReadValue(const pugi::xml_node& xml_element) throw(XMLReadException)
	{
		for (std::vector<ReadWriteXML*>::const_iterator iter_element( element.begin() ); iter_element != element.end(); iter_element++)
		{
			TreeValue* result = (*iter_element)->Read(*this, xml_element);
			if (result != NULL)
				return result;
		}

		return NULL;
	}
}
