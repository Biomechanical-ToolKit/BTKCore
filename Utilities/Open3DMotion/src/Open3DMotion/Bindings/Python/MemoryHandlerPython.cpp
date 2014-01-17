/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "MemoryHandlerPython.h"

namespace Open3DMotion
{
	DEFINE_CLASS_NAME(MemoryHandlerPython);

	MemoryHandlerPython::MemoryHandlerPython(size_t _sizebytes)
	{
		// Since there appears to be no 'new' method,
		// easiest way to efficiently allocate array of correct size appears to be
		// to make one of length 1 and then resize it
		py_bytearray = PyByteArray_FromStringAndSize("", 1);
		PyByteArray_Resize(py_bytearray, _sizebytes);
	}

	MemoryHandlerPython::MemoryHandlerPython(const MemoryHandlerPython& ref)
	{
		// Copy pointer and increment Python reference
		py_bytearray = ref.py_bytearray;
		Py_INCREF(py_bytearray);
	}

	MemoryHandlerPython::MemoryHandlerPython(PyObject* _py_to_acquire)
	{
		// Copy pointer and increment Python reference
		py_bytearray = _py_to_acquire;
		Py_INCREF(py_bytearray);
	}


	MemoryHandlerPython::~MemoryHandlerPython()
	{
		Py_DECREF(py_bytearray);
	}

	MemoryHandlerPython* MemoryHandlerPython::Clone() const
	{
		return new MemoryHandlerPython(*this);
	}

	size_t MemoryHandlerPython::SizeBytes() const
	{
		return PyByteArray_Size(py_bytearray);
	}

	UInt8* MemoryHandlerPython::Data() const
	{
		return (UInt8*) PyByteArray_AsString(py_bytearray);
	}

	size_t MemoryHandlerPython::RefCount() const
	{
		return py_bytearray->ob_refcnt;
	}
}
