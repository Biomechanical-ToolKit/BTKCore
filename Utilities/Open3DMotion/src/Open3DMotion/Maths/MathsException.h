/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _MATHS_EXCEPTION_H_
#define _MATHS_EXCEPTION_H_

#include <string>

#ifdef _MSC_VER
  // Disables MS warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
  #pragma warning( disable : 4290 )
#endif

namespace Open3DMotion
{
	class MathsException
	{
	public:
		MathsException();

		MathsException(const char* _condition);

		MathsException(const MathsException& src);

	public:

		const MathsException& operator=(const MathsException& src);

	public:

		const std::string& Condition() const
		{ return condition; }

	private:
		std::string condition;
	};

}

#define CODAMOTION_MATHS_VERIFY(condition) { if (!(condition)) throw MathsException(#condition); }

#endif
