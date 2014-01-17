/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_XML_READ_EXCEPTION_H_
#define _OPEN3DMOTION_XML_READ_EXCEPTION_H_

#include "Open3DMotion/OpenORM/IO/ORMIOException.h"

namespace Open3DMotion
{
  // Errors produced when reading/writing BSON to file
  class XMLReadException : public ORMIOException
  {
  public:
    // construct specified error
    XMLReadException(const char* msg) :
			ORMIOException(msg)
    {
    }
  };
}

#endif
