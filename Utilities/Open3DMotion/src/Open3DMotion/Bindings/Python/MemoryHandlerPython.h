/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_MEMORY_HANDLER_PYTHON_H_
#define _OPEN3DMOTION_MEMORY_HANDLER_PYTHON_H_

#include "Open3DMotion/OpenORM/Leaves/MemoryHandler.h"
#include <Python.h>

namespace Open3DMotion
{
	/**
	
	A reference-counted memory handler using Python byte array
	for memory (and the Python reference-counting mechanism).

	*/
	class MemoryHandlerPython : public MemoryHandler
	{
		friend class PythonConvert;

	public:
		DECLARE_CLASS_NAME();

		MemoryHandlerPython(size_t _sizebytes);

		MemoryHandlerPython(const MemoryHandlerPython& ref);

		MemoryHandlerPython(PyObject* _py_to_acquire);

		virtual ~MemoryHandlerPython();

		virtual MemoryHandlerPython* Clone() const;

		virtual size_t SizeBytes() const;

		virtual UInt8* Data() const;

		virtual size_t RefCount() const;

	private:
		mutable PyObject* py_bytearray;
	};

}

#endif