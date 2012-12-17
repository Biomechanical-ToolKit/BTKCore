/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORM_IO_READ_EXCEPTION_H_
#define _ORM_IO_READ_EXCEPTION_H_

#ifdef _MSC_VER
  // Disables MS warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
  #pragma warning( disable : 4290 )
#endif

#include <string>

namespace Open3DMotion
{
  // Errors produced when reading/writing BSON to file
  class ORMIOException
  {
  public:
    // error message string
    std::string message;

  public:
    // construct specified error
    ORMIOException(const char* msg) :
			message(msg)
    {
    }
  };

}

#endif
