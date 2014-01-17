/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _ORMPP_BIN_MEM_FACTORY_DEFAULT_H_
#define _ORMPP_BIN_MEM_FACTORY_DEFAULT_H_

#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactory.h"
#include "Open3DMotion/OpenORM/Leaves/MemoryHandlerBasic.h"

namespace Open3DMotion
{
	class BinMemFactoryDefault : public BinMemFactory
	{
	public:
		virtual MemoryHandlerBasic* Allocate(size_t numbytes) const;
	};

}

#endif