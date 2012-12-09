/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#pragma once

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
