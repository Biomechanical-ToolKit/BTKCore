/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#pragma once

#include "Open3DMotion/OpenORM/Types.h"

namespace Open3DMotion
{
	template<class ValueType, unsigned int ValueDim> class ValueN
	{
	public:

		static const unsigned int ELEMENTSIZE = sizeof(ValueType);
		static const unsigned int DIMENSION = ValueDim;

	public:
		ValueN()
		{
		}

		ValueN(ValueType xinit)
		{
			for (unsigned int i = 0; i < ValueDim; i++)
				x[i] = xinit;
		}

		ValueN(const ValueType* arr)
		{
			for (unsigned int i = 0; i < ValueDim; i++)
				x[i] = arr[i];
		}

	public:

    // Cast to array
    operator ValueType * ()
    { return x; }

    // Cast to array (const)
    operator const ValueType * () const
    { return x; }

	public:
    // vector values
    ValueType x[ValueDim];
	};
}
