/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_NO_SUCH_FIELD_EXCEPTION_H_
#define _OPEN3DMOTION_NO_SUCH_FIELD_EXCEPTION_H_

#ifdef _MSC_VER
  // Disables MS warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
  #pragma warning( disable : 4290 )
#endif

#include <string>

namespace Open3DMotion
{
	class NoSuchFieldException
	{
	public:
		enum ErrorCode { missing, wrongtype, wrongdimension };

	public:
		NoSuchFieldException(const char* _fieldname, ErrorCode _errorcode) :
			fieldname(_fieldname),
			errorcode(_errorcode)
		{
		}

	public:
		const std::string& FieldName() const
		{ return fieldname; }

		ErrorCode Error() const
		{ return errorcode; }

	private:
		std::string fieldname;
		ErrorCode errorcode;
	};
}
#endif