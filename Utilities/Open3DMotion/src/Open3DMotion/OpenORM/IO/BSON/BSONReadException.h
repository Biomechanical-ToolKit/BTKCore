/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_BSON_READ_EXCEPTION_H_
#define _ORMPP_BSON_READ_EXCEPTION_H_

#include "Open3DMotion/OpenORM/IO/ORMIOException.h"

namespace Open3DMotion
{
  /** Errors produced when reading/writing BSON to/from stream */
  class BSONReadException : public ORMIOException
  {
  public:
    /** construct specified error */
    BSONReadException(const char* msg) :
			ORMIOException(msg)
    {
    }
  };

}

#endif
