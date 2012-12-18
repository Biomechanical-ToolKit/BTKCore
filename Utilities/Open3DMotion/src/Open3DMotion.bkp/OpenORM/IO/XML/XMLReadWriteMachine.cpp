/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "XMLReadWriteMachine.h"
#include "ReadWriteXMLCompound.h"
#include "ReadWriteXMLList.h"
#include "ReadWriteXMLBool.h"
#include "ReadWriteXMLInt32.h"
#include "ReadWriteXMLFloat64.h"
#include "ReadWriteXMLString.h"
#include "ReadWriteXMLBinary.h"

namespace Open3DMotion
{
	XMLReadWriteMachine::XMLReadWriteMachine()
	{
		element.push_back(new ReadWriteXMLCompound);
		element.push_back(new ReadWriteXMLList);
		element.push_back(new ReadWriteXMLBool);
		element.push_back(new ReadWriteXMLInt32);
		element.push_back(new ReadWriteXMLFloat64);
		element.push_back(new ReadWriteXMLString);
		element.push_back(new ReadWriteXMLBinary);
	}

	XMLReadWriteMachine::~XMLReadWriteMachine()
	{
		for (std::vector<ReadWriteXML*>::iterator iter_element( element.begin() ); iter_element != element.end(); iter_element++)
			delete *iter_element;
	}
}
