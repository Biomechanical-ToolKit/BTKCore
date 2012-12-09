/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#pragma once

#ifdef _MSC_VER
  // Disables MS warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
  #pragma warning( disable : 4290 )
#endif

#include <string>

namespace Open3DMotion
{
  // Errors produced when reading/writing files
  class MotionFileException
  {
  public:
    // Numerical error ID
    enum error_number
    {
			libraryerror = 1,
      noaccess,
      noopen,
      formatnotrecognised,
      formaterror,
      notsupported
    } number;

    // error message string
    std::string message;

  public:
    // construct specified error
    MotionFileException(error_number n, const char* msg) :
      number(n),
			message(msg)
    {
    }
  };

}
