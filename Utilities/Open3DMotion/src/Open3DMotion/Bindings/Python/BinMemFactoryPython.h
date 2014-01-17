/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_BIN_MEM_FACTORY_PYTHON_H_
#define _ORMPP_BIN_MEM_FACTORY_PYTHON_H_

#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactory.h"
#include "Open3DMotion/Bindings/Python/MemoryHandlerPython.h"

namespace Open3DMotion
{
	class BinMemFactoryPython : public BinMemFactory
	{
	public:
		virtual MemoryHandlerPython* Allocate(size_t numbytes) const;
	};

}

#endif